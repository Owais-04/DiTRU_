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
  poly x0,x1,x2,x3, x4,x5,x6;
  poly *a0 =&x0, *a1=&x1, *b0=&x2, *b1=&x3, *tmp1=&x4, *tmp2=&x5, *tmp3=&x6;
  int i;
  for(i=0;i<N;i++)
  {
    a0->coeffs[i] = a->coeffs[i];
    b0->coeffs[i] = b->coeffs[i];
    a1->coeffs[i] = a->coeffs[N+i];
    b1->coeffs[i] = b->coeffs[N+i];
  }
    poly_Rq_mul(tmp1, a0,b0);
    rotinv(a1,tmp3);
    poly_Rq_mul(tmp2,tmp3,b1);

    for(i=0;i<N;i++)
      {
        r->coeffs[i] = MODQ(tmp1->coeffs[i]+tmp2->coeffs[i]);
      }

    poly_Rq_mul(tmp1, a1,b0);
    rotinv(a0,tmp3);
    poly_Rq_mul(tmp2,tmp3,b1);

     for(i=0;i<N;i++)
      {
        r->coeffs[N+i] = MODQ(tmp1->coeffs[i]+tmp2->coeffs[i]);
      }



  // // The multiplication of DiTRU
  // int i=0, j=0;
  // for(i=0;i<ORDER;i++){
  //   r->coeffs[i]=0;
  // }
  // for(i=0;i<N;i++){
  //   for(j=0;j<N;j++){
  //     r->coeffs[(i+j)%N]= MODQ(r->coeffs[(i+j)%N] + MODQ(a->coeffs[i]*b->coeffs[j])); 
  //   }
  // }
  // for(i=0;i<N;i++){
  //   for(j=0;j<N;j++){
  //     r->coeffs[N+(N+j-i)%N]= MODQ(r->coeffs[N+(N+j-i)%N] + MODQ(a->coeffs[i]*b->coeffs[j+N])); 
  //   }
  // }
  // for(i=0;i<N;i++){
  //   for(j=0;j<N;j++){
  //     r->coeffs[N+(i+j)%N]= MODQ(r->coeffs[N+(i+j)%N] + MODQ(a->coeffs[i+N]*b->coeffs[j])); 
  //   }
  // }
  // for(i=0;i<N;i++){
  //   for(j=0;j<N;j++){
  //     r->coeffs[(N-i+j)%N]= MODQ(r->coeffs[(N-i+j)%N] + MODQ(a->coeffs[i+N]*b->coeffs[j+N])); 
  //   }
  // }
}