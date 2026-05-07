// demo_dp.c -- Diagonal Polynomial.

#include <stdio.h>
#include <inttypes.h>
#include "dp.h"

static void hr(void) {
    printf("------------------------------------------------------------\n");
}

// Imprime la tabla 2D para size x size.
static void show_table_2d(pf_int_t size) {
    printf("         ");
    for (pf_int_t w2 = 0; w2 < size; w2++) printf("w2=%-3" PRIu64, w2);
    printf("\n");
    for (pf_int_t w1 = 0; w1 < size; w1++) {
        printf("  w1=%" PRIu64 "    ", w1);
        for (pf_int_t w2 = 0; w2 < size; w2++) {
            printf("%-5" PRIu64, pf_direct_dp_2d(w1, w2));
        }
        printf("\n");
    }
}

// Muestra el calculo para un (w1, w2) dado.
static void dissect_2d(pf_int_t w1, pf_int_t w2) {
    pf_int_t alpha = pf_direct_dp_2d(w1, w2);
    pf_int_t W[2];
    pf_inverse_dp_2d(alpha, W);

    printf("DP(%" PRIu64 ", %" PRIu64 ")\n", w1, w2);
    printf("  direct:  (%" PRIu64 ", %" PRIu64 ") -> alpha = %" PRIu64 "\n",
           w1, w2, alpha);
    printf("  inverse: alpha = %" PRIu64 " -> (%" PRIu64 ", %" PRIu64 ")\n",
           alpha, W[0], W[1]);
    printf("  biyeccion: %s\n",
           (W[0] == w1 && W[1] == w2) ? "OK" : "FAIL");
}

// Imprime un vector con su alpha.
static void show_vector(const pf_int_t *W, size_t m) {
    printf("  (");
    for (size_t i = 0; i < m; i++) {
        printf("%" PRIu64 "%s", W[i], i + 1 < m ? ", " : "");
    }
    printf(") -> alpha = %" PRIu64 "\n", pf_direct_dp(W, m));
}

int main(void) {
    hr();
    printf("DEMO DP -- tabla 5x5 (compararla con la del paper)\n");
    hr();
    show_table_2d(5);

    printf("\n");
    hr();
    printf("DEMO DP -- diseccion de vectores 2D\n");
    hr();
    dissect_2d(2, 3);
    printf("\n");
    dissect_2d(0, 4);
    printf("\n");
    dissect_2d(4, 4);

    printf("\n");
    hr();
    printf("DEMO DP -- vectores en mas dimensiones\n");
    hr();
    pf_int_t V3[3] = {1, 2, 3};
    pf_int_t V4[4] = {0, 1, 0, 2};
    pf_int_t V5[5] = {2, 0, 1, 1, 0};
    show_vector(V3, 3);
    show_vector(V4, 4);
    show_vector(V5, 5);

    printf("\n");
    hr();
    printf("DEMO DP -- biyeccion exhaustiva en 2D para w1, w2 en [0, 4]\n");
    hr();
    int ok = 1;
    for (pf_int_t w1 = 0; w1 <= 4 && ok; w1++) {
        for (pf_int_t w2 = 0; w2 <= 4 && ok; w2++) {
            pf_int_t alpha = pf_direct_dp_2d(w1, w2);
            pf_int_t W[2];
            pf_inverse_dp_2d(alpha, W);
            if (W[0] != w1 || W[1] != w2) ok = 0;
        }
    }
    printf("  inverse(direct(w1, w2)) == (w1, w2) para todos los pares: %s\n",
           ok ? "OK" : "FAIL");

    return 0;
}
