#include <stdio.h>


#define MAXNUM 20000
#define MAXCYC 5
int count_perfect=0, count_amic=0 ,count_total=0;
 
int find_divisor(int i){
    int k;
    int sum = 0;
    for (k = 1; k < i; k++) {
        if (i % k == 0)
            sum += k;
    }
    return sum;
}

int recursive_find_divisor(int i) {
    int k=0,m=0,j=0;
    int cyc=1,count=0;
    int sum = 0;
   // int count_perfect=0, count_amic=0 ;
    m=i;
    while(cyc<=MAXCYC  ){  
       sum=0;
       for (k = 1; k < i; k++) {
            if (i % k == 0)
              sum += k;
       }
        
       
     if (sum==m && cyc>2)
     {
         count_total=1;
         
          printf("\n Cycle of length %d : (%d,) ",cyc, sum);
          for(j=1; j<=cyc; j++){
          sum=0;
               for (k = 1; k < i; k++) {
                    if (i % k == 0)
                         sum += k;
                }
           i=sum;
           printf(" %d ", sum);  
           count=count+1;
          }
        // count=count+1;
          
         return 0; } 
        // break;}
       cyc=cyc+1;
       i=sum; 
      
    }
    return sum;
}



int main(void) {
    int i, previous = 0;
    int c = 1,n,t=1,s1=0,cyc;
    int count_perfect=0, count_amic=0 ;

    for (i = 1; i <= MAXNUM; i++) {
      
        if (previous == i) { 
           continue;
        
        }

        int k = find_divisor(i);
       // t=k;
        if (k == i && find_divisor(k) == i) {
            previous = k;
            printf("Cycle of length 1: (%d,%d)\n", i, k);
             count_perfect=count_perfect+1; 
             
        }
        
       if (k != i && find_divisor(k) == i) {
            previous = k;
            printf("\n Cycle of length 2: (%d,%d)\n", i, k);
             count_amic=count_amic+1;
        }  
        
       if (k != i && find_divisor(k) != i) {    
        recursive_find_divisor(i);  
       }
      
       
    }
    count_total=count_total+count_perfect+count_amic;
    printf(" \n  " ); 
    printf(" \n %d total Cycles  \n ",count_total ); 
    printf(" \n %d Cycles with perfect numbers and  \n ",count_perfect ); 
    printf("  %d Cycles with amicable numbers   \n ",count_amic );
    return 0;
}

