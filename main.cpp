#include "metrics/Metric_CPU.hpp"
#include "metrics/Metric_HTTP_Requests.hpp"
#include "core/MetricLogger.hpp"
#include "core/MetricCollector.hpp"

#include <thread>
#include <random>

int main() {
    MetricCollector collect;

    auto cpu_metric = std::make_shared<CPUMetric>();
    auto http_requests_metric = std::make_shared<HTTPRequestsMetric>();

    collect.CollectMetric(cpu_metric);
    collect.CollectMetric(http_requests_metric);

    MetricLogger logger(collect, "metrics.txt");

    std::default_random_engine eng;
    std::uniform_real_distribution<double> cpuLoad(0.0, 4.0);
    std::uniform_int_distribution<int> httpRps(0, 100);

    for (int i = 0; i < 50; ++i) {
        cpu_metric->update(cpuLoad(eng));
        http_requests_metric->update(httpRps(eng));
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 0;
}
