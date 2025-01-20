#include "poly.h"
#include<stdio.h>
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

  // for 0..4
  //    ai = ai * (2 - a*ai)  mod q
  for(i=0; i<N; i++)
    b.coeffs[i] = -(a->coeffs[i]);

  for(i=0; i<N; i++)
    r->coeffs[i] = ai->coeffs[i];

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




/*dihedral*/
void rotinv(poly *u, poly *u1){
  u1->coeffs[0]=u->coeffs[0];
  for(int i=1;i<N;i++){
    u1->coeffs[i] = u->coeffs[N-i];
  }
}

void get_minus(poly *u){
  for(int i=0;i<N;i++){
    u->coeffs[i] = MODQ(Q-u->coeffs[i]);
  }
}

void poly_Rq_inv_DiTRU(poly *r, const poly *a){

  //printf("Dihedral is here\n");
  poly x1, x2, x3, x4, x5, x6;
  poly *u=&x1, *v=&x2;
  poly *u1=&x3, *v1=&x4;
  poly *uu1=&x5, *vv1=&x6;
  for(int i=0;i<N;i++){
    u->coeffs[i]=a->coeffs[i];
  }
  for(int i=N;i<ORDER;i++){
    v->coeffs[i-N]=a->coeffs[i];
  }
  rotinv(u,u1);
  rotinv(v,v1);
  poly_Rq_mul(uu1,u,u1);
  poly_Rq_mul(vv1,v,v1);
  poly *c = u;
  for(int i=0;i<N;i++){
    c->coeffs[i]= MODQ(vv1->coeffs[i]-uu1->coeffs[i]);
  }
  poly *c_inv = uu1;
  poly_Rq_inv(c_inv,c);
  poly *b = vv1;
  poly_Rq_mul(b,v,c_inv);
  poly *umin = u1;
  get_minus(umin);
  poly_Rq_mul(v1,umin,c_inv);
  for(int i=0;i<N;i++){
    r->coeffs[i]=v1->coeffs[i];
  }  
  for(int i=N;i<(ORDER);i++){
    r->coeffs[i]=b->coeffs[i-N];
  }
}
