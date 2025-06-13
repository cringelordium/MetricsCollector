#include "core/MetricCollector.hpp"

void MetricCollector::CollectMetric(std::shared_ptr<Metric> metric) {
    std::lock_guard lock(mtx_);
    metrics_.push_back(std::move(metric));
}

std::vector<std::shared_ptr<Metric>> MetricCollector::GetMetricsSnapshot() {
    std::lock_guard lock(mtx_);
    return metrics_;
}
