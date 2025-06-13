#pragma once
#include "metrics/Metric.hpp"
#include <atomic>

class CPUMetric : public Metric {
public:
    CPUMetric();
    std::string metric_name() const override;
    void update(double value) override;
    std::string get_and_reset() override;

private:
    std::atomic<double> value_;
};
