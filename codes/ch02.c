
#include<stdio.h>
#include <math.h>

#define MAX_INT pow(2,sizeof(int)*8-1)-1;

void insertionSort(int* a,int n);
void selectSort(int* a, int n);
void merge(int a[],int p,int q, int r);
void mergeSort(int A[],int p,int r);
void mergeWithoutSentinels(int A[],int p,int q,int r);

int linerSearch(int *a, int n, int v);
int binarySearch(int* a,int n, int v);

int main(int argc, char const *argv[]) {
  int a[]={9,8,11,6,5,4,19,0,11,7,3,1,59,13};
  mergeSort(a, 0 ,sizeof(a)/sizeof(a[1])-1);
  int n=sizeof(a)/sizeof(a[1]);
  for(int i=0;i<n;i++){
    printf("%d,",a[i]);
  }
  int k=binarySearch(a, n, 5);
  printf("\n%d,",k);
  return 0;
}

int binarySearch(int* a,int n,int v){
  int i=0,k=n-1,j;
  while(i<=k){
    j=(i+k)/2;
    if(a[j]==v){
      return j;
    }else if(a[j]>v){
      k=j-1;
    }else{
      i=j+1;
    }
  }
  return -1;
}

void mergeWithoutSentinels(int A[],int p,int q,int r){
  int Ln=q-p+1,Rn=r-q;
  int L[Ln];
  int R[Rn];
  int i=0;
  for(i=0;i<q-p+1;i++){
    L[i]=A[p+i];
  }
  for(i=0;i<r-p;i++){
    R[i]=A[q+i+1];
  }
  i=0;
  int j=0;
  for(int k=p;k<=r;k++){
    if(i==Ln){
      A[k]=R[j++];
    }else if(j==Rn){
      A[k]=L[i++];
    }else if(L[i]<R[j]){
      A[k]=L[i++];
    }else{
      A[k]=R[j++];
    }
  }
}

void mergeSort(int A[],int p,int r){
  if(p<r){
    int q=(p+r)/2;
    mergeSort(A,p,q);
    mergeSort(A,q+1,r);
    //mergeWithoutSentinels(A, p, q, r);
    merge(A,  p,  q,  r);
  }
}

void merge(int A[],int p,int q, int r){
  int L[q-p+2];
  int R[r-q+1];
  int i=0;
  for(i=0;i<q-p+1;i++){
    L[i]=A[p+i];
  }
  for(i=0;i<r-q;i++){
    R[i]=A[q+i+1];
  }
  L[q-p+1]=R[r-q]=MAX_INT;
  i=0;
  int j=0;
  for(int k=p;k<=r;k++){
    if(L[i]<R[j]){
      A[k]=L[i];
      i++;
    }
    else{
      A[k]=R[j];
      j++;
    }
  }
}

void selectSort(int* a, int n){
  int min=0;
  for(int i=0;i<n-1;i++){
    min=i;
    for(int j=i+1;j<n;j++){
      if(a[j]<a[min])
        min=j;
    }
    int t=a[i];
    a[i]=a[min];
    a[min]=t;
  }
}

int linerSearch(int *a, int n, int v){
  for(int i=0;i<n;i++){
    if(a[i]==v)
      return i;
  }
  return -1;
}
void insertionSort(int* a,int n){
  for(int j=1;j<n;j++){
    int key=a[j];
    int index=j;
    for(int i=j-1;i>=0;i--){
      if(a[i]>key){
        a[i+1]=a[i];
        index=i;
      }
    }
    a[index]=key;
  }
  return;
}
