#include <pbc.h>
#include <pbc_test.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>
//gcc -o setup setup.c -I ~/.local/include/pbc -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l pbc -lgmp -lm -Wsystem-headers

int main(void){

	pairing_t pairing;
	element_t pk, sk, g, pow, sigma, r;

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
    

    element_set1(pow);

    int n = 1000;
    int i;

    for(i=0;i<n;i++){ //generate n random elements
       element_t y;
       element_init_Zr(y, pairing);

       element_t add;
       element_init_Zr(add, pairing);
       
       element_random(y);
       int base =10;
       
       element_add(add, sk, y);
       element_mul(pow, add, pow);
       element_clear(add);
       
       FILE *file;
       file = fopen("zkacc/set/xout.txt", "a"); //write each random generated member to file


       int n = element_length_in_bytes(y);
       unsigned char* data = malloc(sizeof(*y));
       element_to_bytes(data, y);
       
      
       element_fprintf(file, "%B\n", y);
       //fwrite(data, n, 1, file);
       element_clear(y);
       free(data);
       fclose(file);
       fflush(file);

        }
        //generate random r
    element_random(r);
    element_mul(pow, r, pow);
    //accumulate the n elements and write sigma to the file
    FILE *acc;
    acc = fopen("zkacc/accumulator/acc.txt", "w");
    element_pow_zn(sigma, g, pow);
    element_fprintf(acc, "%B\n", sigma);
    element_printf("The accumulation of the 1000 email addresses is: %B\n", sigma); 

    FILE *powf;
    powf = fopen("zkacc/aux/pow.txt", "w");
    element_fprintf(powf, "%B\n ", pow);   

    // write theta= {g^s, g^s{^2}, .... , g^s{^n} } to theta.txt

    element_t aux_element;
    element_init_G1(aux_element, pairing);
    element_set(aux_element, pk);

    for(i=0;i<n;i++){

        FILE *theta;
        theta = fopen("zkacc/aux/theta.txt", "a");
        element_fprintf(theta, "%B\n", aux_element); 

        element_pow_zn(aux_element, aux_element, sk); // exponentiate aux_element by s

    }

    FILE *ther;
    ther = fopen("zkacc/aux/r.txt", "w");
    element_fprintf(ther, "%B\n", r); 

    
}