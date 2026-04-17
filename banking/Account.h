#pragma once
#include "Transaction.h"
#include <vector>
#include <memory>

class Account {
public:
    Account(int id, double initialBalance);
    virtual ~Account() = default;
    int getId() const;
    double getBalance() const;
    virtual void deposit(double amount);
    virtual bool withdraw(double amount);
    void addTransaction(const Transaction& t);
    const std::vector<Transaction>& getTransactions() const;
protected:
    int id_;
    double balance_;
    std::vector<Transaction> transactions_;
};
