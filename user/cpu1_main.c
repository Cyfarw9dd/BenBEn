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
// buzzer p33.10
#define PIT_CCU60_ms            1                                               // pit��ʱ��1�жϽ�������
#define BUZZER                  ATOM2_CH0_P33_10                                // ����������
#define UART_INDEX              (DEBUG_UART_INDEX   )                           // Ĭ�� UART_0
#define UART_BAUDRATE           (DEBUG_UART_BAUDRATE)                           // Ĭ�� 115200
#define UART_TX_PIN             (DEBUG_UART_TX_PIN  )                           // Ĭ�� UART0_TX_P14_0
#define UART_RX_PIN             (DEBUG_UART_RX_PIN  )                           // Ĭ�� UART0_RX_P14_1
#define REDLIGHT                101                                             // �����Ʊ�־λ
#define NOREDLIGHT              10                                              // �����ƶ�ʧλ
#define YELLOWLIGHT             201                                             // ����ƵƱ�־λ
#define NOYELLOWLIGHT           20                                              // ����Ƶƶ�ʧλ
#define LED1                    (P20_9)

unsigned char outflag = 0;                                                      // �����־λ
extern short speed1, speed2;                                                    // ���ұ�����ֵ
extern S_FLOAT_XYZ GYRO_REAL, REAL_ACC;
unsigned char receive_array[9];
void dl1a_movement(void);
void trafficlights_movement(void);
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
    gpio_init(TOGGLE1, GPI, GPIO_HIGH, GPI_PULL_UP);        // ��ʼ�����뿪��1
    gpio_init(TOGGLE2, GPI, GPIO_HIGH, GPI_PULL_UP);        // ��ʼ�����뿪��2
    pwm_init(BUZZER, 1500, 0);                                // ��ʼ��������
    uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);             // ��ʼ������
    uart_rx_interrupt(UART_INDEX, 1);                                           // ���� UART_INDEX �Ľ����ж�
    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);  
    while(1)
    {
        if(dl1a_init())
            gpio_toggle_level(LED1);                                            
        else
            break;
        system_delay_ms(1000);                                                  
    }
    // �˴���д�û����� ���������ʼ�������
    // tft180_set_color(RGB565_WHITE, RGB565_BLACK);
    tft180_set_font(TFT180_6X8_FONT);
    pit_ms_init(CCU60_CH0, PIT_CCU60_ms);
    
    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        // ���Ʋ��ֲ���ʱ��Ƭ��ѯ����������԰��ٶȻ���һ���ж�
        // pwm_set_duty(BUZZER, 8000);
        TaskProcess();
        image_process();
        tft180_show_gray_image(0, 0, &bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W / 1.5, MT9V03X_H / 1.5, 0);
        dl1a_movement();
        // trafficlights_movement();
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

void dl1a_movement(void)
{
    dl1a_get_distance();
    if(dl1a_finsh_flag == 1)
    {
        dl1a_finsh_flag = 0;
        if(dl1a_distance_mm < 400)
        {        
            pit_disable(CCU60_CH0); // �رն�ʱ���ж�
            motor_ctrl(-1800, -1800);//ʶ�𵹳�   8.1V
            system_delay_ms(80);                
            motor_ctrl(0,0);
            system_delay_ms(200);
            motor_ctrl(-1800, 1800);//��ת
            system_delay_ms(420);
            motor_ctrl(0,0);
            system_delay_ms(200);
            motor_ctrl(1800, 1800);//ֱ��
            system_delay_ms(500);
            motor_ctrl(0,0);
            system_delay_ms(200);
            motor_ctrl(1800, -1800);//��ת
            system_delay_ms(740);
            motor_ctrl(0,0);
            system_delay_ms(200);                                 
            motor_ctrl(1800, 1800);//ֱ��
            system_delay_ms(600);
            motor_ctrl(0,0);
            system_delay_ms(200);
            // motor_ctrl(-1800, 1800);//��ת
            // system_delay_ms(380);
            // motor_ctrl(0,0);
            // system_delay_ms(100);
            get_motor_speed();      // ���²ɼ�һ�α��������ݣ���Ȼ���������ݻᱣ�����жϹر�ǰ����һ�̣�����Ӱ��
            pit_enable(CCU60_CH0);  // �ָ���ʱ���ж�
        }
    }
}

void trafficlights_movement(void)
{
    
    unsigned char receive_array;
    uart_query_byte(UART_INDEX, &receive_array);
    // tft180_show_int(0, 0, receive_array + 1, 5);
    // tft180_show_string(0, 0, receive_array);
    if (receive_array + 1 == REDLIGHT)      // ���ʶ�𵽺��
    {
        // tft180_clear();
        // tft180_show_string(0, 0, "check it");
        aim_speed = 0;                      // �����ٶ�0��ɲ��
        system_delay_ms(20);                // һ����ʱ
        pit_disable(CCU60_CH0);             // �ر�PIT�ж�
    }
    else if (receive_array + 2== YELLOWLIGHT)
    {
        aim_speed = 50;                      // �����ٶ�0��ɲ��
        system_delay_ms(20);
        system_delay_ms(20);                // һ����ʱ
        pit_disable(CCU60_CH0);             // �ر�PIT�ж�
    }
    else
    {
        aim_speed = 100;
        get_motor_speed();
        pit_enable(CCU60_CH0);
    }
}

#pragma section all restore
// **************************** �������� ****************************

// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1��LED ����˸
//      �鿴�����Ƿ�������¼���Ƿ����ر���ȷ���������¸�λ����
//      ���ñ������Ӧ LED ���ŵ�ѹ�Ƿ�仯��������仯֤������δ���У�����仯֤�� LED ������
