
#include "../../include/OrderBook.h"

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


