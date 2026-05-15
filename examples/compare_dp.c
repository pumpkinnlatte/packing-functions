/**
 * @file compare_dp.c
 * @brief Prints one row comparing the GTP route and the analytical closed-form
 *        route for inverting the Diagonal Polynomial.
 *
 * Usage: compare_dp -DIM m -INPUT alpha   (2 <= m <= 4)
 *
 * Output (one line):
 *   alpha | GTP vector V | DP via GTP | DP via closed-form
 *
 * The GTP route applies pf_inverse_gtp followed by pf_gtp_to_dp.
 * The closed-form route applies pf_inverse_dp_2d/3d/4d depending on m.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "gtp.h"
#include "dp.h"

#define MAX_DIM 4

static void die(const char *msg) {
    fprintf(stderr, "compare_dp: %s\n", msg);
    exit(1);
}

static void format_vec(char *buf, size_t bufsz, const pf_int_t *V, size_t m) {
    int pos = snprintf(buf, bufsz, "(");
    for (size_t i = 0; i < m && (size_t)pos < bufsz - 1; i++)
        pos += snprintf(buf + pos, bufsz - (size_t)pos,
                        "%" PRIu64 "%s", V[i], i + 1 < m ? "," : "");
    snprintf(buf + pos, bufsz - (size_t)pos, ")");
}

int main(int argc, char **argv) {
    size_t   dim      = 0;
    pf_int_t alpha    = 0;
    int      has_dim  = 0;
    int      has_in   = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-DIM") == 0 && i + 1 < argc) {
            dim = (size_t)strtoull(argv[++i], NULL, 10);
            has_dim = 1;
        } else if (strcmp(argv[i], "-INPUT") == 0 && i + 1 < argc) {
            alpha = (pf_int_t)strtoull(argv[++i], NULL, 10);
            has_in = 1;
        }
    }

    if (!has_dim)              die("missing -DIM");
    if (!has_in)               die("missing -INPUT");
    if (dim < 2 || dim > MAX_DIM) die("-DIM must be 2, 3, or 4");

    /* GTP route: alpha -> V (GTP) -> W (DP) */
    pf_int_t V[MAX_DIM], W_gtp[MAX_DIM];
    pf_inverse_gtp(alpha, dim, V);
    pf_gtp_to_dp(V, dim, W_gtp);

    /* Analytical closed-form route */
    pf_int_t W_cf[MAX_DIM];
    switch (dim) {
        case 2: pf_inverse_dp_2d(alpha, W_cf); break;
        case 3: pf_inverse_dp_3d(alpha, W_cf); break;
        case 4: pf_inverse_dp_4d(alpha, W_cf); break;
        default: break;
    }

    char buf_v[64], buf_gtp[64], buf_cf[64];
    format_vec(buf_v,   sizeof(buf_v),   V,     dim);
    format_vec(buf_gtp, sizeof(buf_gtp), W_gtp, dim);
    format_vec(buf_cf,  sizeof(buf_cf),  W_cf,  dim);

    printf("%-6" PRIu64 " | %-18s | %-18s | %-18s\n",
           alpha, buf_v, buf_gtp, buf_cf);

    return 0;
}
