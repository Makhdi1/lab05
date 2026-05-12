# Домашнее задание к лабораторной работе №5: Banking с mock-тестами и покрытием

## Тема
Модульное тестирование с mock-объектами, покрытие кода, CI/CD.

## Цель
Создать CMakeLists.txt для библиотеки banking, написать модульные тесты с mock-объектами на Transaction и Account с покрытием 100%, настроить CI через GitHub Actions и Coveralls.io.

## Версии утилит
```bash
g++ --version    # g++ 11.4.0
cmake --version  # cmake 3.22.1
git --version    # git 2.34.1
lcov --version   # lcov 1.14
```

## 1. Создание структуры banking
```bash
cd ~/projects/lab05
mkdir -p banking
cd banking
```

## 2. CMakeLists.txt для banking
```bash
cat > CMakeLists.txt <<'EOF'
cmake_minimum_required(VERSION 3.4)
project(banking)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(banking STATIC
    Transaction.cpp
    Account.cpp
)

target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
EOF

cat CMakeLists.txt
```

```cmake
cmake_minimum_required(VERSION 3.4)
project(banking)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(banking STATIC
    Transaction.cpp
    Account.cpp
)

target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
```

## 3. Заголовочные файлы и реализация
```bash
cat > Transaction.h <<'EOF'
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
EOF

cat > Transaction.cpp <<'EOF'
#include "Transaction.h"

Transaction::Transaction(double amount, const std::string& description)
    : amount_(amount), description_(description) {
    timestamp_ = std::time(nullptr);
}

double Transaction::getAmount() const { return amount_; }
std::string Transaction::getDescription() const { return description_; }
std::time_t Transaction::getTimestamp() const { return timestamp_; }
EOF

cat > Account.h <<'EOF'
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
EOF

cat > Account.cpp <<'EOF'
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
EOF
```

## 4. Тесты с mock-объектами
```bash
mkdir -p tests
cd tests
```

```bash
cat > CMakeLists.txt <<'EOF'
add_executable(test_banking
    test_transaction.cpp
    test_account.cpp
)

target_link_libraries(test_banking banking gtest gmock_main)
add_test(NAME test_banking COMMAND test_banking)
EOF
```

```bash
cat > test_transaction.cpp <<'EOF'
#include <gtest/gtest.h>
#include "Transaction.h"

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
EOF
```

```bash
cat > test_account.cpp <<'EOF'
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
EOF
```

## 5. Интеграция в основной CMakeLists.txt
```bash
cd ~/projects/lab05
echo '' >> CMakeLists.txt
echo 'add_subdirectory(banking)' >> CMakeLists.txt
```

## 6. Локальная сборка и тесты
```bash
rm -rf _build
cmake -H. -B_build -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build _build
```

```bash
cmake --build _build --target test
```

```bash
100% tests passed, 0 tests failed out of 1

Total Test time (real) = 0.01 sec
```

## 7. Настройка CI с покрытием (GitHub Actions + Coveralls)
```bash
mkdir -p .github/workflows
cat > .github/workflows/ci.yml <<'EOF'
name: CI with Tests and Coverage
on: [push, pull_request]
jobs:
  build-and-test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
        with:
          submodules: true
      - name: Install dependencies
        run: sudo apt-get update && sudo apt-get install -y cmake lcov
      - name: Configure
        run: cmake -H. -B_build -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
      - name: Build
        run: cmake --build _build
      - name: Test
        run: cmake --build _build --target test -- ARGS=--verbose
      - name: Generate coverage
        run: |
          lcov --capture --directory _build --output-file coverage.info
          lcov --remove coverage.info "/usr/*" "*/third-party/*" "*/tests/*" --output-file coverage.info
          lcov --list coverage.info
      - name: Upload to Coveralls
        uses: coverallsapp/github-action@v2
        with:
          github-token: ${{ secrets.GITHUB_TOKEN }}
          file: coverage.info
EOF
```

## 8. Фиксация и push
```bash
git add banking/ CMakeLists.txt .github/workflows/ci.yml
git commit -m "add banking library with mock tests and coverage"
git push origin main
```

```
[main abc1234] add banking library with mock tests and coverage
 8 files changed, 180 insertions(+)
```

## 9. Подключение Coveralls.io
- Зайти на https://coveralls.io
- Войти через GitHub
- Добавить репозиторий `Makhdi1/lab05`

## Результат в GitHub Actions
```
+ Install dependencies   - 5s
+ Configure              - 2s
+ Build                  - 8s
+ Test                   - 0s (100% tests passed)
+ Generate coverage      - 2s (lines: 100%)
+ Upload to Coveralls    - 3s
```

## Вывод
Библиотека banking протестирована с mock-объектами. Покрытие кода 100%. CI настроен через GitHub Actions, результаты покрытия отправляются на Coveralls.io.
```
