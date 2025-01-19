/* ctc - 0.1.0 */

// Licensed under the terms of the MIT license.  
// For a copy, see `LICENSE` file or <https://opensource.org/licenses/MIT>.


/**
 * @def constexpr_assert(Cond, Msg)
 * Macro for assertion and throwing exception inside constexpr function.
 *
 * Introduces behavior similiar to static_assert() without need 
 * for constexpr-evaluable condition. Advised to be use as assertions 
 * of parameters in (constexpr) constructors of ctc derived classes.
 *
 * An example of usage inside function:
 * @code{.cpp}
 * constexpr int nonzero(int value) {
 *     // Failed assert will cause compile-time exception when called 
 *     // as a constant expression.
 *     // Otherwise will throw run-time exception.
 *     return constexpr_assert(value != 0, "value cannot be zero");
 * }
 * 
 * constexpr int ZERO = 0;
 * constexpr int ONE  = 1;
 * // constexpr int value_of_zero_1 = nonzero(ZERO);  // error compile-time
 * const int value_of_zero_2 = nonzero(ZERO);      // exception run-time
 * constexpr int value_of_one = nonzero(ONE);      // succeds
 * @endcode
 *
 * Throw expression causes failed assertion by making body not produce 
 * a constant expression, yielding constexpr ill-formed.
 * Depending on where function is called, error 
 * or warning (-Winvalid-constexpr) will be produced.
 *
 * To cause error, function needs to be called in a constant expression 
 * context (e.g. inside static_assert(), initializing to constexpr object, 
 * as constructor initalizing constexpr object).
 *
 * Macro tested and works as intended in C++17, though can be used as early 
 * as C++11.
 */
#ifndef constexpr_assert
#define constexpr_assert(Cond, Msg) \
    (Cond) ? true : \
        throw std::invalid_argument(Msg)

#endif


#ifndef NDEBUG_RUNTIME
/**
 * @def NDEBUG_RUNTIME
 * Disables run-time assertion of `ctc<>::fail_runtime()`, inspired 
 * by NDEBUG which disables `assert()` assertions.
 */
#define NDEBUG_RUNTIME 0
#endif


// Define is_consteval()
#if !defined(is_consteval) && !NDEBUG_RUNTIME

// __cplusplus for MSVC is 199711
#if __cplusplus >= 202002L || _HAS_CXX20
#include <type_traits>
#define is_consteval() std::is_constant_evaluated()

#elif defined(__GNUC__) || defined(__clang__)
#define is_consteval() __builtin_is_constant_evaluated()
//#define is_consteval() std::__is_constant_evaluated()

#elif defined(_MSC_VER)
#pragma message("WARNING: Cannot define is_consteval before C++20 for MSVC. Defaults to false.")
#pragma message("NOTE: Change standard to `-std:c++20` or `-std:c++latest` to define it.")
#pragma message("NOTE: Set NDEBUG_RUNTIME to 1 to prevent definition and disable `fail_runtime`.")
#define is_consteval() false

#else // handle other compiler
#pragma message("WARNING: Cannot define is_consteval. Defaults to false.")
#pragma message("NOTE: Change standard to C++20 to define it.")
#pragma message("NOTE: Set NDEBUG_RUNTIME to 1 to prevent definition and disable `fail_runtime`.")
#define is_consteval() false
#endif

#endif


#ifndef CTC_H
#define CTC_H

/** 
 * Compile-Time Class with Curiously Recurring Template Pattern.
 *
 * Provides helper static methods to call class methods consteval-like 
 * in C++17, which is helpful when introducing assertions on parameters 
 * of constructor in compile-time error.
 *
 * @tparam T Class to be used in constant evaluation methods.
 * @tparam Vs Optional literals passed to shortened Constr operator() call.
 *
 * Using `Constr`, parameters are required to be constexpr literals as part 
 * of non-type template parameters `...params`.
 *
 * Using `from_lambda`, wrapped parameters can be passed as function 
 * parameters. Useful for string literals which can't be passed in 
 * template parameters.
 *
 * Using CRTP, user can define class `Class` with base of `ctc<Class>`, 
 * shortening calls to ctc static methods and associating class with 
 * helper static methods for construction.
 */
template<typename T, auto ...Vs>
struct ctc;

#endif


#ifdef CTC_IMPLEMENTATION

#include <stdexcept>

template<typename T = void, auto ...Vs>
struct ctc {
    /**
     * Construct variable in constantly evaluated way.
     * Inspired by https://artificial-mind.net/blog/2020/11/14/cpp17-consteval
     */
    template<auto ...params>
    static constexpr auto Constr = T{ params... };

    /**
     * Construct variable with `Constr<...Vs>` by shorter function form 
     * `ctc<T, ...Vs>()`.
     */
    constexpr T operator() () const { return Constr<Vs...>; }

    /**
     * Construct variable from lambda wrapping constructor call.
     *
     * Resolves cases, where variable (e.g. `std::string`) wouldn't be able 
     * to be passed because it isn't literal type for non-type template 
     * parameter (pre-N3413) using `Constr<...params>`, yet user wants to 
     * construct constexpr instance by passing parameters to static method 
     * (inner explicit constexpr instance for compile-time assertions).
     * 
     * By passing lambda, user can use variables from scope and literals 
     * without capture, making it act "static". With such lambda type, 
     * compiler knows what value variable of lambda has, so it can be used 
     * in constructing constexpr instance from function parameter.
     *
     * For more information, see: https://mpark.github.io/programming/2017/05/26/constexpr-function-parameters/#c17
     */
    template<typename Lambda>
    static constexpr auto from_lambda(Lambda lam_constr) {
        constexpr T instance = lam_constr();
        return instance;
    }
    
    /**
     * Cause run-time exception if called in non-constantly evaluated way.
     */
    constexpr static void fail_runtime(
        const char* msg = "cannot be called run-time")
    {
#if !NDEBUG_RUNTIME
        if (!is_consteval()) {
            constexpr_assert(false, msg);
        }
#else
        (void) msg;
#endif
    }
};

#endif