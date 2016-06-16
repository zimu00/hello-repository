#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>

#define BUFLEN 260
int size;

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
 
void sort(int *vet,int left,int right){
    int i,j;
    int temp;
    
    for(i=left;i<=right;i++){
       for(j=left;j<=right-i-1;j++)
    	{
    	if(vet[j]>vet[j+1]){
    		temp = vet[j];
    		vet[j] = vet[j+1];
    		vet[j+1] = temp;	
    		}
    	}
    }
}
 
void *mergeSort(void *param){
Threads_t *th = (Threads_t *)param;
int middle,left,right,*vet,i;
pthread_t th1[2];

left = th->left;
right = th->right;
vet = th->vet;
printf("in %ld right=%d,left=%d\n",pthread_self(),th->right,th->left);
  if (right-left>0)
  
  sort(vet,th->left,th->right);
  
  else{
    middle = left + (right - left)/2;  // Same as (left + right)/2, but avoids overflow for large l and r
    
    th->vet = vet;
    th->left = left;
    th->right = middle;
   
    printf("----in %ld right=%d,left=%d\n",pthread_self(),th->right,th->left);
    pthread_create(&th1[0],0,mergeSort,th);
    
    pthread_join(th1[0],NULL);
    //mergeSort(vet, left, middle);
    th->vet = vet;
    th->left = middle+1;
    th->right = right;
   
    printf("----in %ld right=%d,left=%d\n",pthread_self(),th->right,th->left);
    pthread_create(&th1[1],0,mergeSort,th);
    //mergeSort(vet, middle+1, right);
 
    pthread_join(th1[0],NULL);
    
    merge(vet, left, middle, right);
    }
    printf("in %ld\n",pthread_self());
    for(i=left;i<right;i++)
       printf(" %d ",vet[i]);
    printf("\n");
  
 
}

int main(int argc, char ** argv) {
  int i, n, len;
  int *vet;
  Threads_t th;  

  if (argc != 3) {
    printf ("Syntax: %s dimension,size", argv[0]);
    return (1);
  }
  
  n = atoi(argv[1]);
  size = atoi(argv[2]);

  vet = (int*) malloc(n * sizeof(int));
  //th->vet = (int*) malloc(n * sizeof(int));
  
  srand(n);
  
  for(i = 0;i < n;i++) {
    vet[i] = rand() % 100;
	printf("%d\n",vet[i]);
  }
  th.vet = vet;
  th.left = 0;
  th.right = n-1;

  mergeSort(&th);
  //mergeSort(vet,0, n-1);
 // vet = th.vet;

  printf("\n");
  for(i = 0;i < n;i++) 
	printf("%d\n",vet[i]);
	
  return 0;
}
