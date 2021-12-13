#ifndef PULSE_UTIL_H_INCLUDED
#define PULSE_UTIL_H_INCLUDED

#define DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION
#include <doctest/doctest.h>

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define PP_NARG(...) PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define PP_RSEQ_N() 9, 8, 7, 6, 5, 4, 3, 2, 1

#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define DEBRACE(...) __VA_ARGS__

#define FWD(func, ...) func(__VA_ARGS__)

#define FOR_EACH(func, constVar, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER(func, constVar, __VA_ARGS__)))

#define FOR_EACH_HELPER(func, constVar, sub, ...) FWD(func, DEBRACE constVar, sub) \
                                                  __VA_OPT__(FOR_EACH_AGAIN PARENS (func, constVar, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define SUBCASE_VAR_GENERATE(nested, var, ...) FOR_EACH(SUBCASE_GENERATE, (nested, var), __VA_ARGS__)
#define SUBCASE_UNPACK(nested, var) SUBCASE_VAR_GENERATE(nested, var)

#define SUBCASE_SPECIAL1(a) SUBCASE_UNPACK(, DEBRACE a)
#define SUBCASE_SPECIAL2(a, ...) SUBCASE_UNPACK(SUBCASE_SPECIAL1(__VA_ARGS__), DEBRACE a)
#define SUBCASE_SPECIAL3(a, ...) SUBCASE_UNPACK(SUBCASE_SPECIAL2(__VA_ARGS__), DEBRACE a)
#define SUBCASE_SPECIAL4(a, ...) SUBCASE_UNPACK(SUBCASE_SPECIAL3(__VA_ARGS__), DEBRACE a)
#define SUBCASE_SPECIAL5(a, ...) SUBCASE_UNPACK(SUBCASE_SPECIAL4(__VA_ARGS__), DEBRACE a)
#define SUBCASE_SPECIAL6(a, ...) SUBCASE_UNPACK(SUBCASE_SPECIAL5(__VA_ARGS__), DEBRACE a)
#define SUBCASE_SPECIAL7(a, ...) SUBCASE_UNPACK(SUBCASE_SPECIAL6(__VA_ARGS__), DEBRACE a)
#define SUBCASE_SPECIAL8(a, ...) SUBCASE_UNPACK(SUBCASE_SPECIAL7(__VA_ARGS__), DEBRACE a)
#define SUBCASE_SPECIAL9(a, ...) SUBCASE_UNPACK(SUBCASE_SPECIAL8(__VA_ARGS__), DEBRACE a)

#define SUBCASE_GENERATE(nested, var, sub) SUBCASE("AUTOGENERATED RECURSIVE PARAMETERIZATION " #var " = " #sub) { var = sub; nested; }

#define SUBCASE_PARAMETERIZE(...) CONCAT(SUBCASE_SPECIAL, PP_NARG(__VA_ARGS__))(__VA_ARGS__) (void)0


#define CHECK_THROWS_PGE(exp) CHECK_THROWS(exp, PGE::Exception())

#include <iostream>

static std::ostream pgeCout(std::cout.rdbuf());

#endif // PULSE_UTIL_H_INCLUDED
