enable_testing()
include_directories(. ../src)
add_executable(gorotest gorotest.c)
add_test(gorotest test/bin/gorotest)