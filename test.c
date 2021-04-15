/*#include <stdio.h>

#define ADD(str, num) printf("%s and |%i|\n", str, num);

int main(){
    
    int a = 0;
    printf("jopa\n");
    scanf("%i", &a);

    printf("I receive = %i\n", a);
    fclose(stdout);
    printf("after jopa\n");
    scanf("%i", &a);
    fclose(stdin);
}*/

#include <stdio.h>
#define N 10
#define M 10

void print(const int *arr, int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", *(arr + i));
}

int input(int matr[N][M], int *sizen, int *sizem)
{
    printf("Input matrix's sizes\n");
    if (scanf("%d", sizen) != 1 || *sizen > 10 || *sizen < 1)
    {
        printf("Error\n");
        return 1;
    }
    if (scanf("%d", sizem) != 1 || *sizem > 10 || *sizem < 1)
    {
        printf("Error\n");
        return 1;
    }
    printf("Input matrix\n");
    for (int i = 0; i < *sizen; i++)
        for (int j = 0; j < *sizem; j++)
            if (scanf("%d", &matr[i][j]) != 1)
            {
                printf("Error\n");
                return 1;
            }
    return 0;
}

void opera(int matr[N][M], int *arr, int sizen, int sizem)
{
    for (int i = 0; i < sizem; i++)
    {
        int flag = 1;
        for (int j = 1; j < sizen; j++)
        {
            if (matr[j][i] >= matr[j - 1][i])
            {
                flag = 0;
                break;
            }
        }
        arr[i] = flag;
    }
}

int main(void)
{
    setbuf(stdout, NULL);
    printf("枼 𓅌  𓀞𓀟𓀠𓀡𓀢𓀣𓀤𓀥𓀦𓀧𓀨𓀩𓀫𓀬𓀳𓀻𓀽𓀾𓀿𓀀𓀁𓀉𓀊𓀋" );
    printf(" in memory size of int = %d size of int* = %d and size of char* = %d\n", sizeof(int), sizeof(int*), sizeof(char*));

    int matr[N][M] = { { 0 } }, sizen, sizem;
    int arr[M] = { 0 };
    if (input(matr, &sizen, &sizem) == 1)
        return 1;
    opera(matr, arr, sizen, sizem);
    print(arr, sizem);
    return 0;
}