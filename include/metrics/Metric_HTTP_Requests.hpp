#pragma once
#include "metrics/Metric.hpp"
#include <atomic>

class HTTPRequestsMetric : public Metric {
public:
    HTTPRequestsMetric();
    std::string metric_name() const override;
    void update(double value) override;
    std::string get_and_reset() override;

private:
    std::atomic<int> count_;
};
