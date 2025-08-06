#include <gtest/gtest.h>
#include "../include/OrderBook.h"
#include "../include/OrderIdManager.h"

TEST(OrderBook, GetBestBidAskReturnsCorrectValue) {
    const Level l_10000(10000, {}, {Order(OrderType::LIMIT, 203498)});
    const Level l_10025(10025, {}, {Order(OrderType::LIMIT, 203498)});
    const Level l_10100(10100, {Order(OrderType::LIMIT, 908345)}, {});

    const std::vector<Level> bids = {l_10025, l_10000};
    const std::vector<Level> asks = {l_10100};

    const OrderBook ob(bids, asks);

    EXPECT_EQ(ob.GetBestBid(), 10025);
    EXPECT_EQ(ob.GetBestAsk(), 10100);
}

TEST(OrderBook, GetBestBidAskThrowsWhenEmpty) {
    const std::vector<Level> bids = {};
    const std::vector<Level> asks = {};

    const OrderBook ob(bids, asks);

    EXPECT_THROW([&]{auto bb = ob.GetBestBid();}(), std::runtime_error);
    EXPECT_THROW([&]{auto ba = ob.GetBestAsk();}(), std::runtime_error);
}
