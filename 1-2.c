#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	int total_cost = 0, total_payment = 0, change = 0;
	int coin = 0;
	printf("请输入总价\n");
	scanf(" %d", &total_cost);
	printf("请输入金额，只能为1，2，5\n");
	while (total_payment < total_cost) {
		scanf(" %d", &coin);
		while (coin != 1 && coin != 2 && coin != 5) {
			printf("请输入正确的金额\n");
			scanf(" %d", &coin);
		}
		total_payment += coin;
		printf("总价为 %d元，您已支付 %d元", total_cost, total_payment);
	}
	change = total_payment - total_cost;
	printf("找您 %d元\n", change);

	return 0;
}