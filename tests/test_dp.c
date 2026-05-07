// Tests del modulo DP.
//
// Se verifican los siguientes aspectos:
//  - Verificacion contra la tabla 5x5.
//  - Coincidencia entre direct_dp_2d y direct_dp para m=2.
//  - inverse(direct(W)) == W.
//  - direct(inverse(alpha)) == alpha.
#include <stdio.h>
#include <inttypes.h>
#include "dp.h"

static int fails = 0;

#define CHECK(cond, msg) do {                                  \
    if (!(cond)) { printf("  FAIL %s:%d  %s\n",                \
                          __FILE__, __LINE__, msg); fails++; } \
} while (0)

// Compara dos valores enteros. Reporta un fallo si no son iguales. 
#define CHECK_EQ(a, b, msg) do {                               \
    pf_int_t _a = (a), _b = (b);                               \
    if (_a != _b) {                                            \
        printf("  FAIL %s:%d  %s  got=%" PRIu64                \
               " expected=%" PRIu64 "\n",                      \
               __FILE__, __LINE__, msg, _a, _b);               \
        fails++;                                               \
    }                                                          \
} while (0)

// Tabla 5x5.
static const pf_int_t example_table[5][5] = {
    { 0,  1,  3,  6, 10},
    { 2,  4,  7, 11, 16},
    { 5,  8, 12, 17, 23},
    { 9, 13, 18, 24, 31},
    {14, 19, 25, 32, 40}
};

static void test_direct_dp_2d_example_table(void) {
    for (pf_int_t w1 = 0; w1 < 5; w1++) {
        for (pf_int_t w2 = 0; w2 < 5; w2++) {
            CHECK_EQ(pf_direct_dp_2d(w1, w2), example_table[w1][w2],
                     "tabla 5x5 del paper");
        }
    }
}

static void test_direct_dp_general_matches_2d(void) {
    // direct_dp con m=2 debe coincidir con direct_dp_2d.
    for (pf_int_t w1 = 0; w1 < 8; w1++) {
        for (pf_int_t w2 = 0; w2 < 8; w2++) {
            pf_int_t W[2] = {w1, w2};
            CHECK_EQ(pf_direct_dp(W, 2), pf_direct_dp_2d(w1, w2),
                     "direct_dp(2) coincide con direct_dp_2d");
        }
    }
}

static void test_direct_dp_higher_dim_known_values(void) {
    // m=1: la formula reduce a C(w1, 1) = w1.
    pf_int_t W1[1] = {7};
    CHECK_EQ(pf_direct_dp(W1, 1), 7, "DP 1D: alpha = w1");

    // m=3
    pf_int_t A[3] = {1, 0, 0};  
    CHECK_EQ(pf_direct_dp(A, 3), 3,  "DP 3D: (1,0,0) -> 3");
    pf_int_t B[3] = {0, 1, 0};
    CHECK_EQ(pf_direct_dp(B, 3), 2,  "DP 3D: (0,1,0) -> 2");
    pf_int_t C[3] = {1, 2, 3};
    CHECK_EQ(pf_direct_dp(C, 3), 63, "DP 3D: (1,2,3) -> 63");

    // m=5: verificar acumulacion en mas dimensiones.
    pf_int_t D[5] = {2, 0, 1, 0, 3};
    CHECK_EQ(pf_direct_dp(D, 5), 282, "DP 5D: (2,0,1,0,3) -> 282");
}

static void test_inverse_dp_2d_example_table(void) {
    // Para cada celda de la tabla, decodificar alpha y verificar (w1, w2).
    for (pf_int_t w1 = 0; w1 < 5; w1++) {
        for (pf_int_t w2 = 0; w2 < 5; w2++) {
            pf_int_t alpha = example_table[w1][w2];
            pf_int_t W[2];
            pf_inverse_dp_2d(alpha, W);
            CHECK_EQ(W[0], w1, "inverse 2D: w1");
            CHECK_EQ(W[1], w2, "inverse 2D: w2");
        }
    }
}

static void test_bijection_direct_then_inverse(void) {
    // Para todos los (w1, w2) en [0, 30] x [0, 30]:
    //   inverse(direct(w1, w2)) debe devolver (w1, w2).
    for (pf_int_t w1 = 0; w1 <= 30; w1++) {
        for (pf_int_t w2 = 0; w2 <= 30; w2++) {
            pf_int_t alpha = pf_direct_dp_2d(w1, w2);
            pf_int_t W[2];
            pf_inverse_dp_2d(alpha, W);
            CHECK_EQ(W[0], w1, "biyeccion: w1 conservado");
            CHECK_EQ(W[1], w2, "biyeccion: w2 conservado");
        }
    }
}

static void test_bijection_inverse_then_direct(void) {
    // Para todo alpha en [0, 2000]:
    //   direct(inverse(alpha)) debe devolver alpha.
    for (pf_int_t alpha = 0; alpha < 2000; alpha++) {
        pf_int_t W[2];
        pf_inverse_dp_2d(alpha, W);
        CHECK_EQ(pf_direct_dp_2d(W[0], W[1]), alpha,
                 "biyeccion: alpha conservado");
    }
}

int main(void) {
    test_direct_dp_2d_example_table();
    test_direct_dp_general_matches_2d();
    test_direct_dp_higher_dim_known_values();
    test_inverse_dp_2d_example_table();
    test_bijection_direct_then_inverse();
    test_bijection_inverse_then_direct();

    if (fails == 0) printf("dp: OK\n");
    else            printf("dp: %d FAILS\n", fails);
    return fails == 0 ? 0 : 1;
}