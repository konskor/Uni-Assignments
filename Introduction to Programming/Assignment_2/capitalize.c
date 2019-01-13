/* File: capitalize.c */
#include <stdio.h>


int main()
{ int ch; /* Be careful! Declare ch as int because of getchar() and EOF */
  ch = getchar();                               /* Read first character */
  while (ch != EOF) {           /* Go on if we didn't reach end of file */
   
    if (ch >= 'A' && ch <= 'Z')                 /* If lower case letter */
      		ch = ch - ('A'-'a'); /* Move 'a'-'A' positions in the ASCII table */
    
	putchar(ch);                                 /* Print out character */
    ch = getchar();                              /* Read next character */
  }
  
  return 0;
  
}
