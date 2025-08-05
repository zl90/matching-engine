#ifndef LEVEL_H
#define LEVEL_H
#include <cstdint>
#include <vector>

#include "Order.h"

struct Level {
    const uint32_t price;
    std::vector<Order> market_maker_queue;
    std::vector<Order> participants_queue;
};

#endif //LEVEL_H
