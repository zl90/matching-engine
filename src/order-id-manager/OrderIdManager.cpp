
#include "../../include/OrderIdManager.h"

OrderIdManager::OrderIdManager(std::unordered_map<std::size_t, bool> &&existing_order_ids) {
    _existing_order_ids = std::move(existing_order_ids);
    _next_order_id = 0;
}

const auto OrderIdManager::DoesOrderIdExist(const std::size_t &order_id) const -> bool {
    return _existing_order_ids.find(order_id) != _existing_order_ids.end();
}

auto OrderIdManager::GetNextOrderId() -> std::size_t {
    bool is_current_next_order_id_valid = !DoesOrderIdExist(_next_order_id);

    while (!is_current_next_order_id_valid) {
        is_current_next_order_id_valid = !DoesOrderIdExist(++_next_order_id);
    }

    _existing_order_ids[_next_order_id] = true;

    return _next_order_id++;
}

