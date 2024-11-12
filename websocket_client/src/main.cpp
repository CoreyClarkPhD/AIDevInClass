#include "ws_client.hpp"
#include <iostream>
#include <string>
#include <ctime>

int main() {
    // Configure server
    ServerConfig config;
    config.nodePath = "/usr/local/bin/node";
    config.scriptPath = "./scripts/server.js";
    config.port = 9002;
    config.workingDir = ".";
    
    // Optional: Add any needed environment variables
    config.env = {
        "NODE_ENV=development",
        "PORT=9002"
    };

    try {
        // Create client with server management
        WSClient client(config);
        
        std::cout << "Connecting to server..." << std::endl;
        
        // Will automatically start server if needed
        if (client.connect("ws://localhost:9002")) {
            std::cout << "Successfully connected to server" << std::endl;
            
            // Example command parameters
            Json::Value parameters;
            parameters["startTime"] = Json::Int64(std::time(nullptr) - 3600) * 1000; // Last hour
            parameters["endTime"] = Json::Int64(std::time(nullptr)) * 1000;
            
            // Send initial test command
            client.sendCommand("getData", parameters);
            
            // Main application loop
            std::string input;
            while (client.isConnected()) {
                std::cout << "\nCommands available:" << std::endl;
                std::cout << "  send - Send another test message" << std::endl;
                std::cout << "  q    - Quit application" << std::endl;
                std::cout << "\nEnter command: ";
                
                std::getline(std::cin, input);
                
                if (input == "q") {
                    std::cout << "Shutting down..." << std::endl;
                    break;
                } else if (input == "send") {
                    // Update timestamps for fresh data
                    parameters["startTime"] = Json::Int64(std::time(nullptr) - 3600) * 1000;
                    parameters["endTime"] = Json::Int64(std::time(nullptr)) * 1000;
                    client.sendCommand("getData", parameters);
                } else {
                    std::cout << "Unknown command: " << input << std::endl;
                }
            }
        } else {
            std::cerr << "Failed to connect to server" << std::endl;
            return 1;
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}