#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	printf("����һ���Զ��ۻ�����\n");
	printf("������������࣬��������ͨ���ı�ţ�����ĵ����Լ�������");
	int channal = 0;
	char product = 'A';
	int price = 0;
	int quantity = 0;
	scanf(" %c %d %d %d", &product, &channal, &price, &quantity);
	printf("���Ļ���������:%c �����ͨ����:%d ����ĵ�����%d �����������%d\n", product, channal, price, quantity);
	int count = 0;
	for (; count < quantity; count++) {//ѭ����ӡ��Ʒ
		printf(" %c", product);
	};
	return 0;
}