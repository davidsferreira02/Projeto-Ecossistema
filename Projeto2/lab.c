#include <stdio.h>
#include <omp.h>
#include <string.h>
int main(){


int i;
int c;
int a[MAX];
int 

#pragma omp parallel for firstprivate(x) private(b)
for (i = 0; i < N; i++) {
  int c = 0;
  for (j = i; j <= N; j++)
    c += b[j];
  a[i] = c * x--;
}
}