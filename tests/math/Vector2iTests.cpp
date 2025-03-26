#include <gtest/gtest.h>

#include "Vector2i.hpp"

// Tests if constructors lead to proper values
#pragma region Constructor
TEST(Vector2Integer, EmptyConstructor) {
    Vector2i test1 = Vector2i();
    EXPECT_EQ(test1.m_x, 0);
    EXPECT_EQ(test1.m_y, 0);
}

TEST(Vector2Integer, FullConstructor) {
    Vector2i test1 = Vector2i(0,0);
    EXPECT_EQ(test1.m_x, 0);
    EXPECT_EQ(test1.m_y, 0);

    Vector2i test2 = Vector2i(4,7);
    EXPECT_EQ(test2.m_x, 4);
    EXPECT_EQ(test2.m_y, 7);

    Vector2i test3 = Vector2i(2,-23);
    EXPECT_EQ(test3.m_x, 2);
    EXPECT_EQ(test3.m_y, -23);

    Vector2i test4 = Vector2i(-123,345324);
    EXPECT_EQ(test4.m_x, -123);
    EXPECT_EQ(test4.m_y, 345324);
}

TEST(Vector2Integer, CopyConstructor) {
    Vector2i test1 = Vector2i(0,0);
    Vector2i test2 = Vector2i(test1);
    EXPECT_EQ(test2.m_x, 0);
    EXPECT_EQ(test2.m_y, 0);
    EXPECT_EQ(test2, test1);

    Vector2i test3 = Vector2i(-123,345324);
    Vector2i test4 = test3;
    EXPECT_EQ(test4.m_x, -123);
    EXPECT_EQ(test4.m_y, 345324);
    EXPECT_EQ(test3, test4);
}

TEST(Vector2Integer, MoveConstructor) {
    Vector2i test1 = Vector2i(0,0);
    Vector2i test2 = std::move(Vector2i(test1));
    EXPECT_EQ(test2.m_x, 0);
    EXPECT_EQ(test2.m_y, 0);

    Vector2i test3 = Vector2i(-123,345324);
    Vector2i test4 = std::move(test3);
    EXPECT_EQ(test3.m_x, -123);
    EXPECT_EQ(test3.m_y, 345324);
}
#pragma endregion

// Tests if simple operations between Vectors works
#pragma region Math Operations
TEST(Vector2Integer, Equality) {
    Vector2i test1 = Vector2i(0,0);
    Vector2i test2 = Vector2i(4,6);
    Vector2i test3 = Vector2i(0,0);
    Vector2i test4 = Vector2i(4,6);

    EXPECT_NE(test1, test2);
    EXPECT_NE(test3, test4);

    EXPECT_EQ(test1, test3);
    EXPECT_EQ(test2, test4);
}

TEST(Vector2Integer, Addition) {
    Vector2i test1 = Vector2i(0,0);
    Vector2i test2 = Vector2i(4,6);
    Vector2i test3 = Vector2i(3,7);
    Vector2i test4 = Vector2i(-12,3);

    EXPECT_EQ(test1 + test2, Vector2i(4, 6));
    EXPECT_EQ(test2 + test4, Vector2i(-8, 9));
    EXPECT_EQ(test3 + test1, Vector2i(3, 7));
    EXPECT_EQ(test3 + test4, Vector2i(-9,10));
}

TEST(Vector2Integer, Subtraction) {
    Vector2i test1 = Vector2i(0,0);
    Vector2i test2 = Vector2i(4,6);
    Vector2i test3 = Vector2i(3,7);
    Vector2i test4 = Vector2i(-12,3);

    EXPECT_EQ(test1 - test2, Vector2i(-4, -6));
    EXPECT_EQ(test2 - test4, Vector2i(16, 3));
    EXPECT_EQ(test3 - test1, Vector2i(3, 7));
    EXPECT_EQ(test3 - test4, Vector2i(15,4));

    EXPECT_EQ(test2 - test1, Vector2i(4, 6));
    EXPECT_EQ(test4 - test2, Vector2i(-16, -3));
    EXPECT_EQ(test1 - test3, Vector2i(-3, -7));
    EXPECT_EQ(test4 - test3, Vector2i(-15,-4));
}

TEST(Vector2Integer, Multiplication) {
    Vector2i test1 = Vector2i(0,0);
    Vector2i test2 = Vector2i(4,6);
    Vector2i test3 = Vector2i(3,7);
    Vector2i test4 = Vector2i(-12,3);

    EXPECT_EQ(test1 * test2, Vector2i(0, 0));
    EXPECT_EQ(test2 * test4, Vector2i(-48, 18));
    EXPECT_EQ(test3 * test1, Vector2i(0, 0));
    EXPECT_EQ(test3 * test4, Vector2i(-36,21));
    EXPECT_EQ(test2 * test3, Vector2i(12, 42));

    EXPECT_EQ(test2 * test1, Vector2i(0, 0));
    EXPECT_EQ(test4 * test2, Vector2i(-48, 18));
    EXPECT_EQ(test1 * test3, Vector2i(0, 0));
    EXPECT_EQ(test4 * test3, Vector2i(-36,21));
    EXPECT_EQ(test3 * test2, Vector2i(12, 42));

    EXPECT_EQ(test1 * -3, Vector2i(0, 0));
    EXPECT_EQ(test2 * 5, Vector2i(20, 30));
    EXPECT_EQ(test3 * -4, Vector2i(-12, -28));
    EXPECT_EQ(test4 * 2, Vector2i(-24, 6));

    // TODO: Make possible
    //EXPECT_EQ(-3 * test1, Vector2i(0, 0));
    //EXPECT_EQ(5 * test2, Vector2i(-48, 18));
    //EXPECT_EQ(test3 * -4, Vector2i(0, 0));
    //EXPECT_EQ(test4 * 2, Vector2i(12, 42));
}

TEST(Vector2Integer, Division) {
    Vector2i test1 = Vector2i(1,1);
    Vector2i test2 = Vector2i(4,6);
    Vector2i test3 = Vector2i(3,7);
    Vector2i test4 = Vector2i(-12,3);

    EXPECT_EQ(test1 / test2, Vector2i(0, 0));
    EXPECT_EQ(test2 / test4, Vector2i(0, 2));
    EXPECT_EQ(test3 / test1, Vector2i(3, 7));
    EXPECT_EQ(test3 / test4, Vector2i(0, 2));

    EXPECT_EQ(test2 / test1, Vector2i(4, 6));
    EXPECT_EQ(test4 / test2, Vector2i(-3, 0));
    EXPECT_EQ(test1 / test3, Vector2i(0, 0));
    EXPECT_EQ(test4 / test3, Vector2i(-4,0));

    EXPECT_EQ(test1 / 3, Vector2i(0, 0));
    EXPECT_EQ(test2 / 2, Vector2i(2, 3));
    EXPECT_EQ(test3 / -3, Vector2i(-1, -2));
    EXPECT_EQ(test4 / 1, Vector2i(-12, 3));
}
#pragma endregion

// Tests if utility functions of Vector2i work as intended 
#pragma region Helper
TEST(Vector2Integer, Magnitude) {
    // Root cause is 
    EXPECT_EQ(Vector2i(0, 0).mag(), 0);
    EXPECT_EQ(Vector2i(1, 0).mag(), 1);
    EXPECT_EQ(Vector2i(2, 2).mag(), 3);
    EXPECT_EQ(Vector2i(4,5).mag(), 6);
    EXPECT_EQ(Vector2i(18,22).mag(), 28);
    EXPECT_EQ(Vector2i(400,284).mag(), 491);
}
#pragma endregion

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}