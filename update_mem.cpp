
#include "picosha2.h"
#include <iostream>
#include <list>
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


ZZ stringToNumber(string str);
ZZ_p hashToGroup(string src_str);

ZZ stringToNumber(string str);
ZZ_p hashToGroup(string src_str);
int main(void)
{	
	string modulus_p = "730750818665451621361119245571504901405976559617";
    std::string strp(modulus_p);
    NTL::ZZ mod(NTL::INIT_VAL, strp.c_str());
    //set modulus p
    ZZ_p::init(mod);

	
	string line;

  
    cout << "Please enter an email address you want to ADD to the accumulated set: \n"; 
    getline(cin, line); 
    char *cstr = &line[0];


  
	ZZ_p test_mem;
	test_mem = hashToGroup(line);
	std::ofstream out("zkacc/test/update_mem.txt");

    auto *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    std::cout << test_mem << std::endl;
    std::cout.rdbuf(coutbuf);

    std::ofstream out1("zkacc/test/update_mem_email.txt");

    
    std::cout.rdbuf(out1.rdbuf());

    std::cout << line << std::endl;
    std::cout.rdbuf(coutbuf);
 
    std::ofstream out2;

	 // std::ios::app is the open mode "append" meaning
	 // new data will be written to the end of the file.
	out2.open("zkacc/set/1000emails.txt", std::ios::app);


	out2 <<line <<endl;

	std::ofstream out3;

    out3.open("zkacc/set/emailout.txt", std::ios::app);
    out3 <<test_mem <<endl;


}
      
ZZ stringToNumber(string str)
{
    ZZ number = conv<ZZ>(str[0]);
    long len = str.length();
    for(long i = 1; i < len; i++)
    {
        number *= 128;
        number += conv<ZZ>(str[i]);
    }

    return number;
}

ZZ_p hashToGroup(string src_str){
		vector<unsigned char> hash(picosha2::k_digest_size);
		picosha2::hash256(src_str.begin(), src_str.end(), hash.begin(), hash.end());

		string hex_str = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
		/*char cstr[hex_str.size() + 1];
	    strcpy(cstr, hex_str.c_str());*/

		ZZ val;
		val = stringToNumber(hex_str);
		
		
	    ZZ_p mod_val;
	    conv(mod_val, val);

	    return mod_val;
}