#include "Manager.hpp"
#include <iostream>
#include <chrono>
#include <thread>

Manager::Manager(const std::string& ip, int port, int threadCount, const std::vector<std::string>& users, bool exitWhenFound)
    : ip_(ip), port_(port), threadCount_(threadCount), users_(users), index_(0), stopFlag_(false), exitWhenFound_(exitWhenFound) {}

const std::set<std::string>& Manager::getFoundUsers() const {
    return foundUsers_;
}
void Manager::run() {
    std::vector<std::thread> threads;
    for (int i = 0; i < threadCount_; ++i)
        threads.emplace_back(&Manager::threadWorker, this);

    for (auto& t : threads)
        t.join();
}

void Manager::threadWorker() {
    FtpClient client(ip_, port_);
    if (!client.connect()) {
        std::lock_guard<std::mutex> lock(coutMutex_);
        std::cerr << "[-] Thread: Connection error.\n";
        return;
    }

    while (true) {
        if (exitWhenFound_ && stopFlag_.load()) break;
        int i = index_.fetch_add(1);
        if (i >= (int)users_.size()) break;
        const std::string& user = users_[i];

    retry:
        if (!client.isConnected()) {
            if (!client.connect()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                goto retry;
            }
        }
        std::string resp = client.sendUserCommand(user);

        if (resp.empty() ||
            (resp.find("331") == std::string::npos && resp.find("530") == std::string::npos)) {
            client.disconnect();
            goto retry;
        }

        if (resp.find("331") != std::string::npos) {
            {
                std::lock_guard<std::mutex> lock(foundMutex_);
                foundUsers_.insert(user);
            }
            {
                std::lock_guard<std::mutex> lock(coutMutex_);
                std::cout << "\033[1;32m[+] VALID  : " << user << "\033[0m\n";
            }

            if (exitWhenFound_)
                if (!stopFlag_.exchange(true)) {
                    // stopFlag true yapıldı, diğer threadler çıkacak
                }

        }
        else {
            std::lock_guard<std::mutex> lock(coutMutex_);
            std::cout << "[-] INVALID  : " << user << "\n";
        }
    }

    client.disconnect();
}
