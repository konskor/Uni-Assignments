/* This code is from Chapter 4 of the book "Algorithms in C" by Robert Sedgewick. */
#include "Item.h"
typedef struct STACKnode* link;

void STACKinit(int);
 int STACKempty();
void STACKpush(ItemStack item);
ItemStack STACKpop();

void Post_to_Infix(char *,int);
void ShowStack(link);
