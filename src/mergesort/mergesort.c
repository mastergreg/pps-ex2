#include <stdio.h>
#include <stdlib.h>

// Merge two arrays (asceding order) of sizes N and M
int* merge(int * left, int * right, int N, int M) {
    printf("merge(%d, %d)\n", N,M);
    int i = N;
    int j = M;
    int k = 0;
    int* result = malloc((N+M) * sizeof(int));

    while ((i>0) || (j>0)) {
        
        if ((left[N-i] <= right[M-j]) && (i>0)) {
            result[k] = left[N-i];
            i--;
        } else if (j>0){
            result[k] = right[M-j];
            j--;
        }
    }
    return result;
}

// Sort an array of size N
int* mergesort(int * array, int N) {
    printf("mergesort(%d)\n", N);
    if (N == 0) {
        return array;
    }
    return merge(mergesort(array, N/2), mergesort(&array[N/2], N/2), N/2, (N-N/2));
}

int main(){ 
    int size = 15;
    int i; 
    int* res;

    int* A = malloc(size * sizeof(int));

    for (i=0; i<size; i++) {
        A[i] = i;
    }
    for (i=0; i<size; i++) {
        printf("%d ", res[i]);
    }
    res = mergesort(A, size);
    printf("\n");
    for (i=0; i<size; i++) {
        printf("%d ", res[i]);
    }

 }
