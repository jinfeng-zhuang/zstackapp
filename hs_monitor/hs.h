#ifndef HS_H
#define HS_H

struct hs_attr;
struct hs_card;
struct hs_event;

///////////////////////////////////////////////////////////////////////////////

// ˫���������Ա�ʶ�Լ������ܱ�������
// Ӧ�û���������ƥ��Ŀ��ƣ����� evt �� dst ��
typedef void (*hs_on_event)(struct hs_card *card, struct hs_event *evt);

///////////////////////////////////////////////////////////////////////////////

// ������һ���ḻ�Ķ���
// ������ֵ�������㷽��������Ӧ���ƣ��бȽϷ������л���������
// ������Ӧ�������Զ����
struct hs_attr {
    int id;             // �ź�
    int compare;        // ���ڣ�С�ڣ����ڣ���������ʱ������
    int operate;        // ��ֵ���ӷ����˷�
    int value[3];       // �źŵ�ǰ��ƽ����Сֵ�����ֵ
    hs_on_event pre;    // �źű仯ǰ
    hs_on_event post;   // �źű仯��

    // for search
    //struct list_head search;

    struct list_head list;
};

// һ�����ģ���������ɵ�
struct hs_event {
    struct hs_card *src;

    struct list_head *dst;
    struct list_head attr2match; // �������� dst

    struct hs_attr operation;

    struct list_head list;
};

struct hs_card {
    unsigned int uuid;      // ����������̬�Ŀ��ƿ�

    unsigned int id;        // ���������ڴ��еĿ��ƣ����ʱ�Զ�����
    struct list_head attr;  // �������Ժ���Ӧ�����б���ʼ��ʱ��̬���ɣ��� JSON �ļ��ж�ȡ

    struct list_head list;  // ���ڴ���

    // for search
    struct list_head search;
};

///////////////////////////////////////////////////////////////////////////////

extern struct hs_card search_result;

///////////////////////////////////////////////////////////////////////////////
// Based on card list to search

// I/O may be a list
extern struct hs_card *hs_search(struct attr *attr);
extern struct list_head *hs_event_queue(void);
extern struct list_head *hs_card_queue(void);

#endif
