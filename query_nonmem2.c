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

    FILE *q1;
    char paramq1[102400];
    q1 = fopen("zkacc/test/q1_prime.txt","r");
    fread(paramq1,1,102400,q1);
    element_set_str(q1_prime, paramq1, 10);

    FILE *q2;
    char paramq2[102400];
    q2 = fopen("zkacc/test/q2_prime.txt","r");
    fread(paramq2,1,102400,q2);
    element_set_str(q2_prime, paramq2, 10);

    FILE *filer;
    char paramr[102400];
    filer = fopen("zkacc/aux/r.txt","r");
    fread(paramr,1,102400,filer);
    element_set_str(r, paramr, 10);

    //the x query
    FILE *the_x;
    char param5[102400];
    the_x = fopen("zkacc/test/test_nonmem.txt","r");
    fread(param5,1,102400,the_x);
    element_set_str(x, param5, 10);

    element_t rinv, w1pow, w1, w2;

    element_init_Zr(rinv, pairing);
    element_init_Zr(w1pow, pairing);
    element_init_G1(w1, pairing);
    element_init_G1(w2, pairing);

    element_invert(rinv, r);
    element_mul(w1pow, q1_prime, rinv);

    element_pow_zn(w1, g, w1pow);
    element_pow_zn(w2, g, q2_prime);

    FILE *nonmem1;
    nonmem1 = fopen("zkacc/proof/w1.txt", "w");
    element_fprintf(nonmem1, "%B\n", w1);

    FILE *nonmem2;
    nonmem2 = fopen("zkacc/proof/w2.txt", "w");
    element_fprintf(nonmem2, "%B\n", w2);


}