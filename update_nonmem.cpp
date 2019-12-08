
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

//g++ -g -O2 -std=c++11 -pthread -march=native update_nonmem.cpp -o update_nonmem -lntl -lgmp -lm


void eraseFileLine(string path, string eraseLine);
ZZ stringToNumber(string str);
ZZ_p hashToGroup(string src_str);

int main(){
	string modulus_p = "730750818665451621361119245571504901405976559617";
    std::string strp(modulus_p);
    NTL::ZZ mod(NTL::INIT_VAL, strp.c_str());
    //set modulus p
    ZZ_p::init(mod);

	string path_email = "zkacc/set/1000emails.txt";
	string line;
    cout << "Please enter an email address you want to delete: \n"; 
    getline(cin, line);

    std::ofstream out("zkacc/test/update_nonmem.txt");

    auto *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    std::cout << hashToGroup(line) << std::endl;


    string strsec;
    ifstream thefile("zkacc/test/update_nonmem.txt");
    if(thefile.is_open()){
        getline(thefile, strsec);
    }

    eraseFileLine(path_email, line);

    string path_hash = "zkacc/set/emailout.txt";
	string hash = strsec;

    eraseFileLine(path_hash, hash);



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

void eraseFileLine(std::string path, std::string eraseLine) {
std::string line;
std::ifstream fin;

fin.open(path);
std::ofstream temp; // contents of path must be copied to a temp file then renamed back to the path file
temp.open("temp.txt");

while (getline(fin, line)) {
    if (line != eraseLine) // write all lines to temp other than the line marked fro erasing
        temp << line << std::endl;
}

temp.close();
fin.close();

const char * p = path.c_str(); // required conversion for remove and rename functions
remove(p);
rename("temp.txt", p);}