#ifndef __ARRAYS_H__
#define __ARRAYS_H__

#include <stdlib.h>

void fillRand3(int arr[], size_t sz);
int checkRand3(int arr[], size_t sz);
float meanValue(int arr[], size_t sz);
int meanIndex(int arr[], size_t sz);
int minValue(int arr[], size_t sz);
int minIndex(int arr[], size_t sz);
int maxOccurance(int arr[], size_t sz);

int diff(int arr1[], int arr2[], int res[], size_t sz);
void sub(int arr1[], int arr2[], int res[], size_t sz);
int eq(int arr1[], int arr2[], size_t sz);
void land(int arr1[], int arr2[], int res[], size_t s);

#endif
