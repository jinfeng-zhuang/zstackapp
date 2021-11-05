#ifndef MAIN_H
#define MAIN_H

#define LOG_CONFIG_LENGTH   (256)
#define CHANNEL_MAX			(3)
#define SAMPLE_SIZE         (9557)
#define FRAME_SIZE          (4096)
#define SAMPLE_FREQ         (15360)
#define FREQ_LIMIT_LOW      (5)     // 5Hz
#define FREQ_LIMIT_HIGH     (6000)  // 6KHz

#define BUFFER_SIZE			(SAMPLE_SIZE + FRAME_SIZE * 2 + FRAME_SIZE)

#define SAMPLE_CALC_ADDR    (SAMPLE_SIZE)
#define SAMPLE_AVER_ADDR    (SAMPLE_SIZE + FRAME_SIZE * 2)
#define SAMPLE_POWER_ADDR   (0)

#define ADDR_RAW            (0)
#define ADDR_CALC           (SAMPLE_SIZE)
#define ADDR_VEL            (ADDR_CALC)
#define ADDR_ACC            (ADDR_VEL + FRAME_SIZE)
#define ADDR_FFT            (SAMPLE_SIZE + FRAME_SIZE * 2)
#define ADDR_POWER          (SAMPLE_SIZE)

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char filename[FILENAME_MAX];
        unsigned int reference_speed_rpm;
    } param;

	// �����ļ�
	struct {
		unsigned int ref_speed_rpm;
		unsigned char chan_enable[CHANNEL_MAX];
	} config;
	
	// ���������ݻ�����
	// 117 KB, but sram total 112KB + 16KB
	// �Ż��漰����β������ɷ��ã��м��Ƿ��������
	// sample_area
    //union {
	float sample[BUFFER_SIZE];
	//struct {
	//	unsigned int D9557[9557]; // ��������֮��ֻ��Ҫǰ 1024 ����
	//	unsigned int fft_src[5*4096];
		// �����ζ��٣�
	//} sample;

    int lines;
    unsigned int low_cut;
	
	//�����ٲ��� + ����������
    struct {
        float max;
        float min;
        float mean;
        float aver;
        float rms;
        float smra;
        float formf;
        float crestfpos;
        float crestfneg;
        float impulsefpos;
        float impulsefneg;
        float marginfpos;
        float marginfneg;
        float kurtosis;
        float skewness;
    } waveform_parameter;

	//���۷��ֵ

    // ����λ��ͨƵֵ�����ֵ����Ƶ�׵�ƽ���ͣ���������
    float dsp_Overall;
    float dsp_Overall_um;

    // �����ٶ�ͨƵֵ����Ч������ֵ����Ƶ�׵�ƽ���ͣ���������
    float spectrum_vel;
    float vel_Overall;
    float vel_Overall_mmps;

    // ������ٶ�ͨƵֵ����Ч������ֵ����Ƶ�׵�ƽ���ͣ���������
    float spectrum_acc;
    float acc_Overall;
    float acc_Overall_gs;

    // ��ʵת��
    float spd_ref_hz;
    float spd_ref_line;
    float spd_true_line;
    float Amplitude_1xRPM_true;
    float spd_true_hz;
    float spd_true_rpm;

    // Ƶ�����
    float overall_pkpk;
    float para_nonsyn_value[10];
    float para_harmonic_value[10];

	// �������ֵ
    float true_pkpk_digit;
};

#endif
