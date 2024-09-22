#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	printf("这是一个自动售货机。\n");
	printf("请输入货物种类，货物所在通道的标号，货物的单价以及数量。");
	int channal = 0;
	char product = 'A';
	int price = 0;
	int quantity = 0;
	scanf(" %c %d %d %d", &product, &channal, &price, &quantity);
	printf("您的货物种类是:%c 填入的通道是:%d 货物的单价是%d 货物的数量是%d\n", product, channal, price, quantity);
	int count = 0;
	for (; count < quantity; count++) {//循环打印物品
		printf(" %c", product);
	};
	return 0;
}