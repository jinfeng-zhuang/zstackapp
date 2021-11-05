#include <zstack.h>
#include "hs.h"
#include "hs_attr.h"

struct hs_card *hs_draw_a_card(struct list_head *head) // + this
{
    struct list_head *cardpos;
    struct hs_card *card;
    struct list_head *attrpos;
    struct hs_attr *attr;

    // �������п��ƣ��������Ƶ�ÿһ�����ԣ��ҵ��ڿ��еĿ��ƣ�Ȼ�������Ը���
    // ͨ�����̣������¼����������п��ƣ��������Ƶ�ÿһ�����ԣ��ҵ�ƥ��ģ�ִ�����Ը���ǰ���������Ը��ģ�ִ�����Ը��ĺ����
    // ���ԣ����ƣ�������Ҫ�� event ��ӽ�ȥ�ͺ��ˣ�����Ҫ�Լ�ȥ����
    list_for_each(cardpos, head) {
        card = list_entry(cardpos, struct hs_card, list);

        list_for_each(attrpos, &card->attr) {
            attr = list_entry(attrpos, struct hs_attr, list);

            if ((attr->id == HS_ATTR_REGION) && (attr->value == HS_ATTR_REGION_LIB)) {
                // !!! value should be changed be system
                //attr->value = HS_ATTR_REGION_HAND;

                // add region change event to queue

                //list_add_tail();

                return card;
            }
        }
    }

    return NULL;
}
