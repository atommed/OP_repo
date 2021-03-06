#include <stdlib.h> //size_t
#include <errno.h>
#include <limits.h> //INT_MAX, INT_MIN
#include <string.h> //memmove()
#include <stdio.h>  //printf(): print_array

#include "arrays.h"

static int _t_do_copy(int arr1[], int arr2[], size_t arr1_len, size_t l,
                      size_t r);

int foldr(int accum, int *arr_first, int *arr_last, fold_f f) {
  errno = 1;
  int new_acc;

  if (arr_first == arr_last)
    return accum;
  new_acc = f(accum, *arr_first);
  arr_first++;
  return foldr(new_acc, arr_first, arr_last, f);
}

int f_max(int a, int b) { return a > b ? a : b; }

int f_min(int a, int b) { return a < b ? a : b; }

int f_sum(int acc, int next) {
  long t_sum = acc + next;
  if (t_sum < INT_MIN || t_sum > INT_MAX)
    errno = EOVERFLOW;
  return acc + next;
}

size_t find_first(int value, int arr[], size_t len) {
  for (size_t i = 0; i < len; i++)
    if (arr[i] == value)
      return i;
  return len + 1;
}

size_t find(int arr[], size_t len) {
  size_t min = find_first(foldr(arr[0], arr, arr + len, f_min), arr, len);
  size_t max = find_first(foldr(arr[0], arr, arr + len, f_max), arr, len);
  return (min + max) / 2;
}

void print_array(int arr[], size_t len) {
  size_t i;
  for (i = 0; i < len - 1; i++)
    printf("%d ", arr[i]);
  printf("%d\n", arr[i]);
}

void initialize_array(int arr[], size_t len) {
  for (size_t i = 0; i < len; i++)
    arr[i] = 10 + rand() % 90;
}

static int _t_do_copy(int arr1[], int arr2[], size_t arr2_len, size_t l,
                      size_t r) {
  if (r < l)
    return -1;
  if ((r - l) > arr2_len)
    return -1;
  memmove(arr2, arr1 + l, sizeof(int) * (r - l + 1));
  return 0;
}

int copy(int arr1[], int arr2[], size_t arr1_len, size_t arr2_len) {
  if (arr1_len < 2)
    return -1;
  int index = find(arr1, arr1_len);
  return _t_do_copy(arr1, arr2, arr2_len, 1, index);
}

double sum_average(int arr[], size_t arr_len) {
  return foldr(0, arr, arr + arr_len, f_sum) / (double)arr_len;
}
