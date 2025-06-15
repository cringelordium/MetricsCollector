#pragma once
#include "Metric.hpp"
#include <string>
#include <atomic>

class AdEngagementMetric : public IMetric {
public:
    AdEngagementMetric() = default;
    ~AdEngagementMetric() override = default;

    void update(double value) override;
    std::string get_and_reset() override;
    double getCurrentValue() const;

private:
    std::atomic<double> current_value{0.0};
}; 