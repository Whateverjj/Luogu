/*
���ܣ�Huffman���ļ����б���ͽ���
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//���ṹ��
typedef struct Node
{
	unsigned char value; //���ֵ
	int frequency;		 //���Ƶ��
	struct Node *Lchild; //�����
	struct Node *Rchild; //�Ҷ���
};

//�����ַ��ṹ��
typedef struct
{
	unsigned char value; //�ַ�ֵ
	char code[100];		 //�ַ�����
	int frequency;		 // �ַ�Ƶ��
	int codelen;		 //�ַ����볤��
} HuffmanCode;

struct Node *root; //�����������ڵ�

//��ȡ�ļ�ͳ�Ƹ�����ĸ����Ƶ�ʣ������ַ�����
//filenameΪ�ļ�����*nodeΪ���ڱ������ݵ�����
int count(char filename[], HuffmanCode *node)
{
	FILE *fp = fopen(filename, "rb"); //���ļ�
	unsigned char now;				  //��ȡ�ĵ�ǰ�ַ�
	int num = 0;					  //�ַ�����

	fread(&now, sizeof(unsigned char), 1, fp); //��ȡ�ַ�
	while (!feof(fp))						   //δ�����ļ�β
	{
		int i;
		//�ظ��ַ�
		for (i = 0; i < num; i++)
		{
			if (now == node[i].value)
			{
				node[i].frequency++;
				break;
			}
		}
		//δ�ظ��ַ�
		if (i == num)
		{
			node[i].value = now;
			node[i].frequency = 1;
			num++;
		}
		fread(&now, sizeof(unsigned char), 1, fp); //��ȡ�ַ�
	}
	fclose(fp);
	return num;
}

//����Huffman��
//*nodeΪ���ڱ������ݵ����飬nΪ�ַ�����
struct Node *buildHuffman(HuffmanCode *node, int n)
{
	struct Node **arr, *temp; //arrΪ���������飬tempΪ���շ��ص���
	arr = (struct Node **)malloc(n * sizeof(struct Node));

	//��ʼ������
	for (int i = 0; i < n; i++)
	{
		arr[i] = (struct Node *)malloc(sizeof(struct Node));
		arr[i]->value = node[i].value;
		arr[i]->frequency = node[i].frequency;
		arr[i]->Lchild = arr[i]->Rchild = NULL;
	}

	//����
	for (int i = 0; i < n - 1; i++)
	{
		int m1 = -1, m2; //��С������Ȩֵ���������±�

		//��m1,m2ָ��ͷ������
		for (int j = 0; j < n; j++)
		{
			if (m1 == -1 && arr[j] != NULL)
			{
				m1 = j;
				continue;
			}
			if (arr[j] != NULL)
			{
				m2 = j;
				break;
			}
		}

		//�Ƚ�Ȩֵ��С,�ҵ���С������
		for (int j = m2; j < n; j++)
		{
			if (arr[j] != NULL && arr[j]->frequency < arr[m1]->frequency)
			{
				m2 = m1;
				m1 = j;
			}
			else if (arr[j] != NULL && arr[j]->frequency < arr[m2]->frequency)
				m2 = j;
		}
		//������Ȩֵ��С�Ĺ�������
		temp = (struct Node *)malloc(sizeof(struct Node));
		temp->Lchild = arr[m1];
		temp->Rchild = arr[m2];
		temp->frequency = arr[m1]->frequency + arr[m2]->frequency;
		temp->value = NULL;
		//��������������
		arr[m1] = temp;
		arr[m2] = NULL;
	}
	free(arr);
	return temp; //����tempΪ����
}

//Huffman����
//*nodeΪ���ڵ㣬lenΪ��ǰ���볤��,*arrΪ���ڱ������ݵ����飬numΪ�ַ�����
void codeByHuffman(struct Node *node, int len, HuffmanCode *arr, int num)
{
	static char code[100]; //����

	//����Ӳ�Ϊ�գ�����Ϊ0
	if (node->Lchild != NULL)
	{
		code[len] = '0';
		code[len + 1] = '\0';
		codeByHuffman(node->Lchild, len + 1, arr, num); //������ӵ��ô˺���
	}
	//�Ҷ��Ӳ�Ϊ�գ�����Ϊ1
	if (node->Rchild != NULL)
	{
		code[len] = '1';
		code[len + 1] = '\0';
		codeByHuffman(node->Rchild, len + 1, arr, num); //���Ҷ��ӵ��ô˺���
	}
	//Ҷ�ڵ�
	else
	{
		for (int i = 0; i < num; i++)
		{
			//�����븴�Ƹ��������Ԫ��
			if (arr[i].value == node->value)
			{
				strcpy(arr[i].code, code);
				arr[i].codelen = 0;
				for (int j = 0; arr[i].code[j] != '\0'; j++)
					arr[i].codelen++;
				return;
			}
		}
	}
}

//���ļ�����תΪHuffman����
//sourceFileΪԭ�ļ�����*arrΪ���ڱ������ݵ����飬nΪ�ַ�������aimFileΪĿ���ļ���
void writeCode(char sourceFile[], HuffmanCode *arr, int n, char aimFile[])
{
	FILE *fSource, *fAim;	//ԭ�ļ��Լ�Ŀ���ļ�
	unsigned char now;		//��ȡ�ĵ�ǰ�ַ�
	unsigned char save = 0; //ÿ�α����һ���ֽڳ���
	int len = 0;			//ÿ�α����һ���ֽ��Ѵ��˶��ٳ���
	int totalLen = 0;		//�ļ������ܳ���
	int last;				//���һ��д��ʱ��λ��

	//���ֽڳ���
	for (int i = 0; i < n; i++)
		totalLen = totalLen + arr[i].codelen * arr[i].frequency;
	//�������һ��д�����λ
	last = totalLen % 8;

	fSource = fopen(sourceFile, "rb");
	fAim = fopen(aimFile, "wb");

	//�Ƚ�huffman��������ļ�,��">"��Ϊ��ʶ�� ��������
	fwrite("->", 1, 2, fAim);
	fwrite(&n, sizeof(int), 1, fAim);	 //д���ַ�����
	fwrite(&last, sizeof(int), 1, fAim); //д�����һ��д���λ��
	//д����ַ�ֵ���ַ�Ƶ��
	for (int i = 0; i < n; i++)
	{
		fwrite(&arr[i].value, sizeof(unsigned char), 1, fAim);
		fwrite(&arr[i].frequency, sizeof(int), 1, fAim);
	}

	fread(&now, sizeof(unsigned char), 1, fSource); //��ȡԴ�ļ��ַ�

	while (!feof(fSource))
	{
		for (int i = 0; 1; i++)
		{
			if (now == arr[i].value)
			{
				for (int j = 0; j < arr[i].codelen; j++)
				{
					//��λ�����������
					if (len != 0)
						save = save << 1;
					save = save | (arr[i].code[j] - '0');
					len++;
					//һ���ֽ��Ѵ�����д���ļ�
					if (len == 8)
					{
						fwrite(&save, sizeof(unsigned char), 1, fAim);
						save = 0;
						len = 0;
					}
				}
				break;
			}
		}
		fread(&now, sizeof(unsigned char), 1, fSource); //��ȡ�ַ�
	}
	//���һ�ο���ûд��һ���ֽ�ҲҪд���ļ�
	if (len != 0)
		fwrite(&save, sizeof(unsigned char), 1, fAim);
	fclose(fSource);
	fclose(fAim);
}

//����
//sourceFileΪԭ�ļ�����*arrΪ���ڱ������ݵ����飬aimFileΪĿ���ļ���
void writeDecode(char sourceFile[], HuffmanCode *arr, char aimFile[])
{
	FILE *fSource, *fAim; //ԭ�ļ��Լ�Ŀ���ļ�
	int n;				  //�ַ�����
	int last = 0;		  //���һ�ζ�ȡ��λ��
	unsigned char now;	  //��ȡ�ĵ�ǰ�ַ�

	fSource = fopen(sourceFile, "rb");

	//ȷ���Ƿ��Դ˴�������Huffman
	for (int i = 0; i < 2; i++)
	{
		fread(&now, sizeof(char), 1, fSource); //��ȡ�ַ�
		if (i == 0)
		{
			if (now == '-')
				continue;
			else
			{
				printf("ԭ�ļ�huffman�����ʽ����ȷ\n");
				fclose(fSource);
				return;
			}
		}
		if (now == '>')
			break;
		else
		{
			printf("ԭ�ļ�huffman�����ʽ����ȷ\n");
			fclose(fSource);
			return;
		}
	}
	//
	fread(&n, sizeof(int), 1, fSource);	   //�ַ�����
	fread(&last, sizeof(int), 1, fSource); //���һ�ζ���λ��
	for (int i = 0; i < n; i++)
	{
		fread(&arr[i].value, sizeof(unsigned char), 1, fSource); //�ַ�
		fread(&arr[i].frequency, sizeof(int), 1, fSource);		 //�ַ�Ƶ��
	}

	struct Node *root = buildHuffman(arr, n); //����
	codeByHuffman(root, 0, arr, n);
	struct Node *pNow = root; //��ǰ���
	unsigned char temp;		  //ÿ�ζ�1���ֽ�
	fAim = fopen(aimFile, "wb");

	fread(&temp, sizeof(unsigned char), 1, fSource);
	while (!feof(fSource))
	{
		unsigned char ifLast; //�����ж��Ƿ�����ļ�ĩβ�������ȡ�̶�λ��
		fread(&ifLast, sizeof(unsigned char), 1, fSource);
		int i;
		if (feof(fSource))
			i = last - 1;
		else
			i = 7;
		for (; i >= 0; i--)
		{
			if ((temp >> i & 1) == 0)
				pNow = pNow->Lchild;
			else
				pNow = pNow->Rchild;
			if (pNow->Lchild == NULL && pNow->Rchild == NULL)
			{
				fwrite(&pNow->value, sizeof(unsigned char), 1, fAim);
				pNow = root;
			}
		}
		temp = ifLast;
	}
	fclose(fSource);
	fclose(fAim);
}

int main()
{
	while (1)
	{
		HuffmanCode node[2000];
		int num;				  //�ַ�����
		char Sfilename[100] = ""; //ԭ�ļ���
		char Afilename[100] = ""; //����ļ���
		char choose[3] = "";

		printf("��ѡ��:\n");
		printf("\t1.����\n");
		printf("\t2.����\n");
		printf("\t3.�˳�\n");

		scanf("%s", choose);
		fflush(stdin);
		if (!strcmp(choose, "1"))
		{
			while (1)
			{
				printf("����������Ҫ������ļ���(�������ո�):\n");
				scanf("%s", Sfilename);
				fflush(stdin);//ˢ���� stream �����������
				FILE *fp = fopen(Sfilename, "rb");
				if (fp == NULL)
				{
					printf("δ�ҵ��ļ�.\n");
					continue;
				}
				else
				{
					fclose(fp);
					break;
				}
			}
			printf("������������ļ���(�������ո�):\n");
			scanf("%s", Afilename);
			fflush(stdin);
			num = count(Sfilename, node);	//ͳ��
			root = buildHuffman(node, num); //����

			codeByHuffman(root, 0, node, num); //�����ַ���Huffman����

			writeCode(Sfilename, node, num, Afilename); //���Huffman�����ļ�
			printf("�������!\n\n");
		}
		else if (!strcmp(choose, "2"))
		{
			while (1)
			{
				printf("����������Ҫ������ļ���(�������ո�):\n");
				scanf("%s", Sfilename);
				fflush(stdin);
				FILE *fp = fopen(Sfilename, "rb");
				if (fp == NULL)
				{
					printf("δ�ҵ��ļ�.\n");
					continue;
				}
				else
				{
					fclose(fp);
					break;
				}
			}
			printf("�������������ļ���(�������ո�):\n");
			scanf("%s", Afilename);
			fflush(stdin);
			writeDecode(Sfilename, node, Afilename); //���벢д�ļ�
			printf("�������!\n\n");
		}
		else if (!strcmp(choose, "3"))
			break;
		else
		{
			printf("��������!\n");
			continue;
		}
	}
	return 0;
}