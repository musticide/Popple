#include "NetworkReceiver.h"
#include "raylib.h"
#include <asm-generic/fcntl.h>
#include <asm-generic/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string>
#include <sys/endian.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

NetworkReceiver::NetworkReceiver(int port)
    : port(port)
{
}

NetworkReceiver::~NetworkReceiver() { Stop(); }

void NetworkReceiver::Start()
{
    if (running)
        return;
    running = true;
    thread = std::thread(&NetworkReceiver::ServerThread, this);
}

void NetworkReceiver::Stop()
{
    running = false;
    if (clientSocket >= 0)
        close(clientSocket);
    if (serverSocket >= 0)
        close(serverSocket);
    if (thread.joinable())
        thread.join();
}

void NetworkReceiver::SetOnDataReceived(std::function<void(const json&)> callback) { onDataReceived = callback; }

void NetworkReceiver::SendCurrentState(const json& state)
{
    if (clientSocket < 0)
        return;
    std::string msg = state.dump() + "\n";
    send(clientSocket, msg.c_str(), msg.size(), 0);
}

void NetworkReceiver::ServerThread()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        TraceLog(LOG_ERROR, "Failed to create socket");
        return;
    }
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&addr, sizeof(addr)) < 0) {
        TraceLog(LOG_ERROR, "Failed to bind socket");
        close(serverSocket);
        return;
    }

    listen(serverSocket, 1);
    TraceLog(LOG_INFO, "Waiting for editor connection on port %d", port);

    while (running) {
        clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0)
            continue;

        // Set non blocking
        fcntl(clientSocket, F_SETFL, O_NONBLOCK);
        TraceLog(LOG_INFO, "Editor Connected!");

        std::string buffer;
        char readBuf[1024];

        while (running) {
            int n = recv(clientSocket, readBuf, sizeof(readBuf) - 1, 0);
            if (n > 0) {
                readBuf[n] = '\0';
                buffer += readBuf;

                size_t pos;
                while ((pos = buffer.find('\n')) != std::string::npos) {
                    std::string line = buffer.substr(0, pos);
                    buffer.erase(0, pos + 1);

                    try {
                        json data = json::parse(line);
                        if (onDataReceived)
                            onDataReceived(data);
                    } catch (...) {
                        TraceLog(LOG_WARNING, "Failed to parse JSON");
                    }
                }
            } else if (n == 0) {
                TraceLog(LOG_INFO, "Editor Disconnected");
                break;
            }
            usleep(16000);
        }
        close(clientSocket);
        clientSocket = -1;
    }
}
