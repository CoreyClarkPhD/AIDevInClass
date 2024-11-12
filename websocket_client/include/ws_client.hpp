#pragma once

#include "server_manager.hpp"
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <json/json.h>
#include <atomic>
#include <thread>
#include <memory>

// Typedef for easier reference
using WebsocketClient = websocketpp::client<websocketpp::config::asio_client>;
using MessagePtr = websocketpp::config::asio_client::message_type::ptr;
using ConnectionHdl = websocketpp::connection_hdl;

class WSClient {
public:
    explicit WSClient(const ServerConfig& config);
    ~WSClient();

    bool connect(const std::string& uri);
    void sendCommand(const std::string& commandName, const Json::Value& parameters);
    void stop();
    bool isConnected() const;
    
    // Server management methods
    bool ensureServerRunning();
    bool shutdownServer();

private:
    // WebSocket members
    WebsocketClient endpoint_;
    ConnectionHdl connection_;
    std::atomic<bool> connected_;
    std::atomic<bool> should_run_;
    std::thread asio_thread_;

    // Server management members
    std::unique_ptr<ServerManager> serverManager_;
    bool autoStartServer_;

    // Message handling methods
    void handleMessage(const std::string& message);
    void handleEvent(const Json::Value& message);
    void handleResponse(const Json::Value& message);
    void handleError(const Json::Value& message);
    void sendMessage(const Json::Value& message);

    // Utility methods
    std::string generateUUID();
    int64_t getCurrentTimestamp();
};