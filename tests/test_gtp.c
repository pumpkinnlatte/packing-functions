// Tests del modulo GTP.

#include <stdio.h>
#include <inttypes.h>
#include "gtp.h"
#include "dp.h"

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

// Tabla 5x5 para GTP. 
static const pf_int_t paper_table_gtp[5][5] = {
    {999,   0,   1,   3,   6},
    {999, 999,   2,   4,   7},
    {999, 999, 999,   5,   8},
    {999, 999, 999, 999,   9},
    {999, 999, 999, 999, 999}
};

static void test_direct_gtp_paper_table(void) {
    for (pf_int_t v1 = 0; v1 < 5; v1++) {
        for (pf_int_t v2 = v1 + 1; v2 < 5; v2++) {
            pf_int_t V[2] = {v1, v2};
            CHECK_EQ(pf_direct_gtp(V, 2), paper_table_gtp[v1][v2],
                     "tabla GTP del paper");
        }
    }
}

static void test_direct_gtp_known(void) {
    // Casos triviales.
    pf_int_t V1[1] = {0};
    CHECK_EQ(pf_direct_gtp(V1, 1), 0, "GTP m=1: V=(0) -> 0");

    pf_int_t V1b[1] = {7};
    CHECK_EQ(pf_direct_gtp(V1b, 1), 7, "GTP m=1: V=(7) -> 7");

    // Vector creciente con valores pequenos.
    pf_int_t V3[3] = {0, 1, 2};
    CHECK_EQ(pf_direct_gtp(V3, 3), 0, "GTP m=3: V=(0,1,2) -> 0");
}

static void test_bijections_gtp_dp(void) {
    // Tomamos un DP arbitrario, lo pasamos a GTP, y de vuelta.
    // El resultado debe coincidir con el original.
    for (pf_int_t a = 0; a < 5; a++) {
        for (pf_int_t b = 0; b < 5; b++) {
            for (pf_int_t c = 0; c < 5; c++) {
                pf_int_t W[3] = {a, b, c};
                pf_int_t V[3], W2[3];
                pf_dp_to_gtp(W, 3, V);
                pf_gtp_to_dp(V, 3, W2);
                CHECK_EQ(W2[0], W[0], "DP->GTP->DP componente 0");
                CHECK_EQ(W2[1], W[1], "DP->GTP->DP componente 1");
                CHECK_EQ(W2[2], W[2], "DP->GTP->DP componente 2");
            }
        }
    }
}

static void test_bijection_preserves_alpha(void) {
    // direct_dp(W) debe ser igual a direct_gtp(dp_to_gtp(W)).
    // Es la propiedad clave de las dos representaciones.
    for (pf_int_t a = 0; a <= 6; a++) {
        for (pf_int_t b = 0; b <= 6; b++) {
            for (pf_int_t c = 0; c <= 6; c++) {
                pf_int_t W[3] = {a, b, c};
                pf_int_t V[3];
                pf_dp_to_gtp(W, 3, V);
                CHECK_EQ(pf_direct_dp(W, 3), pf_direct_gtp(V, 3),
                         "DP y GTP via biyeccion dan el mismo alpha");
            }
        }
    }
}

static void test_inverse_gtp_2d(void) {
    // Para cada celda valida de la tabla, decodificar alpha.
    for (pf_int_t v1 = 0; v1 < 5; v1++) {
        for (pf_int_t v2 = v1 + 1; v2 < 5; v2++) {
            pf_int_t alpha = paper_table_gtp[v1][v2];
            pf_int_t V[2];
            pf_inverse_gtp(alpha, 2, V);
            CHECK_EQ(V[0], v1, "inverse_gtp 2D: v1");
            CHECK_EQ(V[1], v2, "inverse_gtp 2D: v2");
        }
    }
}

static void test_bijection_inverse_gtp(void) {
    // Invariante GTP. inverse_gtp(direct_gtp(V)) == V.
    // Recorremos vectores GTP de tamaño 3 con componentes pequenas.
    for (pf_int_t v1 = 0; v1 <= 8; v1++) {
        for (pf_int_t v2 = v1 + 1; v2 <= 9; v2++) {
            for (pf_int_t v3 = v2 + 1; v3 <= 10; v3++) {
                pf_int_t V[3] = {v1, v2, v3};
                pf_int_t alpha = pf_direct_gtp(V, 3);
                pf_int_t Vrec[3];
                pf_inverse_gtp(alpha, 3, Vrec);
                CHECK_EQ(Vrec[0], v1, "inverse_gtp 3D: v1");
                CHECK_EQ(Vrec[1], v2, "inverse_gtp 3D: v2");
                CHECK_EQ(Vrec[2], v3, "inverse_gtp 3D: v3");
            }
        }
    }
}

