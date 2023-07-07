/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu1_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.8.0
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "isr_config.h"
#include "zf_common_headfile.h"

#pragma section all "cpu1_dsram"


// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��

// **************************** �������� ****************************
#define TEST  1
#define BLOCK 0
void core1_main(void)
{
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    // �˴���д�û����� ���������ʼ�������
    // outflag = 1;
    mt9v03x_init();
    tft180_init();
    // ips200_init(IPS200_TYPE_PARALLEL8);
    imu660ra_init();
    wireless_uart_init();
    dl1a_init();
    PID_int();
    gyroOffsetInit();
    Traits_status_init();
    // ��ʼ��pwm
    pwm_init(ATOM0_CH0_P21_2, 17 * 1000, 0);
    pwm_init(ATOM0_CH1_P21_3, 17 * 1000, 0);
    pwm_init(ATOM0_CH2_P21_4, 17 * 1000, 0);
    pwm_init(ATOM0_CH3_P21_5, 17 * 1000, 0);
    pwm_init(BUZZER, 3 * 1000, 0);


    encoder_dir_init(ENCODER_DIR_L, ENCODER_DIR_PULSE_L, ENCODER_DIR_DIR_L);
    encoder_dir_init(ENCODER_DIR_R, ENCODER_DIR_PULSE_R, ENCODER_DIR_DIR_R);

    gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY1 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY2 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY3 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY4 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(TOGGLE1, GPI, GPIO_HIGH, GPI_PULL_UP);        // ���뿪��1
    gpio_init(TOGGLE2, GPI, GPIO_HIGH, GPI_PULL_UP);        // ���뿪��2

    // ��ʼ��8·�˷�
    adc_init(ADC0_CH0_A0, ADC_8BIT);
    adc_init(ADC0_CH1_A1, ADC_8BIT);
    adc_init(ADC0_CH2_A2, ADC_8BIT);
    adc_init(ADC0_CH3_A3, ADC_8BIT);
    adc_init(ADC0_CH4_A4, ADC_8BIT);
    adc_init(ADC0_CH5_A5, ADC_8BIT);
    adc_init(ADC0_CH6_A6, ADC_8BIT);
    adc_init(ADC0_CH7_A7, ADC_8BIT);

    // tft180_set_color(RGB565_WHITE, RGB565_BLACK);
    tft180_set_font(TFT180_6X8_FONT);
    pit_ms_init(CCU60_CH0, 1);
    pit_ms_init(CCU60_CH1, 1);
    pit_ms_init(CCU61_CH0, 20);

    // Ĭ�����������ѭ��
    track_mode = NORMAL;

    int test_flag = 1;
    left_distance = 0;
    right_distance = 0;
    aim_theta = 90;
    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����

    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        TaskProcess();	
        clip_imageprocess();
        Traits_process();
        #if TEST
        track_mode = TURN;
        if (theta < aim_theta)
        {
            motor_ctrl(2000, -2000);
            Buzzer();
            if (theta > aim_theta)
                ;  
        }
        #endif


        // ����λ����������
        // sendimg_binary_CHK(clip_bin_image[0], MT9V03X_W, CLIP_IMAGE_H, image_thereshold, 35);     
        // sendimg_A(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);  
        // wireless_uart_send_image(clip_image[0], MT9V03X_IMAGE_SIZE);
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}



#pragma section all restore
// **************************** �������� ****************************

// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
