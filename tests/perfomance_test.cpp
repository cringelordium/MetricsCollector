#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <filesystem>
#include "metrics/Metric_CPU.hpp"
#include "core/MetricCollector.hpp"
#include "core/MetricLogger.hpp"

TEST(PerformanceTest, BasicLogging) {
    MetricCollector collect;
    auto metric = std::make_shared<CPUMetric>();
    collect.CollectMetric(metric);

    const std::string filename = "test_perfor_output.txt";
    MetricLogger logger(collect, filename);

    for (int i = 0; i < 10; ++i) {
        metric->update(1.0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // ждем, чтобы логгер успел записать данные
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Проверяем, что файл создан и содержит данные
    std::ifstream file(filename);
    EXPECT_TRUE(file.is_open()) << "Log file was not created";

    std::string line;
    int line_count = 0;
    while (std::getline(file, line)) {
        line_count++;
        // проверяем
        EXPECT_TRUE(line.find("CPU") != std::string::npos) 
            << "Log line doesn't contain metric name: " << line;
    }

    EXPECT_GT(line_count, 0) << "No data was written to the log file";

    // Очищаем
    std::filesystem::remove(filename);
}

