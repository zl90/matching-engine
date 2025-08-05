#ifndef ORDERBOOK_H
#define ORDERBOOK_H
#include "Level.h"
#include "Trade.h"

class OrderBook {
public:
    OrderBook(std::vector<Level> &&bids, std::vector<Level> &&asks) {
    };

    OrderBook() = delete;

    OrderBook(const OrderBook &order_book) = delete;

    OrderBook(OrderBook &&order_book) = delete;

    OrderBook &operator=(const OrderBook &order_book) = delete;

    OrderBook &operator=(OrderBook &&order_book) = delete;

    ~OrderBook() = default;

    auto SubmitOrder(const Order &order) -> std::vector<Trade>;

    [[nodiscard]] const auto GetBook() const -> std::pair<std::vector<Trade>, std::vector<Trade> >;

    [[nodiscard]] const auto GetBestBid() const -> uint32_t;

    [[nodiscard]] const auto GetBestAsk() const -> uint32_t;

private:
    std::vector<Level> _bids;
    std::vector<Level> _asks;
};


#endif //ORDERBOOK_H
