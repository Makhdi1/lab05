#pragma once
#include <string>
#include <ctime>

class Transaction {
public:
    Transaction(double amount, const std::string& description);
    double getAmount() const;
    std::string getDescription() const;
    std::time_t getTimestamp() const;
private:
    double amount_;
    std::string description_;
    std::time_t timestamp_;
};
