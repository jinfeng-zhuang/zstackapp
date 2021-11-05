# README

## Usage

xg9900 input_reference.txt

## Files

* source.txt - �� Python ���ɵ�ģ������һ����Ϊ��ȷ���ҵļ������Ե���

## Workflow

��ȡ�����ļ����ο�ת�٣�channel_enable��
�ϴ�������Ϣ�������װ

for channel=0,1,2 & channel_enable
	sample 3072
	delta = max - min
	if delta > threshold:
		channel_workable = 1

for channel=0,1,2 & channel_enable & channel_workable
	machine_running = 1

// don't implement it now
if 0 == machine_running
	// max - min => server
	// trigger 10 seconds wait

// ��һ�汾��Ĭ��ֻ��һ��ͨ��
for channel=0,1,2 & channel_enable & channel_workable & machine_running
	sample 9557 (12bit)
	convert to 32 bit float for future calc
	ȥ��������
	����ʱ���������ٲ���
	����ʱ���ε�����������
	�������۷��ֵ
	
	if (exist reference speed)
		for i = 0 ~ 5
			sample 4096
			hanning window
			FFT
			��һ��
		average => 1601 data
		
		��ȡ��ʵת�٣�������������
		
		�����ȡ��ʵת��
			����Ƶ��3��������ͬ����������ͬ����������ͬ������
			
		��ȡ�������ֵ ��ȥ���ƺ��9557 ���������㣩
		
		1601Ƶ�� float ת�� 16 bit �� + 9557 ��ǰ 1024 ���� ���͵�������
		
		���Ҫ�ϴ����Σ����ȼ���ͣ���
			�ɼ����������ݲ��ϴ�

## Data Base

#define CHANNEL_MAX		(3)

struct xg9900 {
	struct {
		unsigned int ref_speed;
		unsigned char chan_enable[CHANNEL_MAX];
	} config;
	
	// 117 KB, but sram total 112KB + 16KB
	// �Ż��漰����β������ɷ��ã��м��Ƿ��������
	// sample_area
	struct {
		unsigned int D9557[9557]; // ��������֮��ֻ��Ҫǰ 1024 ����
		unsigned int fft_src[5*4096];
		// �����ζ��٣�
	} sample;
	
	�����ٲ���
	����������
	���۷��ֵ
	Ƶ��3����
	�������ֵ
};
