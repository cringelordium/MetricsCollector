#include "metrics/Metric_CPU.hpp"

CPUMetric::CPUMetric() : value_(0.0) {}

std::string CPUMetric::metric_name() const {
    return "CPU";
}

void CPUMetric::update(double v) {
    value_.store(v, std::memory_order_relaxed);
}

std::string CPUMetric::get_and_reset() {
    double v = value_.exchange(0.0, std::memory_order_relaxed);
    return std::to_string(v);
}
