#ifndef TRADE_H
#define TRADE_H
#include "Order.h"

enum class TradeType {
    REJECT = 0,
    FILL = 1,
    PARTIAL_FILL = 2,
    ACCEPTED = 3,
};

enum class RejectReason {
    KILLED = 0,
    MARKET_CLOSED = 1,
    OTHER = 2
};

struct Trade {
    const TradeType type;
    const uint32_t user_id;

    std::optional<uint64_t> order_id;
    std::optional<uint32_t> level;
    std::optional<uint32_t> quantity;
    std::optional<Side> side;
    std::optional<RejectReason> reject_reason;

    Trade(const TradeType type, const uint32_t user_id, const std::optional<uint32_t> order_id = std::nullopt,
          const std::optional<uint32_t> level = std::nullopt,
          const std::optional<uint32_t> quantity = std::nullopt, const std::optional<Side> side = std::nullopt,
          const std::optional<RejectReason> reject_reason = std::nullopt) : type(type), user_id(user_id),
                                                                            order_id(order_id), level(level),
                                                                            quantity(quantity),
                                                                            side(side), reject_reason(reject_reason) {
    }
};

#endif //TRADE_H
