#pragma once
#include "json.hpp"
#include <atomic>
#include <functional>
#include <thread>

using json = nlohmann::json;

class NetworkReceiver{
public:
    NetworkReceiver(int port = 8888);
    ~NetworkReceiver();

    void Start();
    void Stop();
    void SetOnDataReceived(std::function<void(const json&)> callback);
    void SendCurrentState(const json& state);

private:
    void ServerThread();
    int port;
    int serverSocket = -1;
    int clientSocket = -1;
    std::thread thread;
    std::atomic<bool> running{false};
    std::function<void(const json&)> onDataReceived;


};
