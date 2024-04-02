RED='\033[0;31m'
GREEN='\033[0;32m'

NC='\033[0m' # No Color
Purple='\033[0;35m' 
UGreen='\033[4;32m'
On_Red='\033[41m' 

echo -e "${UGreen} ---- START ---- ${NC}"
echo -e "${Purple} ---- ROBIE STATYCZNIE ---- ${NC}"

gcc -c NWD.c -o NWD.o
gcc -c NWW.c -o NWW.o
gcc -c prime.c -o prime.o
gcc -c fib.c -o fib.o
# gcc -c *.c -o *.o
ar r lib_arytm.a NWD.o NWW.o prime.o fib.o #extra *.o files
ar tv lib_arytm.a
gcc -c -Wno-implicit main.c -o main.o
gcc main.o lib_arytm.a -o wynik.out
echo -e "${Purple} ---- ZROBIONE STATYCZNIE ---- ${NC}"
./wynik.out

echo -e "${On_Red} ---- USUWANIE ---- ${NC}"
rm lib_arytm.a
rm main.o
rm NWD.o
rm NWW.o
rm prime.o
rm fib.o
# rm *.o
rm wynik.out

echo -e "${Purple} ---- ROBIE DYNAMICZNIE ---- ${NC}"
gcc -c -Wall -fPIC -D_GNU_SOURCE NWD.c
gcc -c -Wall -fPIC -D_GNU_SOURCE NWW.c
gcc -c -Wall -fPIC -D_GNU_SOURCE prime.c
gcc -c -Wall -fPIC -D_GNU_SOURCE fib.c
# gcc -c -Wall -fPIC -D_GNU_SOURCE *.c
gcc NWD.o NWW.o prime.o fib.o -shared -o lib_pita.so # extra *.o files
gcc -c -Wno-implicit main.c -o main.o
gcc main.o -L. -l_pita -o wynik.out
ldd wynik.out
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:."
echo -e "${Purple} ---- ZROBIONE DYNAMICZNIE ---- ${NC}"
./wynik.out 

echo -e "${On_Red} ---- USUWANIE ---- ${NC}"
rm main.o
rm lib_pita.so
rm NWD.o
rm NWW.o
rm prime.o
rm fib.o
# rm *.o
rm wynik.out

echo -e "${UGreen} ---- KONIEC ---- ${NC}"
