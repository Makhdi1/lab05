#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Account.h"

class MockAccount : public Account {
public:
    MockAccount(int id, double balance) : Account(id, balance) {}
    MOCK_METHOD1(deposit, void(double amount));
    MOCK_METHOD1(withdraw, bool(double amount));
};

TEST(AccountTest, ConstructorAndGetters) {
    Account acc(1, 1000.0);
    EXPECT_EQ(acc.getId(), 1);
    EXPECT_DOUBLE_EQ(acc.getBalance(), 1000.0);
    EXPECT_TRUE(acc.getTransactions().empty());
}

TEST(AccountTest, Deposit) {
    Account acc(1, 100.0);
    acc.deposit(50.0);
    EXPECT_DOUBLE_EQ(acc.getBalance(), 150.0);
    EXPECT_EQ(acc.getTransactions().size(), 1);
    EXPECT_DOUBLE_EQ(acc.getTransactions()[0].getAmount(), 50.0);
}

TEST(AccountTest, DepositThrowsOnNegative) {
    Account acc(1, 100.0);
    EXPECT_THROW(acc.deposit(-10.0), std::invalid_argument);
}

TEST(AccountTest, WithdrawSuccess) {
    Account acc(1, 100.0);
    EXPECT_TRUE(acc.withdraw(30.0));
    EXPECT_DOUBLE_EQ(acc.getBalance(), 70.0);
    EXPECT_EQ(acc.getTransactions().size(), 1);
    EXPECT_DOUBLE_EQ(acc.getTransactions()[0].getAmount(), -30.0);
}

TEST(AccountTest, WithdrawFailInsufficientFunds) {
    Account acc(1, 100.0);
    EXPECT_FALSE(acc.withdraw(150.0));
    EXPECT_DOUBLE_EQ(acc.getBalance(), 100.0);
    EXPECT_TRUE(acc.getTransactions().empty());
}

TEST(AccountTest, WithdrawThrowsOnNegative) {
    Account acc(1, 100.0);
    EXPECT_THROW(acc.withdraw(-10.0), std::invalid_argument);
}

TEST(AccountTest, MockDeposit) {
    MockAccount mock(1, 100.0);
    EXPECT_CALL(mock, deposit(50.0)).Times(1);
    mock.deposit(50.0);
}

TEST(AccountTest, MockWithdraw) {
    MockAccount mock(1, 100.0);
    EXPECT_CALL(mock, withdraw(30.0)).WillOnce(testing::Return(true));
    EXPECT_TRUE(mock.withdraw(30.0));
}
