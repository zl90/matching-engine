cmake_minimum_required(VERSION 3.31)
project(matching_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(matching_engine src/main.cpp
        src/order-book/OrderBook.cpp
        include/OrderBook.h
        src/ingress-buffer/IngressBuffer.cpp
        include/IngressBuffer.h
        include/Order.h)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        orderbook-test
        tests/OrderBook.cpp
        src/order-book/OrderBook.cpp
        include/OrderBook.h
        src/ingress-buffer/IngressBuffer.cpp
        include/IngressBuffer.h
        include/Order.h
)
target_link_libraries(
        orderbook-test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(orderbook-test)
