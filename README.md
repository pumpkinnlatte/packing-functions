# gtp-implementation
Implementacion didactica de las funciones de empaquetamiento estudiadas
en Torres-Jimenez et al. (2015) y Fraenkel (1983).
 
Los 8 algoritmos del paper estan implementados, documentados y
testeados con invariantes de biyeccion.
 
| # | Algoritmo                  |
|---|----------------------------|
| 1 | DirectGTP                  | 
| 2 | Binomial                   |
| 3 | InverseGTP                 | 
| 4 | NextBinomial               | 
| 5 | AllSubsets                 | 
| 6 | DirectDP                   |
| 7 | DirectBP                   | 
| 8 | InverseBP                  | 
 
Adicionales: InverseDP 2D (formula cerrada), InverseDP m-dimensional
(via biyeccion GTP <-> DP), gtp_to_dp y dp_to_gtp.
 
## Estructura
 
    include/    headers (arith.h, dp.h, gtp.h, bp.h, packing_types.h)
    src/        implementacion
    tests/      suite de pruebas
    examples/   programas demostrativos
    build/      artefactos (creado por make)
 
## Uso
 
    make                                    # compila todo
    make test                               # corre suite con resumen
    make demo                               # ejecuta examples/demo
    make cli ARGS="dp encode 2 3"           # interfaz por linea de comandos
    make debug                              
    make clean
 
### CLI interactiva
 
El programa `demo_cli` permite ejecutar los polinomios por linea de comandos.
 
Ejemplos:
 
    ./build/demo_cli dp encode 2 3          # vector DP -> alpha (= 17)
    ./build/demo_cli dp decode 2 17         # alpha -> vector DP (= (2,3))
    ./build/demo_cli gtp encode 0 2 5       # vector GTP -> alpha
    ./build/demo_cli gtp decode 3 8         # alpha -> vector GTP
    ./build/demo_cli bp encode 1 2 1        # vector BP -> alpha (= 21)
    ./build/demo_cli bp decode 3 21         # alpha -> vector BP (= (1,2,1))
    ./build/demo_cli dp table 6             # imprime tabla 6x6 de DP
    ./build/demo_cli help                   # ayuda completa

## Limites del tipo entero

Todo opera sobre `pf_int_t = uint64_t` (rango: 0 a 2^64 - 1).
Se tienen limites:

**`pf_binomial`**: el Algoritmo 2 multiplica antes de dividir. Los
intermedios pueden desbordar uint64_t.

**`pf_direct_bp`**: phi_1 = beta^m y phi_2 = (beta+1)^m crecen muy
rapido. Para m=10, beta = 100 ya excede 2^64.

**`pf_inverse_gtp`**: usa f * alpha donde f = m! puede ser grande.
