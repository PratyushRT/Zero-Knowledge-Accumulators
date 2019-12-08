 #!/bin/bash

echo "Hi! We will look at how great accumulators are! "
sleep 5
echo " "
echo "First I will run the Key Generation Algorithm which generates the Public Key- Secret Key pair"
echo " "
sleep 6
gcc -o keygen keygen.c -I ~/.local/include/pbc -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l pbc -lgmp -lm -Wsystem-headers
./keygen < a.param
echo " "
sleep 3
echo "Now that we have the keys ready, let's setup the succinct representation, i.e., the Accumulation"
echo " "
sleep 6
echo "We will accumulate 1000 email addresses for our demo, but even for 10 million addresses the protocol is efficient"
echo " "
sleep 6
echo "Only accumulation setup(the current process) will take more time"
echo " "
sleep 4
echo "The Query and Verification is constant time"
echo " "
sleep 2
gcc -o setup setup.c -I ~/.local/include/pbc -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l pbc -lgmp -lm -Wsystem-headers
./setup < a.param
echo " "
echo "Now, let us see the membership verification"
g++ -g -O2 -std=c++11 -pthread -march=native emailmem.cpp -o emailmem -lntl -lgmp -lm
./emailmem
echo " "
echo "We will now generate the proof for the email address you entered"
sleep 3
gcc -o query query.c -I ~/.local/include/pbc -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l pbc -lgmp -lm -Wsystem-headers
./query < a.param
echo " "
echo " Let's check the validity of this proof"
sleep 2
echo " "
echo "Running membership proof verification"
gcc -o verify_mem verify_mem.c -I ~/.local/include/pbc -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l pbc -lgmp -lm -Wsystem-headers
./verify_mem < a.param