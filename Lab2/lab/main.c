#include <stdio.h>

int main(int argc, char* argv[]) {

  int x = 20, y = 30, a = 2137, n = 100;

  printf("NWD liczb %d oraz %d to: %d\n", x, y, nwd(x, y));
  printf("NWW liczb %d oraz %d to: %d\n", x, y, nww(x, y));
  printf("Liczba %d jest pierwsza: %s\n", a, is_prime(a) ? "tak" : "nie");
  printf("Sekwencja Fibonacciego do liczby %d: ", n);
  print_fibonacci(n);

  return 0;
}