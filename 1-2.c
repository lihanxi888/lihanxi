#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	int total_cost = 0, total_payment = 0, change = 0;
	int coin = 0;
	printf("�������ܼ�\n");
	scanf(" %d", &total_cost);
	printf("�������ֻ��Ϊ1��2��5\n");
	while (total_payment < total_cost) {
		scanf(" %d", &coin);
		while (coin != 1 && coin != 2 && coin != 5) {
			printf("��������ȷ�Ľ��\n");
			scanf(" %d", &coin);
		}
		total_payment += coin;
		printf("�ܼ�Ϊ %dԪ������֧�� %dԪ", total_cost, total_payment);
	}
	change = total_payment - total_cost;
	printf("���� %dԪ\n", change);

	return 0;
}