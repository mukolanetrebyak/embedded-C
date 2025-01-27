#include <stdio.h>

void average_num(int a, int b, int c);
int average_num_return(int a, int b, int c);

int main()
{
    average_num(13,55,13);
    average_num(13,55,1);
    average_num(13,55,1344);
    
    int get_avrg = average_num_return(13,55,13);
    
    printf("Сума чисел з поверненням = %d\n", get_avrg);
    printf("Сума чисел з поверненням = %d\n", average_num_return(33,33,33));
    return 0;   
}

void average_num(int a, int b, int c)
{
    int avrg;
    avrg = (a+b+c)/3;
    printf("Сума чисел без поверненням = %d\n", avrg);
}

int average_num_return(int a, int b, int c)
{
    int ret_avrg;
    ret_avrg = (a+b+c)/3;
    return ret_avrg;
}

