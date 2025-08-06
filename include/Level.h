#ifndef LEVEL_H
#define LEVEL_H
#include <cstdint>
#include <vector>

#include "Order.h"

struct Level {
    Level(const uint32_t price, std::vector<Order> &&market_maker_queue,
          std::vector<Order> &&participants_queue): price(price),
                                                    market_maker_queue(std::move(market_maker_queue)),
                                                    participants_queue(std::move(participants_queue)) {
    };

    const uint32_t price;
    std::vector<Order> market_maker_queue;
    std::vector<Order> participants_queue;
};

#endif //LEVEL_H
