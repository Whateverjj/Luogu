#include <stdio.h>

const int M = 19106206; //ѧ��

int Sum(int arr[][8])
{
    int sum = 0;
    for (int i = 0; i < 8; i++) //˫��ѭ���ṹ��������
    {
        for (int j = 0; j < 8; j++)
        {
            sum += arr[i][j];
        }
    }
    return sum;
}

void Print(int arr[][8])
{
    for (int i = 0; i < 8; i++) //˫��ѭ���ṹ��������
    {
        for (int j = 0; j < 8; j++)
        {
            printf("%10d", arr[i][j]);
        }
        printf("\n");
    }
}

void createA(int arr[][8])
{
    for (int i = 0; i < 8; i++) //˫��ѭ���ṹ��������
    {
        for (int j = 0; j < 8; j++)
        {
            if (i == j) //ѡ���֧�ṹ���Բ�ͬ�±��Ԫ�ظ�ֵ
            {
                arr[i][j] = M;
            }
            else if (i < j)
            {
                arr[i][j] = i + j;
            }
            else
            {
                arr[i][j] = i * j;
            }
        }
    }
}

int main()
{
    

    int arr[8][8] = {0}; //��ʼ����ά����
    
    createA(arr);

    printf("��ά�������£�\n");

    Print(arr); //��ӡ����

    printf("��ά����Ԫ����ͽ��Ϊ��"); //��ӡ������
    int sum = Sum(arr);
    printf("%d", sum);
}

