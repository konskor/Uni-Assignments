#include <stdio.h>
#include <stdlib.h>
#include "QueueInterface.h"



int main(void)
{
   int i,j,M,N;
   
   Queue *Q;

   Q=malloc(sizeof(Q));
 
   InitializeQueue(Q);

printf("How many will be in the circle?\n");
	scanf("%d\n",&N);
	getchar();
   
   for(i=1; i<=N; ++i){
      Insert(i, Q);
     
   }




printf("How many people will be spared each time (number)?\n");
scanf("%d\n",&M);
getchar();

LastManStanding(Q,M);

return 0;
}

















