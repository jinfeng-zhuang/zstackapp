
// ��������ֵ�ͼӷ����˷��������������ţ���������ѧϵ��ң�

// �����ɵ�
// event_add(NULL, "range:type=retinue|hero,camp=enemy,select=random", blood, add, -1)
// event_add(evt, "range:type=retinue|hero,camp=enemy,select=random", blood, add, -1)
// event_add(evt, "range:type=retinue|hero,camp=enemy,select=random", blood, add, -1)

// �������ǣ�����˵�����ֿܷ������� target ��ʧЧ�ˣ�����
// event_add(NULL, "range:type=retinue,camp=enemy,select=target", blood, add, -1)
// left = target.left, right = target.right // �����ڴ˴����ɽ������Ҫ��ʱ����
// event_add(evt, "range:type=retinue,camp=enemy,select=target.left", blood, add, -1)
// event_add(evt, "range:type=retinue,camp=enemy,select=target.right", blood, add, -1)
// ���ϣ���Ҫ֧��һЩ�����������û�ѡ��� TARGET ���� ID ���ͼ�һЩ����

// ǯ������
// �޸�������ɵ� dst ֱ����������ֵ <= 0

// ��
// event_add(NULL, "dst:type=retinue,attack>4", field, equal, die)
