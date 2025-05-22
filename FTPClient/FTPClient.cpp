#include "FTPClient.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

FtpClient::FtpClient(const std::string& ip, int port)
    : ip_(ip), port_(port), sockfd_(-1), connected_(false) {}

FtpClient::~FtpClient() {
    disconnect();
}

bool FtpClient::connect() {
    if (connected_) return true;

    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0) {
        perror("socket");
        return false;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_);
    if (inet_pton(AF_INET, ip_.c_str(), &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        ::close(sockfd_);
        sockfd_ = -1;
        return false;
    }

    if (::connect(sockfd_, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        ::close(sockfd_);
        sockfd_ = -1;
        return false;
    }

    std::string resp;
    if (!recvResponse(resp)) {
        disconnect();
        return false;
    }

    connected_ = true;
    return true;
}

void FtpClient::disconnect() {
    if (sockfd_ != -1) {
        ::close(sockfd_);
        sockfd_ = -1;
    }
    connected_ = false;
}

bool FtpClient::isConnected() const {
    return connected_;
}

bool FtpClient::recvResponse(std::string& out) {
    char buffer[1024];
    ssize_t len = recv(sockfd_, buffer, sizeof(buffer) - 1, 0);
    if (len <= 0) return false;

    buffer[len] = '\0';
    out = std::string(buffer);
    return true;
}

std::string FtpClient::sendUserCommand(const std::string& user) {
    if (!connected_) return "";

    std::string cmd = "USER " + user + "\r\n";
    ssize_t sent = send(sockfd_, cmd.c_str(), cmd.size(), 0);
    if (sent != (ssize_t)cmd.size()) return "";

    std::string resp;
    if (!recvResponse(resp)) return "";

    return resp;
}
