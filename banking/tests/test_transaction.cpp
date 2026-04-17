#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Transaction.h"
#include <ctime>

TEST(TransactionTest, ConstructorAndGetters) {
    Transaction t(100.50, "Salary");
    EXPECT_DOUBLE_EQ(t.getAmount(), 100.50);
    EXPECT_EQ(t.getDescription(), "Salary");
    EXPECT_NE(t.getTimestamp(), 0);
}

TEST(TransactionTest, NegativeAmount) {
    Transaction t(-50.25, "Withdrawal");
    EXPECT_DOUBLE_EQ(t.getAmount(), -50.25);
    EXPECT_EQ(t.getDescription(), "Withdrawal");
}
