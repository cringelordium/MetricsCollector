#include "core/MetricLogger.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

MetricLogger::MetricLogger(MetricCollector& c, const std::string& filename)
    : collect_(c), out_(filename), running_(true) {
    worker_ = std::thread([this] { run(); });
}

MetricLogger::~MetricLogger() {
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
}

void MetricLogger::run() {
    using namespace std::chrono_literals;
    while (running_) {
        log_once();
        std::this_thread::sleep_for(1s);
    }
}

void MetricLogger::log_once() {
    auto now = std::chrono::system_clock::now();
    auto in_time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&in_time), "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    for (auto& metric : collect_.GetMetricsSnapshot()) {
        oss << " \"" << metric->metric_name() << "\" " << metric->get_and_reset();
    }

    out_ << oss.str() << "\n";
    out_.flush();
}
