#include "dp.h"
#include "arith.h"

// Caso 2D del paper:
//   alpha = w1 + C(1 + w1 + w2, 2)
pf_int_t pf_direct_dp_2d(pf_int_t w1, pf_int_t w2) {
    return w1 + pf_binomial(1 + w1 + w2, 2);
}

// Algoritmo 6 del paper: DirectDP(W).
//
//   s := 0, alpha := 0
//   for i := 1 to m:
//       s := s + w_i
//       alpha := alpha + C(i - 1 + s, i)
//   return alpha
//
// La suma acumulada s = w_1 + w_2 + ... + w_i se mantiene a lo largo
// del bucle y se reutiliza en cada termino.
pf_int_t pf_direct_dp(const pf_int_t *W, size_t m) {
    pf_int_t s = 0;
    pf_int_t alpha = 0;
    for (size_t i = 1; i <= m; i++) {
        s += W[i - 1];                          // W[i-1] es w_i en la notacion del paper
        alpha += pf_binomial(i - 1 + s, i);
    }
    return alpha;
}

// Inverso 2D del paper.
//
// Procedimiento del paper:
//   d := floor( (sqrt(1 + 8*alpha) - 1) / 2 )
//   w1 := alpha - d*(d+1)/2
//   w2 := d - w1
//
// d es la diagonal a la que pertenece (w1, w2).
void pf_inverse_dp_2d(pf_int_t alpha, pf_int_t *W) {
    pf_int_t d = (pf_isqrt(1 + 8 * alpha) - 1) / 2;
    pf_int_t w1 = alpha - d * (d + 1) / 2;
    pf_int_t w2 = d - w1;
    W[0] = w1;
    W[1] = w2;
}
