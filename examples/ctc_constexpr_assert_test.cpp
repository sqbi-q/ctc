/**
 * @example ctc_constexpr_assert_test.cpp
 * Example usage of constexpr_assert macro inside function.
 */

#define CTC_IMPLEMENTATION
#include "../ctc.hpp"

constexpr int nonzero(int value) {
    // Failed assert will cause compile-time exception when called 
    // as a constant expression.
    // Otherwise will throw run-time exception.
    return constexpr_assert(value != 0, "value cannot be zero");
}

constexpr int ZERO = 0;
constexpr int ONE  = 1;

// constexpr int value_of_zero_1 = nonzero(ZERO);  // error compile-time
const int value_of_zero_2 = nonzero(ZERO);      // exception run-time
constexpr int value_of_one = nonzero(ONE);      // succeds

int main(void) {}
