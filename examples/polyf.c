/**
 * @file polyf.c
 * @brief Command-line interface for evaluating packing functions.
 *
 * Computes one packing-function operation per invocation. Either encodes a
 * vector into a non-negative integer, or decodes a non-negative integer into
 * a vector, depending on the direction specified by -FROM and -TO.
 
 *   polyf  ::= "./polyf" flag+
 *   flag   ::= "-TYPE" type | "-FROM" dir | "-TO" dir
 *            | "-DIM" integer | "-INPUT" value+
 *   type   ::= "DP" | "BP" | "GTP"
 *   dir    ::= "int" | "poly"
 *   value  ::= non-negative integer
 *
 * -TYPE   Selects the packing function.
 *           DP  (Diagonal Polynomial)  -- unrestricted vectors.
 *           BP  (Box Polynomial)       -- unrestricted vectors, faster than DP.
 *           GTP (Greater-Than Polynomial) -- strictly increasing vectors.
 *
 * -FROM   Domain of the input.
 *           int  -- a single non-negative integer alpha.
 *           poly -- a vector of m non-negative integers.
 *
 * -TO     Domain of the output; must be the complement of -FROM.
 *           int  -- prints the encoded integer.
 *           poly -- prints the decoded vector.
 *
 * -DIM    Number of vector dimensions m, with m >= 2.
 *
 * -INPUT  The input data.
 *           When -FROM int:  exactly one integer alpha.
 *           When -FROM poly: exactly m integers, one per dimension.
 *         Consumes all following tokens until the next flag or end of arguments.
 *
 * Case Uses:
 *   ./polyf -TYPE GTP -FROM poly -TO int  -DIM 3 -INPUT 0 2 5
 *   ./polyf -TYPE GTP -FROM int  -TO poly -DIM 3 -INPUT 8
 *   ./polyf -TYPE DP  -FROM poly -TO int  -DIM 2 -INPUT 2 3
 *   ./polyf -TYPE BP  -FROM int  -TO poly -DIM 3 -INPUT 21
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "gtp.h"
#include "dp.h"
#include "bp.h"

#define MAX_DIM 64

// ------------------------------------------------------------------ types 

typedef enum { TYPE_NONE, TYPE_DP, TYPE_BP, TYPE_GTP } poly_type_t;
typedef enum { DIR_NONE, DIR_INT, DIR_POLY }           direction_t;

// ------------------------------------------------------------ arg parsing

static poly_type_t parse_type(const char *s) {
    if (strcmp(s, "DP")  == 0) return TYPE_DP;
    if (strcmp(s, "BP")  == 0) return TYPE_BP;
    if (strcmp(s, "GTP") == 0) return TYPE_GTP;
    return TYPE_NONE;
}

static direction_t parse_dir(const char *s) {
    if (strcmp(s, "int")  == 0) return DIR_INT;
    if (strcmp(s, "poly") == 0) return DIR_POLY;
    return DIR_NONE;
}

// Returns 1 if s looks like a flag. 
static int is_flag(const char *s) {
    return s[0] == '-' && s[1] != '\0' && (s[1] < '0' || s[1] > '9');
}

// ------------------------------------------------------------------ output

static void print_vector(const pf_int_t *V, size_t m) {
    for (size_t i = 0; i < m; i++)
        printf("%" PRIu64 "%s", V[i], i + 1 < m ? " " : "\n");
}

// --------------------------------------------------------- error handling 

static void die(const char *msg) {
    fprintf(stderr, "polyf: %s\n", msg);
    exit(1);
}

static void usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s -TYPE <DP|BP|GTP> -FROM <int|poly> -TO <int|poly>"
        " -DIM <m> -INPUT <value...>\n"
        "\n"
        "  -TYPE   DP | BP | GTP\n"
        "  -FROM   int (single alpha) | poly (m-component vector)\n"
        "  -TO     int | poly  (opposite of -FROM)\n"
        "  -DIM    number of dimensions, m >= 2\n"
        "  -INPUT  one integer when -FROM int; m integers when -FROM poly\n"
        "\n"
        "Examples:\n"
        "  %s -TYPE GTP -FROM poly -TO int  -DIM 3 -INPUT 0 2 5\n"
        "  %s -TYPE GTP -FROM int  -TO poly -DIM 3 -INPUT 8\n"
        "  %s -TYPE DP  -FROM poly -TO int  -DIM 2 -INPUT 2 3\n"
        "  %s -TYPE BP  -FROM int  -TO poly -DIM 3 -INPUT 21\n",
        prog, prog, prog, prog, prog);
    exit(1);
}

/* ------------------------------------------------------------------- main */

