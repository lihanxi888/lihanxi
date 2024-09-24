#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define CHANNELS 5
#define MAX_ITEMS 50
#define MAX_PRICE 9

typedef struct {
    char product;
    int price;
    int quantity;
} Item;

Item inventory[CHANNELS][MAX_ITEMS] = { 0 }; // 初始化库存
int total_cost = 0;
int state = 1;  // 当前状态

// 状态定义
enum {
    PLACING = 1,
    SELECTING,
    PAYING
};

// 摆放商品
void place(char product, int channel, int price, int quantity) {
    if (channel < 1 || channel > CHANNELS) {
        printf("通道编号无效，必须在 1 到 %d 之间。\n", CHANNELS);
        return;
    }

    if (price < 0 || price>MAX_PRICE || quantity > 50 || quantity < 0) {
        printf("价格必须为1-9的整数，数量必须为1-50的整数。\n");
        return;
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (inventory[channel - 1][i].quantity == 0) { // 检查是否有空位，如果有则将货物信息设置上去
            inventory[channel - 1][i] = (Item){ product, price, quantity };
            printf("您已摆放货物: %c 在通道 %d，单价 %d，数量 %d，您可以输入END结束摆放。\n", product, channel, price, quantity);
            return; // 成功摆放后返回
        }
    }
    printf("通道 %d 无法继续摆放货物。\n", channel);
}

// 选择商品
void select(char product, int channel, int quantity) {
    if (channel < 1 || channel > CHANNELS) {
        printf("通道编号无效，必须为 1 到 %d 之间的整数。\n", CHANNELS);
        return;
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (inventory[channel - 1][i].product == product && inventory[channel - 1][i].quantity >= quantity) {
            total_cost += inventory[channel - 1][i].price * quantity;
            inventory[channel - 1][i].quantity -= quantity;
            printf("选择购买: %c 从通道 %d，购买数量 %d，您可以输入END结束选择\n", product, channel, quantity);
            return;
        }
    }
    printf("通道 %d 中的 %c 货物不足或不存在。\n", channel, product);
}

// 投币
void pay(int payment, int* total_payment) {
    *total_payment += payment;
    printf("投币 %d 元，当前总金额 %d 元，所需金额 %d 元\n", payment, *total_payment, total_cost);

    while (*total_payment < total_cost) {
        printf("请继续投币\n");
        int coin = 0;
        scanf(" %d", &coin);
        while (coin != 1 && coin != 2 && coin != 5) {
            printf("我们只支持1，2，5的面额\n");
            scanf(" %d", &coin);
        }
        *total_payment += coin;
    }

    int change = *total_payment - total_cost;
    printf("找零 %d 元\n", change);
    *total_payment = 0;  // 重置投币金额
    total_cost = 0;      // 重置总花费
    state = SELECTING; // 状态转为选择商品
}

int main() {
    char input[100]; // 增加输入缓冲区大小
    printf("请摆放商品，按照货物，通道，单价，数量的格式摆放，且一个通道最多设置一次\n");

    while (1) {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';  // 移除换行符

        if (strcmp(input, "END") == 0) {
            if (state == PLACING) {
                state = SELECTING;  // 完成摆货，进入选择状态
                printf("请选择商品\n");
            }
            else if (state == SELECTING) {
                state = PAYING;  // 完成选择，进入投币状态
                printf("总价 %d\n", total_cost);
                printf("请投币\n");
            }
            else {
                break;
            }
        }

        if (state == PLACING) {
            // 摆货
            char product;
            int channel, price, quantity;
            if (sscanf(input, "%c %d %d %d", &product, &channel, &price, &quantity) == 4) {
                place(product, channel, price, quantity);
            }
            else {
                printf("请按照商品，通道，单价，数量的格式输入\n");
            }
        }
        else if (state == SELECTING) {
            // 选择商品
            char product;
            int channel, quantity;
            if (sscanf(input, "%c %d %d", &product, &channel, &quantity) == 3) {
                select(product, channel, quantity);
            }
            else {
                printf("请按正确的格式购买(ex.A 1 2表示选择1通道的A货物2个。)\n");
            }
        }
        else if (state == PAYING) {
            // 投币
            int payment;
            if (sscanf(input, "%d", &payment) == 1 && (payment == 1 || payment == 2 || payment == 5)) {
                static int total_payment = 0; // 记录总投币金额
                pay(payment, &total_payment);
                break; // 支付后退出循环
            }
            else {
                printf("请输入有效硬币\n");
            }
        }
    }
    printf("这是结束的标志\n");
    return 0;
}
