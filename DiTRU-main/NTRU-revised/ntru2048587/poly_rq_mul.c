#include "poly.h"

void poly_Rq_mul(poly *r, const poly *a, const poly *b)
{
  int k,i;

  for(k=0; k<N; k++)
  {
    r->coeffs[k] = 0;
    for(i=1; i<N-k; i++)
      r->coeffs[k] += a->coeffs[k+i] * b->coeffs[N-i];
    for(i=0; i<k+1; i++)
      r->coeffs[k] += a->coeffs[k-i] * b->coeffs[i];
    r->coeffs[k] = MODQ(r->coeffs[k]);
  }
}


void poly_Rq_DiTRU_mul(poly *r, const poly *a, const poly *b)
{
  // The multiplication of DiTRU
}