// Tests para arith.c
//
// LIMITE DEL ALGORITMO LITERAL: la implementacion fiel al Algoritmo 2
// del paper sufre overflow intermedio en uint64_t para algunos C(k,r)
// con k >= 63. Empiricamente, todos los C(k,r) para k en [0, 62] son
// correctos. Los tests respetan ese rango.

#include <stdio.h>
#include <inttypes.h>
#include "arith.h"

static int fails = 0;

#define CHECK(cond, msg) do {                                  \
    if (!(cond)) { printf("  FAIL %s:%d  %s\n",                \
                          __FILE__, __LINE__, msg); fails++; } \
} while (0)

#define CHECK_EQ(a, b, msg) do {                               \
    pf_int_t _a = (a), _b = (b);                               \
    if (_a != _b) {                                            \
        printf("  FAIL %s:%d  %s  got=%" PRIu64                \
               " expected=%" PRIu64 "\n",                      \
               __FILE__, __LINE__, msg, _a, _b);               \
        fails++;                                               \
    }                                                          \
} while (0)

// ---------- gcd ----------

static void test_gcd_basic(void) {
    CHECK_EQ(gcd_u64(12, 18), 6, "gcd(12,18)");
    CHECK_EQ(gcd_u64(100, 75), 25, "gcd(100,75)");
    CHECK_EQ(gcd_u64(17, 13), 1, "gcd primos");
    CHECK_EQ(gcd_u64(0, 5), 5, "gcd(0,5)");
    CHECK_EQ(gcd_u64(5, 0), 5, "gcd(5,0)");
}

// ---------- binomial ----------

static void test_binomial_trivial(void) {
    CHECK_EQ(pf_binomial(0, 0), 1, "C(0,0)");
    CHECK_EQ(pf_binomial(5, 0), 1, "C(5,0)");
    CHECK_EQ(pf_binomial(5, 5), 1, "C(5,5)");
    CHECK_EQ(pf_binomial(3, 5), 0, "C(3,5)=0 si k<r");
}

static void test_binomial_pascal(void) {
    pf_int_t row5[] = {1, 5, 10, 10, 5, 1};
    for (pf_int_t i = 0; i <= 5; i++)
        CHECK_EQ(pf_binomial(5, i), row5[i], "fila 5");
}

static void test_binomial_known(void) {
    // todos con k <= 62, dentro del rango seguro
    CHECK_EQ(pf_binomial(20, 10), 184756ULL, "C(20,10)");
    CHECK_EQ(pf_binomial(30, 15), 155117520ULL, "C(30,15)");
    CHECK_EQ(pf_binomial(50, 25), 126410606437752ULL, "C(50,25)");
    CHECK_EQ(pf_binomial(62, 31), 465428353255261088ULL, "C(62,31) limite seguro");
}

// ---------- next_binomial ----------

static void test_next_binomial_consistency(void) {
    // hasta k=30, dentro del rango seguro
    for (pf_int_t k = 0; k <= 30; k++) {
        for (pf_int_t r = 0; r <= k; r++) {
            pf_int_t bk   = pf_binomial(k, r);
            pf_int_t bk1  = pf_binomial(k + 1, r);
            pf_int_t next = pf_next_binomial(bk, k, r);
            CHECK_EQ(next, bk1, "next vs directo");
        }
    }
}

static void test_next_binomial_edges(void) {
    CHECK_EQ(pf_next_binomial(0, 1, 5), 0, "k+1<r");
    CHECK_EQ(pf_next_binomial(0, 4, 5), 1, "k+1==r");
    CHECK_EQ(pf_next_binomial(10, 5, 2), 15, "C(6,2)=15");
}

// ---------- isqrt ----------

static void test_isqrt_small(void) {
    CHECK_EQ(pf_isqrt(0), 0, "isqrt(0)");
    CHECK_EQ(pf_isqrt(1), 1, "isqrt(1)");
    CHECK_EQ(pf_isqrt(8), 2, "isqrt(8)");
    CHECK_EQ(pf_isqrt(9), 3, "isqrt(9)");
    CHECK_EQ(pf_isqrt(99), 9, "isqrt(99)");
    CHECK_EQ(pf_isqrt(100), 10, "isqrt(100)");
}

static void test_isqrt_squares(void) {
    for (pf_int_t n = 0; n <= 10000; n++)
        CHECK_EQ(pf_isqrt(n * n), n, "isqrt(n*n)");
}

static void test_isqrt_large(void) {
    pf_int_t big = ((pf_int_t)1u << 32) - 1;
    CHECK_EQ(pf_isqrt(big * big), big, "isqrt((2^32-1)^2)");
    CHECK_EQ(pf_isqrt(UINT64_MAX), big, "isqrt(2^64-1)");
}

// ---------- iroot ----------

static void test_iroot_small(void) {
    CHECK_EQ(pf_iroot(0, 3), 0, "cbrt(0)");
    CHECK_EQ(pf_iroot(1, 3), 1, "cbrt(1)");
    CHECK_EQ(pf_iroot(7, 3), 1, "cbrt(7)");
    CHECK_EQ(pf_iroot(8, 3), 2, "cbrt(8)");
    CHECK_EQ(pf_iroot(26, 3), 2, "cbrt(26)");
    CHECK_EQ(pf_iroot(27, 3), 3, "cbrt(27)");
    CHECK_EQ(pf_iroot(1000, 3), 10, "cbrt(1000)");
}

static void test_iroot_perfect_powers(void) {
    for (pf_int_t n = 0; n <= 100; n++) {
        for (pf_int_t i = 2; i <= 6; i++) {
            pf_int_t p = 1;
            int overflow = 0;
            for (pf_int_t k = 0; k < i; k++) {
                if (n != 0 && p > UINT64_MAX / n) { overflow = 1; break; }
                p *= n;
            }
            if (!overflow)
                CHECK_EQ(pf_iroot(p, i), n, "iroot(n^i, i) = n");
        }
    }
}

static void test_iroot_consistency_with_isqrt(void) {
    for (pf_int_t x = 0; x <= 5000; x++)
        CHECK_EQ(pf_iroot(x, 2), pf_isqrt(x), "iroot(x,2)==isqrt(x)");
}

static void test_iroot_invariant(void) {
    // r = iroot(x,i)  =>  r^i <= x  y  (r+1)^i > x
    for (pf_int_t x = 0; x <= 2000; x++) {
        for (pf_int_t i = 2; i <= 5; i++) {
            pf_int_t r = pf_iroot(x, i);
            pf_int_t lo = 1, hi = 1;
            for (pf_int_t k = 0; k < i; k++) lo *= r;
            for (pf_int_t k = 0; k < i; k++) hi *= (r + 1);
            CHECK(lo <= x, "r^i <= x");
            CHECK(hi > x,  "(r+1)^i > x");
        }
    }
}

// ---------- main ----------

int main(void) {
    test_gcd_basic();

    test_binomial_trivial();
    test_binomial_pascal();
    test_binomial_known();

    test_next_binomial_consistency();
    test_next_binomial_edges();

    test_isqrt_small();
    test_isqrt_squares();
    test_isqrt_large();

    test_iroot_small();
    test_iroot_perfect_powers();
    test_iroot_consistency_with_isqrt();
    test_iroot_invariant();

    if (fails == 0) printf("arith: OK\n");
    else            printf("arith: %d FAILS\n", fails);
    return fails == 0 ? 0 : 1;
}