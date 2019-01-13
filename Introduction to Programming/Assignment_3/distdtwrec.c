




#include <stdio.h>
#include <stdlib.h>
double dtwrec(double *a, int n, double *b, int k);
/*
 * 
 */
int main(void) {
    int i,j;
    int r,t;
  
    double *a,*b;
    double DTW;
    
    scanf("%d",&r);
    a=calloc(i,sizeof(int));
    scanf("%d",&t);
    b=calloc(j,sizeof(int));
    
    for(i=0;i<r;i++){
        scanf("%lf",&a[i]);
	}

     for(j=0;j<t;j++){
         scanf("%lf",&b[j]);
	}

    for (i=0; i<r; i++){printf(" %2.2f ",a[i]);}
    printf("\n");
    for (j=0; j<t; j++){printf(" %2.2f ",b[j]);}
    
   // printf("\n DTW = %f ",dtwrec( a, i, b, j));
    DTW=dtwrec( a, i-1, b, j-1);
    printf("\n DTW = %f ",DTW);
        
    return 0;
}

double dtwrec(double *a, int n, double *b, int k){
    double  x1=0,x2=0,x3=0, min=0;
   
             if (n==0 && k==0) return (a[n]-b[k])* (a[n]-b[k]);
             if (n==0 && k>0) return (a[n]-b[k])* (a[n]-b[k])+dtwrec( a, n, b, k-1);
             if (n>0 && k==0) return (a[n]-b[k])* (a[n]-b[k])+dtwrec( a, n-1, b, k);
             if (n>0 && k>0){
                  x1=dtwrec( a, n-1, b, k);
                  x2=dtwrec( a, n, b, k-1);
                  x3=dtwrec( a, n-1, b, k-1);
                  min=x1;
                  if (x2<min) min=x2;
                  if (x3<min) min=x3;
               return (a[n]-b[k])* (a[n]-b[k])+ min;
              }
}
