#ifndef ORDERBOOK_H
#define ORDERBOOK_H
#include "Level.h"
#include "Trade.h"

class OrderBook {
public:
    OrderBook(const std::vector<Level> &bids, const std::vector<Level> &asks) : _bids(bids),
        _asks(asks) {
    }

    OrderBook(): _bids({}), _asks({}) {
    };

    OrderBook(const OrderBook &order_book) = delete;

    OrderBook(OrderBook &&order_book) = delete;

    OrderBook &operator=(const OrderBook &order_book) = delete;

    OrderBook &operator=(OrderBook &&order_book) = delete;

    ~OrderBook() = default;

    auto SubmitOrder(const Order &order) -> std::vector<Trade>;

    /** @desc Used for exporting the book for recovery broadcast and storing after shutdown */
    [[nodiscard]] const auto GetBook() const -> std::pair<std::vector<Level>, std::vector<Level> >;

    /** @desc Used for testing purposes */
    [[nodiscard]] const auto GetBestBid() const -> uint32_t;

    /** @desc Used for testing purposes */
    [[nodiscard]] const auto GetBestAsk() const -> uint32_t;

private:
    std::vector<Level> _bids;
    std::vector<Level> _asks;
};

#endif //ORDERBOOK_H
