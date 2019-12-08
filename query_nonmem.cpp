#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <cstring>
#include <set>
#include <vector>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/vec_ZZ.h>
#include <NTL/ZZ_pX.h>
#include <NTL/ZZXFactoring.h>
using namespace std;
using namespace NTL;
NTL_CLIENT
//TO RUN: g++ -g -O2 -std=c++11 -pthread -march=native query_nonmem.cpp -o query_nonmem -lntl -lgmp -lm

int main(){


    string line;
    string nonmem = "303776771461640372455980615941851778318895258822";
    std::vector<string> data;
    string test;
    int counter=0;
    //read the x's
    ifstream myfile ("zkacc/set/xout.txt");
    if (myfile.is_open())
    {   
        for(line; getline(myfile, line);/**/){
            
            data.push_back(line);
            if(counter==0){
            test= line;
            counter+=1;
        }
        }

        myfile.close();
        
    }

    string modulus_p = "730750818665451621361119245571504901405976559617";
    std::string strp(modulus_p);
    NTL::ZZ mod(NTL::INIT_VAL, strp.c_str());
    //set modulus p
    ZZ_p::init(mod);
    
    /*for(int i = 0; i < data.size(); i++){
        std::cout << data[i] << '\n';
    }*/

    //convert string test to ZZ(unlimited size int) a 
    std::string str(test);
    NTL::ZZ a(NTL::INIT_VAL, str.c_str());

    string strsec;
    ifstream thefile("zkacc/secretkey/seckey.txt");
    if(thefile.is_open()){
        getline(thefile, strsec);
    }

    std::string strs(strsec);
    NTL::ZZ ss(NTL::INIT_VAL, strs.c_str());
    ZZ_p strsec_p;
    conv(strsec_p, ss);
    //a= x[0] and poly = x+a
    ZZX poly;
    poly.SetLength(2);
    poly[0]= a;
    poly[1]= 1;

    // get Chx()
    for(int i=1; i<1000;i++){

        std::string str2(data[i]);
        NTL::ZZ k(NTL::INIT_VAL, str2.c_str());

        ZZX poly1;
        poly1.SetLength(2);
        poly[0] = k;
        poly1[1] = 1;
        
        mul(poly, poly, poly1);
    }
   /* ZZ_pX modpoly;
    conv(modpoly,poly );
    ZZ_p a_;
    conv(a_, a);
    ZZ_p result(NTL::eval(modpoly, a));*/

    std::string str1(nonmem);
    NTL::ZZ z(NTL::INIT_VAL, str1.c_str());

    ZZX poly_nonmem;
    poly_nonmem.SetLength(2);
    poly_nonmem[0]= z;
    poly_nonmem[1]= 1;
    ZZ_pX modpoly, modpoly1;
    conv(modpoly,poly );
    conv(modpoly1, poly_nonmem);

    // calculate xtended gcd 
    ZZ_pX gcd, q1 , q2;
    XGCD(gcd, q1, q2, modpoly, modpoly1 ); 
    std::cout<<gcd<<'\n'; 
    
    ZZ_p q1_s, q2_s;
    //calculate q1(s)
    eval(q1_s ,q1, strsec_p);
    std::cout<<q1_s<<'\n';

    eval(q2_s ,q2 , strsec_p);
    std::cout<<q2_s<<'\n';

    //choose gamma
    ZZ_p gamma;
    random(gamma);
    random_ZZ_p();
    

    //to do: calculate q1'(s) and q2'(s) and read g from syspar.txt and write W1, W2 to file as non mem witness
    ZZ_p q1_prime, q2_prime;
    ZZ_p rhq1, rhq2;

    //q1_prime = q1(s) + gamma. (x+s)
    eval(rhq1, modpoly1, strsec_p);
    mul(rhq1, gamma, rhq1);
    add(q1_prime, q1_s, rhq1);

    //q2_prime = q2(s) + chx(s)

    eval(rhq2, modpoly, strsec_p);
    mul(rhq2, gamma, rhq2);
    sub(q2_prime, q2_s, rhq2);

    std::cout<<q1_prime<<'\n';
    std::cout<<q2_prime<<'\n';


    std::ofstream out("zkacc/test/q1_prime.txt");

    auto *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    std::cout << q1_prime << std::endl;

    /** reset cout buffer **/
    std::cout.rdbuf(coutbuf);

    std::ofstream out1("zkacc/test/q2_prime.txt");

    std::cout.rdbuf(out1.rdbuf());

    std::cout << q2_prime << std::endl;

    /** reset cout buffer **/
    std::cout.rdbuf(coutbuf);

    std::ofstream out2("zkacc/test/test_nonmem.txt");

    std::cout.rdbuf(out2.rdbuf());

    std::cout << nonmem << std::endl;

    /** reset cout buffer **/
    std::cout.rdbuf(coutbuf);

    /*ZZ_p check, check1, check2;
    eval(check, modpoly, strsec_p);
    eval(check1, modpoly1, strsec_p);
    mul(check, check, q1_prime);
    mul(check1, check1, q2_prime);
    add(check2, check, check1);

    std::cout<<check2<<'\n';*/
}