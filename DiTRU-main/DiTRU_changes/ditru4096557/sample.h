#ifndef SAMPLE_H
#define SAMPLE_H

#include "params.h"
#include "poly.h"

#include "crypto_sort_int32.h"





#define sample_iid CRYPTO_NAMESPACE(sample_iid)
void sample_iid(poly *r, const unsigned char uniformbytes[SAMPLE_IID_BYTES]);


#define sample_fixed_type CRYPTO_NAMESPACE(sample_fixed_type)
void sample_fixed_type(poly *r, const unsigned char uniformbytes[SAMPLE_FT_BYTES], int d);

#define sample_r CRYPTO_NAMESPACE(sample_r) //new
void sample_r(poly *r, const poly *h, const poly *m); //new


#define sample_mask CRYPTO_NAMESPACE(sample_mask) //new
void sample_mask(poly *mask, unsigned char t_seed[LENGTH_OF_HASH]); //new

#endif
