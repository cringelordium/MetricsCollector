#include "Metric.hpp"
#include <string>
#include <atomic>

// fetch_add для std::atomic<double> реализован только в C++20, при этом есть ограничения
// здесь будет использоваться примитив синхронизации на атомиках
// по итогу ВСЕ lock-free!

class MemoryUsageMetric : public Metric {
public:
    MemoryUsageMetric();

    std::string metric_name() const override;
    void update(double value);
    std::string get_and_reset() override;

private:
    std::atomic<double> total_;
    std::atomic<int> count_;
};
