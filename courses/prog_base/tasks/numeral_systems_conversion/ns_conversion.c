#include <stdio.h>
#include <tgmath.h>

#define MAX_LENGTH 64
#define PRECISION 12

static inline int symToInt(char ch) {
  if (ch < 'A')
    return ch - '1' + 1;
  else
    return ch - 'A' + 10;
}

static inline char intToSym(int s) {
  if (s < 10)
    return '1' + s - 1;
  else
    return 'A' + s - 10;
}

char result[MAX_LENGTH + 1];
char *ns_convert(char *number, unsigned int sourceBase, unsigned int destBase) {
  char *current;
  char *res = result;
  if (*number == '-') {
    res[0] = '-';
    res++;
    number++;
  }

  if (*number == '\0' || sourceBase < 2 || sourceBase > 36 || destBase < 2 ||
      destBase > 36)
    return "\0";

  int ipN = 0; //
  int fpN = 0; // integer/fractional part symbols count

  int *c = &ipN;
  char maxSource = intToSym(sourceBase - 1);
  for (current = number; *current != '\0'; current++) {
    if (*current > maxSource)
      return "\0";
    if (*current == '.') {
      c = &fpN;
      continue;
    }
    (*c)++;
  }

  current = number;
  int numI = 0;
  double numD = 0;
  int Pow = powl(sourceBase, ipN - 1);
  for (int i = ipN - 1; i >= 0; i--) {
    numI += symToInt(*current++) * Pow;
    Pow /= sourceBase;
  }

  current++;

  double dPow = pow(sourceBase, -1);
  for (int i = 1; i <= fpN; i++) {
    numD += symToInt(*current++) * dPow;
    dPow /= sourceBase;
  }

  current = res;
  if (numI == 0) {
    *(current++) = '0';
  }
  while (numI != 0) {
    *(current++) = intToSym(numI % destBase);
    numI /= destBase;
  }

  char *b, *e; // Revert string
  for (b = res, e = current - 1; b < e; b++, e--) {
    char t = *b;
    *b = *e;
    *e = t;
  }

  *current = '\0';

  if (fpN > 0)
    *current = '.';
  else
    return result;
  current++;

  double integral;
  for (int i = 0; i < PRECISION; i++) {
    numD = modf(numD * destBase, &integral);
    *(current++) = intToSym(integral);
    if (numD == 0)
      break;
  }
  *current = '\0';

  return result;
}

int main(void) {
  puts(ns_convert("-60EA4.54DF", 16, 25));
  return 0;
}
