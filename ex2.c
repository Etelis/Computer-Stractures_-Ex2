//
// Itay Etelis - 209041474
//
#include "ex2.h"
#include "stdio.h"

/**
 * receives a number and checks if equals to negative zero, if so returns 0, if not will return nubmber.
 * @param a - the number in magnitude format.
 * @return 0 - if number is equals to negative zero, otherwise the number itself.
 */
// int null_fixer(magnitude a){
  //  int neg_zero = 0x80000000;
   // // check if a' equals zero.
   // return a == neg_zero ? 0 : a;
//}


/**
 * sign - will calculate the sign of the number sent in magnitude.
 * @param x - a singed magnitude based number.
 * @return 0 - if the number is possitive, 1 - if negative.
 */
int sign(magnitude x){
    unsigned int sign_mask = 0x80000000;
    return (sign_mask & x) > 0 ? 1 : 0;
}

/**
 * flip_sign - will flip the sign of a given number returning the same number with the flipped sign.
 * @param x - a singed magnitude based number.
 * @return the same number with flipped sign.
 */
magnitude flip_sign(magnitude x){
    if (x==0) return 0;
    unsigned int sign_mask = 0x80000000;
    return (magnitude) (x ^ sign_mask);
}
/**
 * add - will add two magnitude numbers and return the outcome.
 *  the given numbers could follow four simple formats:
 *  1 - a is positive, b is positive => a + b
 *  2 - a is positive, b is negative => a - b (use subtract function instead).
 *  3 - a is negative, b is positive => b - a (could use subtract function with flipped numbers).
 *  4 - a is negative, b is negative => -(a + b) (could return flipped sign of addon function).
 * @param a - first number.
 * @param b - second number.
 * @return a + b
 */
magnitude add(magnitude a, magnitude b){
    // calculate the sign of each number.
    unsigned int a_sign = sign(a);
    unsigned int b_sign = sign(b);

    // this if statement follows condition 2.
    if (!a_sign && b_sign)
        return sub(a, flip_sign(b));

    // this if statement follows condition 3.
    if (a_sign && !b_sign)
        return sub(b, flip_sign(a));

    // this if statement follows condition 4.
    if (a_sign && b_sign)
        return flip_sign(add(flip_sign(a), flip_sign(b)));

    // otherwise follows condition 1.
    int carry;

    while (b) {
        // calculate the carry and shift it once to achieve correct carry.
        carry = (a & b) << 1;
        // use xor to remain with only digits that will provide no carry.
        a = a ^ b;

        // apply carry on second number as the carry is the remaining number after the addon.
        b = carry;
    }
    a_sign = sign(a);
    // in case of an overflow switch two's compliment with int.
    return  (a_sign ? flip_sign(a) : a);
}

/**
 * sub - will subtract two magnitude numbers and return the outcome.
 *  the given numbers could follow four simple formats:
 *  1 - a is positive, b is positive => a - b
 *  2 - a is positive, b is negative => a - (-b) = a + b (use addon function instead).
 *  3 - a is negative, b is positive => -a - b = -(a+b) (could use addon function with flipped numbers).
 *  4 - a is negative, b is negative => -a -(-b) = -a + b = b - a (could return flipped sign of subtract function).
 * @param a - first number.
 * @param b - second number.
 * @return a + b
 */
magnitude sub(magnitude a, magnitude b){

    // calculate sign of each number.
    unsigned int a_sign = sign(a);
    unsigned int b_sign = sign(b);
    int borrow = 0;

    // this if statement follows condition 2.
    if (!a_sign && b_sign)
        return add(a, flip_sign(b));

    // this if statement follows condition 3.
    if (a_sign && !b_sign)
        return flip_sign(add(flip_sign(a), b));

    // this if statement follows condition 4.
    if (a_sign && b_sign)
        return sub(flip_sign(b), flip_sign(a));

    // the next if statement will check if b is larger than a' to preform opposite subtraction.
    if (greater(b, a))
        return (flip_sign(sub(b, a)));


    while (b != 0)
    {
        // borrow will hold the bits that needs the borrow.
        borrow = (~a) & b;

        // Subtraction of a and b where no borrow is needed.
        a = a ^ b;

        // borrow is shifted by 1 so the subtraction with a gives correct number.
        b = borrow << 1;
    }

    a_sign = sign(a);
    // in case of an overflow switch two's compliment with int.
    return a_sign ? flip_sign(a) : a;
}

/**
 * multi - will calculate the outcome of multiplying a time b.
 * @param a - first number.
 * @param b - second number.
 * @return a * b
 */
magnitude multi(magnitude a, magnitude b){

    // output sign will hold the return sign.
   unsigned int output_sign = sign(a) ^ sign(b);
   // pos_mask - will be used to unsign the numbers, neg_mask - will be used to sign the numbers.
   unsigned int pos_mask = 0x7FFFFFFF, neg_mask = 0x80000000;
   // make sure both numbers are positive numbers.
   a = (magnitude )(a & pos_mask);
   b = (magnitude )(b & pos_mask);
   // answer - the return value, count - will hold the amount of shifts needed for each step of the multiplication.
   int answer = 0, count = 0;

   while (b) {
       // check LSB of b is set - if so shift a by the stage (count) value
       if (b % 2 == 1)
           answer += a << count;
       // increment the stage.
       count++;
       // loose the LSB
       b = b >> 1;
   }
   // calculate answer based on expected output sign.
   answer = output_sign > 0 ? (magnitude)(answer | neg_mask) : (magnitude) (answer & pos_mask);

   // check if answer is neg zero, if so return zero.
   return (answer == neg_mask) ? 0 : answer;

}

/**
 * equal - check if both numbers represent same number.
 * @param a - first number.
 * @param b - second number.
 * @return a == b
 */
int equal(magnitude a, magnitude b){

    a = flip_sign(a) == 0 ? 0 : a;
    b = flip_sign(b) == 0 ? 0 : b;
    return a == b;
}

/**
 * greater - function will check if a is greater then b.
 * @param a - first number.
 * @param b - second number.
 * @return a > b?
 */
int greater(magnitude a, magnitude b){

    unsigned int a_sign, b_sign;
    // check if b equals 0 or neg zero, if so apply 0 to b otherwise leave as is.
    b = equal(b, 0) ? 0 : b;
    a = equal(a, 0 ) ? 0 : a;

    a_sign = sign(a), b_sign = sign(b);

    // if a and b has different signs return the positive number.
    if (a_sign != b_sign)
        return a_sign > b_sign ? 0 : 1;

    // if a and b are negative return the comp a >? b
    if (a_sign)
        return flip_sign(a) < flip_sign(b);

    // otherwise, both are positive numbers return a>?b.
    return a > b;
}

int print_magnitude(magnitude x){
    unsigned int mask = 0x7FFFFFFF, sign_mask = 0x80000000;
    int sign = (sign_mask & x) > 0 ? -1:1;
    return (int) ((x & mask) * sign);
}