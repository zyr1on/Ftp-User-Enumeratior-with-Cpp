#pragma once

#include "FTPClient.h"
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <set>

class Manager {
public:
    Manager(const std::string& ip, int port, int threadCount, const std::vector<std::string>& users, bool exitWhenFound);

    void run();

    const std::set<std::string>& getFoundUsers() const;

private:
    void threadWorker();

    std::string ip_;
    int port_;
    int threadCount_;
    const std::vector<std::string>& users_;

    std::atomic<int> index_;
    std::atomic<bool> stopFlag_;
    bool exitWhenFound_;

    std::mutex foundMutex_;
    std::set<std::string> foundUsers_;

    std::mutex coutMutex_;
};
