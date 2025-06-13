#include "metrics/Metric_HTTP_Requests.hpp"

HTTPRequestsMetric::HTTPRequestsMetric() : count_(0) {}

std::string HTTPRequestsMetric::metric_name() const {
    return "HTTP requests RPS";
}

void HTTPRequestsMetric::update(double v) {
    count_.fetch_add(static_cast<int>(v), std::memory_order_relaxed);
}

std::string HTTPRequestsMetric::get_and_reset() {
    int v = count_.exchange(0, std::memory_order_relaxed);
    return std::to_string(v);
}
