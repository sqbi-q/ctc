#define NDEBUG_RUNTIME 0
#define CTC_IMPLEMENTATION
#include "../ctc.hpp"

struct Natural : public ctc<Natural> {
    int value;
    Natural() = delete;

    // ctc requires constructor to be constexpr
    constexpr Natural(int value) : value{ value } {
        fail_runtime("constructor cannot be called run-time");
        constexpr_assert(value >= 0, "value needs to be nonnegative integer");
        /* (value >= 0) ? true */
        /*     : throw std::invalid_argument("value needs to be nonnegative integer"); */
    }
};


constexpr bool construct_natural(Natural) { return true; }
constexpr bool func_pass_test() {
    construct_natural(1);
    construct_natural(0);

    /* construct_natural(-1); */
    /* construct_natural(Natural{-1}); */

    return true;
}

constexpr void inits_test() {
    [[maybe_unused]] constexpr Natural ZERO {0};
    [[maybe_unused]] constexpr Natural ONE      = Natural::Constr<1>;
    [[maybe_unused]] constexpr Natural TWO      = ctc<Natural>::Constr<2>;
    [[maybe_unused]] constexpr Natural THREE    = Natural::ctc::Constr<3>;

    [[maybe_unused]] Natural Zero = Natural::Constr<0>;
    /* [[maybe_unused]] Natural Zero_2 {0}; // may cause run-time fail exception */

    /* [[maybe_unused]] constexpr Natural NegativeOne = Natural::Constr<-1>; */
    /* [[maybe_unused]] Natural NegativeOne_2 = Natural::Constr<-1>; */
    /* [[maybe_unused]] constexpr Natural NegativeOne_3 {-1}; */
    /* [[maybe_unused]] Natural NegativeOne_4 {-1}; // may cause run-time fail exception, */
                                                 // constexpr nonzero assertion run-time exception
                                                 // and compile-time error in constexpr-eval

    /* [[maybe_unused]] Natural _PI_1 = Natural::Constr<3.14>; // narrowing double to int */
    /* [[maybe_unused]] Natural _PI_2 {3.14}; // narrowing double to int */
    /* [[maybe_unused]] Natural _PI_3 (3.14); // implicit conversion in parentheses */
}

constexpr Natural quadratic_discriminant(int a, int b, int c) {
    const int d = b*b - 4*a*c;
    return d;
}

constexpr void misc_test() {
    /* constexpr Natural d1 = quadratic_discriminant(1, 1, 1); */
    [[maybe_unused]] constexpr Natural d2 = quadratic_discriminant(-1, 1, 1);
}

int main(void) {
    /* func_pass_test(); // will produce run-time fail exceptions */
    [[maybe_unused]] constexpr bool result = func_pass_test(); // will allow ctc errors
    inits_test(); // run in non constant expression context
    /* [[maybe_unused]] constexpr auto x = []() { inits_test(); return true; }(); */
    misc_test();

    return 0;
}
