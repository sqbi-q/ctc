#include "../ctc.hpp"
#include <cassert>
#include <string_view>

struct Palindrome {
    std::string_view str; 

    constexpr Palindrome(const char* str) : str{ str } {
        ctc<>::fail_runtime(); // could be fail_runtime(); if base was ctc
        assert_palindrom();
    }

private:
    constexpr void assert_palindrom() const {
        const unsigned long str_len = str.length();
        for (unsigned long i = 0; i < str.size() / 2; i++) {
            char start = str[i];
            char end = str[str_len - 1 - i];
            constexpr_assert(start == end, "string is not a palindrome");
        } 
    }
};


int main(void) {
    // Always compile-time cuz every object needs to be constexpr-evaluated
    [[maybe_unused]] constexpr bool test_01 = []() constexpr {
        (void) Palindrome{ "racecar" };
        (void) Palindrome{ "kamilslimak" };
        /* (void) Palindrome{ "jacob" }; // not a palindrome */
        return true;
    }();

    {
        // same as in above constexpr lambda body, but requires name
        constexpr Palindrome p { "racecar" };
        (void) p;
        /* constexpr Palindrome p1 { "jacob" }; */
        // run-time fail assert, run-time palindrome assert
        /* const Palindrome p2 { "jacob" }; */
    }

    {
        // no N3413 - cannot pass string literal
        // cannot also pass it as pointer
        /* ctc<Palindrome>::Constr<"aaa">; */
    }

    {
        // N3413 workaround using `ctc<T>:from_lambda()`
        // with instantiation inside lambda.
        // Always compile-time
        ctc<Palindrome>::from_lambda([] { return "kamilslimak"; });
        ctc<Palindrome>::from_lambda([]() -> Palindrome { return "racecar"; });
        /* ctc<Palindrome>::from_lambda([] { return "jacob"; }); // not a palindrome */
       
        struct Palindrome_ctc : ctc<Palindrome> {};
        Palindrome_ctc::from_lambda([] { return "racecar"; });
        /* Palindrome_ctc::from_lambda([] { return "jacob"; }); // not a palindrome */
    }
}
