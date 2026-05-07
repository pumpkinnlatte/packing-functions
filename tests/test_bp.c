// Tests del modulo BP.

#include <stdio.h>
#include <inttypes.h>
#include "bp.h"

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

// Tabla 5x5 
static const pf_int_t example_table[5][5] = {
    { 0,  3,  7, 13, 21},
    { 1,  2,  8, 14, 22},
    { 4,  5,  6, 15, 23},
    { 9, 10, 11, 12, 24},
    {16, 17, 18, 19, 20}
};

static void test_direct_bp_example_table(void) {
    for (pf_int_t b1 = 0; b1 < 5; b1++) {
        for (pf_int_t b2 = 0; b2 < 5; b2++) {
            pf_int_t B[2] = {b1, b2};
            CHECK_EQ(pf_direct_bp(B, 2), example_table[b1][b2],
                     "tabla BP del paper");
        }
    }
}

static void test_direct_bp_known(void) {
    // m=1: alpha = b_1.
    pf_int_t B1[1] = {7};
    CHECK_EQ(pf_direct_bp(B1, 1), 7, "BP m=1: alpha = b_1");

    // Vector cero en cualquier dimension -> alpha = 0.
    pf_int_t Z3[3] = {0, 0, 0};
    CHECK_EQ(pf_direct_bp(Z3, 3), 0, "BP 3D: ceros -> 0");
    pf_int_t Z5[5] = {0, 0, 0, 0, 0};
    CHECK_EQ(pf_direct_bp(Z5, 5), 0, "BP 5D: ceros -> 0");
}

static void test_inverse_bp_example_table(void) {
    // Recuperar cada (b1, b2) a partir de su alpha en la tabla.
    for (pf_int_t b1 = 0; b1 < 5; b1++) {
        for (pf_int_t b2 = 0; b2 < 5; b2++) {
            pf_int_t alpha = example_table[b1][b2];
            pf_int_t B[2];
            pf_inverse_bp(alpha, 2, B);
            CHECK_EQ(B[0], b1, "inverse BP 2D: b1");
            CHECK_EQ(B[1], b2, "inverse BP 2D: b2");
        }
    }
}

static void test_bijection_2d(void) {
    // inverse(direct(B)) == B exhaustivo en [0, 12]^2.
    for (pf_int_t b1 = 0; b1 <= 12; b1++) {
        for (pf_int_t b2 = 0; b2 <= 12; b2++) {
            pf_int_t B[2] = {b1, b2};
            pf_int_t alpha = pf_direct_bp(B, 2);
            pf_int_t Brec[2];
            pf_inverse_bp(alpha, 2, Brec);
            CHECK_EQ(Brec[0], b1, "biyeccion BP 2D: b1");
            CHECK_EQ(Brec[1], b2, "biyeccion BP 2D: b2");
        }
    }
}

static void test_bijection_3d(void) {
    // m=3
    for (pf_int_t b1 = 0; b1 <= 5; b1++) {
        for (pf_int_t b2 = 0; b2 <= 5; b2++) {
            for (pf_int_t b3 = 0; b3 <= 5; b3++) {
                pf_int_t B[3] = {b1, b2, b3};
                pf_int_t alpha = pf_direct_bp(B, 3);
                pf_int_t Brec[3];
                pf_inverse_bp(alpha, 3, Brec);
                CHECK_EQ(Brec[0], b1, "biyeccion BP 3D: b1");
                CHECK_EQ(Brec[1], b2, "biyeccion BP 3D: b2");
                CHECK_EQ(Brec[2], b3, "biyeccion BP 3D: b3");
            }
        }
    }
}

static void test_bijection_4d(void) {
    // m=4: cubre iota en {1, 2, 3, 4},
    for (pf_int_t b1 = 0; b1 <= 3; b1++) {
        for (pf_int_t b2 = 0; b2 <= 3; b2++) {
            for (pf_int_t b3 = 0; b3 <= 3; b3++) {
                for (pf_int_t b4 = 0; b4 <= 3; b4++) {
                    pf_int_t B[4] = {b1, b2, b3, b4};
                    pf_int_t alpha = pf_direct_bp(B, 4);
                    pf_int_t Brec[4];
                    pf_inverse_bp(alpha, 4, Brec);
                    CHECK_EQ(Brec[0], b1, "biyeccion BP 4D: b1");
                    CHECK_EQ(Brec[1], b2, "biyeccion BP 4D: b2");
                    CHECK_EQ(Brec[2], b3, "biyeccion BP 4D: b3");
                    CHECK_EQ(Brec[3], b4, "biyeccion BP 4D: b4");
                }
            }
        }
    }
}

static void test_inverse_then_direct(void) {
    // direct(inverse(alpha)) == alpha para alpha en [0, 1000) y m=2,3.
    for (pf_int_t alpha = 0; alpha < 1000; alpha++) {
        pf_int_t B2[2], B3[3];
        pf_inverse_bp(alpha, 2, B2);
        CHECK_EQ(pf_direct_bp(B2, 2), alpha, "direct(inverse(alpha)) m=2");
        pf_inverse_bp(alpha, 3, B3);
        CHECK_EQ(pf_direct_bp(B3, 3), alpha, "direct(inverse(alpha)) m=3");
    }
}

int main(void) {
    test_direct_bp_example_table();
    test_direct_bp_known();
    test_inverse_bp_example_table();
    test_bijection_2d();
    test_bijection_3d();
    test_bijection_4d();
    test_inverse_then_direct();

    if (fails == 0) printf("bp: OK\n");
    else            printf("bp: %d FAILS\n", fails);
    return fails == 0 ? 0 : 1;
}
