#include "metrics/Metric_AdEngagement.hpp"

std::string AdImpressionsMetric::metric_name() const {
    return "ad_engagement";
}

AdImpressionsMetric::AdImpressionsMetric() : total_(0) {}

void AdImpressionsMetric::update(double value) {
    total_.fetch_add(static_cast<int>(value), std::memory_order_relaxed);
}

std::string AdImpressionsMetric::get_and_reset() {
    int current = total_.exchange(0);
    return std::to_string(current);
}