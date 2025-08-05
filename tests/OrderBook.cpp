#include <gtest/gtest.h>
#include "../include/OrderBook.h"
#include "../include/OrderIdManager.h"

TEST(OrderIdManager, ConstructorGeneratesCorrectNextOrderID) {
    std::unordered_map<std::size_t, bool> empty_map = {};
    OrderIdManager om(std::move(empty_map));

    EXPECT_EQ(om.GetNextOrderId(), 0);
    EXPECT_EQ(om.GetNextOrderId(), 1);
    EXPECT_EQ(om.GetNextOrderId(), 2);

    EXPECT_TRUE(om.DoesOrderIdExist(0));
    EXPECT_TRUE(om.DoesOrderIdExist(1));
    EXPECT_TRUE(om.DoesOrderIdExist(2));

    EXPECT_FALSE(om.DoesOrderIdExist(3));

    std::unordered_map<std::size_t, bool> map;
    map[0] = true;
    map[2] = true;
    OrderIdManager om2(std::move(map));

    EXPECT_EQ(om2.GetNextOrderId(), 1);
    EXPECT_EQ(om2.GetNextOrderId(), 3);
    EXPECT_EQ(om2.GetNextOrderId(), 4);

    EXPECT_TRUE(om2.DoesOrderIdExist(0));
    EXPECT_TRUE(om2.DoesOrderIdExist(1));
    EXPECT_TRUE(om2.DoesOrderIdExist(2));
    EXPECT_TRUE(om2.DoesOrderIdExist(3));
    EXPECT_TRUE(om2.DoesOrderIdExist(4));

    EXPECT_FALSE(om2.DoesOrderIdExist(5));
}
