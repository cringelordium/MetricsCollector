#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <chrono>
#include "core/MetricCollector.hpp"
#include "metrics/Metric_CPU.hpp"
#include "core/MetricLogger.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>

class ConcurrencyTest : public ::testing::Test {
protected:
    void SetUp() override {
        collector = std::make_unique<MetricCollector>();
        logger = std::make_unique<MetricLogger>(*collector, "concurrency_test.log");
        
        // создадим и добавляем метрику
        cpu_metric = std::make_shared<CPUMetric>();
        collector->CollectMetric(cpu_metric);
    }

    void TearDown() override {
        logger.reset();
        collector.reset();
        std::filesystem::remove("concurrency_test.log");
    }

    std::unique_ptr<MetricCollector> collector;
    std::unique_ptr<MetricLogger> logger;
    std::shared_ptr<CPUMetric> cpu_metric;
};


//

TEST_F(ConcurrencyTest, ConcurrentUpdates) {
    const int num_threads = 10;
    const int updates_per_thread = 100;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, updates_per_thread]() {
            for (int j = 0; j < updates_per_thread; ++j) {
                cpu_metric->update(1.0);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
    }

    // жддем завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    // чекаем, что последнее значение метрики равно 1.0
    // (так как все потоки обновляют до одного и того же значения)
    EXPECT_DOUBLE_EQ(std::stod(cpu_metric->get_and_reset()), 1.0);
}

TEST_F(ConcurrencyTest, NegativeValues) {
    const int num_threads = 5;
    const int updates_per_thread = 50;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, updates_per_thread]() {
            for (int j = 0; j < updates_per_thread; ++j) {
                cpu_metric->update(-1.0);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_DOUBLE_EQ(std::stod(cpu_metric->get_and_reset()), -1.0);
}

TEST_F(ConcurrencyTest, ZeroValues) {
    const int num_threads = 5;
    const int updates_per_thread = 50;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, updates_per_thread]() {
            for (int j = 0; j < updates_per_thread; ++j) {
                cpu_metric->update(0.0);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_DOUBLE_EQ(std::stod(cpu_metric->get_and_reset()), 0.0);
}

TEST_F(ConcurrencyTest, MixedValues) {
    const int num_threads = 4;
    std::vector<std::thread> threads;

    std::vector<double> values = {1.0, -1.0, 0.5, -0.5};
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, value = values[i]]() {
            for (int j = 0; j < 50; ++j) {
                cpu_metric->update(value);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    double result = std::stod(cpu_metric->get_and_reset());
    bool is_valid = false;
    for (double value : values) {
        if (std::abs(result - value) < 1e-6) {
            is_valid = true;
            break;
        }
    }
    EXPECT_TRUE(is_valid);
}

TEST_F(ConcurrencyTest, DifferentUpdateRates) {
    const int num_threads = 3;
    std::vector<std::thread> threads;

    std::vector<int> delays = {1, 5, 10}; // миллисекунды
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, delay = delays[i]]() {
            for (int j = 0; j < 50; ++j) {
                cpu_metric->update(1.0);
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_DOUBLE_EQ(std::stod(cpu_metric->get_and_reset()), 1.0);
}
