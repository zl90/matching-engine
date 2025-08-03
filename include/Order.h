#ifndef ORDER_H
#define ORDER_H
#include <cstdint>
#include <optional>

enum class OrderType {
    LIMIT = 0,
    MARKET = 1,
    FILL_OR_KILL = 2,
    CANCEL = 3
};

enum class Side {
    BUY = 0,
    SELL = 1
};

struct Order {
    const OrderType type;
    const uint32_t user_id;

    std::optional<uint32_t> level;
    std::optional<uint32_t> quantity;
    std::optional<Side> side;
    std::optional<std::size_t> order_id;

    Order(const OrderType type, const uint32_t user_id, const std::optional<uint32_t> level = std::nullopt,
          const std::optional<uint32_t> quantity = std::nullopt, const std::optional<Side> side = std::nullopt,
          const std::optional<std::size_t> order_id = std::nullopt) : type(type), user_id(user_id), level(level),
                                                                      quantity(quantity), side(side),
                                                                      order_id(order_id) {
    }
};

#endif //ORDER_H
