#include "metrics/Metric_Memory.hpp"        
#include <sstream>

MemoryUsageMetric::MemoryUsageMetric() : total_(0.0), count_(0) {}

std::string MemoryUsageMetric::metric_name() const {
    return "Memory Usage";
}

void MemoryUsageMetric::update(double value) {
    double current;
    do {
        current = total_.load(std::memory_order_relaxed);
    } while (!total_.compare_exchange_weak(current, current + value, std::memory_order_relaxed)); // основная магия 
    count_.fetch_add(1, std::memory_order_relaxed); // count интовый
}

std::string MemoryUsageMetric::get_and_reset() {
    double sum = total_.exchange(0.0, std::memory_order_relaxed);
    int cnt = count_.exchange(0, std::memory_order_relaxed);
    double avg = cnt > 0 ? sum / cnt : 0.0;
    return std::to_string(avg);
}
