# Project Documentation

## Содержание
1. [Основные классы](#основные-классы)
2. [Интерфейсы](#интерфейсы)
3. [Метрики](#метрики)
4. [Логирование](#логирование)

## Основные классы

### MetricCollector

Основной класс для сбора метрик.

```cpp
class MetricCollector;
```

Это центральный класс для сбора метрик
Основные функции:

```CollectMetric(metric)```- добавляет новую метрику в коллекцию

```GetMetricsSnapshot()``` - возвращает копию всех собранных метрик, использует мьютекс для потокобезопасности

Хранит метрики в векторе ```std::vector<std::shared_ptr<Metric>>```

### MetricLogger

Класс для логирования метрик.

```cpp
class MetricLogger;
```

Отвечает за логирование метрик в файл

Конструктор принимает ```MetricCollector``` и имя файла для логов

Запускает __отдельный поток__ для логирования
Логирует метрики каждую секунду

### Особенности:

Использует отдельный поток для асинхронного логирования
Форматирует время с миллисекундами

Записывает все метрики в одну строку с временной меткой
Автоматически останавливается при уничтожении объекта

### Metric

Базовый интерфейс для всех метрик.

```cpp
class Metric {
public:
    virtual void update(double value) = 0;
    virtual double get_and_reset() = 0;
    virtual ~Metric() = default;
};
```

#### Методы

- `update(value)`: Обновляет значение метрики
- `get_and_reset()`: Возвращает текущее значение и сбрасывает его

- Все атомарно

## Метрики

### CPUMetric

Метрика использования CPU.


### MemoryUsageMetric

Метрика использования памяти.


### AdImpressionsMetric

Метрика показов рекламы.


### HTTPRequestsMetric

Метрика HTTP-запросов.

```cpp
// Все методы используют атомарные операции
// Единственное где могла бы возникать проблема это в MemoryUsageMetric 
```

# MemoryUsageMetric

В этой метрике из-за особенностей класса, даже в __C++20__ не гарантируется поддержка  ```fetch_add()``` с  ```std::atomic<double>```поэтому был использован ```compare_exchange_weak()```, с которым я уже был [знаком](https://gitlab.com/hse-cpp/cpp-advanced-hse/-/tree/main/tasks/lock-free/fast-queue?ref_type=heads). 

## Логирование

### Формат

```
[timestamp] METRIC_NAME: value
```

### Примеры

```cpp
// Создание и настройка коллектора
auto collector = std::make_unique<MetricCollector>();
auto logger = std::make_unique<MetricLogger>(*collector, "metrics.log");

// Добавление метрик
auto cpu_metric = std::make_shared<CPUMetric>();
collector->CollectMetric(cpu_metric);

// Обновление метрик
cpu_metric->update(75.5);

// Получение значений
double cpu_usage = cpu_metric->get_and_reset();
```

## Обработка ошибок

Все методы могут выбрасывать следующие исключения:

- `std::runtime_error`: Общие ошибки выполнения
- `std::invalid_argument`: Некорректные аргументы
- `std::system_error`: Системные ошибки

## Потокобезопасность

- Все методы потокобезопасны
- Используются атомарные операции
- Lock-free реализация там где это возможно

## Примеры использования

### Базовое использование

```cpp
int main() {
    auto collector = std::make_unique<MetricCollector>();
    auto logger = std::make_unique<MetricLogger>(*collector, "metrics.log");

    auto cpu_metric = std::make_shared<CPUMetric>();
    collector->CollectMetric(cpu_metric);

    logger->StartLogging();

    // Обновление метрик
    for (int i = 0; i < 10; ++i) {
        cpu_metric->update(75.5);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    logger->StopLogging();
    return 0;
}
```

### Многопоточное использование

```cpp
void update_metrics(std::shared_ptr<CPUMetric> metric) {
    for (int i = 0; i < 1000; ++i) {
        metric->update(75.5);
    }
}

int main() {
    auto collector = std::make_unique<MetricCollector>();
    auto cpu_metric = std::make_shared<CPUMetric>();
    collector->CollectMetric(cpu_metric);

    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(update_metrics, cpu_metric);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
``` 