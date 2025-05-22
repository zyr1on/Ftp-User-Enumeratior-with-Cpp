#pragma once

#include <string>

class FtpClient {
public:
    FtpClient(const std::string& ip, int port = 21);
    ~FtpClient();

    bool connect();
    void disconnect();
    bool isConnected() const;

    std::string sendUserCommand(const std::string& user);

private:
    int sockfd_;
    std::string ip_;
    int port_;
    bool connected_;

    bool recvResponse(std::string& out);
};