static void test_inverse_dp_general_matches_2d(void) {
    // El inverso m-dim general debe coincidir con el 2D para m=2.
    for (pf_int_t alpha = 0; alpha < 200; alpha++) {
        pf_int_t W2[2], Wgen[2];
        pf_inverse_dp_2d(alpha, W2);
        pf_inverse_dp(alpha, 2, Wgen);
        CHECK_EQ(Wgen[0], W2[0], "inverse_dp general vs 2D: w1");
        CHECK_EQ(Wgen[1], W2[1], "inverse_dp general vs 2D: w2");
    }
}

static void test_bijection_inverse_dp_higher_dim(void) {
    // Invariante DP. inverse(direct(W)) == W.
    for (pf_int_t a = 0; a <= 4; a++) {
        for (pf_int_t b = 0; b <= 4; b++) {
            for (pf_int_t c = 0; c <= 4; c++) {
                pf_int_t W[3] = {a, b, c};
                pf_int_t alpha = pf_direct_dp(W, 3);
                pf_int_t Wrec[3];
                pf_inverse_dp(alpha, 3, Wrec);
                CHECK_EQ(Wrec[0], a, "inverse_dp 3D: w1");
                CHECK_EQ(Wrec[1], b, "inverse_dp 3D: w2");
                CHECK_EQ(Wrec[2], c, "inverse_dp 3D: w3");
            }
        }
    }
}

// ---------- all_subsets ----------

typedef struct {
    pf_int_t count;
    pf_int_t alpha_seen[64];   // alphas en orden de enumeracion
    pf_int_t max_alpha;
} subset_state;

static void count_cb(const pf_int_t *V, size_t t, void *user) {
    subset_state *s = (subset_state *)user;
    pf_int_t alpha = pf_direct_gtp(V, t);
    if (s->count < 64) s->alpha_seen[s->count] = alpha;
    if (alpha > s->max_alpha) s->max_alpha = alpha;
    s->count++;
}

static void test_all_subsets_count(void) {
    // Total de subconjuntos de tamaño t
    pf_int_t cases[][3] = {
        // k, t, esperado
        {5, 2, 10},
        {6, 3, 20},
        {7, 4, 35},
        {4, 4, 1},      // solo (0,1,2,3)
    };
    size_t n = sizeof(cases) / sizeof(cases[0]);
    for (size_t i = 0; i < n; i++) {
        subset_state s = {0, {0}, 0};
        pf_all_subsets(cases[i][0], (size_t)cases[i][1], count_cb, &s);
        CHECK_EQ(s.count, cases[i][2], "all_subsets cuenta correcta");
    }
}

static void test_all_subsets_covers_all_alphas(void) {
    subset_state s = {0, {0}, 0};
    pf_all_subsets(5, 3, count_cb, &s);
    CHECK_EQ(s.count, 10, "all_subsets(5,3) genera 10 vectores");
    CHECK_EQ(s.max_alpha, 9, "max alpha = C(5,3) - 1 = 9");

    // Verificar que aparecen todos los alphas en [0, 9].
    int seen[16] = {0};
    for (pf_int_t i = 0; i < s.count; i++) seen[s.alpha_seen[i]] = 1;
    for (int a = 0; a < 10; a++) {
        CHECK(seen[a] == 1, "alpha presente en la enumeracion");
    }
}

int main(void) {
    test_direct_gtp_paper_table();
    test_direct_gtp_known();
    test_bijections_gtp_dp();
    test_bijection_preserves_alpha();
    test_inverse_gtp_2d();
    test_bijection_inverse_gtp();
    test_inverse_dp_general_matches_2d();
    test_bijection_inverse_dp_higher_dim();
    test_all_subsets_count();
    test_all_subsets_covers_all_alphas();

    if (fails == 0) printf("gtp: OK\n");
    else            printf("gtp: %d FAILS\n", fails);
    return fails == 0 ? 0 : 1;
}
