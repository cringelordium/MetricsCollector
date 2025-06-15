# Project Documentation

## Содержание
1. [Основные классы](#основные-классы)
2. [Интерфейсы](#интерфейсы)
3. [Метрики](#метрики)
4. [Логирование](#логирование)

## Основные классы

### MetricCollector

Класс регистрации метрик.

```cpp
class MetricCollector;
```

Это центральный класс для регистрации метрик.

__На самом деле класс регистрирует метрики а не собирает их, изначально класс задымывался для сбора метрик (нейминг так себе)__

Основные функции:

```CollectMetric(metric)```- регает новую метрику

```GetMetricsSnapshot()``` - возвращает копию всех (зарегестрированных) собранных метрик, использует мьютекс для потокобезопасности


Хранит их в векторе умных указателей на объкт метрики: ```std::vector<std::shared_ptr<Metric>>```

### MetricLogger

Класс для логирования метрик.

```cpp
class MetricLogger;
```

```
MetricCollector& collect_;    // Ссылка на регистратор метрик
std::ofstream out_;           // Файловый поток для записи
std::atomic<bool> running_;   // Флаг работы логгера
std::thread worker_;          // Рабочий поток

// Основной цикл работы логгера
void run();

// Одна итерация логирования
void log_once();
```


Конструктор принимает ```MetricCollector``` и имя файла для логов

Запускает __отдельный поток__ для логирования и логирует метрики каждую секунду

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
