#include "util.h"

/** Calculates the next prime number
 *
 * Code taken from StackOverflow:
 *   http://stackoverflow.com/questions/30052316/find-next-prime-number-algorithm
 */
bool is_prime(int number) {
  if (number <= 1) {
    return false;
  }

  if (number == 2 || number == 3) {
    return true;
  }

  if (number % 2 == 0 || number % 3 == 0) {
    return false;
  }

  int divisor = 6;
  while (divisor * divisor - 2 * divisor + 1 <= number) {
    if (number % (divisor - 1) == 0) {
      return false;
    }
    if (number % (divisor + 1) == 0) {
      return false;
    }
    divisor += 6;
  }

  return true;
}

int next_prime(int a) {
  a++;
  while (!is_prime(a)) { a++; }
  return a;
}
