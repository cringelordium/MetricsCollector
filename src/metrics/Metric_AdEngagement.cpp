#include "metrics/Metric_AdEngagement.hpp"

void AdEngagementMetric::update(double value) {
    current_value = value;
}

std::string AdEngagementMetric::get_and_reset() {
    std::string result = std::to_string(current_value);
    current_value = 0.0;
    return result;
}

double AdEngagementMetric::getCurrentValue() const {
    return current_value;
} 