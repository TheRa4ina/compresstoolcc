#include <gtest/gtest.h>
#include <src/hello.hpp>

TEST(helloClass, helloStuff_GivingOne_ReturnsOne)
{
    Hello a;
    EXPECT_EQ(a.helloStuff(1),1);
}
