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

Item inventory[CHANNELS][MAX_ITEMS] = { 0 }; // ��ʼ�����
int total_cost = 0;
int state = 1;  // ��ǰ״̬

// ״̬����
enum {
    PLACING = 1,
    SELECTING,
    PAYING
};

// �ڷ���Ʒ
void place(char product, int channel, int price, int quantity) {
    if (channel < 1 || channel > CHANNELS) {
        printf("ͨ�������Ч�������� 1 �� %d ֮�䡣\n", CHANNELS);
        return;
    }

    if (price < 0 || price>MAX_PRICE || quantity > 50 || quantity < 0) {
        printf("�۸����Ϊ1-9����������������Ϊ1-50��������\n");
        return;
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (inventory[channel - 1][i].quantity == 0) { // ����Ƿ��п�λ��������򽫻�����Ϣ������ȥ
            inventory[channel - 1][i] = (Item){ product, price, quantity };
            printf("���ѰڷŻ���: %c ��ͨ�� %d������ %d������ %d������������END�����ڷš�\n", product, channel, price, quantity);
            return; // �ɹ��ڷź󷵻�
        }
    }
    printf("ͨ�� %d �޷������ڷŻ��\n", channel);
}

// ѡ����Ʒ
void select(char product, int channel, int quantity) {
    if (channel < 1 || channel > CHANNELS) {
        printf("ͨ�������Ч������Ϊ 1 �� %d ֮���������\n", CHANNELS);
        return;
    }

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (inventory[channel - 1][i].product == product && inventory[channel - 1][i].quantity >= quantity) {
            total_cost += inventory[channel - 1][i].price * quantity;
            inventory[channel - 1][i].quantity -= quantity;
            printf("ѡ����: %c ��ͨ�� %d���������� %d������������END����ѡ��\n", product, channel, quantity);
            return;
        }
    }
    printf("ͨ�� %d �е� %c ���ﲻ��򲻴��ڡ�\n", channel, product);
}

// Ͷ��
void pay(int payment, int* total_payment) {
    *total_payment += payment;
    printf("Ͷ�� %d Ԫ����ǰ�ܽ�� %d Ԫ�������� %d Ԫ\n", payment, *total_payment, total_cost);

    while (*total_payment < total_cost) {
        printf("�����Ͷ��\n");
        int coin = 0;
        scanf(" %d", &coin);
        while (coin != 1 && coin != 2 && coin != 5) {
            printf("����ֻ֧��1��2��5�����\n");
            scanf(" %d", &coin);
        }
        *total_payment += coin;
    }

    int change = *total_payment - total_cost;
    printf("���� %d Ԫ\n", change);
    *total_payment = 0;  // ����Ͷ�ҽ��
    total_cost = 0;      // �����ܻ���
    state = SELECTING; // ״̬תΪѡ����Ʒ
}

int main() {
    char input[100]; // �������뻺������С
    printf("��ڷ���Ʒ�����ջ��ͨ�������ۣ������ĸ�ʽ�ڷţ���һ��ͨ���������һ��\n");

    while (1) {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';  // �Ƴ����з�

        if (strcmp(input, "END") == 0) {
            if (state == PLACING) {
                state = SELECTING;  // ��ɰڻ�������ѡ��״̬
                printf("��ѡ����Ʒ\n");
            }
            else if (state == SELECTING) {
                state = PAYING;  // ���ѡ�񣬽���Ͷ��״̬
                printf("�ܼ� %d\n", total_cost);
                printf("��Ͷ��\n");
            }
            else {
                break;
            }
        }

        if (state == PLACING) {
            // �ڻ�
            char product;
            int channel, price, quantity;
            if (sscanf(input, "%c %d %d %d", &product, &channel, &price, &quantity) == 4) {
                place(product, channel, price, quantity);
            }
            else {
                printf("�밴����Ʒ��ͨ�������ۣ������ĸ�ʽ����\n");
            }
        }
        else if (state == SELECTING) {
            // ѡ����Ʒ
            char product;
            int channel, quantity;
            if (sscanf(input, "%c %d %d", &product, &channel, &quantity) == 3) {
                select(product, channel, quantity);
            }
            else {
                printf("�밴��ȷ�ĸ�ʽ����(ex.A 1 2��ʾѡ��1ͨ����A����2����)\n");
            }
        }
        else if (state == PAYING) {
            // Ͷ��
            int payment;
            if (sscanf(input, "%d", &payment) == 1 && (payment == 1 || payment == 2 || payment == 5)) {
                static int total_payment = 0; // ��¼��Ͷ�ҽ��
                pay(payment, &total_payment);
                break; // ֧�����˳�ѭ��
            }
            else {
                printf("��������ЧӲ��\n");
            }
        }
    }
    printf("���ǽ����ı�־\n");
    return 0;
}
