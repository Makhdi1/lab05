add_executable(test_banking
    test_transaction.cpp
    test_account.cpp
)

target_link_libraries(test_banking banking gtest gmock_main)
add_test(NAME test_banking COMMAND test_banking)
