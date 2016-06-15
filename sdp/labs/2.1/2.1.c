#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>

typedef struct Threads{

	int *vet;
	int left;
	int right;
	
}Threads_t;

void 
merge(int *vet, int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 =  right - middle;

    int L[n1], R[n2];  // temp arrays
    
    for (i = 0; i <= n1; i++)  // make a copy
        L[i] = vet[left + i];
    for (j = 0; j <= n2; j++)
        R[j] = vet[middle + 1 + j];
        
    // Merge the temp arrays in vet[l..r]
    i = 0; 
    j = 0; 
    k = left; // Initial index of merged subarray
    while (i < n1 && j < n2)  {
      if (L[i] <= R[j])
        vet[k++] = L[i++];
      else
        vet[k++] = R[j++];
    }
    // Copy the remaining elements of L[]
    while (i < n1)
      vet[k++] = L[i++];
    // Copy the remaining elements of R[]
    while (j < n2)
      vet[k++] = R[j++];
}
 
void mergeSort(Threads_t *th){
int middle,left,right,*vet;
pthread_t th[2];

left = th->left;
right = th->right;
vet = th->vet;
  if (left < right){
    middle = left + (right - left)/2;  // Same as (left + right)/2, but avoids overflow for large l and r
    
    th->vet = vet;
    th->left = middle;
    th->right = right;
    pthread_create(th[0],0,mergeSort,th);
    //mergeSort(vet, left, middle);
    th->vet = vet;
    th->left = middle+1;
    th->right = right;
    pthread_create(th[1],0,mergeSort,th);
    //mergeSort(vet, middle+1, right);
 
    merge(vet, left, middle, right);
  }
}

int main(int argc, char ** argv) {
  int i, n, len;
  int *vet;
  Thread_t *th;  

  if (argc != 2) {
    printf ("Syntax: %s dimension", argv[0]);
    return (1);
  }
  
  n = atoi(argv[1]);

  vet = (int*) malloc(n * sizeof(int));
  
  srand(n);
  
  for(i = 0;i < n;i++) {
    vet[i] = rand() % 100;
	printf("%d\n",vet[i]);
  }
  th->vet = vet;
  th->left = 0;
  th->right = n-1;

  mergeSort(th);
  //mergeSort(vet,0, n-1);

  printf("\n");
  for(i = 0;i < n;i++) 
	printf("%d\n",vet[i]);
	
  return 0;
}
