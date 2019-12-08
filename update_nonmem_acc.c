#include <pbc.h>
#include <pbc_test.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>
//gcc -o update_nonmem_acc update_nonmem_acc.c -I ~/.local/include/pbc -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l pbc -lgmp -lm -Wsystem-headers

int main(void){

	pairing_t pairing;
	element_t pk, sk, g, sigma, r, upd, pow;

	char param[1024];
    size_t count = fread(param, 1, 1024, stdin);
    if (!count) pbc_die("input error");
    pairing_init_set_buf(pairing, param, count);
    if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");

    element_init_G1(pk, pairing);
    element_init_Zr(sk, pairing);
    element_init_Zr(pow, pairing);
    element_init_Zr(r, pairing);
    element_init_G1(g, pairing);
    element_init_Zr(upd, pairing);
    element_init_G1(sigma, pairing);
    //read public key from the file
    FILE *pubk; 
    char param1[204800];
    pubk=fopen("zkacc/publickey/pubkey.txt","r");
    fread(param1,1,204800,pubk);
    element_set_str(pk,param1, 10);
    

    //read secret key from the file
    FILE *seck;
    char param2[102400];
    seck = fopen("zkacc/secretkey/seckey.txt","r");
    fread(param2,1,102400,seck);
    element_set_str(sk, param2, 10);
    

    //read generator g from the file
    FILE *filepar;
    char param3[204800];
    filepar = fopen("zkacc/generator/syspar.txt","r");
    fread(param3,1,204800,filepar);
    element_set_str(g, param3, 10);

    // read accumulator from file
    FILE *acc;
    char param4[102400];
    acc = fopen("zkacc/accumulator/acc.txt","r");
    fread(param4,1,102400,acc);
    element_set_str(sigma, param4, 10);
    fclose(acc);

    FILE *pow1;
    char parampow[102400];
    pow1 = fopen("zkacc/aux/pow.txt","r");
    fread(parampow,1,102400,pow1);
    element_set_str(pow, parampow, 10);
    fclose(pow1);

    FILE *update;
    char param5[102400];
    update = fopen("zkacc/test/update_nonmem.txt","r");
    fread(param5,1,102400, update);
    element_set_str(upd, param5, 10);

    element_t acc_upd, inv;

    element_init_Zr(acc_upd, pairing);
    element_init_Zr(inv, pairing);

    element_mul(pow, pow, inv);

    element_add(acc_upd, sk, upd);
    element_invert(inv, acc_upd);
    element_pow_zn(sigma, sigma, inv);

    FILE *acc1;
    acc1 = fopen("zkacc/accumulator/acc.txt", "w");
    element_fprintf(acc1, "%B\n", sigma);

    FILE *pow2;
    pow2 = fopen("zkacc/aux/pow.txt", "w");
    element_fprintf(pow2, "%B\n", pow);

    element_printf("The updated accumulation is: %B\n", sigma);
    fclose(acc1);
}