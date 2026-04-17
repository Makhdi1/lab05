#include "Transaction.h"

Transaction::Transaction(double amount, const std::string& description)
    : amount_(amount), description_(description) {
    timestamp_ = std::time(nullptr);
}

double Transaction::getAmount() const { return amount_; }
std::string Transaction::getDescription() const { return description_; }
std::time_t Transaction::getTimestamp() const { return timestamp_; }
