
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
int main(void)
{	
	string modulus_p = "730750818665451621361119245571504901405976559617";
    std::string strp(modulus_p);
    NTL::ZZ mod(NTL::INIT_VAL, strp.c_str());
    //set modulus p
    ZZ_p::init(mod);

	vector<string> data;
	string line;

	ifstream myfile ("zkacc/set/1000emails.txt");
    if (myfile.is_open())
    {   
        for(line; getline(myfile, line);/**/){
            
            data.push_back(line);
            /*if(counter==0){
            counter+=1;
        }*/
        }

        myfile.close();
    }
    int i;
    ofstream file1("zkacc/set/emailout.txt");
    for(i=0; i<1000; i++){
		
		

		
	    if( file1.fail() )
	    {
	        cerr << "!Error opening "  << endl;
	        return EXIT_FAILURE;
	    }
	   
	    file1 << hashToGroup(data[i]) << endl;

	    if(i==456){
	    	ZZ_p test_mem;
	    	test_mem = hashToGroup(data[i]);
	    	std::ofstream out("zkacc/test/test_mem.txt");

		    auto *coutbuf = std::cout.rdbuf();
		    std::cout.rdbuf(out.rdbuf());

		    std::cout << test_mem << std::endl;
		    std::cout.rdbuf(coutbuf);

		    std::ofstream out1("zkacc/test/test_email.txt");

		    
		    std::cout.rdbuf(out1.rdbuf());

		    std::cout << data[i] << std::endl;
		    std::cout.rdbuf(coutbuf);
	    }
	    
	    
    }
    
    file1.close();


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