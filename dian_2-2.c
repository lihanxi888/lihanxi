#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define CHANNELS 5
#define MAX_ITEMS 50
#define MAX_PRICE 9

typedef struct {//定义了一个货物的结构体
    char product;
    int price;
    int quantity;
} Item;

Item inventory[CHANNELS] = { 0 };//库【5】
int total_cost = 0;
int state = 1; // 当前状态
static int total_payment = 0; // 记录总投币金额

// 状态定义
enum {//枚举，三种状态，摆货，选择，购买(赋值为1，递增，否则默认为0）
    PLACING = 1,
    SELECTING,
    PAYING
};

typedef struct {
    char product;
    int channel;
    int quantity;
} Action;//定义了结构体：行为.这是为了下面的栈储存操作的。

Action action_stack[3]; // 用于存储最近的三个操作
int action_index = 0; // 当前操作索引

// 摆放商品
void place(char product, int channel, int price, int quantity) {//摆放函数，检验通道号与价格、数量
    if (channel < 1 || channel > CHANNELS) {
        printf("通道编号无效，必须在 1 到 %d 之间。\n", CHANNELS);
        return;
    }

    if (price < 1 || price > MAX_PRICE || quantity > MAX_ITEMS || quantity < 1) {
        printf("价格必须为1-9的整数，数量必须为1-50的整数。\n");
        return;
    }
    if (inventory[channel - 1].quantity == 0) {//仅数量为0时才可以摆放
        inventory[channel - 1] = (Item){ product, price, quantity };//将用户输入的信息整合赋值进去
        printf("您已摆放货物: %c 在通道 %d，单价 %d，数量 %d\n", product, channel, price, quantity);
        return;
    }
    printf("通道 %d 无法继续摆放货物。\n", channel);
}

// 选择商品
void select(char product, int channel, int quantity) {//选择函数，检验通道号
    if (channel < 1 || channel > CHANNELS) {
        printf("通道编号无效，必须在 1 到 %d 之间的整数。\n", CHANNELS);
        return;
    }

    if (inventory[channel - 1].product == product && inventory[channel - 1].quantity >= quantity) {//检验产品与库存，channel-1是因为数组第一个元素代号为0
        total_cost += inventory[channel - 1].price * quantity;
        inventory[channel - 1].quantity -= quantity;

        // 记录选择操作
        if (action_index < 3) {//首先检查操作数是否小于3，因为最多只能进行3步撤销操作
            action_stack[action_index++] = (Action){ product, channel, quantity };//如果可以，将此操作记录，这是一个奇妙的操作，我使用的是action_index++而不是++action_index
            //前者是先赋值再自加，后者是先自加再赋值，如果使用后者，可能会造成action_index等于3的情况，这是错误的。
            //这样的好处就是赋值以后，自加一次，相当于记录了一步操作。
        }

        printf("选择购买: %c 从通道 %d，购买数量 %d\n", product, channel, quantity);
        return;
    }
    printf("通道 %d 中的 %c 货物不足或不存在。\n", channel, product);
}

// 撤销选择
void cancel() {
    if (action_index > 0) {
        //这里面也有学问，之所以先自减后赋值，原因在于数组以0作为第一个元素下标的奇怪设定。
        action_index--; // 撤销最后一个操作
        Action last_action = action_stack[action_index];//自减以后，才能够将最后一步操作记为last_action
        printf("撤销操作: %c 从通道 %d，数量 %d\n", last_action.product, last_action.channel, last_action.quantity);//将最后一步操作的产品，通道，数量打印
        // 恢复商品数量
        inventory[last_action.channel - 1].quantity += last_action.quantity;
        total_cost -= inventory[last_action.channel - 1].price * last_action.quantity; // 恢复总花费
    }
    else {
        printf("没有可以撤销的操作。\n");
    }
}
// 投币
void pay(int payment) {
    total_payment += payment;
    printf("投币 %d 元，当前总金额 %d 元，所需金额 %d 元\n", payment, total_payment, total_cost);

    while (total_payment < total_cost) {
        printf("请继续投币\n");
        int coin = 0;
        scanf(" %d", &coin);
        while (coin != 1 && coin != 2 && coin != 5) {
            printf("我们只支持1，2，5的面额\n");
            scanf(" %d", &coin);
        }
        total_payment += coin;
    }

    int change = total_payment - total_cost;
    printf("找零 %d 元\n", change);
    total_payment = 0;  // 重置投币金额
    total_cost = 0;      // 重置总花费
    state = SELECTING; // 状态转为选择商品
}

int main() {
    char input[100];
    printf("请摆放商品，按照货物，通道，单价，数量的格式摆放，且一个通道最多设置一次\n");

    while (1) {
        fgets(input, sizeof(input), stdin);//用户的输入全靠这个fgets函数，输入的内容不同，引发不同结果。
        input[strcspn(input, "\n")] = '\0'; // 移除换行符

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

        if (strcmp(input, "BACK") == 0) {
            cancel(); // 调用撤销函数
            continue;
        }

        if (state == PLACING) {
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
            int payment;
            if (sscanf(input, "%d", &payment) == 1 && (payment == 1 || payment == 2 || payment == 5)) {
                pay(payment);
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



