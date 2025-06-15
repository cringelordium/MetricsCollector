#include "core/MetricCollector.hpp"
#include "metrics/Metric_CPU.hpp"
#include "metrics/Metric_Memory.hpp"
#include "metrics/Metric_AdEngagement.hpp"
#include "metrics/Metric_HTTP_Requests.hpp"
#include "core/MetricLogger.hpp"
#include <thread>
#include <chrono>
#include <iostream>

int main() {
    try {
        std::cout << "Running metrics collection...\n";
        auto collector = std::make_unique<MetricCollector>();
        auto logger = std::make_unique<MetricLogger>(*collector, "metrics.log");

        auto cpu_metric = std::make_shared<CPUMetric>();
        auto memory_metric = std::make_shared<MemoryMetric>();
        auto ad_engagement_metric = std::make_shared<AdEngagementMetric>();
        auto http_metric = std::make_shared<HTTPRequestsMetric>();

        collector->CollectMetric(cpu_metric);
        collector->CollectMetric(memory_metric);
        collector->CollectMetric(ad_engagement_metric);
        collector->CollectMetric(http_metric);

        for (int i = 0; i < 10; ++i) {
            cpu_metric->update(75.5 + i);
            memory_metric->update(1024.0 + i * 100);
            ad_engagement_metric->update(0.15 + (i * 0.01));
            http_metric->update(100 + i * 10);

            // Ждем 1 секунду
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Собираем финальные значения
        cpu_metric->get_and_reset();
        memory_metric->get_and_reset();
        ad_engagement_metric->get_and_reset();
        http_metric->get_and_reset();

        std::cout << "Metrics collection completed successfully! :)\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
