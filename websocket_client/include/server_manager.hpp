#pragma once

#include <string>
#include <vector>
#include <atomic>
#include <memory>
#include <spawn.h>
#include <sys/wait.h>

struct ServerConfig {
    std::string nodePath;
    std::string scriptPath;
    int port;
    std::string workingDir;
    std::vector<std::string> env;
};

struct ServerStatus {
    bool isRunning;
    pid_t pid;
    std::string error;
};

enum class ServerError {
    NONE = 0,
    NODE_NOT_FOUND = 100,
    SCRIPT_NOT_FOUND = 101,
    PORT_IN_USE = 102,
    SPAWN_FAILED = 103,
    TIMEOUT = 104,
    SHUTDOWN_FAILED = 105
};

class ServerManager {
public:
    explicit ServerManager(const ServerConfig& config);
    ~ServerManager();

    bool startServer();
    bool stopServer();
    ServerStatus checkStatus();
    bool isServerResponding();
    bool waitForServerStart(int timeoutSecs);

private:
    ServerConfig config_;
    pid_t serverPid_;
    std::atomic<bool> isRunning_;
    
    bool spawnServerProcess();
    bool sendShutdownSignal();
    void cleanup();
    bool checkPortAvailable() const;
    bool validateNodeInstallation() const;
};