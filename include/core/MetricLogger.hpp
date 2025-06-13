#pragma once
#include "core/MetricCollector.hpp"
#include <fstream>
#include <thread>
#include <atomic>

class MetricLogger {
public:
    MetricLogger(MetricCollector& collect, const std::string& filename);
    ~MetricLogger();

private:
    void run();
    void log_once();

private:
    MetricCollector& collect_;
    std::ofstream out_;
    std::atomic<bool> running_;
    std::thread worker_;
};
