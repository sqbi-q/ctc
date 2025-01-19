#include "../ctc.hpp"
#include "../ctc.hpp"

// struct _Test : ctc<_Test> {};

#define CTC_IMPLEMENTATION
#include "../ctc.hpp"

struct Test : ctc<Test> {};

int main(void) {
    constexpr_assert(true, "test");
    return 0;
}
