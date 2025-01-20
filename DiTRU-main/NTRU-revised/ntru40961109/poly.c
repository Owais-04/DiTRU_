#include "poly.h"

/* Map {0, 1, 2} -> {0,1,q-1} in place */
void poly_Z3_to_Zq(poly *r)
{
  int i;
  for(i=0; i<ORDER; i++)
    r->coeffs[i] = r->coeffs[i] | ((-(r->coeffs[i]>>1)) & (Q-1));
}

/* Map {0, 1, q-1} -> {0,1,2} in place */
void poly_trinary_Zq_to_Z3(poly *r)
{
  int i;
  for(i=0; i<ORDER; i++)
  {
    r->coeffs[i] = MODQ(r->coeffs[i]);
    r->coeffs[i] = 3 & (r->coeffs[i] ^ (r->coeffs[i]>>(LOGQ-1)));
  }
}



static void poly_R2_inv_to_Rq_inv(poly *r, const poly *ai, const poly *a)
{

  //Take care N is the order of the cyclic group and half the order for dihedral
#if Q <= 256 || Q >= 65536
#error "poly_R2_inv_to_Rq_inv in poly.c assumes 256 < q < 65536"
#endif

  int i;
  poly b, c;
  poly s;


  for(i=0; i<N; i++)
    b.coeffs[i] = -(a->coeffs[i]);

  for(i=0; i<N; i++)
    r->coeffs[i] = ai->coeffs[i];
  
  // for 0..4
  //    ai = ai * (2 - a*ai)  mod q
  poly_Rq_mul(&c, r, &b);
  c.coeffs[0] += 2; // c = 2 - a*ai
  poly_Rq_mul(&s, &c, r); // s = ai*c

  poly_Rq_mul(&c, &s, &b);
  c.coeffs[0] += 2; // c = 2 - a*s
  poly_Rq_mul(r, &c, &s); // r = s*c

  poly_Rq_mul(&c, r, &b);
  c.coeffs[0] += 2; // c = 2 - a*r
  poly_Rq_mul(&s, &c, r); // s = r*c

  poly_Rq_mul(&c, &s, &b);
  c.coeffs[0] += 2; // c = 2 - a*s
  poly_Rq_mul(r, &c, &s); // r = s*c
}

void poly_Rq_inv(poly *r, const poly *a)
{
  poly ai2;
  poly_R2_inv(&ai2, a);
  poly_R2_inv_to_Rq_inv(r, &ai2, a);
}


void poly_Rq_inv_v2(poly *r, const poly *a)
{
  poly ai2;
  poly_R2_inv_v2(&ai2, a);
  poly_R2_inv_to_Rq_inv(r, &ai2, a);
}
