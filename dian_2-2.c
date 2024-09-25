#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define CHANNELS 5
#define MAX_ITEMS 50
#define MAX_PRICE 9

typedef struct {//������һ������Ľṹ��
    char product;
    int price;
    int quantity;
} Item;

Item inventory[CHANNELS] = { 0 };//�⡾5��
int total_cost = 0;
int state = 1; // ��ǰ״̬
static int total_payment = 0; // ��¼��Ͷ�ҽ��

// ״̬����
enum {//ö�٣�����״̬���ڻ���ѡ�񣬹���(��ֵΪ1������������Ĭ��Ϊ0��
    PLACING = 1,
    SELECTING,
    PAYING
};

typedef struct {
    char product;
    int channel;
    int quantity;
} Action;//�����˽ṹ�壺��Ϊ.����Ϊ�������ջ��������ġ�

Action action_stack[3]; // ���ڴ洢�������������
int action_index = 0; // ��ǰ��������

// �ڷ���Ʒ
void place(char product, int channel, int price, int quantity) {//�ڷź���������ͨ������۸�����
    if (channel < 1 || channel > CHANNELS) {
        printf("ͨ�������Ч�������� 1 �� %d ֮�䡣\n", CHANNELS);
        return;
    }

    if (price < 1 || price > MAX_PRICE || quantity > MAX_ITEMS || quantity < 1) {
        printf("�۸����Ϊ1-9����������������Ϊ1-50��������\n");
        return;
    }
    if (inventory[channel - 1].quantity == 0) {//������Ϊ0ʱ�ſ��԰ڷ�
        inventory[channel - 1] = (Item){ product, price, quantity };//���û��������Ϣ���ϸ�ֵ��ȥ
        printf("���ѰڷŻ���: %c ��ͨ�� %d������ %d������ %d\n", product, channel, price, quantity);
        return;
    }
    printf("ͨ�� %d �޷������ڷŻ��\n", channel);
}

// ѡ����Ʒ
void select(char product, int channel, int quantity) {//ѡ����������ͨ����
    if (channel < 1 || channel > CHANNELS) {
        printf("ͨ�������Ч�������� 1 �� %d ֮���������\n", CHANNELS);
        return;
    }

    if (inventory[channel - 1].product == product && inventory[channel - 1].quantity >= quantity) {//�����Ʒ���棬channel-1����Ϊ�����һ��Ԫ�ش���Ϊ0
        total_cost += inventory[channel - 1].price * quantity;
        inventory[channel - 1].quantity -= quantity;

        // ��¼ѡ�����
        if (action_index < 3) {//���ȼ��������Ƿ�С��3����Ϊ���ֻ�ܽ���3����������
            action_stack[action_index++] = (Action){ product, channel, quantity };//������ԣ����˲�����¼������һ������Ĳ�������ʹ�õ���action_index++������++action_index
            //ǰ�����ȸ�ֵ���Լӣ����������Լ��ٸ�ֵ�����ʹ�ú��ߣ����ܻ����action_index����3����������Ǵ���ġ�
            //�����ĺô����Ǹ�ֵ�Ժ��Լ�һ�Σ��൱�ڼ�¼��һ��������
        }

        printf("ѡ����: %c ��ͨ�� %d���������� %d\n", product, channel, quantity);
        return;
    }
    printf("ͨ�� %d �е� %c ���ﲻ��򲻴��ڡ�\n", channel, product);
}

// ����ѡ��
void cancel() {
    if (action_index > 0) {
        //������Ҳ��ѧ�ʣ�֮�������Լ���ֵ��ԭ������������0��Ϊ��һ��Ԫ���±������趨��
        action_index--; // �������һ������
        Action last_action = action_stack[action_index];//�Լ��Ժ󣬲��ܹ������һ��������Ϊlast_action
        printf("��������: %c ��ͨ�� %d������ %d\n", last_action.product, last_action.channel, last_action.quantity);//�����һ�������Ĳ�Ʒ��ͨ����������ӡ
        // �ָ���Ʒ����
        inventory[last_action.channel - 1].quantity += last_action.quantity;
        total_cost -= inventory[last_action.channel - 1].price * last_action.quantity; // �ָ��ܻ���
    }
    else {
        printf("û�п��Գ����Ĳ�����\n");
    }
}
// Ͷ��
void pay(int payment) {
    total_payment += payment;
    printf("Ͷ�� %d Ԫ����ǰ�ܽ�� %d Ԫ�������� %d Ԫ\n", payment, total_payment, total_cost);

    while (total_payment < total_cost) {
        printf("�����Ͷ��\n");
        int coin = 0;
        scanf(" %d", &coin);
        while (coin != 1 && coin != 2 && coin != 5) {
            printf("����ֻ֧��1��2��5�����\n");
            scanf(" %d", &coin);
        }
        total_payment += coin;
    }

    int change = total_payment - total_cost;
    printf("���� %d Ԫ\n", change);
    total_payment = 0;  // ����Ͷ�ҽ��
    total_cost = 0;      // �����ܻ���
    state = SELECTING; // ״̬תΪѡ����Ʒ
}

int main() {
    char input[100];
    printf("��ڷ���Ʒ�����ջ��ͨ�������ۣ������ĸ�ʽ�ڷţ���һ��ͨ���������һ��\n");

    while (1) {
        fgets(input, sizeof(input), stdin);//�û�������ȫ�����fgets��������������ݲ�ͬ��������ͬ�����
        input[strcspn(input, "\n")] = '\0'; // �Ƴ����з�

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

        if (strcmp(input, "BACK") == 0) {
            cancel(); // ���ó�������
            continue;
        }

        if (state == PLACING) {
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
            int payment;
            if (sscanf(input, "%d", &payment) == 1 && (payment == 1 || payment == 2 || payment == 5)) {
                pay(payment);
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



