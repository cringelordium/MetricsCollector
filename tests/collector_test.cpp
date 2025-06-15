#include <gtest/gtest.h>
#include "core/MetricCollector.hpp"
#include "metrics/Metric_CPU.hpp"
#include <cstdint>

TEST(MetricCollectorTest, CollectAndGetMetrics) {
    MetricCollector collect;
    // auto test = std::make_shared<CPUMetric>();
    // const int n = 1e6;
    auto m1 = std::make_shared<CPUMetric>();
    collect.CollectMetric(m1);

    auto list = collect.GetMetricsSnapshot();
    ASSERT_EQ(list.size(), 1);
    EXPECT_EQ(list[0]->metric_name(), "CPU");
}
