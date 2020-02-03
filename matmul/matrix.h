#ifndef __MATRIX_H__
#define __MATRIX_H__

/***********************************************************************
 *   Includes                                                          *
 ***********************************************************************/
#include <stdint.h>
#include "config.h"


/***********************************************************************
 *   Defines                                                           *
 ***********************************************************************/
#define MATRIX_ACCESS(n, i, j) \
    ((i) * (n) + (j))


/***********************************************************************
 *   Prototypes of exported functions                                  *
 ***********************************************************************/
extern fpv_t *
matrix_Alloc(uint32_t dimX, uint32_t dimY);

extern void
matrix_Free(fpv_t **mat);

extern double
matrix_MulQuad0(const fpv_t *a,
                const fpv_t *b,
                fpv_t      *c,
                uint32_t    n);

extern double
matrix_MulQuad1(const fpv_t *a,
                const fpv_t *b,
                fpv_t      *c,
                uint32_t    n);

extern double
matrix_MulQuad2(const fpv_t *a,
                const fpv_t *b,
                fpv_t      *c,
                uint32_t    n);

extern double
matrix_MulQuad3(const fpv_t *a,
                const fpv_t *b,
                fpv_t      *c,
                uint32_t    n);

extern double
matrix_MulQuad4(const fpv_t *a,
                const fpv_t *b,
                fpv_t      *c,
                uint32_t    n);

extern double
matrix_MulQuad5(const fpv_t *a,
                const fpv_t *b,
                fpv_t      *c,
                uint32_t    n);


#endif /* __MATRIX_H__ */
