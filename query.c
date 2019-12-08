#include <pbc.h>
#include <pbc_test.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>

int main(void){

	pairing_t pairing;
	element_t pk, sk, g, pow, sigma, r, x;

	char param[1024];
    size_t count = fread(param, 1, 1024, stdin);
    if (!count) pbc_die("input error");
    pairing_init_set_buf(pairing, param, count);
    if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");

    element_init_G1(pk, pairing);
    element_init_Zr(sk, pairing);
    element_init_Zr(r, pairing);
    element_init_G1(g, pairing);
    element_init_Zr(pow, pairing);
    element_init_G1(sigma, pairing);
    element_init_Zr(x, pairing);

    FILE *seck;
    char param2[102400];
    seck = fopen("zkacc/secretkey/seckey.txt","r");
    fread(param2,1,102400,seck);
    element_set_str(sk, param2, 10);

    FILE *filepar;
    char param3[102400];
    filepar = fopen("zkacc/generator/syspar.txt","r");
    fread(param3,1,102400,filepar);
    element_set_str(g, param3, 10);

    FILE *test;
    char paramtest[102400];
    test = fopen("zkacc/test/test_mem.txt","r");
    fread(paramtest,1,102400,test);
    element_set_str(x, paramtest, 10);    

    //pairing initialization 
    element_t addmem, memproofex, memproof, powmem, rhsmem, lhsmem;
    element_init_Zr(addmem, pairing);
    element_init_Zr(memproofex, pairing);
    element_init_G1(memproof, pairing);
    element_init_G1(powmem, pairing);
    element_init_GT(lhsmem, pairing);
    element_init_GT(rhsmem, pairing);
    

    FILE *powf;
    char parampow[102400];
    powf = fopen("zkacc/aux/pow.txt","r");
    fread(parampow,1,102400,powf);
    element_set_str(pow, parampow, 10);
    element_add(addmem, x, sk); // addmem = (x+s)
    element_div(memproofex, pow, addmem); // pow= r.Chx(s), therefore memproofex = r.Chx(s)/(x+s)

    element_pow_zn(memproof, g, memproofex );
    element_printf("The membership proof/witness is: %B\n", memproof);
    //element_pow_zn(powmem, g, addmem); //powmem= g^(x+s)

    FILE *proof_mem;
    proof_mem = fopen("zkacc/proof/proof_mem.txt","w");
    element_fprintf(proof_mem, "%B\n", memproof);
    fclose(proof_mem);


}