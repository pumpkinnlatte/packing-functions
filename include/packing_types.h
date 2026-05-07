/*
 * packing_types.h
 * Tipos y constantes base del proyecto de packing functions.
 */

#ifndef PACKING_TYPES_H
#define PACKING_TYPES_H

#include <stdint.h>
#include <stddef.h>

/* Tipo entero.
 * La idea es despues actualizarlo a uno que permita manejar enteros mas grandes 
 */
typedef uint64_t pf_int_t;

#define PF_INT_MAX UINT64_MAX

// Codigos de estado para operaciones. 
typedef enum {
    PF_OK = 0,
    PF_ERR_OVERFLOW,        // el resultado no cabe en pf_int_t   
    PF_ERR_INVALID_INPUT,   // parametros fuera de dominio        
    PF_ERR_NULL_POINTER,    // puntero de salida nulo             
    PF_ERR_DOMAIN           // p.ej. vector no estrictamente
                            //   creciente cuando se exigia           
} pf_status_t;

/* Vector de m componentes en N (enteros no negativos).
 * Se usa tanto para entradas como salidas de las packing functions. 
 */
typedef struct {
    pf_int_t *data;
    size_t    m;
} pf_vector_t;

const char *pf_status_str(pf_status_t s);

#endif /* PACKING_TYPES_H */
