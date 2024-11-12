#include "server_manager.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>    // For kill() and signal constants (SIGTERM, SIGKILL)
#include <filesystem>
#include <chrono>
#include <thread>
#include <sstream>
#include <iostream>

ServerManager::ServerManager(const ServerConfig& config)
    : config_(config), serverPid_(-1), isRunning_(false) {}

ServerManager::~ServerManager() {
    if (isRunning_) {
        stopServer();
    }
    cleanup();
}

bool ServerManager::startServer() {
    if (isRunning_) {
        return true;
    }

    if (!validateNodeInstallation()) {
        return false;
    }

    if (!checkPortAvailable()) {
        return false;
    }

    return spawnServerProcess();
}

bool ServerManager::spawnServerProcess() {
    posix_spawn_file_actions_t actions;
    posix_spawnattr_t attr;
    
    posix_spawn_file_actions_init(&actions);
    posix_spawnattr_init(&attr);

    // Redirect stdout and stderr to log files
    int stdout_fd = open((config_.workingDir + "/server_out.log").c_str(), 
                        O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int stderr_fd = open((config_.workingDir + "/server_err.log").c_str(), 
                        O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    posix_spawn_file_actions_adddup2(&actions, stdout_fd, STDOUT_FILENO);
    posix_spawn_file_actions_adddup2(&actions, stderr_fd, STDERR_FILENO);

    // Prepare arguments
    std::vector<char*> args;
    args.push_back(const_cast<char*>(config_.nodePath.c_str()));
    args.push_back(const_cast<char*>(config_.scriptPath.c_str()));
    args.push_back(nullptr);

    // Prepare environment
    std::vector<char*> env;
    for (const auto& envVar : config_.env) {
        env.push_back(const_cast<char*>(envVar.c_str()));
    }
    env.push_back(nullptr);

    // Spawn process
    int result = posix_spawn(&serverPid_, config_.nodePath.c_str(),
                            &actions, &attr,
                            args.data(), env.data());

    // Cleanup
    posix_spawn_file_actions_destroy(&actions);
    posix_spawnattr_destroy(&attr);
    close(stdout_fd);
    close(stderr_fd);

    if (result == 0) {
        isRunning_ = true;
        return waitForServerStart(10); // Wait up to 10 seconds
    }

    return false;
}

bool ServerManager::stopServer() {
    if (!isRunning_) {
        return true;
    }

    // Try graceful shutdown first
    if (sendShutdownSignal()) {
        // Wait for up to 5 seconds
        for (int i = 0; i < 50; ++i) {
            if (kill(serverPid_, 0) != 0) {
                isRunning_ = false;
                serverPid_ = -1;
                return true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    // Force termination if necessary
    kill(serverPid_, SIGTERM);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    if (kill(serverPid_, 0) == 0) {
        kill(serverPid_, SIGKILL);
    }

    isRunning_ = false;
    serverPid_ = -1;
    return true;
}

ServerStatus ServerManager::checkStatus() {
    ServerStatus status;
    status.isRunning = isRunning_;
    status.pid = serverPid_;
    
    if (isRunning_) {
        if (kill(serverPid_, 0) != 0) {
            status.isRunning = false;
            status.error = "Process not responding";
        } else if (!isServerResponding()) {
            status.error = "Server not responding";
        }
    }
    
    return status;
}

bool ServerManager::isServerResponding() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return false;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(config_.port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    bool result = connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0;
    close(sock);
    return result;
}

bool ServerManager::waitForServerStart(int timeoutSecs) {
    for (int i = 0; i < timeoutSecs * 10; ++i) {
        if (isServerResponding()) {
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return false;
}

void ServerManager::cleanup() {
    if (serverPid_ != -1) {
        kill(serverPid_, SIGKILL);
        serverPid_ = -1;
    }
    isRunning_ = false;
}

bool ServerManager::checkPortAvailable() const {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return false;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(config_.port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int result = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    close(sock);
    return result == 0;
}

bool ServerManager::validateNodeInstallation() const {
    return std::filesystem::exists(config_.nodePath) &&
           std::filesystem::exists(config_.scriptPath);
}

bool ServerManager::sendShutdownSignal() {
    // Implementation depends on your server's shutdown mechanism
    // This is a placeholder that always returns true
    return true;
}