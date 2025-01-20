#include "ppke.h"
#include "poly.h"
#include "sample.h"
#include "helper.h"
#include "rng.h"
#include "crypto_hash_sha3256.h"
#include <stdio.h>
#include <time.h>



int ppke_keypair(unsigned char *pk,
                   unsigned char *sk,
                   unsigned char seed[SAMPLE_FG_BYTES])
{
  int i;
  int loop=0;
  poly x1, x2, x3, x4, x5;

  poly *f=&x1, *g=&x2, *invf=&x3, *modified_f=&x4;
  poly *h=&x4, *tmp=&x5;
  
  
  while (1)
  {

  loop++;
  //printf("inside the loop");  
  sample_fixed_type(f,seed,d_f);

  // int count1=0, count2=0;
  // for(i=0;i<ORDER;i++)
  // {
  //   if(f->coeffs[i]==1)
  //   {
  //     count1++;
  //   }
  //   else if (f->coeffs[i]==2)
  //   {
  //     count2++;
  //   }
    
  // }
  // printf("\n count1 %d", count1);
  // printf("\n count2 %d", count2);

  /* 3F+1*/
  for(i=0;i<ORDER;i++)
    modified_f->coeffs[i] = f->coeffs[i]; 
 
  poly_Z3_to_Zq(modified_f);
  for(i=0;i<ORDER;i++)
    modified_f->coeffs[i] = MODQ(3*modified_f->coeffs[i]);
  modified_f->coeffs[0] = MODQ(modified_f->coeffs[0]+1);
  
  //print_polynomial_array(ORDER, modified_f);
  #ifdef NTRU
  poly_Rq_inv(invf,modified_f);
  poly_Rq_mul(tmp,modified_f,invf);
  #endif

  #ifdef DiTRU
  poly_Rq_inv_DiTRU(invf,modified_f);
  poly_Rq_DiTRU_mul(tmp,modified_f,invf);

  #endif
  int inverse_exist=1;
    for(int i=1;i<ORDER;i++){
      if(tmp->coeffs[i]!=0){
        inverse_exist=0;
        break;
      }
    }
    if(tmp->coeffs[0]!=1){
      inverse_exist=0;
    }
    if(inverse_exist){
      break;
    }
  randombytes(seed, SAMPLE_FG_BYTES);
  }

  
  poly_S3_tobytes(sk, f);
  sample_fixed_type(g,seed+SAMPLE_FT_BYTES,d_g);
  poly_Z3_to_Zq(g);

 
  
  /* 3g */
   for(i=0; i<ORDER;i++)
  {
    g->coeffs[i] = MODQ(3*g->coeffs[i]);
  }


   /* h = 3g*f^-1*/

  #ifdef NTRU 
  poly_Rq_mul(h,g,invf);
  #endif

  #ifdef DiTRU
  poly_Rq_DiTRU_mul(h,g,invf);
  #endif
  /* pack the public key*/
  poly_Sq_tobytes(pk,h);

 

  /* Do unpacking and check*/
  // poly_Sq_frombytes(h,pk);
  // printf("\n check h: ");
  // print_polynomial_array(ORDER,h);

  // poly_S3_frombytes(f,sk);
  // printf("\n check f");
  // print_polynomial_array(ORDER,f);

  // poly_Z3_to_Zq(f);
  // for(i=0;i<ORDER;i++)
  //   f->coeffs[i] = MODQ(3*f->coeffs[i]);
  // f->coeffs[0] = MODQ(f->coeffs[0]+1);

  // poly_Rq_mul(tmp, h,f);
  // printf("\n suppose to be 3g");
  // print_polynomial_array(ORDER,tmp);
  // poly_centerlifting(tmp);
  // printf("\n centerlifted");
  // print_polynomial_array(ORDER, tmp);
  // poly_mod_3(tmp);
  // printf("\n suppose to be 0");
  // print_polynomial_array(ORDER,tmp);

  return loop;
  
}



void ppke_enc(unsigned char *c,
               const poly *r,
               poly *m,
               const poly *h)

    {
      //printf("inside encryption function \n");
      //r and m are coming from outside
      poly s;
      
      #ifdef NTRU
      printf("for NTRU");
      poly_Rq_mul(&s,r,h); //s = r*h mod(q, phi_1 phi_n)
      #endif

      #ifdef DiTRU
      //printf("For dihedral");
      poly_Rq_DiTRU_mul(&s,r,h);// s = r*h mod Z_qD_N
      #endif 

      unsigned char t_seed[LENGTH_OF_HASH];
      poly mask;
      sha3_512(t_seed, (unsigned char *)(&s), ORDER);
      sample_mask(&mask, t_seed);
      for (int i=0;i<ORDER;i++)
      {
          if (mask.coeffs[i] == 2)
              m->coeffs[i] --;
          else if (mask.coeffs[i] == 1)
              m->coeffs[i] ++;

          if (m->coeffs[i] == 65535)
              m->coeffs[i] =  Q-1;
          if (m->coeffs[i] == 2)
              m->coeffs[i] =  Q-1;
          if (m->coeffs[i] == 3)
              m->coeffs[i] =  0;
      }
     

      poly cipher;
      for (int i=0;i<ORDER;i++){
      cipher.coeffs[i] = MODQ(s.coeffs[i] + m->coeffs[i]);
      }
      poly_Sq_tobytes(c,&cipher);
      

    }



int ppke_dec(poly *m,
              const unsigned char *ciphertext,
              const poly *f)
{
//r and m are coming from outside

//printf("inside decryption function!");
poly x1, x2;
poly  *c=&x1, *s=&x2;
poly_Sq_frombytes(c,ciphertext);

      
#ifdef NTRU
poly_Rq_mul(m,c,f); //a = c*f mod(q, phi_1 phi_n)
//print_polynomial_array(ORDER,&s);
#endif

#ifdef DiTRU
poly_Rq_DiTRU_mul(m,c,f);// a = c*f mod Z_qD_N
#endif 


poly_centerlifting(m);
poly_mod_3(m); //this step should be mprime

poly_Z3_to_Zq(m); //mprime written zq
for(int i=0;i<ORDER;i++)
{
  s->coeffs[i] = MODQ(c->coeffs[i]-m->coeffs[i]); //s = c-mprime
}

unsigned char t_seed[LENGTH_OF_HASH];
poly mask;
sha3_512(t_seed, (unsigned char *)(s), ORDER);
sample_mask(&mask, t_seed);
 for (int i=0;i<ORDER;i++)
  {
    if (mask.coeffs[i] == 2)
        m->coeffs[i] ++;
    else if (mask.coeffs[i] == 1)
        m->coeffs[i] --;

    if (m->coeffs[i] == 65535 || m->coeffs[i] == Q-1)
        m->coeffs[i] =  2;
    if (m->coeffs[i] == Q)
        m->coeffs[i] =  0;
    if (m->coeffs[i] == Q-2)
        m->coeffs[i] =  1;
  }

}
