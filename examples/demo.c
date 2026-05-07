// demo.c — ejecuta los algoritmos del paper con ejemplos visuales.

#include <stdio.h>
#include <inttypes.h>
#include "arith.h"

static void hr(void) {
    printf("------------------------------------------------------------\n");
}

// demo 1. pf_binomial paso a paso 
// Calcula el algoritmo iteracion por iteracion.
static void demo_binomial_steps(pf_int_t k, pf_int_t r) {
    printf("calculando C(%" PRIu64 ", %" PRIu64 "):\n", k, r);
    printf("  i=0 (inicial)              b = 1\n");

    pf_int_t b = 1;
    pf_int_t rr = (r > k - r) ? k - r : r;   // refleja la simetria interna
    for (pf_int_t i = 1; i <= rr; i++) {
        pf_int_t num = k - i + 1;
        pf_int_t den = i;
        pf_int_t prev = b;
        b = b * num / den;
        printf("  i=%" PRIu64 "  b = %" PRIu64 " * %" PRIu64 " / %" PRIu64
               " = %" PRIu64 "\n",
               i, prev, num, den, b);
    }
    printf("  resultado: C(%" PRIu64 ", %" PRIu64 ") = %" PRIu64 "\n",
           k, r, pf_binomial(k, r));
}

// demo 2. pf_next_binomial recorriendo una columna 
// Se empieza en C(r, r) = 1 y se baja por la columna r
// usando next_binomial en cada salto. Ilustrativo.
static void demo_next_binomial(pf_int_t r, pf_int_t hasta) {
    printf("recorriendo la columna r=%" PRIu64 " del triangulo de Pascal\n", r);
    printf("usando solo pf_next_binomial:\n");

    pf_int_t k = r;
    pf_int_t b = 1;                         // C(r, r) = 1
    printf("  C(%" PRIu64 ", %" PRIu64 ") = %" PRIu64 "\n", k, r, b);

    while (k < hasta) {
        b = pf_next_binomial(b, k, r);
        k++;
        printf("  C(%" PRIu64 ", %" PRIu64 ") = %" PRIu64 "\n", k, r, b);
    }
}

// demo 3. tabla DP 2D 
// Reproduce una matriz n x n aplicando la formula
//   alpha = w1 + C(1 + w1 + w2, 2) con pf_binomial.
static void demo_dp_table(pf_int_t size) {
    printf("tabla DP en 2D (alpha = w1 + C(1+w1+w2, 2)):\n\n");

    printf("         ");
    for (pf_int_t w2 = 0; w2 < size; w2++) printf("w2=%" PRIu64 "   ", w2);
    printf("\n");

    for (pf_int_t w1 = 0; w1 < size; w1++) {
        printf("  w1=%" PRIu64 "    ", w1);
        for (pf_int_t w2 = 0; w2 < size; w2++) {
            pf_int_t alpha = w1 + pf_binomial(1 + w1 + w2, 2);
            printf("%4" PRIu64 "   ", alpha);
        }
        printf("\n");
    }
}

// demo 4. vector concreto
static void demo_dp_vector(pf_int_t w1, pf_int_t w2) {
    pf_int_t d = w1 + w2;
    pf_int_t before = pf_binomial(d + 1, 2);
    pf_int_t alpha = w1 + before;

    printf("DP(w1=%" PRIu64 ", w2=%" PRIu64 "):\n", w1, w2);
    printf("  diagonal d = w1 + w2 = %" PRIu64 "\n", d);
    printf("  celdas antes de esta diagonal: C(%" PRIu64 ", 2) = %" PRIu64 "\n",
           d + 1, before);
    printf("  posicion dentro de la diagonal: w1 = %" PRIu64 "\n", w1);
    printf("  alpha = w1 + C(d+1, 2) = %" PRIu64 " + %" PRIu64
           " = %" PRIu64 "\n", w1, before, alpha);
}

int main(void) {
    hr();
    printf("DEMO 1: pf_binomial paso a paso\n");
    hr();
    demo_binomial_steps(5, 3);
    printf("\n");
    demo_binomial_steps(10, 4);

    printf("\n");
    hr();
    printf("DEMO 2: pf_next_binomial recorriendo una columna\n");
    hr();
    demo_next_binomial(2, 8);
    printf("\n");
    demo_next_binomial(3, 9);

    printf("\n");
    hr();
    printf("DEMO 3: tabla DP 2D\n");
    hr();
    demo_dp_table(5);

    printf("\n");
    hr();
    printf("DEMO 4: diseccion de vectores concretos\n");
    hr();
    demo_dp_vector(2, 3);
    printf("\n");
    demo_dp_vector(0, 4);
    printf("\n");
    demo_dp_vector(4, 0);

    return 0;
}
