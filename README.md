# LempelZivWelch
This code is an extension to the parsing assignment for the "COMSM1201: Programming in C" course at the University of Bristol (UK). For this assignment I initially created a parser for a simple derivation of the "logo" language, and then extended it with the repetition detection algorithm. You can find a short description of the algorithm below.

What if the parser could detect (nested) loops and propose a denser way of writing your program?
 
 In this exercise I used a pattern detection algorithm which recursively looks for loops within the code.  If there are any loops in the code, it proposes how to rewrite the code. It does so by scanning sets  (of length 2 to half the program size) of consecutive strings from all the different positions within the program.  When it finds a loop, it re-writes the code and checks for new patters which might have emerged. It does so until  it does not find any loops larger than 2. The code is inspired by compression algorithms like the Lempel-Ziv-Welch algorithm.   I put some examples in the folder, so you can test it for yourself :).  As an example, the code transforms this program:  
 
{    
   DO A FROM 1 TO 8 {
     FD 30       
     LT 45      
     FD 30       
     LT 45       
     FD 30       
     LT 45    
   }
   DO A FROM 1 TO 8 {
     FD 30       
     LT 45      
     FD 30       
     LT 45       
     FD 30       
     LT 45    
   } 
   DO A FROM 1 TO 8 {
     FD 30       
     LT 45      
     FD 30       
     LT 45       
     FD 30       
     LT 45    
   } 
}

into:

{
   DO Z FROM 1 TO 3 {
      DO A FROM 1 TO 8 {
         DO Z FROM 1 TO 3 {
            FD 30 LT 45
         }
      }
   }
}
