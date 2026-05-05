#ifndef PACKING_ARITH_H
#define PACKING_ARITH_H

#include "packing_types.h"

// Maximo comun divisor (Euclides).
pf_int_t gcd_u64(pf_int_t a, pf_int_t b);

// C(k, r). Devuelve 0 si k < r..
pf_int_t pf_binomial(pf_int_t k, pf_int_t r);

// Dado p = C(k, r), devuelve C(k+1, r). Algoritmo 4.
pf_int_t pf_next_binomial(pf_int_t p, pf_int_t k, pf_int_t r);

// floor(sqrt(x)) por busqueda binaria.
pf_int_t pf_isqrt(pf_int_t x);

// floor(x^(1/i)) por busqueda binaria.
pf_int_t pf_iroot(pf_int_t x, pf_int_t i);

#endif