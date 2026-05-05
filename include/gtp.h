#ifndef PACKING_GTP_H
#define PACKING_GTP_H

#include "packing_types.h"

// Algoritmo 1: vector V -> alpha.
// V debe cumplir 0 <= v_1 < v_2 < ... < v_m.
pf_int_t pf_direct_gtp(const pf_int_t *V, size_t m);

// Algoritmo 3: alpha -> vector V.
// Escribe el resultado en V (debe tener al menos m elementos).
void pf_inverse_gtp(pf_int_t alpha, size_t m, pf_int_t *V);

// Biyeccion GTP -> DP: w_1 = v_1, w_i = v_i - v_{i-1} - 1 para i > 1.
void pf_gtp_to_dp(const pf_int_t *V, size_t m, pf_int_t *W);

// Biyeccion DP -> GTP: v_i = (i - 1) + (w_1 + w_2 + ... + w_i).
void pf_dp_to_gtp(const pf_int_t *W, size_t m, pf_int_t *V);

// Algoritmo 5: AllSubsets(k, t).
//
// Enumera todos los vectores GTP de tamaño t con componentes en
// [0, k-1] estrictamente crecientes. Hay C(k, t) en total.
//
typedef void (*pf_subset_cb)(const pf_int_t *V, size_t t, void *user);

void pf_all_subsets(pf_int_t k, size_t t, pf_subset_cb cb, void *user);

#endif