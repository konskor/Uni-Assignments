

#include <stdio.h>
#include <stdlib.h>
double dtwdp(double *a, int n, double *b, int k);

int main(void) {
    int i,j,p1,p2;
     int r,t;
   
    double num;
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
    
    printf("\n");
    DTW=dtwdp( a, i, b, j);
    printf(" Distance of time series is  %f ",DTW);
    
    
    
   
    return 0;
}

double dtwdp(double *a, int n, double *b, int k){
    double  x1=0,x2=0,x3=0, min=0;
    double p[n][k];
    double num;
    int i,j;
    
    printf("\n");
    for(i=0;i<n;i++){
        for(j=0;j<k;j++) {
     
             if (i==0 && j==0){ p[i][j] = (a[i]-b[j])* (a[i]-b[j]);}
                               //printf(" %2.2f ",p[i][j]);}
             
             if (i==0 && j>0) { p[i][j] = (a[i]-b[j])* (a[i]-b[j])+ p[i][j-1];}
                               // printf(" %2.2f ",p[i][j]);}
             
             if (i>0 && j==0){ min = p[i-1][j];
                              // if (p[i-1][j+1]<min) min=p[i-1][j+1];
                               p[i][j] = (a[i]-b[j])* (a[i]-b[j])+ min;} 
                              //printf(" %2.2f ",p[i][j]);}
             if (i>0 && j>0){
                  x1=p[i-1][j];
                  x2=p[i][j-1];
                  x3=p[i-1][j-1];
                  min=x1;
                  if (x2<min) min=x2;
                  if (x3<min) min=x3;
               p[i][j] = (a[i]-b[j])* (a[i]-b[j])+ min;
              // printf(" %2.2f ",p[i][j]);
               num=p[i][j];
              }
        }
    }
      return num;
  //  printf(" %2.2f ",p[i][j]);
   // return p[n][k];
}
             