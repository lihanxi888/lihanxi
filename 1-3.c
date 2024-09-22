#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define MAX_CHANNELS 5
#define MAX_PRICE 9
#define MAX_QUANTITY 50

int main() {
    int Channel_num = 0, price = 0, Quantity = 0, Total_cost = 0, Total_payment = 0;
    char Item = ' ';

    printf("现在请设置通道号，货物名称，单价及储量\n");
    scanf(" %d %c %d %d", &Channel_num, &Item, &price, &Quantity);
    while (Channel_num < 1 || Channel_num > MAX_CHANNELS || price < 1 || price > MAX_PRICE || Quantity < 1 || Quantity > MAX_QUANTITY) {
        printf("请设置正确的通道号，单价及存量\n");
        scanf(" %d %c %d %d", &Channel_num, &Item, &price, &Quantity);
    }

    int Num = 0, quantity = 0;
    char product = ' ';
    printf("请输入通道号及货物名\n");
    scanf(" %d %c", &Num, &product);
    while (Num != Channel_num || Item != product) {
        printf("请输入正确的通道号与货物名！\n");
        scanf(" %d %c", &Num, &product);
    }

    while (Quantity > 0) {
        printf("请选择购买数量\n");
        scanf(" %d", &quantity);
        while (quantity > Quantity) {
            printf("库存不够，请重新选择购买数量\n");
            scanf(" %d", &quantity);
        }

        Total_cost = price * quantity;
        printf("您所需购买物品的总价为%d元\n", Total_cost);
        printf("请投币\n");

        int coin = 0;
        scanf(" %d", &coin);
        while (coin != 1 && coin != 2 && coin != 5) {
            printf("我们仅支持1，2，5的面额\n");
            scanf(" %d", &coin);
        }
        Total_payment += coin;

        while (Total_payment < Total_cost) {
            printf("请继续投币\n");
            scanf(" %d", &coin);
            while (coin != 1 && coin != 2 && coin != 5) {
                printf("我们仅支持1，2，5的面额\n");
                scanf(" %d", &coin);
            }
            Total_payment += coin;
        }

        int change = Total_payment - Total_cost;
        printf("找您 %d元\n", change);
        Total_payment = 0;
        Total_cost = 0;
        Quantity -= quantity; // 更新库存
    }

    return 0;
}
