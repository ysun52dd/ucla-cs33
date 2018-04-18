/* 
 * CS:APP Data Lab 
 * 
 * <Yutong Sun 104850256>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    //use and to get or operator
    int or_op=~((~x)&(~y));
    //get rid of the condition that x and y are both 1, bitwise
  return or_op&(~(x&y));
}
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {

return ((x + (x>>31))^(x>>31));
}
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
    int n=x+1;
    return !(n^(~x))&!!(~x);
    
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
    int dif=y+(~x+1);
    int checkSign=dif >> 31 & 1; //1 when y-x is negative, y<n
    
    //take care of one positive and one negative number
    //we need to use this case to handle overflow when y-x is too positive or too negative
    int xLeft= x & 1 << 31;
    int yLeft= y & 1 << 31;
    int xOrY= (xLeft ^ yLeft) >> 31 & 1;
    
    //Retrun 1 if one number is negative and one is positive and x is the negativve one
    //or y-x returns positive and x and y has the same sign
    return (xOrY & (xLeft>>31) )| (!checkSign & !xOrY);
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
    int a=0xFF; //11111111
    //calculate how many bits we need to shift to get the byte, n*8 or n<<3
    int nShift=n<<3;
    int mShift=m<<3;
    int nByte=(a<<nShift) & x;
    int mByte=(a<<mShift) & x;
    int combo=(a<<nShift) | (a<<mShift);//1s at positions at n m bytes
    nByte = (nByte >> nShift)&a;
    mByte = (mByte >> mShift)&a;
    /*shift swapping bytes to their new position*/
    nByte = nByte << mShift;
    mByte = mByte << nShift;
    /*store the nonchanging part of the int in "combo"*/
    combo = ~combo & x;
    
    return combo | mByte | nByte;
}
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
    
    int a, x2, x3, negN;
    negN = ~ n + 1;
    a = ~ (~ 0 << n);
    x2 = x >> (32+negN);
    x3 = x << n;
    return (x2 & a) | x3;
}


/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
    int sign = !(x >> 31);
    return (!!x) & sign & !((x + ~1 + 1) & x);
}
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
    //generate number with all 1s
    int n1=(0xAA<<8) | 0xAA;
    n1=n1 | (n1<<16);
    //xor when all even bits are 1 will return all 1s
    return !(~((x|n1)));
}
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
    x = ( x >> 0x10 ) ^ x;
    x = ( x >> 0x08 ) ^ x;
    x = ( x >> 0x04 ) ^ x;
    x = ( x >> 0x02 ) ^ x;
    x = ( x >> 0x01 ) ^ x;
    return (x & 0x01);
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
    // Mask 1 encompasses the 2 least significant bytes
    int mask1 = 0x11 | (0x11 << 8);

    // Mask 2 encompasses the final bytes
    int mask2 = mask1 | (mask1 << 16);

   
    int sum = x & mask2;
    sum = sum + ((x >> 1) & mask2);
    sum = sum + ((x >> 2) & mask2);
    sum = sum + ((x >> 3) & mask2);


    sum = sum + (sum >> 16);

    mask1 = 0xF | (0xF << 8);
    sum = (sum & mask1) + ((sum >> 4) & mask1);

    return((sum + (sum >> 8)) & 0x3F);

}
/* 
 * upperBits - pads n upper bits with 1's
 *  You may assume 0 <= n <= 32
 *  Example: upperBits(4) = 0xF0000000
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 1
 */
int upperBits(int n) {
    return ((1<<31<<!n)>>(n+ (1<<31>>31)));
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
    int sign = (x>>31) & 1;
    int signChain =~sign+1;
    //throwaway variable for various operations
    int placeHolder = 0;
    //counter to increment to count the bits
    int c = 2;
    //to be used for checking if power of 2
    int copy = x;
    //checking 0
    int copy2 = x;
    int tminCheck =  x ^ (1 << 31);
    tminCheck = !tminCheck;
    //all 1s if x was tmin
    tminCheck = ~tminCheck+1;
    //now a positive value
    x = (x & ~signChain) | ((~x +1) & signChain);
    x = x + ~0;
    x = (x | x >> 1);
    x = (x | x >> 2);
    x = (x | x >> 4);
    x = (x | x >> 8);
    x = (x | x >> 16);
    x = (x + 1);
    placeHolder = !(x & (0xFF | 0xFF << 8));
    placeHolder = ~placeHolder+1;
    c += (placeHolder & 16);
    
    placeHolder = !(x & (0xFF | 0xFF << 16));
    placeHolder = ~placeHolder + 1;
    c += (placeHolder & 8);
    
    placeHolder = 0x0F | 0x0F <<8;
    placeHolder = placeHolder | placeHolder <<16;
    placeHolder = !(x & placeHolder);
    placeHolder = ~placeHolder+1;
    c += (placeHolder & 4);
    
    placeHolder = 0x33 | 0x33 << 8;
    placeHolder = placeHolder | placeHolder << 16;
    placeHolder = !(x & placeHolder);
    placeHolder = ~placeHolder+1;
    c += (placeHolder & 2);
    
    placeHolder = 0x55 | 0x55 << 8;
    placeHolder = placeHolder | placeHolder << 16;
    placeHolder = !(x & placeHolder);
    placeHolder = ~placeHolder+1;
    c += (placeHolder & 1);
    
    
    copy2 = !copy2;
    copy2 = ~copy2+1;
    
    c += ~((tminCheck)&1);
    
    copy = copy & (copy+~0);
    c += !copy;
    return (c & ~copy2) | (copy2 & 1);
}
/*
 * satMul3 - multiplies by 3, saturating to Tmin or Tmax if overflow
 *  Examples: satMul3(0x10000000) = 0x30000000
 *            satMul3(0x30000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0x70000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0xD0000000) = 0x80000000 (Saturate to TMin)
 *            satMul3(0xA0000000) = 0x80000000 (Saturate to TMin)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 3
 */
int satMul3(int x) {
    int max = 1;
    int min = 0;
    int sign = 0;
    int aSign = 0;
    int midSign = 0;
    int sat = 0;
    int a = (x << 1);
   
    midSign = a >> 31;
    a = a + x;
    
    sign = x >> 31;
    aSign = a >> 31;
    
    //generate tmin and tmax
    min = max << 31;
    max = ~min;
    
    
    //checks to see if any of the steps of multiplying by 3 cause an overflow
    sat = (sign ^ aSign)  | (aSign ^ midSign);
    //saturates to tmin or tmax if any of the steps cause an overflow
    x = (a & ~sat) | (sat & ((sign & min) | (~sign & max)));
    return x;


}
