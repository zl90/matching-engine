#include <gtest/gtest.h>
#include "../include/OrderBook.h"
#include "../include/OrderIdManager.h"

TEST(OrderBook, GetBestBidAskReturnsCorrectValue) {
    const Level l_10000(10000, {}, {Order(OrderType::LIMIT, 203498)});
    const Level l_10025(10025, {}, {Order(OrderType::LIMIT, 203498)});
    const Level l_10100(10100, {Order(OrderType::LIMIT, 908345)}, {});

    const std::unordered_map<uint32_t, Level> bids = {
        {10025, l_10025},
        {10000, l_10000}
    };
    const std::unordered_map<uint32_t, Level> asks = {
        {10100, l_10100}
    };

    const OrderBook ob(bids, asks);

    EXPECT_EQ(ob.GetBestBid(), 10025);
    EXPECT_EQ(ob.GetBestAsk(), 10100);
}

TEST(OrderBook, GetBestBidAskThrowsWhenEmpty) {
    const std::unordered_map<uint32_t, Level> bids;
    const std::unordered_map<uint32_t, Level> asks;

    const OrderBook ob(bids, asks);

    EXPECT_THROW([&]{auto bb = ob.GetBestBid();}(), std::runtime_error);
    EXPECT_THROW([&]{auto ba = ob.GetBestAsk();}(), std::runtime_error);
}

TEST(OrderBook, LimitOrderAcceptsInEmptyBook) {
    OrderBook ob;
    const Order o1(OrderType::LIMIT, 1, 10025, 10, Side::BUY, 1);

    const auto trades = ob.SubmitOrder(o1);

    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(ob.GetBestBid(), 10025);
    ASSERT_EQ(ob.GetBook().first.size(), 1);
    EXPECT_EQ(ob.GetBook().first.at(10025).price, 10025);
    EXPECT_EQ(trades[0].type, TradeType::ACCEPTED);
    EXPECT_EQ(trades[0].quantity, 10);
    EXPECT_EQ(trades[0].level, 10025);
    EXPECT_EQ(trades[0].side, Side::BUY);
    EXPECT_EQ(trades[0].taker_user_id, 1);
    EXPECT_EQ(trades[0].taker_order_id, 1);
    EXPECT_EQ(trades[0].maker_user_id, std::nullopt);
    EXPECT_EQ(trades[0].maker_order_id, std::nullopt);
}

TEST(OrderBook, LimitOrderFillsRestingOrder) {
    const Level l_10000(10000, {}, {Order(OrderType::LIMIT, 1, 10000, 15, Side::SELL, 1)});
    const std::unordered_map<uint32_t, Level> asks = {
        {10000, l_10000}
    };
    const std::unordered_map<uint32_t, Level> bids;

    OrderBook ob(bids, asks);

    ASSERT_EQ(ob.GetBestAsk(), 10000);

    const Order o1(OrderType::LIMIT, 2, 10000, 15, Side::BUY, 2);
    const auto trades = ob.SubmitOrder(o1);

    ASSERT_EQ(trades.size(), 1);
    EXPECT_THROW([&]{auto bb = ob.GetBestBid();}(), std::runtime_error);
    EXPECT_THROW([&]{auto bb = ob.GetBestAsk();}(), std::runtime_error);
    EXPECT_EQ(ob.GetBook().first.size(), 0);
    EXPECT_EQ(ob.GetBook().second.size(), 0);
    EXPECT_EQ(trades[0].type, TradeType::FILL);
    EXPECT_EQ(trades[0].quantity, 15);
    EXPECT_EQ(trades[0].level, 10000);
    EXPECT_EQ(trades[0].side, Side::BUY);
    EXPECT_EQ(trades[0].taker_user_id, 2);
    EXPECT_EQ(trades[0].taker_order_id, 2);
    EXPECT_EQ(trades[0].maker_user_id, 1);
    EXPECT_EQ(trades[0].maker_order_id, 1);
}

TEST(OrderBook, LimitOrderFillsRestingOrderAndAcceptsRemainingQuantity) {
    const Level l_10000(10000, {}, {Order(OrderType::LIMIT, 1, 10000, 15, Side::SELL, 1)});
    const std::unordered_map<uint32_t, Level> asks = {
        {10000, l_10000}
    };
    const std::unordered_map<uint32_t, Level> bids;

    OrderBook ob(bids, asks);

    ASSERT_EQ(ob.GetBestAsk(), 10000);

    const Order o1(OrderType::LIMIT, 2, 10000, 20, Side::BUY, 2);
    const auto trades = ob.SubmitOrder(o1);

    ASSERT_EQ(trades.size(), 2);
    EXPECT_EQ(ob.GetBestBid(), 10000);
    EXPECT_EQ(ob.GetBook().first.size(), 1);
    EXPECT_EQ(ob.GetBook().second.size(), 0);

    EXPECT_EQ(trades[0].type, TradeType::PARTIAL_FILL);
    EXPECT_EQ(trades[0].quantity, 15);
    EXPECT_EQ(trades[0].level, 10000);
    EXPECT_EQ(trades[0].side, Side::BUY);
    EXPECT_EQ(trades[0].taker_user_id, 2);
    EXPECT_EQ(trades[0].taker_order_id, 2);
    EXPECT_EQ(trades[0].maker_user_id, 1);
    EXPECT_EQ(trades[0].maker_order_id, 1);
    EXPECT_EQ(trades[0].filled_side, FilledSide::MAKER);

    EXPECT_EQ(trades[1].type, TradeType::ACCEPTED);
    EXPECT_EQ(trades[1].quantity, 5);
    EXPECT_EQ(trades[1].level, 10000);
    EXPECT_EQ(trades[1].side, Side::BUY);
    EXPECT_EQ(trades[1].taker_user_id, 2);
    EXPECT_EQ(trades[1].taker_order_id, 2);
    EXPECT_EQ(trades[1].maker_user_id, std::nullopt);
    EXPECT_EQ(trades[1].maker_order_id, std::nullopt);
}