int main(int argc, char **argv) {
    if (argc < 2) usage(argv[0]);

    poly_type_t type    = TYPE_NONE;
    direction_t from    = DIR_NONE;
    direction_t to      = DIR_NONE;
    size_t      dim     = 0;
    pf_int_t    input[MAX_DIM];
    int         n_input = 0;
    int         has_dim = 0;

    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-TYPE") == 0) {
            if (++i >= argc) die("missing value for -TYPE");
            type = parse_type(argv[i]);
            if (type == TYPE_NONE) die("unknown TYPE; use DP, BP, or GTP");

        } else if (strcmp(argv[i], "-FROM") == 0) {
            if (++i >= argc) die("missing value for -FROM");
            from = parse_dir(argv[i]);
            if (from == DIR_NONE) die("unknown direction for -FROM; use int or poly");

        } else if (strcmp(argv[i], "-TO") == 0) {
            if (++i >= argc) die("missing value for -TO");
            to = parse_dir(argv[i]);
            if (to == DIR_NONE) die("unknown direction for -TO; use int or poly");

        } else if (strcmp(argv[i], "-DIM") == 0) {
            if (++i >= argc) die("missing value for -DIM");
            dim = (size_t)strtoull(argv[i], NULL, 10);
            has_dim = 1;

        } else if (strcmp(argv[i], "-INPUT") == 0) {
            while (i + 1 < argc && !is_flag(argv[i + 1])) {
                if (n_input >= MAX_DIM) die("too many input values");
                input[n_input++] = (pf_int_t)strtoull(argv[++i], NULL, 10);
            }

        } else {
            fprintf(stderr, "polyf: unknown flag '%s'\n", argv[i]);
            usage(argv[0]);
        }
    }

    // Validate that all required flags were provided. 
    if (type    == TYPE_NONE) die("missing -TYPE");
    if (from    == DIR_NONE)  die("missing -FROM");
    if (to      == DIR_NONE)  die("missing -TO");
    if (!has_dim)             die("missing -DIM");
    if (n_input == 0)         die("missing -INPUT");

    if (from == to)  die("-FROM and -TO must differ");
    if (dim < 2)     die("-DIM must be >= 2");

    // poly to int 
    if (from == DIR_POLY && to == DIR_INT) {
        if ((size_t)n_input != dim) {
            fprintf(stderr,
                "polyf: -DIM is %zu but %d input values were given\n", dim, n_input);
            return 1;
        }
        pf_int_t alpha;
        switch (type) {
            case TYPE_GTP: alpha = pf_direct_gtp(input, dim); break;
            case TYPE_DP:  alpha = pf_direct_dp(input, dim);  break;
            case TYPE_BP:  alpha = pf_direct_bp(input, dim);  break;
            default:       return 1;
        }
        printf("%" PRIu64 "\n", alpha);

    // int to poly
    } else {
        if (n_input != 1) {
            fprintf(stderr,
                "polyf: -FROM int requires exactly one input value, got %d\n", n_input);
            return 1;
        }
        pf_int_t out[MAX_DIM];
        pf_int_t alpha = input[0];
        switch (type) {
            case TYPE_GTP: pf_inverse_gtp(alpha, dim, out); break;
            case TYPE_DP:  pf_inverse_dp(alpha, dim, out);  break;
            case TYPE_BP:  pf_inverse_bp(alpha, dim, out);  break;
            default:       return 1;
        }
        print_vector(out, dim);
    }

    return 0;
}
