#include "ws_client.hpp"
#include <iostream>
#include <sstream>
#include <random>
#include <chrono>

WSClient::WSClient(const ServerConfig& config) 
    : endpoint_()
    , connected_(false)
    , should_run_(true)
    , serverManager_(std::make_unique<ServerManager>(config))
    , autoStartServer_(true) {
    
    endpoint_.clear_access_channels(websocketpp::log::alevel::all);
    endpoint_.clear_error_channels(websocketpp::log::elevel::all);

    endpoint_.init_asio();

    endpoint_.set_open_handler([this](ConnectionHdl hdl) {
        connected_ = true;
        connection_ = hdl;
        std::cout << "Connected to server" << std::endl;
    });

    endpoint_.set_close_handler([this](ConnectionHdl) {
        connected_ = false;
        std::cout << "Disconnected from server" << std::endl;
    });

    endpoint_.set_message_handler([this](ConnectionHdl, MessagePtr msg) {
        handleMessage(msg->get_payload());
    });
}

WSClient::~WSClient() {
    stop();
    if (autoStartServer_) {
        shutdownServer();
    }
}

bool WSClient::connect(const std::string& uri) {
    if (autoStartServer_) {
        if (!ensureServerRunning()) {
            std::cout << "Failed to start server" << std::endl;
            return false;
        }
    }

    websocketpp::lib::error_code ec;
    WebsocketClient::connection_ptr con = endpoint_.get_connection(uri, ec);
    
    if (ec) {
        std::cout << "Connect initialization error: " << ec.message() << std::endl;
        return false;
    }

    endpoint_.connect(con);

    asio_thread_ = std::thread([this]() {
        try {
            endpoint_.run();
        } catch (const std::exception& e) {
            std::cout << "Error in ASIO thread: " << e.what() << std::endl;
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return connected_;
}

void WSClient::sendCommand(const std::string& commandName, const Json::Value& parameters) {
    Json::Value message;
    message["type"] = "command";
    message["id"] = generateUUID();
    message["timestamp"] = getCurrentTimestamp();
    
    Json::Value& command = message["payload"]["command"];
    command["name"] = commandName;
    command["parameters"] = parameters;

    sendMessage(message);
}

void WSClient::stop() {
    if (should_run_.exchange(false)) {
        if (connected_) {
            websocketpp::lib::error_code ec;
            endpoint_.close(connection_, websocketpp::close::status::normal, 
                          "Closing connection", ec);
        }
        
        endpoint_.stop();
        
        if (asio_thread_.joinable()) {
            asio_thread_.join();
        }
    }
}

bool WSClient::isConnected() const {
    return connected_;
}

bool WSClient::ensureServerRunning() {
    return serverManager_->startServer();
}

bool WSClient::shutdownServer() {
    return serverManager_->stopServer();
}

void WSClient::handleMessage(const std::string& message) {
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::string errors;
    std::istringstream iss(message);

    if (!Json::parseFromStream(builder, iss, &root, &errors)) {
        std::cout << "Failed to parse message: " << errors << std::endl;
        return;
    }

    std::cout << "Received message:" << std::endl;
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder["indentation"] = "    ";
    std::cout << Json::writeString(writerBuilder, root) << std::endl;

    std::string type = root["type"].asString();
    if (type == "event") {
        handleEvent(root);
    } else if (type == "response") {
        handleResponse(root);
    } else if (type == "error") {
        handleError(root);
    }
}

void WSClient::handleEvent(const Json::Value& message) {
    const Json::Value& event = message["payload"]["event"];
    std::cout << "Received event: " << event["name"].asString() << std::endl;
}

void WSClient::handleResponse(const Json::Value& message) {
    const Json::Value& response = message["payload"]["response"];
    std::cout << "Received response: " << response["success"].asBool() << std::endl;
}

void WSClient::handleError(const Json::Value& message) {
    const Json::Value& error = message["payload"]["error"];
    std::cout << "Received error: " << error["message"].asString() 
              << " (Code: " << error["code"].asInt() << ")" << std::endl;
}

void WSClient::sendMessage(const Json::Value& message) {
    if (!connected_) {
        std::cout << "Not connected to server" << std::endl;
        return;
    }

    Json::StreamWriterBuilder builder;
    builder["indentation"] = ""; // Don't include whitespace
    std::string messageStr = Json::writeString(builder, message);
    
    websocketpp::lib::error_code ec;
    endpoint_.send(connection_, messageStr, websocketpp::frame::opcode::text, ec);
    
    if (ec) {
        std::cout << "Error sending message: " << ec.message() << std::endl;
        return;
    }

    std::cout << "Sent message:" << std::endl;
    builder["indentation"] = "    "; // Pretty print for console
    std::cout << Json::writeString(builder, message) << std::endl;
}

std::string WSClient::generateUUID() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    };
    return ss.str();
}

int64_t WSClient::getCurrentTimestamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}