#include <stdio.h>
#include <math.h>


 int count=3;
 int num = 0;
 int total= 0;
 int i=4;
 int lastchar;
 int endchar=0;
/*
 * 
 */
int main() {
    int start1,start2;
    int ch;
    int binnum;
    int char_count=0;
    int lasttotal=0;
    
    start1=getchar();     //Start check
    start2=getchar();
    if (start1 != '<' || start2 != '~'){
       printf ("Bad Start\n");
       return 0;
    }
   
    ch=getchar();
    if (ch==' ' || ch=='\t' || ch=='\n') ch=getchar();  // Non-permitted characters
    if (ch != EOF)char_count=char_count+1;
  
    
   
     while(ch != EOF ){
         lasttotal=total;
       
         num=ch;
         num=num-33;                // move chars in ASCII
         num=num * pow (85,i);      // Converse to base 85
         total=total + num;
         num=0;
         i=i-1;
    
         if (i <0) {
             i=4; 
             Converse(total);  // Converse() call
             total=0;
         } 
         ch=getchar(); 
         
           if (ch==' ' || ch=='\t' || ch=='\n') ch=getchar();        /// Non-permitted characters
          // if (ch < 32 || ch > 177) printf(" Bad input character \n");
           if (ch !=10 && ch != -1) endchar = ch;                     //keep last char
          
           if (ch != EOF)char_count=char_count+1;
           if (char_count==5) char_count=0;
       
    }
    
   
     if (char_count==3) lasttotal=lasttotal + 614124;
     if (char_count==4) lasttotal=lasttotal + 7224;
     if (char_count==5) lasttotal=lasttotal + 84;
     if (char_count>2 && char_count<6) Last_Converse(lasttotal,char_count);     // For last chars
     
  

     if (endchar != '>' ){         //end check (if we were successfull )              
    	printf ("\n Bad End\n");
       	return -1;
    } 
   
    return 0;
}



int Converse(int num)     // Converse from base 85 to 256
{
   int binnum;
   char new_char;
   
   binnum=num%256;
   num=num/256;
   if (num!=0)  Converse(num);   // Recursive call, print in reverse
   
   new_char=putchar(binnum);
   
   return 0;
}

int Last_Converse(int num, int lastchar)    // Converse, if there are less than 3 chars at the end
{
   int binnum;
   char new_char;
    
   binnum=num%256;
   num=num/256;
   
   if (num!=0)  metatropi_telous(num,lastchar);   // Recursive call, print in reverse
   
   if (count < lastchar ) {     
   new_char=putchar(binnum);
   count = count +1 ;
   }
   return 0;
}

