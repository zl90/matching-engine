#include "../../include/OrderBook.h"

#include <algorithm>
#include <stdexcept>
#include <limits>

auto OrderBook::GetBook() const
    -> std::pair<std::unordered_map<PriceType, Level>, std::unordered_map<PriceType, Level> > {
    return {_bids, _asks};
}

const auto OrderBook::GetBestBid() const -> PriceType {
    if (_bids.empty()) {
        throw std::runtime_error("Bids is empty");
    }
    PriceType best_price = std::numeric_limits<PriceType>::min();
    for (const auto &[key, value]: _bids) {
        if (key > best_price) {
            best_price = key;
        }
    }
    return best_price;
}

const auto OrderBook::GetBestAsk() const -> PriceType {
    if (_asks.empty()) {
        throw std::runtime_error("Asks is empty");
    }
    PriceType best_price = std::numeric_limits<PriceType>::max();

    for (const auto &[key, value]: _asks) {
        if (key < best_price) {
            best_price = key;
        }
    }
    return best_price;
}

auto OrderBook::InsertRestingLimitOrder(const Order &order) -> void {
    auto [type, user_id, level, quantity, side, order_id] = order;

    if (type != OrderType::LIMIT) {
        throw std::runtime_error("Limit insert failed: only LIMIT orders can be inserted into the book");
    }

    if (side == Side::BUY) {
        if (_asks.empty() || GetBestAsk() > level.value()) {
            auto it = _bids.find(level.value());

            if (it != _bids.end()) {
                it->second.participants_queue.emplace_back(type, user_id, level, quantity, side, order_id);
            } else {
                Level new_level{level.value(), {}, {}};
                new_level.participants_queue.emplace_back(type, user_id, level, quantity, side, order_id);
                _bids.emplace(level.value(), std::move(new_level));
            }
        } else {
            throw std::runtime_error("Bid insert failed: preventing trade-through!");
        }
    } else if (side == Side::SELL) {
        if (_bids.empty() || GetBestBid() < level.value()) {
            auto it = _asks.find(level.value());

            if (it != _asks.end()) {
                it->second.participants_queue.emplace_back(type, user_id, level, quantity, side, order_id);
            } else {
                Level new_level{level.value(), {}, {}};
                new_level.participants_queue.emplace_back(type, user_id, level, quantity, side, order_id);
                _asks.emplace(level.value(), std::move(new_level));
            }
        } else {
            throw std::runtime_error("Ask insert failed: preventing trade-through!");
        }
    } else {
        throw std::runtime_error("Unknown side in order");
    }
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


auto OrderBook::HandleLimitOrder(const Order &order) -> std::vector<Trade> {
    auto [type, user_id, level, quantity, side, order_id] = order;
    if (side == Side::BUY) {
        std::optional<PriceType> best_ask = !_asks.empty() ? GetBestAsk() : std::nullopt;
        if (_asks.empty() || best_ask.value() > level.value()) {
            const Trade accept_trade{
                TradeType::ACCEPTED, user_id, std::nullopt, order_id, std::nullopt, level, quantity, side
            };
            InsertRestingLimitOrder(order);
            return {accept_trade};
        } else {
            auto &mm_queue = _asks.find(level.value())->second.market_maker_queue;
            auto &pt_queue = _asks.find(level.value())->second.participants_queue;
            const auto &best_offer = mm_queue.size() > 0 ? mm_queue.begin() : pt_queue.begin();
            const bool is_best_offer_in_mm_queue = mm_queue.size() > 0;

            const Trade fill_trade = {
                TradeType::FILL, user_id, best_offer->user_id, order_id, best_offer->order_id, level.value(), quantity,
                side, std::nullopt, FilledSide::MAKER
            };

            if (is_best_offer_in_mm_queue) {
                mm_queue.erase(best_offer);
                mm_queue.erase(std::remove(mm_queue.begin(), mm_queue.end(), best_offer), mm_queue.end());
            } else {
                pt_queue.erase(best_offer);
                pt_queue.erase(std::remove(pt_queue.begin(), pt_queue.end(), best_offer), pt_queue.end());
            }

            return {fill_trade};
        }
    } else if (side == Side::SELL) {
        std::optional<PriceType> best_bid = !_bids.empty() ? GetBestBid() : std::nullopt;

        if (_bids.empty() || best_bid.value() < level.value()) {
            const Trade accept_trade{
                TradeType::ACCEPTED, user_id, std::nullopt, order_id, std::nullopt, level, quantity, side
            };
            InsertRestingLimitOrder(order);
            return {accept_trade};
        } else {
            auto &mm_queue = _bids.find(level.value())->second.market_maker_queue;
            auto &pt_queue = _bids.find(level.value())->second.participants_queue;
            const auto &best_bid = mm_queue.size() > 0 ? mm_queue.begin() : pt_queue.begin();
            const bool is_best_bid_in_mm_queue = mm_queue.size() > 0;

            const Trade fill_trade = {
                TradeType::FILL, user_id, best_bid->user_id, order_id, best_bid->order_id, level.value(), quantity,
                side, std::nullopt, FilledSide::MAKER
            };

            if (is_best_bid_in_mm_queue) {
                mm_queue.erase(best_bid);
                mm_queue.erase(std::remove(mm_queue.begin(), mm_queue.end(), best_bid), mm_queue.end());
            } else {
                pt_queue.erase(best_bid);
                pt_queue.erase(std::remove(pt_queue.begin(), pt_queue.end(), best_bid), pt_queue.end());
            }

            return {fill_trade};
        }
    } else {
        throw std::runtime_error("Unknown side in HandleLimitOrder");
    }
}
