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
#include "image.h"
#include "control.h"
#include "pid.h"
#pragma section all "cpu1_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��


// ���̵��뵽���֮��Ӧ��ѡ�й���Ȼ����refreshˢ��һ��֮���ٱ���
// ����Ĭ������Ϊ�ر��Ż��������Լ��һ�����ѡ��properties->C/C++ Build->Setting
// Ȼ�����Ҳ�Ĵ������ҵ�C/C++ Compiler->Optimization->Optimization level�������Ż��ȼ�
// һ��Ĭ���½����Ĺ��̶���Ĭ�Ͽ�2���Ż�����˴��Ҳ��������Ϊ2���Ż�

// ����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ�� enableInterrupts(); �������ж�Ƕ��
// �򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ������� disableInterrupts(); ���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ����� enableInterrupts(); �������жϵ���Ӧ��

// *************************** ����Ӳ������˵�� ***********************
// ���İ��������缴�� �����������


// *************************** ���̲���˵�� **************************
// 1.���İ���¼��ɱ����̣�����ϵ�
// 2.���Կ������İ������� LED ����ͬ��Ƶ����˸
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�

// *************************** �ر�˵�� **************************
// ������Ϊ���ʹ������ LED1�ɺ���0���ƣ�����ÿ��ʱ200����ͷ�תһ��LED��ʾ״̬
//               LED2�ɺ���1���ƣ�����ÿ��ʱ300����ͷ�תһ��LED��ʾ״̬

// **************************** �������� ****************************

#define ENCODER_DIR_L                     (TIM5_ENCODER)                         // ��������������Ӧʹ�õı������ӿ�
#define ENCODER_DIR_PULSE_L               (TIM5_ENCODER_CH1_P10_3)               // PULSE ��Ӧ������
#define ENCODER_DIR_DIR_L                 (TIM5_ENCODER_CH2_P10_1)               // DIR ��Ӧ������

#define ENCODER_DIR_R                     (TIM6_ENCODER)                         // ��������������Ӧʹ�õı������ӿ�
#define ENCODER_DIR_PULSE_R               (TIM6_ENCODER_CH1_P20_3)               // PULSE ��Ӧ������
#define ENCODER_DIR_DIR_R                 (TIM6_ENCODER_CH2_P20_0)               // DIR ��Ӧ������

#define ENCODER_PIT                       (CCU60_CH0 )

void core1_main(void)
{
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    // �˴���д�û����� ���������ʼ�������
    mt9v03x_init();
    tft180_init();
    // icm20602_init();
    wireless_uart_init();
    pwm_init(ATOM0_CH4_P02_4, 50, 0);
    pwm_init(ATOM0_CH5_P02_5, 50, 0);
    pwm_init(ATOM0_CH6_P02_6, 50, 0);
    pwm_init(ATOM0_CH7_P02_7, 50, 0);

    encoder_dir_init(ENCODER_DIR_L, ENCODER_DIR_PULSE_L, ENCODER_DIR_DIR_L);
    encoder_dir_init(ENCODER_DIR_R, ENCODER_DIR_PULSE_R, ENCODER_DIR_DIR_R);

//    pit_ms_init(ENCODER_PIT, 50);
    // �˴���д�û����� ���������ʼ�������

    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        // Camera();
        tft180_show_gray_image(0 ,0, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W / 1.5, MT9V03X_H / 1.5, 0);
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

// IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
// {
//    interrupt_global_enable(0);                     // �����ж�Ƕ��
//    pit_clear_flag(CCU60_CH0);

//    get_motor_speed();

// }


#pragma section all restore
// **************************** �������� ****************************

// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1��LED ����˸
//      �鿴�����Ƿ�������¼���Ƿ����ر���ȷ���������¸�λ����
//      ���ñ������Ӧ LED ���ŵ�ѹ�Ƿ�仯��������仯֤������δ���У�����仯֤�� LED ������
