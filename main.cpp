#include "core/MetricCollector.hpp"
#include "metrics/Metric_CPU.hpp"
#include "metrics/Metric_Memory.hpp"
#include "metrics/Metric_AdEngagement.hpp"
#include "metrics/Metric_HTTP_Requests.hpp"
#include "core/MetricLogger.hpp"
#include <thread>
#include <chrono>
#include <iostream>
#include <random>

int main() {
    try {
        std::cout << "Running metrics collection...\n";
        auto collector = std::make_unique<MetricCollector>();
        auto logger = std::make_unique<MetricLogger>(*collector, "metrics.log");

        auto cpu_metric = std::make_shared<CPUMetric>();
        auto memory_metric = std::make_shared<MemoryUsageMetric>();
        auto ad_engagement_metric = std::make_shared<AdImpressionsMetric>();
        auto http_metric = std::make_shared<HTTPRequestsMetric>();

        collector->CollectMetric(cpu_metric);
        collector->CollectMetric(memory_metric);
        collector->CollectMetric(ad_engagement_metric);
        collector->CollectMetric(http_metric);

        // ген рандом чисел
        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::normal_distribution<> cpu_dist(70.0, 5.0);  // среднее 70%, отклонение 5%
        std::normal_distribution<> memory_dist(1500.0, 200.0);  // среднее 1.5GB, отклонение 200MB
        std::poisson_distribution<> ad_dist(100);  // среднее 100 показов в секунду
        std::normal_distribution<> http_dist(150.0, 30.0);  // среднее 150 запросов, отклонение 30

        for (int i = 0; i < 10; ++i) {
            double cpu_value = std::max(0.0, std::min(100.0, cpu_dist(gen)));
            double memory_value = std::max(0.0, memory_dist(gen));
            double ad_value = ad_dist(gen);  // Пуассоновское распределение для количества показов
            double http_value = std::max(0.0, http_dist(gen));

            cpu_metric->update(cpu_value);
            memory_metric->update(memory_value);
            ad_engagement_metric->update(ad_value);
            http_metric->update(http_value);

            // Ждем 1 секунду
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Собираем финальные значении
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
