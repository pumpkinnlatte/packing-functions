#ifndef PACKING_DP_H
#define PACKING_DP_H

#include "packing_types.h"

// Algoritmo 6: vector W -> alpha.
// W es un vector de m componentes en N.
pf_int_t pf_direct_dp(const pf_int_t *W, size_t m);

// Caso 2D del paper: alpha = w1 + C(1 + w1 + w2, 2).
pf_int_t pf_direct_dp_2d(pf_int_t w1, pf_int_t w2);

// Inverso 2D del paper: alpha -> (w1, w2).
// Escribe el resultado en W (que debe tener al menos 2 elementos).
void pf_inverse_dp_2d(pf_int_t alpha, pf_int_t *W);

#endif
