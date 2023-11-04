/* This program should read a single integer from standard input then
 * compute its factorial. For example, if the user enters 5, the program should
 * output 120 because 1*2*3*4*5=120. */

#include <stdio.h>

int factorial(int n)
{
    int result = n;
    
    //while n is a non-negative int 
    
    if (n == 0)
    {
        return 1;
    }
    result = factorial(n-1) * result;

    return result;
    
}

int main()
{
    int n, result;

    //if input is 0 or negative 
    if (scanf("%d", &n) != 1) 
    {
        return 1;
    }
    else if (n < 0)
    {
        printf("Error: Please enter a non-negative integer.\n");
    }
    //if input is non-negative 
    else
    {
       
        result = factorial(n);
        printf("%d\n", result);
    }  
    return 0;

}