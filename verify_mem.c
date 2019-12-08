#include <pbc.h>
#include <pbc_test.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>

int main(void){

	pairing_t pairing;
	element_t pk, g, sigma, x;

	char param[1024];
    size_t count = fread(param, 1, 1024, stdin);
    if (!count) pbc_die("input error");
    pairing_init_set_buf(pairing, param, count);
    if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");

    element_init_G1(pk, pairing);
    element_init_G1(g, pairing);
    element_init_G1(sigma, pairing);
    element_init_Zr(x, pairing);

    //read public key from the file
    FILE *pubk; 
    char param1[204800];
    pubk=fopen("zkacc/publickey/pubkey.txt","r");
    fread(param1,1,204800,pubk);
    element_set_str(pk,param1, 10);
    

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
    the_x = fopen("zkacc/test/test_mem.txt","r");
    fread(param5,1,102400,the_x);
    element_set_str(x, param5, 10);

    //initiate pairing elements and checks
    element_t lhsmem, rhsmem, powmem, memproof;

    element_init_GT(lhsmem, pairing);
    element_init_GT(rhsmem, pairing);
    element_init_G1(powmem, pairing);
    element_init_G1(memproof, pairing);

    // read mem proof
    FILE *proof;
    char param6[102400];
    proof = fopen("zkacc/proof/proof_mem.txt","r");
    fread(param6,1,102400,proof);
    element_set_str(memproof, param6, 10);

    // write x in query and read and use    
    element_pow_zn(powmem, g, x);
    element_mul(powmem, powmem, pk);

    element_pairing(lhsmem, sigma, g);
    element_pairing(rhsmem, powmem, memproof);
    element_printf("The membership witness for x = %B\n", memproof);    


    if (!element_cmp(lhsmem, rhsmem)) {
      printf("Member witness is VALID\n");
      char c[1000];
        FILE *fptr;
        if ((fptr = fopen("zkacc/test/test_mem_email.txt", "r")) == NULL)
        {
            printf("Error! opening file");
            // Program exits if file pointer returns NULL.
            exit(1);         
        }
        // reads text until newline 
        fscanf(fptr,"%[^\n]", c);
        printf("Witness valid for: %s \n", c);
        fclose(fptr);

    } else {
      printf("Member witness is INVALID\n");
      FILE *fptr;
        if ((fptr = fopen("zkacc/test/test_mem_email.txt", "r")) == NULL)
        {
            printf("Error! opening file");
            // Program exits if file pointer returns NULL.
            exit(1);         
        }
        // reads text until newline 
        fscanf(fptr,"%[^\n]", c);
        printf("Witness invalid for: %s \n", c);
        fclose(fptr);
    }
}