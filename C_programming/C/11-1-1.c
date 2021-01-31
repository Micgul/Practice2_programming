#include <stdio.h>

int main(void)
{
    int arr[5];
    int max, min, sum, i, j;

    //입력
    for (i = 0; i < 5; i++)
    {
        printf("Input five number: ");
        scanf_s("%d", &arr[i]);
    }

    sum = max = min = arr[0];
    for (j = 0; j < 5; j++)
    {
        sum += arr[j];
        if (max < arr[j])
            max = arr[j];
        if (min > arr[j])
            min = arr[j];
    }

    printf("Max: %d \n", max);
    printf("Min: %d \n", min);
    printf("SUm: %d \n", sum);
    return 0;

}
