#ifndef ORDERIDMANAGER_H
#define ORDERIDMANAGER_H
#include <cstddef>
#include <unordered_map>

class OrderIdManager {
public:
    explicit OrderIdManager(std::unordered_map<std::size_t, bool> &&existing_order_ids);

    ~OrderIdManager() = default;

    OrderIdManager() = delete;

    OrderIdManager(const OrderIdManager &) = delete;

    OrderIdManager &operator=(const OrderIdManager &) = delete;

    OrderIdManager(OrderIdManager &&) = delete;

    OrderIdManager &operator=(OrderIdManager &&) = delete;

    [[nodiscard]] const auto GetNextOrderId() -> std::size_t;

    [[nodiscard]] const auto DoesOrderIdExist(const std::size_t &order_id) const -> bool;

private:
    std::size_t _next_order_id;
    std::unordered_map<std::size_t, bool> _existing_order_ids;
};

#endif //ORDERIDMANAGER_H
