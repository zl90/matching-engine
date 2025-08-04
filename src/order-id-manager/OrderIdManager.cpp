
#include "../../include/OrderIdManager.h"

const auto OrderIdManager::DoesOrderIdExist(const std::size_t &order_id) const -> bool {
    return _existing_order_ids.find(order_id) != _existing_order_ids.end();
}

auto OrderIdManager::GetNextOrderId() const -> std::size_t {
    bool is_current_next_order_id_valid = !DoesOrderIdExist(_next_order_id);

    while (!is_current_next_order_id_valid) {
        _next_order_id++;
    }
}

