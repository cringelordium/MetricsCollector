#include "metrics/Metric_Memory.hpp"

void MemoryMetric::update(double value) {
    current_value = value;
}

std::string MemoryMetric::get_and_reset() {
    std::string result = std::to_string(current_value);
    current_value = 0.0;
    return result;
}

double MemoryMetric::getCurrentValue() const {
    return current_value;
} 