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
// buzzer p33.1
#define PIT_CCU60_ms 5
unsigned char outflag = 0;      // �����־λ

extern short speed1, speed2;
extern S_FLOAT_XYZ GYRO_REAL, REAL_ACC;
void core1_main(void)
{
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    // �˴���д�û����� ���������ʼ�������
    outflag = 1;
    mt9v03x_init();
    tft180_init();
    imu660ra_init();
    wireless_uart_init();
    PID_int();

    pwm_init(ATOM0_CH0_P21_2, 17 * 1000, 0);
    pwm_init(ATOM0_CH1_P21_3, 17 * 1000, 0);
    pwm_init(ATOM0_CH2_P21_4, 17 * 1000, 0);
    pwm_init(ATOM0_CH3_P21_5, 17 * 1000, 0);

    encoder_dir_init(ENCODER_DIR_L, ENCODER_DIR_PULSE_L, ENCODER_DIR_DIR_L);
    encoder_dir_init(ENCODER_DIR_R, ENCODER_DIR_PULSE_R, ENCODER_DIR_DIR_R);

    gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY1 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY2 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY3 ���� Ĭ�ϸߵ�ƽ ��������
    gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP);           // ��ʼ�� KEY4 ���� Ĭ�ϸߵ�ƽ ��������

    // �˴���д�û����� ���������ʼ�������
    // tft180_set_color(RGB565_WHITE, RGB565_BLACK);
    tft180_set_font(TFT180_6X8_FONT);
    pit_ms_init(CCU60_CH0, 1);
    
    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        // while (outflag)
        // {
        //     motor_ctrl(3000, 2000);
        //     system_delay_ms(500);
        //     outflag = 0;
        // }
        // List_Switch();
        // cal_curvature(&(MyRoad_Characteristics.Curve_Err));
        // ����Ϊ���õĲ��Դ���

        // get_motor_speed();
        // motor_ctrl(3000, 3000);   // (0, 3000)����ת��(3000, 0)����ת
        // tft180_show_gray_image(0 ,0, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W / 1.5, MT9V03X_H / 1.5, 0);
        // tft180_show_int(0, 90, GYRO_REAL.Z, 5);
        // tft180_show

        gyroOffsetInit();
        // Camera();
		// sendimg_binary_CHK(&bin_image[0], MT9V03X_W, MT9V03X_H, image_thereshold, 25);
        // tft180_show_gray_image(0, 0, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W / 1.5, MT9V03X_H / 1.5, 0);
        put_float(0, real_real_speed);
        tft180_show_string(0, 30, "BlackPoints_Nums");         tft180_show_float(60, 30, real_real_speed, 5, 2);
        // tft180_show_string(0, 50 , "TurnNei_I");        tft180_show_float(60, 50, Turn_NeiPID.Ki, 5, 2);
        // tft180_show_string(0, 70, "TurnNei_D");        tft180_show_float(60, 70, Turn_NeiPID.Kd, 5, 2);
        // tft180_show_string(0, 110, "Turn_P");         tft180_show_float(45, 110, TurnPID.Kp, 5, 2);
        tft180_show_string(0, 110, "Turn_I");        tft180_show_float(45, 110, TurnPID.Ki, 5, 2);
        tft180_show_string(0, 130, "Turn_d");        tft180_show_float(45, 130, TurnPID.Kd, 5, 2);
        // tft180_show_float(0, 90, Centerline_Err, 5, 2);         tft180_show_float(60, 90, Prospect_Err, 5, 2);
        TaskProcess();
        image_process();
        tft180_show_float(0, 90, speed1, 5, 2);         tft180_show_float(60, 90, speed2, 5, 2); 
        Deal_Road_Characteristics(&bin_image[0], &MyRoad_Characteristics);
        Hightlight_Lines(&bin_image[0]);                 
        if(Key1 == onepress){
			Key1 = nopress;
			Turn_NeiPID.Kp += 0.1;
			// system_delay_ms(300);
		}
		if(Key2 == onepress){
            Key2 = nopress;
			TurnPID.Kp += 10;
			// system_delay_ms(300);
		}
		if(Key3 == onepress){
            Key3 = nopress;
			TurnPID.Kd += 1;
			// system_delay_ms(300);
		}
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}



#pragma section all restore
// **************************** �������� ****************************

// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1��LED ����˸
//      �鿴�����Ƿ�������¼���Ƿ����ر���ȷ���������¸�λ����
//      ���ñ������Ӧ LED ���ŵ�ѹ�Ƿ�仯��������仯֤������δ���У�����仯֤�� LED ������
