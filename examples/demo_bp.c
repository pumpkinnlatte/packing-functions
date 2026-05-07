// demo_bp.c -- Box Polynomial.

#include <stdio.h>
#include <inttypes.h>
#include "bp.h"

static void hr(void) {
    printf("------------------------------------------------------------\n");
}

// Imprime la tabla 2D para size x size.
static void show_table_2d(pf_int_t size) {
    printf("         ");
    for (pf_int_t b2 = 0; b2 < size; b2++) printf("b2=%-4" PRIu64, b2);
    printf("\n");
    for (pf_int_t b1 = 0; b1 < size; b1++) {
        printf("  b1=%" PRIu64 "    ", b1);
        for (pf_int_t b2 = 0; b2 < size; b2++) {
            pf_int_t B[2] = {b1, b2};
            printf("%-6" PRIu64, pf_direct_bp(B, 2));
        }
        printf("\n");
    }
}

static void show_vector(const pf_int_t *B, size_t m, const char *label) {
    printf("  %s = (", label);
    for (size_t i = 0; i < m; i++) {
        printf("%" PRIu64 "%s", B[i], i + 1 < m ? ", " : "");
    }
    printf(")\n");
}

// Muestra direct e inverse para un vector dado.
static void dissect(const pf_int_t *B, size_t m) {
    pf_int_t alpha = pf_direct_bp(B, m);
    pf_int_t Brec[8];
    pf_inverse_bp(alpha, m, Brec);

    show_vector(B, m, "B    ");
    printf("  direct  : B -> alpha = %" PRIu64 "\n", alpha);
    show_vector(Brec, m, "Brec ");
    int ok = 1;
    for (size_t i = 0; i < m; i++) if (Brec[i] != B[i]) ok = 0;
    printf("  biyeccion: %s\n", ok ? "OK" : "FAIL");
}

int main(void) {
    hr();
    printf("DEMO BP -- tabla 5x5 (compararla con la del paper)\n");
    hr();
    show_table_2d(5);

    printf("\n");
    hr();
    printf("DEMO BP -- diseccion de vectores 2D\n");
    hr();
    pf_int_t A[2] = {2, 3};
    pf_int_t B[2] = {0, 4};
    pf_int_t C[2] = {4, 0};
    dissect(A, 2); printf("\n");
    dissect(B, 2); printf("\n");
    dissect(C, 2);

    printf("\n");
    hr();
    printf("DEMO BP -- vectores en mas dimensiones\n");
    hr();
    pf_int_t V3[3] = {0, 1, 2};
    pf_int_t V4[4] = {1, 3, 2, 1};
    pf_int_t V5[5] = {2, 0, 1, 4, 0};
    dissect(V3, 3); printf("\n");
    dissect(V4, 4); printf("\n");
    dissect(V5, 5);

    printf("\n");
    hr();
    printf("DEMO BP -- biyeccion exhaustiva en 3D para B en [0, 4]^3\n");
    hr();
    int total = 0, ok_count = 0;
    for (pf_int_t b1 = 0; b1 <= 4; b1++) {
        for (pf_int_t b2 = 0; b2 <= 4; b2++) {
            for (pf_int_t b3 = 0; b3 <= 4; b3++) {
                pf_int_t Vt[3] = {b1, b2, b3};
                pf_int_t alpha = pf_direct_bp(Vt, 3);
                pf_int_t Brec[3];
                pf_inverse_bp(alpha, 3, Brec);
                total++;
                if (Brec[0] == b1 && Brec[1] == b2 && Brec[2] == b3) ok_count++;
            }
        }
    }
    printf("  vectores probados: %d, biyeccion OK en: %d\n", total, ok_count);

    return 0;
}
