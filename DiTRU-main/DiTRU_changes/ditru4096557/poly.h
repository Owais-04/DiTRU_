#ifndef POLY_H
#define POLY_H

#include "params.h"

#include <stddef.h>
#include <stdint.h>

#define MODQ(X) ((X) & (Q-1))

typedef struct{
  uint16_t coeffs[ORDER];
} poly;

typedef struct{
  uint16_t coeffs[N];
} cyclic_poly;



#define poly_mod_3 CRYPTO_NAMESPACE(poly_mod_3) //new
#define poly_mod_2 CRYPTO_NAMESPACE(poly_mod_2) //new
#define poly_centerlifting CRYPTO_NAMESPACE(poly_centerlifting) //new


void poly_mod_3(poly *r);//new
void poly_mod_2(poly *r);//new
void poly_centerlifting(poly *r); //new

#define poly_Sq_tobytes CRYPTO_NAMESPACE(poly_Sq_tobytes)
#define poly_Sq_frombytes CRYPTO_NAMESPACE(poly_Sq_frombytes)
void poly_Sq_tobytes(unsigned char *r, const poly *a);
void poly_Sq_frombytes(poly *r, const unsigned char *a);



#define poly_S3_tobytes CRYPTO_NAMESPACE(poly_S3_tobytes)
#define poly_S3_frombytes CRYPTO_NAMESPACE(poly_S3_frombytes)
void poly_S3_tobytes(unsigned char msg[PACK_TRINARY_BYTES], const poly *a);
void poly_S3_frombytes(poly *r, const unsigned char msg[PACK_TRINARY_BYTES]);


#define poly_Rq_mul CRYPTO_NAMESPACE(poly_Rq_mul)
#define rotinv CRYPTO_NAMESPACE(rotinv)
void rotinv(poly *u, poly *u1);
void poly_Rq_mul(poly *r, const poly *a, const poly *b);




#define poly_Rq_DiTRU_mul CRYPTO_NAMESPACE(poly_Rq_DiTRU_mul) //new
void poly_Rq_DiTRU_mul(poly *r, const poly *a, const poly *b); //new

#define poly_R2_inv CRYPTO_NAMESPACE(poly_R2_inv)
#define poly_R2_inv_v2 CRYPTO_NAMESPACE(poly_R2_inv_v2) //new

#define poly_Rq_inv CRYPTO_NAMESPACE(poly_Rq_inv)
#define poly_Rq_inv_v2 CRYPTO_NAMESPACE(poly_Rq_inv_v2) //new

#define poly_Rq_inv_DiTRU CRYPTO_NAMESPACE(poly_Rq_inv_DiTRU) //new

void poly_R2_inv(poly *r, const poly *a);

void poly_R2_inv_v2(poly *r, const poly *a); //new 
void poly_Rq_inv_v2(poly *r, const poly *a);

void poly_Rq_inv(poly *r, const poly *a);
void poly_Rq_inv_DiTRU(poly *r, const poly *a);

#define poly_Z3_to_Zq CRYPTO_NAMESPACE(poly_Z3_to_Zq)
#define poly_trinary_Zq_to_Z3 CRYPTO_NAMESPACE(poly_trinary_Zq_to_Z3)
void poly_Z3_to_Zq(poly *r);
void poly_trinary_Zq_to_Z3(poly *r);
#endif
