#include <pbc.h>
#include <pbc_test.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>

int main(void){

	pairing_t pairing;
	element_t pk, sk, g;

	char param[1024];
    size_t count = fread(param, 1, 1024, stdin);
    if (!count) pbc_die("input error");
    pairing_init_set_buf(pairing, param, count);
    if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");

    element_init_G1(pk, pairing);
    element_init_Zr(sk, pairing);
    element_init_G1(g, pairing);

    FILE* filepar = NULL;
    FILE* pubk = NULL;
    FILE* seck = NULL;
    

    filepar = fopen("zkacc/generator/syspar.txt", "w");
    pubk = fopen("zkacc/publickey/pubkey.txt", "w");
    seck = fopen("zkacc/secretkey/seckey.txt", "w");
    

	  //generate system parameters
    element_random(g);
    element_fprintf(filepar, "%B\n", g);
    element_printf("The generator for the underlying group is: %B\n", g);

  //generate private key
    element_random(sk);
    element_fprintf(seck, "%B\n", sk);
    element_printf("The secret key: %B\n", sk);


	element_pow_zn(pk, g, sk);// pk = g^sk
	element_fprintf(pubk, "%B\n", pk);
    element_printf("The Public key: %B\n", pk);


}