/* 
 * The Data Lab 
 * CSCI 2467 Fall 2019
 *
 * SPECIAL EDITION: floating point puzzles
 * 
 **********
 * STEP 0 * <Please put your name and UNO email address here>
 **********
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
/******************************************************
 * STEP 1: Read the following instructions carefully. *
 ******************************************************/

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

You will ONLY turn in this file (bits.c), everything else
is provided to help you check your work.

/**********************************
 *  FLOATING POINT CODING RULES   *
 **********************************/

For this EXTRA CREDIT assignment, you will implement some common
single-precision floating-point operations.  In this section, you are
allowed to use standard control structures (conditionals, loops), and
you may use both int and unsigned data types, including
arbitrary unsigned and integer constants.  You may not use any unions,
structs, or arrays.  Most significantly, you may not use any floating
point data types, operations, or constants.  Instead, any
floating-point operand will be passed to the function as having type
unsigned, and any returned floating-point value will be of type
unsigned.  Your code should perform the bit manipulations that
implement the specified floating point operations.

Functions float_neg and float_twice must handle the full
range of possible argument values, including not-a-number (NaN) and
infinity.  The IEEE standard does not specify precisely how to handle
NaN's, and the IA32 behavior is a bit obscure.  We will follow a
convention that for any function returning a NaN value, it will return
the one with bit representation 0x7FC00000.

The included program fshow helps you understand the structure
of floating point numbers. You can use fshow to see what an
arbitrary pattern represents as a floating-point number:

  unix> ./fshow 2080374784

  Floating point value 2.658455992e+36
  Bit Representation 0x7c000000, sign = 0, exponent = f8, fraction = 000000
  Normalized.  1.0000000000 X 2^(121)

You can also give fshow hexadecimal and floating point values,
and it will decipher their bit structure.



NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of operations for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.
*/

/**********************************************************************************
 * STEP 2: Modify the following functions according the coding rules given above. *
 *                                                                                *
 *         You MUST explain each function in a comment                            *
 *         if you want to receive credit                                          *
 **********************************************************************************/
#endif
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
  return 2;
}
/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
  return 2;
}
/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
  return 2;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  return 2;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
 return 2;
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  return 2;
}
