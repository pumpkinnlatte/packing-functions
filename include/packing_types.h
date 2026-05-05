/*
 * packing_types.h
 * Tipos y constantes base del proyecto de packing functions.
 */

#ifndef PACKING_TYPES_H
#define PACKING_TYPES_H

#include <stdint.h>
#include <stddef.h>

/* Tipo entero abstracto para toda la libreria.
 * Hoy: uint64_t.  Manana: posiblemente mpz_t (GMP). */
typedef uint64_t pf_int_t;

#define PF_INT_MAX UINT64_MAX

/* Codigos de estado uniformes para todas las operaciones. */
typedef enum {
    PF_OK = 0,
    PF_ERR_OVERFLOW,        /* el resultado no cabe en pf_int_t   */
    PF_ERR_INVALID_INPUT,   /* parametros fuera de dominio        */
    PF_ERR_NULL_POINTER,    /* puntero de salida nulo             */
    PF_ERR_DOMAIN           /* p.ej. vector no estrictamente
                               creciente cuando se exigia           */
} pf_status_t;

/* Vector de m componentes en N (enteros no negativos).
 * Se usa tanto para entradas como salidas de las packing functions. */
typedef struct {
    pf_int_t *data;
    size_t    m;
} pf_vector_t;

/* Devuelve un literal humano para un codigo de estado.
 * Util en mensajes de error y tests. */
const char *pf_status_str(pf_status_t s);

#endif /* PACKING_TYPES_H */
