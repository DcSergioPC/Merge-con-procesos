#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h> // Incluye la biblioteca para mmap()
#include <bits/mman-linux.h>
//Solo en UNIX Linux
#define MAX_SIZE 100
// static int arr[] = {5,4,8,9,3,1,4,7,8,9,5,4,8,7,9,6};

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    // Create temp arrays
    int L[n1], R[n2];
 
    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    // Merge the temp arrays back into arr[l..r
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    // Copy the remaining elements of L[],
    // if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    // Copy the remaining elements of R[],
    // if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void imprimir(int *arr, int l, int r,int mid){
    printArray(arr,l,r);

    for(int i=0;i<32-(r-l)*2;i++){
        printf(" ");
    }
    printf("\t<==\t");

    printArray(arr,l,mid);

    for(int i=0;i<16-(mid-l)*2;i++){
        printf(" ");
    }
    printf("-\t");
    printArray(arr,mid+1,r);
    printf("\n");
}
void printProcess(int *arr, int l, int r, int mid,char* izq, char* der){
    printArray(arr,l,r);

    for(int i=0;i<32-(r-l)*2;i++){
        printf(" ");
    }
    printf("\t<==\t");

    // printArray(arr,l,mid);
    printf("%s",izq);

    for(int i=0;i<16-(mid-l)*2;i++){
        printf(" ");
    }
    printf("-\t");
    // printArray(arr,mid+1,r);
    printf("%s",der);
    printf("\n");
}

void printArray(int *arr,int start,int end){
    printf("{");
    for (int i = start; i < end; i++) {
        printf("%d,",arr[i]);
    }
    printf("%d}",arr[end]);
}

char *stringArray(int *arr,int start,int end){
    // char a[(end-start)*2+4];
    char *a = malloc((end - start) * 2 + 4);
    // char *a = (char*)mmap(NULL, (end - start) * 2 + 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    a[0]='{';
    for (int i = start; i < end; i++) {
        a[(i-start)*2+1] = arr[i] + '0';
        a[(i-start)*2+2] = ',';
    }
    // printf("%d}",arr[end]);
    a[(end-start)*2+1]=arr[end] + '0';
    a[(end-start)*2+2]='}';
    a[(end-start)*2+3]='\0';
    return a;
}
void burbuja(int arr[], int start,int end) {
    int i, j, temp;
    for (i = start; i < end; i++) {
        for (j = start; j < end-1; j++) {
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}
// Función de ordenamiento Merge Sort
void mergeSort(int arr[], int l, int r, int *n,int *left,char **mapeo) {
    int status;
    if (l < r) {
        // printf("-%d-\n",*n);
        // Crear memoria compartida para *n
        *n=*n+100;
        int num_p = *n;
        int mid = l + (r - l) / 2;
        // printf("\t");

        // printf("|Mapeo|\n");
        // usleep(*n*3);
        printf("|%2d|\t",*n/100);
        // printArray(arr,l,r);
        char * pruebas = stringArray(arr,l,r);
        printf("%s",pruebas);
        int j;
        for(j=0;pruebas[j]!='\0';j++){
            mapeo[*n/100-1][j]=pruebas[j];
        }
        free(pruebas);
        mapeo[*n/100-1][j]='\0';
        printf("\n");
        // sleep(0.5);
        while(*left){}
        *left=!*left;
        // sleep(0.5);
        // Crear proceso hijo para ordenar la mitad izquierda del arreglo
        printf("num = %d",num_p);
        if(num_p/100>7) {
            *left=0;
            printf("--------------------sera que funciona--------------------\n");
            mergeSort(arr, l, mid,n,left,mapeo);
            mergeSort(arr, mid + 1, r,n,left,mapeo);
        }
        else {
            pid_t left_child = fork();
            if (left_child == 0) { // Proceso hijo
                *left=1;
                mergeSort(arr, l, mid,n,left,mapeo);
                exit(EXIT_SUCCESS);
            }
            // Proceso padre
            // Crear proceso hijo para ordenar la mitad derecha del arreglo
            pid_t right_child = fork();
            if (right_child == 0) { // Proceso hijo
                usleep(1);
                *left=0;
                mergeSort(arr, mid + 1, r,n,left,mapeo);
                exit(EXIT_SUCCESS);
            }
            // Proceso padre
            // Esperar a que ambos hijos terminen
            waitpid(left_child, &status, 0);
            waitpid(right_child, &status, 0);
        }
        
        char* izq = stringArray(arr,l,mid);
        char* der = stringArray(arr,mid+1,r);

        // printf("|%d|\t",*n);
        // Fusionar los dos sub-arreglos ordenados

        // printf("--------------------------------------------\n");
        merge(arr, l, mid, r);
        if(num_p/100==7){
            printf("=================================Procesamiento=================================\n");
        }
        // sleep(1.5);
        printf("|%2d|\t",num_p/100);
        // imprimir(arr,l,r,mid);
        printProcess(arr,l,r,mid,izq,der);
        free(izq);
        free(der);
        
        // munmap(izq, sizeof(char)*((l - mid) * 2 + 4));
        // munmap(der, sizeof(char)*((mid+1 - r) * 2 + 4));
        sleep(0.1);
    }
}


int main() {
    int n = 16;
    char **mapeo = (char**)mmap(NULL, sizeof(char*)*7, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for(int i=0;i<7;i++){
        *(mapeo+i)=(char*)mmap(NULL, sizeof(char)*16, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    }
    int *left = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *left = 0;
    // int count = 0;
    int *number = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *number = 0;

    
    int *arr = (int*)mmap(NULL, sizeof(int)*16, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int next[] = {5,4,8,9,3,1,4,7,8,9,5,4,8,7,9,6};
    for(int i=0; i<16;i++){
        arr[i]=next[i];
    }
    // Llamar a la función de ordenamiento Merge Sort
    mergeSort(arr, 0, n - 1,number,left,mapeo);
    munmap(number, sizeof(int));
    munmap(arr, sizeof(int)*16);
    munmap(left, sizeof(int));
    printf("=========================Mapeos===========================\n");
    for (int i = 0; i < 7; i++) {
        printf("|%2d|\t%s\n",i,mapeo[i]);
    }
    printf("\n");
    return 0;
}