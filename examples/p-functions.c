// demo_cli.c -- Interfaz de linea de comandos.
//
// Uso:
//   demo_cli <tipo> encode <v1> [v2 ...]   vector -> alpha
//   demo_cli <tipo> decode <m> <alpha>      alpha -> vector
//   demo_cli <tipo> table  <size>           tabla 2D (dp y bp)
//   demo_cli help
//
// <tipo>: dp | gtp | bp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "gtp.h"
#include "dp.h"
#include "bp.h"

#define MAX_M 64

static void usage(void) {
    puts("Uso: p-functions <tipo> <operacion> [argumentos]");
    puts("");
    puts("  Tipos:  dp | gtp | bp");
    puts("");
    puts("  encode <v1> [v2 ...]    vector -> alpha");
    puts("  decode <m> <alpha>      alpha -> vector");
    puts("  table  <size>           tabla 2D (dp y bp unicamente)");
    puts("");
    puts("Ejemplos:");
    puts("  p-functions dp  encode 2 3");
    puts("  p-functions dp  decode 2 17");
    puts("  p-functions gtp encode 0 2 5");
    puts("  p-functions gtp decode 3 8");
    puts("  p-functions bp  encode 1 2 1");
    puts("  p-functions bp  decode 3 21");
    puts("  p-functions dp  table 5");
    puts("  p-functions bp  table 5");
}

static void print_vector(const pf_int_t *V, size_t m) {
    printf("(");
    for (size_t i = 0; i < m; i++)
        printf("%" PRIu64 "%s", V[i], i + 1 < m ? ", " : "");
    printf(")\n");
}

// Parsea argv[0..n-1] como pf_int_t hacia V.
static size_t parse_vector(char **argv, int n, pf_int_t *V) {
    for (int i = 0; i < n; i++)
        V[i] = (pf_int_t)strtoull(argv[i], NULL, 10);
    return (size_t)n;
}

// ---------- encode ----------

static void encode_dp(char **argv, int n) {
    pf_int_t W[MAX_M];
    size_t m = parse_vector(argv, n, W);
    printf("%" PRIu64 "\n", pf_direct_dp(W, m));
}

static void encode_gtp(char **argv, int n) {
    pf_int_t V[MAX_M];
    size_t m = parse_vector(argv, n, V);
    printf("%" PRIu64 "\n", pf_direct_gtp(V, m));
}

static void encode_bp(char **argv, int n) {
    pf_int_t B[MAX_M];
    size_t m = parse_vector(argv, n, B);
    printf("%" PRIu64 "\n", pf_direct_bp(B, m));
}

// ---------- decode ----------

static void decode_dp(char **argv, int n) {
    if (n < 2) { fputs("decode: se necesita <m> <alpha>\n", stderr); return; }
    size_t m    = (size_t)strtoull(argv[0], NULL, 10);
    pf_int_t alpha = (pf_int_t)strtoull(argv[1], NULL, 10);
    pf_int_t W[MAX_M];
    pf_inverse_dp(alpha, m, W);
    print_vector(W, m);
}

static void decode_gtp(char **argv, int n) {
    if (n < 2) { fputs("decode: se necesita <m> <alpha>\n", stderr); return; }
    size_t m    = (size_t)strtoull(argv[0], NULL, 10);
    pf_int_t alpha = (pf_int_t)strtoull(argv[1], NULL, 10);
    pf_int_t V[MAX_M];
    pf_inverse_gtp(alpha, m, V);
    print_vector(V, m);
}

static void decode_bp(char **argv, int n) {
    if (n < 2) { fputs("decode: se necesita <m> <alpha>\n", stderr); return; }
    size_t m    = (size_t)strtoull(argv[0], NULL, 10);
    pf_int_t alpha = (pf_int_t)strtoull(argv[1], NULL, 10);
    pf_int_t B[MAX_M];
    pf_inverse_bp(alpha, m, B);
    print_vector(B, m);
}

// ---------- table 2D ----------

static void table_dp(pf_int_t size) {
    printf("      ");
    for (pf_int_t w2 = 0; w2 < size; w2++) printf("w2=%-5" PRIu64, w2);
    printf("\n");
    for (pf_int_t w1 = 0; w1 < size; w1++) {
        printf("w1=%" PRIu64 "  ", w1);
        for (pf_int_t w2 = 0; w2 < size; w2++)
            printf("%-8" PRIu64, pf_direct_dp_2d(w1, w2));
        printf("\n");
    }
}

static void table_bp(pf_int_t size) {
    printf("      ");
    for (pf_int_t b2 = 0; b2 < size; b2++) printf("b2=%-5" PRIu64, b2);
    printf("\n");
    for (pf_int_t b1 = 0; b1 < size; b1++) {
        printf("b1=%" PRIu64 "  ", b1);
        for (pf_int_t b2 = 0; b2 < size; b2++) {
            pf_int_t B[2] = {b1, b2};
            printf("%-8" PRIu64, pf_direct_bp(B, 2));
        }
        printf("\n");
    }
}

// ---------- main ----------

int main(int argc, char **argv) {
    if (argc < 2 || strcmp(argv[1], "help") == 0) {
        usage();
        return 0;
    }

    const char *tipo = argv[1];
    if (argc < 3) {
        fprintf(stderr, "Falta operacion. Usa 'help' para ver el uso.\n");
        return 1;
    }
    const char *op   = argv[2];
    char **rest      = argv + 3;
    int nrest        = argc - 3;

    if (strcmp(op, "encode") == 0) {
        if (nrest < 1) { fputs("encode: falta el vector\n", stderr); return 1; }
        if      (strcmp(tipo, "dp")  == 0) encode_dp(rest, nrest);
        else if (strcmp(tipo, "gtp") == 0) encode_gtp(rest, nrest);
        else if (strcmp(tipo, "bp")  == 0) encode_bp(rest, nrest);
        else { fprintf(stderr, "tipo desconocido: %s\n", tipo); return 1; }

    } else if (strcmp(op, "decode") == 0) {
        if (nrest < 2) { fputs("decode: faltan argumentos\n", stderr); return 1; }
        if      (strcmp(tipo, "dp")  == 0) decode_dp(rest, nrest);
        else if (strcmp(tipo, "gtp") == 0) decode_gtp(rest, nrest);
        else if (strcmp(tipo, "bp")  == 0) decode_bp(rest, nrest);
        else { fprintf(stderr, "tipo desconocido: %s\n", tipo); return 1; }

    } else if (strcmp(op, "table") == 0) {
        if (nrest < 1) { fputs("table: falta el tamaño\n", stderr); return 1; }
        pf_int_t size = (pf_int_t)strtoull(rest[0], NULL, 10);
        if      (strcmp(tipo, "dp")  == 0) table_dp(size);
        else if (strcmp(tipo, "bp")  == 0) table_bp(size);
        else { fprintf(stderr, "table solo disponible para dp y bp\n"); return 1; }

    } else {
        fprintf(stderr, "operacion desconocida: %s\n", op);
        usage();
        return 1;
    }

    return 0;
}
