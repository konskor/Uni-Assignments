

#include <stdio.h>
#include <stdlib.h>
//#define REC

const double infty=1.0/0.0;
double distcdtwrec(double *a, int n, double *b, int k,  unsigned int c);
double distcdtwdp(double *a, int n, double *b, int k, unsigned int c);
/*
 * 
 */
int main(int argc ,const char *argv[]) {
    int i,j;
    int r,t;
    double num;
    
    unsigned int c;
  
    double *a,*b;
    double DTW;
    
    if (argc>1){
        c=atoi(argv[1]);
         printf("\n c = %u ",c);
    }
    if (argc == 1 ) {
     c=99999;
     printf("\n c = %f  ",infty);}
    
    scanf("%d",&t);
    r=t;
    a=calloc(i,sizeof(int));
    b=calloc(j,sizeof(int));
    
    for(i=0;i<r;i++){
        scanf("%lf",&a[i]);
	}

     for(j=0;j<t;j++){
         scanf("%lf",&b[j]);
	}
     printf("\n");
    for (i=0; i<r; i++){printf(" %2.2f ",a[i]);}
    printf("\n");
    for (j=0; j<t; j++){printf(" %2.2f ",b[j]);}
    
#ifdef REC 
    DTW=distcdtwrec( a, i-1, b, j-1, c);
#else
    DTW=distcdtwdp( a, i, b, j, c);
#endif
    printf("\n DTW = %f ",DTW);
    return 0;
}

double distcdtwrec(double *a, int n , double *b, int k,  unsigned int c){
    double  x1=99999,x2=99999,x3=99999, min=0;
    int d;
    
    d=abs(n-k);
    
             if (n==0 && k==0 && d<=c) return (a[n]-b[k])* (a[n]-b[k]);
             if (n==0 && k>0 && d<=c) return (a[n]-b[k])* (a[n]-b[k])+distcdtwrec( a, n, b, k-1, c);
             if (n>0 && k==0 && d<=c) return (a[n]-b[k])* (a[n]-b[k])+distcdtwrec( a, n-1, b, k, c);
             if (n>0 && k>0 && d<=c ){
                  x1=distcdtwrec( a, n-1, b, k, c);
                  x2=distcdtwrec( a, n, b, k-1, c);
                  x3=distcdtwrec( a, n-1, b, k-1, c);
                  if (x1 != x1) x1=99999;
                  if (x2 != x2) x2=99999;
                  if (x3 != x3) x3=99999;
                
                  min=x1;
                  if (x2<min) min=x2;
                  if (x3<min) min=x3;
                 
                  return (a[n]-b[k])* (a[n]-b[k])+ min;
              }
  
}

double distcdtwdp(double *a, int n, double *b, int k, unsigned int z  ){
    double  x1=0,x2=0,x3=0, min=0;
    double p[n][k];
    double num;
    int i,j;
    int d;
    
     for(j=0;j<k;j++){
        for(i=0;i<n;i++) {
            p[i][j]=99999;
        }
     } 
    
    printf("\n");

    for(j=0;j<k;j++){
        for(i=0;i<n;i++) {
            d=abs(i-j);
      
             if (i==0 && j==0 && d<=z){ p[i][j] = (a[i]-b[j])* (a[i]-b[j]);}
       
             if (i==0 && j>0 && d<=z) { p[i][j] = (a[i]-b[j])* (a[i]-b[j])+ p[i][j-1];}
            
             if (i>0 && j==0 && d<=z ){ min = p[i-1][j];
                               p[i][j] = (a[i]-b[j])* (a[i]-b[j])+ min; }
       
             if (i>0 && j>0 && d<=z){
                  x1=p[i-1][j];
                  x2=p[i][j-1];
                  x3=p[i-1][j-1];
                  min=x1;
                  if (x2<min) min=x2;
                  if (x3<min) min=x3;
               p[i][j] = (a[i]-b[j])* (a[i]-b[j])+ min;
           
               num=p[i][j];
              }
   
       }      
    }
      return num;
  
}
 