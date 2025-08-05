#include <gtest/gtest.h>
#include "../include/OrderBook.h"
#include "../include/OrderIdManager.h"

TEST(OrderIdManager, ConstructorGeneratesCorrectNextOrderID) {
    std::unordered_map<std::size_t, bool> empty_map = {};
    OrderIdManager om(std::move(empty_map));

    std::size_t next = om.GetNextOrderId();
    EXPECT_EQ(next, 0);
    next = om.GetNextOrderId();
    EXPECT_NE(next, 0);

    EXPECT_TRUE(om.DoesOrderIdExist(0));
    EXPECT_TRUE(om.DoesOrderIdExist(next));

    std::unordered_map<std::size_t, bool> map;
    map[0] = true;
    map[2] = true;
    OrderIdManager om2(std::move(map));

    next = om2.GetNextOrderId();
    EXPECT_NE(next, 0);

    EXPECT_TRUE(om2.DoesOrderIdExist(0));
    EXPECT_TRUE(om2.DoesOrderIdExist(2));
    EXPECT_TRUE(om2.DoesOrderIdExist(next));
}
