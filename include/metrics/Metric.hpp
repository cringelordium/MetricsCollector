#pragma once
#include <string>

class Metric { // фиксануть обнуление
public:
    virtual ~Metric() = default;
    virtual std::string metric_name() const = 0;
    virtual std::string get_and_reset() = 0;
    virtual void update(double value) = 0;
};
