# ctc

Single-header library for compile-time assertions and such. 
Works via instancing constexpr object inside ctc's methods.

For more information, see documentation in [ctc.hpp](./ctc.hpp).

## Usage

Do: `#define CTC_IMPLEMENTATION` \
before you include header in C++ file to create the implementation.

Example using ctc's CRTP, `constexpr_assert` and `Constr`:
```c++
#define CTC_IMPLEMENTATION
#include "ctc.hpp"

struct Natural : ctc<Natural> {
    int value;
    Natural() = delete;

    constexpr Natural(int value) : value{ value } {
        fail_runtime("constructor cannot be called run-time");
        constexpr_assert(value >= 0, "value needs to be nonnegative integer");
    }
};

Natural Zero = Natural::Constr<0>; // succeeds
Natural NegativeOne = Natural::Constr<-1>; // compile-time error!
```

[See more examples](./examples)

## License

[MIT](https://choosealicense.com/licenses/mit/)
