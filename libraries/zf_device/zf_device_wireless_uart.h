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
* �ļ�����          zf_device_wireless_uart
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
/*********************************************************************************************************************
* ���߶��壺
*                  ------------------------------------
*                  ģ��ܽ�             ��Ƭ���ܽ�
*                  RX                 �鿴 zf_device_wireless_uart.h �� WIRELESS_UART_RX_PINx �궨��
*                  TX                 �鿴 zf_device_wireless_uart.h �� WIRELESS_UART_TX_PINx �궨��
*                  RTS                �鿴 zf_device_wireless_uart.h �� WIRELESS_UART_RTS_PINx �궨��
*                  VCC                3.3V��Դ
*                  GND                ��Դ��
*                  ������������
*                  ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_wireless_uart_h_
#define _zf_device_wireless_uart_h_

#include "zf_common_typedef.h"
//=================================================���ߴ��� ��������====================================================
#define WIRELESS_UART_INDEX         (UART_2)                                    // ���ߴ��ڶ�Ӧʹ�õĴ��ں�
#define WIRELESS_UART_BUAD_RATE     (115200)                                    // ���ߴ��ڶ�Ӧʹ�õĴ��ڲ�����
#define WIRELESS_UART_TX_PIN        (UART2_RX_P10_6)                            // ���ߴ��ڶ�Ӧģ��� TX Ҫ�ӵ���Ƭ���� RX
#define WIRELESS_UART_RX_PIN        (UART2_TX_P10_5)                            // ���ߴ��ڶ�Ӧģ��� RX Ҫ�ӵ���Ƭ���� TX
#define WIRELESS_UART_RTS_PIN       (P10_2)                                     // ���ߴ��ڶ�Ӧģ��� RTS ����
//=================================================���ߴ��� ��������====================================================

//================================================���ߴ��� �Զ�������====================================================
// ע������1������ת����ģ��汾��V2.0���µ����޷������Զ������ʵġ�
// ע������2�������Զ��������������RTS���� ����Ὺ��ʧ�ܡ�
// ע������3��ģ���Զ�������ʧ�ܵĻ� ���Գ��Զϵ�����

// �����Զ�����������Ķ��������� ע������
// �����Զ�����������Ķ��������� ע������
// �����Զ�����������Ķ��������� ע������

// 0���ر��Զ�������
// 1�������Զ������� �Զ������ʵ��������޸� WIRELESS_UART_BAUD ֮����Ҫ��ģ��������� ģ����Զ�����Ϊ��Ӧ�Ĳ�����

#define WIRELESS_UART_AUTO_BAUD_RATE    (0)
//================================================���ߴ��� �Զ�������====================================================

#define WIRELESS_UART_BUFFER_SIZE       (64  )
#define WIRELESS_UART_TIMEOUT_COUNT     (0x64)

//=================================================���ߴ��� ��������====================================================
uint32      wireless_uart_send_byte         (const uint8 data);
uint32      wireless_uart_send_buff         (const uint8 *buff, uint32 len);
uint32      wireless_uart_send_string       (const char *str);
void        wireless_uart_send_image        (const uint8 *image_addr, uint32 image_size);

uint32      wireless_uart_read_buff         (uint8 *buff, uint32 len);

void        wireless_uart_callback          (void);

uint8       wireless_uart_init              (void);
//=================================================���ߴ��� ��������====================================================


void sendimg(uint8* image, uint8 width, uint8 height);
void sendimg_A( uint8* image, uint8 width, uint8 height);
void sendimg_zoom(uint8* image, uint8 width, uint8 height, uint8 dis_width, uint8 dis_height);
void sendimg_binary( uint8* image, uint8 width, uint8 height,uint8 otu);
void sendimg_binary_zoom( uint8* image, uint8 width, uint8 height, uint8 dis_width, uint8 dis_height,uint8 otu);
void sendimg_binary_CHK(uint8* image, uint8 width, uint8 height,uint8 otu,uint8 chk);
void put_int32(uint8 name, int dat);
void put_float(uint8 name, float dat);
void sendline_clear( uint8 color,uint8 width, uint8 height);
void sendline_xy( uint8 *line_zx,uint8 *line_yx, uint32 len);
void sendline( uint8 color,uint8 *buff, uint32 len);
void sendline2( uint8 color,uint8 *linex,uint8 *liney, uint32 len);
void sendpoint(uint8 color,uint8 x,uint8 y, uint8 type);
//�ڲ�����
uint16 swj_CRC(uint8 *Buf,uint8 CRC_sta, uint8 CRC_CNT);
//int32 ByteToInt(int8 *ndsk);
//float ByteToFloat(unsigned char* byteArry);

#endif


