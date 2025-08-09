#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "Level.h"
#include "Trade.h"
#include <unordered_map>
#include <cstdint>

class OrderBook {
public:
    using PriceType = uint32_t;

    OrderBook(const std::unordered_map<PriceType, Level> &bids,
              const std::unordered_map<PriceType, Level> &asks)
        : _bids(bids), _asks(asks) {
    }

    OrderBook() : _bids{}, _asks{} {
    }

    OrderBook(const OrderBook &order_book) = delete;

    OrderBook(OrderBook &&order_book) = delete;

    OrderBook &operator=(const OrderBook &order_book) = delete;

    OrderBook &operator=(OrderBook &&order_book) = delete;

    ~OrderBook() = default;

    auto SubmitOrder(const Order &order) -> std::vector<Trade>;

    /** @desc Used for exporting the book for recovery broadcast and storing after shutdown */
    [[nodiscard]] const auto GetBook() const
        -> std::pair<std::unordered_map<PriceType, Level>,
            std::unordered_map<PriceType, Level> >;

    /** @desc Used for testing purposes */
    [[nodiscard]] const auto GetBestBid() const -> PriceType;

    /** @desc Used for testing purposes */
    [[nodiscard]] const auto GetBestAsk() const -> PriceType;

private:
    std::unordered_map<PriceType, Level> _bids;
    std::unordered_map<PriceType, Level> _asks;

    auto InsertRestingLimitOrder(const Order &order) -> void;

    auto HandleLimitOrder(const Order &order) -> std::vector<Trade>;
};

#endif // ORDERBOOK_H
