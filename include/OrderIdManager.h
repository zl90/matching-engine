#ifndef ORDERIDMANAGER_H
#define ORDERIDMANAGER_H
#include <cstddef>
#include <unordered_map>
#include <random>

class OrderIdManager {
public:
    explicit OrderIdManager(const std::unordered_map<std::size_t, bool> &existing_order_ids);

    OrderIdManager();

    ~OrderIdManager() = default;


    OrderIdManager(const OrderIdManager &) = delete;

    OrderIdManager &operator=(const OrderIdManager &) = delete;

    OrderIdManager(OrderIdManager &&) = delete;

    OrderIdManager &operator=(OrderIdManager &&) = delete;

    [[nodiscard]] const auto GetNextOrderId() -> std::size_t;

    [[nodiscard]] const auto DoesOrderIdExist(const std::size_t &order_id) const -> bool;

private:
    [[nodiscard]] const auto GetRandomOrderId() -> std::size_t;

    std::mt19937 _mt;
    std::uniform_int_distribution<std::size_t> _dist;

    std::size_t _next_order_id{};
    std::unordered_map<std::size_t, bool> _existing_order_ids;
};

#endif //ORDERIDMANAGER_H
