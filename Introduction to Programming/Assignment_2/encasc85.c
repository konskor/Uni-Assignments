#include <stdio.h>
#include <math.h>

int  ypologismos_num( int ch);
 int count=0;             // char counter
 int num = 0;
 int total= 0;
 int i=3;                  // power rank 
 int line_count=0;         // char counter (of a single line)
 int lastchar;
/*
 * 
 */
int main() {
    int ch;
    int binnum;
    int char_count=0;
    int lasttotal=0;
    
    ch=getchar();                     // read 1st char
    if (ch != EOF)char_count=char_count+1;
    printf ("<~\n");
    
     while(ch != EOF ){           //read until EndOfFile
         lasttotal=total;
    
         num=ch;
         num=num * pow (256,i);      // exponentiate to 256 and create integer
         total=total + num;
      
         num=0;
         i=i-1;
         if (i <0) {
             i=3; 
             Conversion(total);  // KLISI ROUTINE METATROPH
             total=0;
         } 
         ch=getchar(); 
       
         if (ch != EOF)char_count=char_count+1;
         if (char_count==4) char_count=0;
       
    }
   
     if (char_count>1 && char_count<5) Last_Conversion(lasttotal,char_count);     // For last characters
     printf ("\n~>\n");
     
   
    return ;
}



int Conversion(int num)               // Conversion of char
{
   int binnum;
   char new_char;
   
   binnum=num%85;
   num=num/85;                        // converse to base 85
   
   if (num!=0)  Conversion(num);     // Recursive call , print from end to start
   binnum=binnum+33;                // Increase position of char (to the appropriate position in Ascii Table)
   
   new_char=putchar(binnum);        // Print new char
   
   
   line_count=line_count+1;           // Increment line_counter (characters in line)
   if (line_count==50){              // change line if line_counter = 50 
       binnum=putchar(10);           // putchar('\n')
       line_count=0;                 // resetting line counter
   }
  
   return 0;
}

int Last_Conversion(int num, int lastchar)       // Conversion of last chars
{
   int binnum;
   char new_char;
   
   binnum=num%85;                            // Converse to base 85
   num=num/85;
   
   if (num!=0)  Last_Conversion(num,lastchar);   // Recursive call , print from end to start
   binnum=binnum+33;                            // Increase position of char (to the appropriate position in Ascii Table)
   
  
   
   if (count < lastchar ) {          
   new_char=putchar(binnum);            // Print new char
   count = count +1 ;
 
   
   line_count=line_count+1;            // Increment line_counter (characters in line)       
   if (line_count==50){              // change line if line_counter = 50 
       binnum=putchar(10);                // putchar('\n')
       line_count=0;                       // resetting line counter
   }
   
   }
    
   return 0;
}
