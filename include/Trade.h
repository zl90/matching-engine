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

enum class FilledSide {
    TAKER = 0,
    MAKER = 1
};

struct Trade {
    const TradeType type;
    const uint32_t taker_user_id;

    std::optional<uint32_t> maker_user_id;
    std::optional<std::size_t> taker_order_id;
    std::optional<std::size_t> maker_order_id;
    std::optional<uint32_t> level;
    std::optional<uint32_t> quantity;
    std::optional<Side> side;
    std::optional<RejectReason> reject_reason;
    std::optional<FilledSide> filled_side;

    Trade(const TradeType type, const uint32_t taker_user_id,
          const std::optional<uint32_t> maker_user_id = std::nullopt,
          const std::optional<std::size_t> taker_order_id = std::nullopt,
          const std::optional<std::size_t> maker_order_id = std::nullopt,
          const std::optional<uint32_t> level = std::nullopt,
          const std::optional<uint32_t> quantity = std::nullopt, const std::optional<Side> side = std::nullopt,
          const std::optional<RejectReason> reject_reason = std::nullopt,
          const std::optional<FilledSide> filled_side = std::nullopt) : type(type), taker_user_id(taker_user_id),
                                                                        maker_user_id(taker_user_id),
                                                                        taker_order_id(taker_order_id),
                                                                        maker_order_id(maker_order_id),
                                                                        level(level),
                                                                        quantity(quantity),
                                                                        side(side), reject_reason(reject_reason),
                                                                        filled_side(filled_side) {
    }
};

#endif //TRADE_H
