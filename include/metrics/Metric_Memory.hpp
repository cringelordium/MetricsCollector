#pragma once
#include "IMetric.hpp"
#include <string>
#include <atomic>

class MemoryMetric : public IMetric {
public:
    MemoryMetric() = default;
    ~MemoryMetric() override = default;

    void update(double value) override;
    std::string get_and_reset() override;
    double getCurrentValue() const;

private:
    std::atomic<double> current_value{0.0};
}; 