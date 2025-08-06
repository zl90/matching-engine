#include "../../include/OrderIdManager.h"

#include <map>

OrderIdManager::OrderIdManager(): _mt(std::random_device{}()), _next_order_id(GetRandomOrderId()),
                                  _existing_order_ids({}) {
};

OrderIdManager::OrderIdManager(const std::unordered_map<std::size_t, bool> &existing_order_ids) : _mt(
        std::random_device{}()),
    _next_order_id(GetRandomOrderId()), _existing_order_ids(existing_order_ids) {
}

const auto OrderIdManager::DoesOrderIdExist(const std::size_t &order_id) const -> bool {
    return _existing_order_ids.find(order_id) != _existing_order_ids.end();
}

auto OrderIdManager::GetNextOrderId() -> std::size_t {
    bool is_current_next_order_id_valid = !DoesOrderIdExist(_next_order_id);

    while (!is_current_next_order_id_valid) {
        _next_order_id = GetRandomOrderId();
        is_current_next_order_id_valid = !DoesOrderIdExist(_next_order_id);
    }

    _existing_order_ids[_next_order_id] = true;

    return _next_order_id;
}

const auto OrderIdManager::GetRandomOrderId() -> std::size_t {
    return _dist(_mt);
}


