#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define MAX_CHANNELS 5
#define MAX_PRICE 9
#define MAX_QUANTITY 50

int main() {
    int Channel_num = 0, price = 0, Quantity = 0, Total_cost = 0, Total_payment = 0;
    char Item = ' ';

    printf("����������ͨ���ţ��������ƣ����ۼ�����\n");
    scanf(" %d %c %d %d", &Channel_num, &Item, &price, &Quantity);
    while (Channel_num < 1 || Channel_num > MAX_CHANNELS || price < 1 || price > MAX_PRICE || Quantity < 1 || Quantity > MAX_QUANTITY) {
        printf("��������ȷ��ͨ���ţ����ۼ�����\n");
        scanf(" %d %c %d %d", &Channel_num, &Item, &price, &Quantity);
    }

    int Num = 0, quantity = 0;
    char product = ' ';
    printf("������ͨ���ż�������\n");
    scanf(" %d %c", &Num, &product);
    while (Num != Channel_num || Item != product) {
        printf("��������ȷ��ͨ�������������\n");
        scanf(" %d %c", &Num, &product);
    }

    while (Quantity > 0) {
        printf("��ѡ��������\n");
        scanf(" %d", &quantity);
        while (quantity > Quantity) {
            printf("��治����������ѡ��������\n");
            scanf(" %d", &quantity);
        }

        Total_cost = price * quantity;
        printf("�����蹺����Ʒ���ܼ�Ϊ%dԪ\n", Total_cost);
        printf("��Ͷ��\n");

        int coin = 0;
        scanf(" %d", &coin);
        while (coin != 1 && coin != 2 && coin != 5) {
            printf("���ǽ�֧��1��2��5�����\n");
            scanf(" %d", &coin);
        }
        Total_payment += coin;

        while (Total_payment < Total_cost) {
            printf("�����Ͷ��\n");
            scanf(" %d", &coin);
            while (coin != 1 && coin != 2 && coin != 5) {
                printf("���ǽ�֧��1��2��5�����\n");
                scanf(" %d", &coin);
            }
            Total_payment += coin;
        }

        int change = Total_payment - Total_cost;
        printf("���� %dԪ\n", change);
        Total_payment = 0;
        Total_cost = 0;
        Quantity -= quantity; // ���¿��
    }

    return 0;
}
