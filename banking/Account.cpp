#include "Account.h"
#include <stdexcept>

Account::Account(int id, double initialBalance)
    : id_(id), balance_(initialBalance) {}

int Account::getId() const { return id_; }
double Account::getBalance() const { return balance_; }

void Account::deposit(double amount) {
    if (amount <= 0) throw std::invalid_argument("Amount must be positive");
    balance_ += amount;
    addTransaction(Transaction(amount, "Deposit"));
}

bool Account::withdraw(double amount) {
    if (amount <= 0) throw std::invalid_argument("Amount must be positive");
    if (amount > balance_) return false;
    balance_ -= amount;
    addTransaction(Transaction(-amount, "Withdrawal"));
    return true;
}

void Account::addTransaction(const Transaction& t) {
    transactions_.push_back(t);
}

const std::vector<Transaction>& Account::getTransactions() const {
    return transactions_;
}
