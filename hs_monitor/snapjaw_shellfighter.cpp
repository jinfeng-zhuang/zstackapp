#include "main.h"

// Ϊʲô��ִ�е������Ϊ�� health ��Ϊ key ע��ģ���ô�϶� evt ����Ľ�ɫ health �ǽ�Ҫ�����仯��
void HS_FUNC_SNAPJAW_SHELLFIGHTER(struct hs_card *card, struct hs_event *evt)
{
    struct list_head *card_pos, *event_pos;
    struct hs_card *tmp_card;
    //struct hs_event *evt;
    int ret;
    int left, right, pos;

    // �ж� HEALTH �Ƿ���� 0 ������ǣ��ж����� ATTR.HEALTH �Ƿ������٣������
    // �Ƴ� evt �еĸ���ӣ���Ϊ�����
    //

    // �� evt dst ��������λ�������Ŀ���
    // �� API ֧������������Ļ��������������ǵ�һ������������һ�ε��������
    // tmp_card = hs_search(evt->dst, HS_ATTR_BATTLE_POS, HS_OPERATION_EQUAL, left)
    // if (card)
    //   // �滻�ɸ���ӣ�evt ��������ֶ����ͬ�����
    //  hs_replace(evt->dst, tmp_card, card);
    //
    // API ��Ҫ��
    // CALLBACK ֻ���޸� evt �е���Ϣ�����Ը���Ҫ����ϵͳ���������Ҳ��
    // PRE FUNC �� event ���в��������߲����µ� event ���������Ƕ� event �����ɾ�����޸Ĳ�������Ҫ��� API
    // POST FUNC �أ������µ� event
}
