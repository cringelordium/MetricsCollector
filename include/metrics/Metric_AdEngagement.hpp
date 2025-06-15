#include "Metric.hpp"
#include <string>
#include <atomic>

class AdImpressionsMetric : public Metric {
public:
    AdImpressionsMetric();

    std::string metric_name() const override;
    void update(double value) override;
    std::string get_and_reset() override;

private:
    std::atomic<int> total_;
};