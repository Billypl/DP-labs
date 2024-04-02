#include "NWD.h"

int nwd(int a, int b) {
  int temp;
  while (b != 0) {
      temp = b;
      b = a % b;
      a = temp;
  }
  return a;
}
