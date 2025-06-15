#include <benchmark/benchmark.h>
#include "metrics/Metric_CPU.hpp"
#include "core/MetricCollector.hpp"
#include "core/MetricLogger.hpp"
#include <filesystem>

// производительность сбора
static void BM_MetricCollection(benchmark::State& state) {
    MetricCollector collect;
    auto metric = std::make_shared<CPUMetric>();
    
    for (auto _ : state) {
        metric->update(1.0);
        collect.CollectMetric(metric);
    }
}
BENCHMARK(BM_MetricCollection);

// производительность логирования
static void BM_MetricLogging(benchmark::State& state) {
    MetricCollector collect;
    auto metric = std::make_shared<CPUMetric>();
    const std::string filename = "benchmark_output.txt";
    MetricLogger logger(collect, filename);
    
    for (auto _ : state) {
        metric->update(1.0);
        collect.CollectMetric(metric);
    }
    
    std::filesystem::remove(filename);
}
BENCHMARK(BM_MetricLogging);

//производительность обновления метрик
static void BM_MetricUpdate(benchmark::State& state) {
    auto metric = std::make_shared<CPUMetric>();
    
    for (auto _ : state) {
        metric->update(1.0);
    }
}
BENCHMARK(BM_MetricUpdate);

BENCHMARK_MAIN(); 