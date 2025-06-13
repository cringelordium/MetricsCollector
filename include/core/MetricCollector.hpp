#pragma once
#include "metrics/Metric.hpp"
#include <vector>
#include <memory>
#include <mutex>

class MetricCollector {
public:
    void CollectMetric(std::shared_ptr<Metric> metric);
    std::vector<std::shared_ptr<Metric>> GetMetricsSnapshot();

private:
    std::vector<std::shared_ptr<Metric>> metrics_;
    std::mutex mtx_;
};
