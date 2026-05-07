// demo_gtp.c -- Greater-Than Polynomial.

#include <stdio.h>
#include <inttypes.h>
#include "gtp.h"
#include "dp.h"

static void hr(void) {
    printf("------------------------------------------------------------\n");
}

// Imprime la tabla 2D de GTP 
static void show_table_gtp(pf_int_t size) {
    printf("         ");
    for (pf_int_t v2 = 0; v2 < size; v2++) printf("v2=%-4" PRIu64, v2);
    printf("\n");
    for (pf_int_t v1 = 0; v1 < size; v1++) {
        printf("  v1=%" PRIu64 "    ", v1);
        for (pf_int_t v2 = 0; v2 < size; v2++) {
            if (v2 > v1) {
                pf_int_t V[2] = {v1, v2};
                printf("%-6" PRIu64, pf_direct_gtp(V, 2));
            } else {
                printf(".     ");
            }
        }
        printf("\n");
    }
}

static void show_vector(const pf_int_t *V, size_t m, const char *label) {
    printf("  %s = (", label);
    for (size_t i = 0; i < m; i++) {
        printf("%" PRIu64 "%s", V[i], i + 1 < m ? ", " : "");
    }
    printf(")\n");
}


// Muestra direct e inverse para un vector GTP.
static void dissect_gtp(const pf_int_t *V, size_t m) {
    pf_int_t alpha = pf_direct_gtp(V, m);
    pf_int_t Vrec[8];
    pf_inverse_gtp(alpha, m, Vrec);

    show_vector(V, m, "V    ");
    printf("  direct  : V -> alpha = %" PRIu64 "\n", alpha);
    show_vector(Vrec, m, "Vrec ");
    int ok = 1;
    for (size_t i = 0; i < m; i++) if (Vrec[i] != V[i]) ok = 0;
    printf("  biyeccion: %s\n", ok ? "OK" : "FAIL");
}

// Callback para mostrar cada subconjunto generado.
typedef struct { pf_int_t count; } print_state;

static void print_cb(const pf_int_t *V, size_t t, void *user) {
    print_state *s = (print_state *)user;
    printf("  [%2" PRIu64 "] (", s->count);
    for (size_t i = 0; i < t; i++) {
        printf("%" PRIu64 "%s", V[i], i + 1 < t ? ", " : "");
    }
    printf(") -> alpha = %" PRIu64 "\n", pf_direct_gtp(V, t));
    s->count++;
}

int main(void) {
    hr();
    printf("DEMO GTP \n");
    hr();
    show_table_gtp(5);

    printf("\n");
    hr();
    printf("DEMO GTP -- diseccion de vectores 2D y 3D\n");
    hr();
    pf_int_t A[2] = {2, 4};
    pf_int_t B[3] = {0, 2, 5};
    pf_int_t C[4] = {1, 3, 4, 7};
    dissect_gtp(A, 2); printf("\n");
    dissect_gtp(B, 3); printf("\n");
    dissect_gtp(C, 4);

    printf("\n");
    hr();
    printf("DEMO GTP -- biyeccion GTP <-> DP\n");
    hr();
    pf_int_t W[3] = {1, 0, 2};
    pf_int_t Vfrom[3], Wback[3];
    show_vector(W, 3, "W (DP)");
    pf_dp_to_gtp(W, 3, Vfrom);
    show_vector(Vfrom, 3, "V (GTP, via biyeccion)");
    pf_gtp_to_dp(Vfrom, 3, Wback);
    show_vector(Wback, 3, "W reconstruido");
    printf("  alpha como DP : %" PRIu64 "\n", pf_direct_dp(W, 3));
    printf("  alpha como GTP: %" PRIu64 "\n", pf_direct_gtp(Vfrom, 3));
    printf("  (deben coincidir)\n");

    printf("\n");
    hr();
    printf("DEMO GTP -- inverse_dp m-dimensional usando GTP internamente\n");
    hr();
    pf_int_t Wtest[4] = {0, 1, 2, 0};
    pf_int_t a = pf_direct_dp(Wtest, 4);
    pf_int_t Wrec[4];
    pf_inverse_dp(a, 4, Wrec);
    show_vector(Wtest, 4, "W original");
    printf("  alpha = %" PRIu64 "\n", a);
    show_vector(Wrec, 4, "W recuperado");

    printf("\n");
    hr();
    printf("DEMO GTP -- AllSubsets(k=5, t=3): los C(5,3) = 10 subconjuntos\n");
    hr();
    print_state ps = {0};
    pf_all_subsets(5, 3, print_cb, &ps);

    printf("\n");
    hr();
    printf("DEMO GTP -- AllSubsets(k=4, t=2): los C(4,2) = 6 subconjuntos\n");
    hr();
    ps.count = 0;
    pf_all_subsets(4, 2, print_cb, &ps);

    return 0;
}
