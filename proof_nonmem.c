#include <pbc.h>
#include <pbc_test.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>

int main(void){

	pairing_t pairing;
	element_t pk, g, sigma, x, q1_prime, q2_prime, r;

	char param[1024];
    size_t count = fread(param, 1, 1024, stdin);
    if (!count) pbc_die("input error");
    pairing_init_set_buf(pairing, param, count);
    if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");

    element_init_G1(pk, pairing);
    element_init_G1(g, pairing);
    element_init_G1(sigma, pairing);
    element_init_Zr(x, pairing);
    element_init_Zr(q1_prime, pairing);
    element_init_Zr(q2_prime, pairing);
    element_init_Zr(r, pairing);

    //read public key from the file
    FILE *pubk; 
    char param1[204800];
    pubk=fopen("zkacc/publickey/pubkey.txt","r");
    fread(param1,1,204800,pubk);
    element_set_str(pk,param1, 10);
    element_printf("Public Key: %B\n", pk);

    //read generator g from the file
    FILE *filepar;
    char param3[102400];
    filepar = fopen("zkacc/generator/syspar.txt","r");
    fread(param3,1,102400,filepar);
    element_set_str(g, param3, 10);

    // read accumulator from file
    FILE *acc;
    char param4[102400];
    acc = fopen("zkacc/accumulator/acc.txt","r");
    fread(param4,1,102400,acc);
    element_set_str(sigma, param4, 10);

    //the x query
    FILE *the_x;
    char param5[102400];
    the_x = fopen("zkacc/test/test_nonmem.txt","r");
    fread(param5,1,102400,the_x);
    element_set_str(x, param5, 10);

    element_t w1, w2, g_to_x, lhspair, lhspair1, lhspair2, rhspair, pow_xs;

    element_init_G1(w1, pairing);
    element_init_G1(w2, pairing);
    element_init_G1(g_to_x, pairing);
    element_init_GT(lhspair, pairing);
    element_init_GT(lhspair1, pairing);
    element_init_GT(lhspair2, pairing);
    element_init_GT(rhspair, pairing);
    element_init_G1(pow_xs, pairing);

    // proof part w1
    FILE *the_w1;
    char paramw1[102400];
    the_w1 = fopen("zkacc/proof/w1.txt","r");
    fread(paramw1,1,102400,the_w1);
    element_set_str(w1, paramw1, 10);

    // proof part w2
    FILE *the_w2;
    char paramw2[102400];
    the_w2 = fopen("zkacc/proof/w2.txt","r");
    fread(paramw2,1,102400,the_w2);
    element_set_str(w2, paramw2, 10);

    element_pow_zn(g_to_x, g, x);
    element_mul(pow_xs, g_to_x, pk);

    element_pairing(lhspair1, w1, sigma);
    element_pairing(lhspair2, w2, pow_xs);
    element_mul(lhspair, lhspair1, lhspair2);
    element_pairing(rhspair, g, g);
    element_printf("%B\n",lhspair );
    element_printf("%B\n",rhspair );

    if (!element_cmp(lhspair, rhspair)) {
      printf("Non-Member witness is VALID\n");

    } else {
      printf("Non-Member witness is INVALID\n");
    }


}