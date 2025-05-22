#include "Manager.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    std::time_t start = std::time(nullptr);
    std::cout << "Started at: " << std::ctime(&start);

    std::string ip, filename;
    int port = 21;
    int threads = 4;
    bool exitWhenFound = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--ip" && i + 1 < argc) ip = argv[++i];
        else if (arg == "--file" && i + 1 < argc) filename = argv[++i];
        else if (arg == "--port" && i + 1 < argc) port = std::atoi(argv[++i]);
        else if (arg == "--thread" && i + 1 < argc) threads = std::atoi(argv[++i]);
        else if (arg == "--exitWhenFound" || arg == "--exitwhenfound" || arg == "-exitWhenFound" || arg == "-exitwhenfound") exitWhenFound = true;
    }

    if (ip.empty() || filename.empty()) {
        std::cerr << "Usage:\n ./ftp_user_enum --ip <ip> --file <file> [--port <port>] [--thread <N>] [--exitWhenFound]\n\ndefault port: 21 | default thread:4\n";
        return 1;
    }

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "[-] File cannot opened: " << filename << "\n";
        return 1;
    }
    std::cout << "TARGET: " << ip <<  " PORT: " << port  << " FILE: " << filename << " THREAD: " << threads << "\n" <<  std::endl;


    std::vector<std::string> users;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty())
            users.push_back(line);
    }

    Manager manager(ip, port, threads, users, exitWhenFound);
    manager.run();

    const auto& foundUsers = manager.getFoundUsers();
    if (!foundUsers.empty()) {
        std::cout << "\nVALID USERS:\n";
        for (const auto& u : foundUsers) {
            std::cout << " - " << u << "\n";
        }
    }
    else {
        std::cout << "\nNO VALID USER.\n";
    }

    std::time_t end = std::time(nullptr);
    std::cout << "\nFinished at: " << std::ctime(&end);

    double duration = std::difftime(end, start);
    std::cout << "Duration: " << duration << " seconds\n";

    return 0;
}
