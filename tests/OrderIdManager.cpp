#include <gtest/gtest.h>
#include "../include/OrderIdManager.h"

TEST(OrderIdManager, NextOrderIdExistsInSet) {
    OrderIdManager om;

    const std::size_t next = om.GetNextOrderId();
    EXPECT_TRUE(om.DoesOrderIdExist(next));
}

TEST(OrderIdManager, GeneratedOrderIdIsNotInOriginalSet) {
    std::unordered_map<std::size_t, bool> map = {{0, true}, {1, true}, {2, true}};
    OrderIdManager om(map);

    EXPECT_TRUE(om.DoesOrderIdExist(0));
    EXPECT_TRUE(om.DoesOrderIdExist(1));
    EXPECT_TRUE(om.DoesOrderIdExist(2));

    const std::size_t next = om.GetNextOrderId();
    EXPECT_TRUE(map.find(next) == map.end());
}
