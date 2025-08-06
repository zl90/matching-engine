
#include "../../include/OrderBook.h"

#include <algorithm>
#include <stdexcept>

auto OrderBook::GetBook() const -> std::pair<std::vector<Level>, std::vector<Level> > {
    return std::pair{_bids, _asks};
}

const auto OrderBook::GetBestBid() const -> uint32_t {
    if (_bids.empty()) {
        throw std::runtime_error("Bids is empty");
    }

    return _bids[0].price;
}

const auto OrderBook::GetBestAsk() const -> uint32_t {
    if (_asks.empty()) {
        throw std::runtime_error("Asks is empty");
    }

    return _asks[0].price;
}

auto OrderBook::SubmitOrder(const Order &order) -> std::vector<Trade> {
    switch (order.type) {
        case OrderType::LIMIT: {
            return HandleLimitOrder(order);
        }
        default:
            throw std::runtime_error("Invalid order type");
    }
}

auto OrderBook::InsertRestingLimitOrder(const Order &order) -> void {
    auto [type, user_id, level, quantity, side, order_id] = order;

    if (type != OrderType::LIMIT) {
        throw std::runtime_error("Limit insert failed: only LIMIT orders can be inserted into the book");
    }

    if (side == Side::BUY) {
        if (_asks.empty() || GetBestAsk() > level.value()) {
            auto level_it = std::find_if(_bids.begin(), _bids.end(), [level](const auto &elem) {
                elem.price == level.value();
            });

            const auto level_exists = level_it != _bids.end();

            if (level_exists) {
                // @todo Add ability for users to insert to the Market Maker queue.
                auto &selected_level = *level_it;
                selected_level.participants_queue.emplace_back(type, user_id, level, quantity, side, order_id);
            } else {
                Level new_level{level.value(), {}, {}};
                new_level.participants_queue.emplace_back(type, user_id, level, quantity, side, order_id);

                const auto insert_position = std::lower_bound(
                    _bids.begin(),
                    _bids.end(),
                    new_level,
                    [](const Level &a, const Level &b) {
                        return a.price > b.price; // descending order
                    });

                _bids.insert(insert_position, std::move(new_level));
            }
        } else {
            throw std::runtime_error("Bid insert failed: preventing trade-through!");
        }
    } else {
    }
}

auto OrderBook::HandleLimitOrder(const Order &order) -> std::vector<Trade> {
    auto [type, user_id, level, quantity, side, order_id] = order;
    if (side == Side::BUY) {
        if (_asks.empty() || GetBestAsk() > level) {
            const Trade accept_trade{
                TradeType::ACCEPTED, user_id, std::nullopt, order_id, std::nullopt, level, quantity, side
            };

            // @todo: add an interface for easily inserting resting limit orders.

            return std::vector<Trade>{accept_trade};
        }
    } else {
    }
}






