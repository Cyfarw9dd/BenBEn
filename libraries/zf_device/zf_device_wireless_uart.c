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

#include "zf_common_debug.h"
#include "zf_common_fifo.h"
#include "zf_driver_delay.h"
#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"
#include "zf_device_type.h"
#include "zf_device_wireless_uart.h"

//��Ҫ�޸ĵĵط�
#define  sw_write_byte(dat)  uart_putchar(UART_2,dat)//���ڷ����ֽ�
#define  sw_write_buffer(dat,len)  wireless_uart_send_buff(dat,len)//���ߴ��ڷ�������򴮿ڷ�������

static   fifo_struct    wireless_uart_fifo;
static   uint8          wireless_uart_buffer[WIRELESS_UART_BUFFER_SIZE];    // ���ݴ������

static   uint8          wireless_uart_data;
volatile uint32         wireless_auto_baud_flag = 0;
volatile uint8          wireless_auto_baud_data[3] = {0x00, 0x01, 0x03};

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ת����ģ�� ��������
// ����˵��     data            8bit ����
// ���ز���     uint32          ʣ�෢�ͳ���
// ʹ��ʾ��     wireless_uart_send_byte(data);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_send_byte (const uint8 data)
{
    uint16 time_count = WIRELESS_UART_TIMEOUT_COUNT;
    while(time_count)
    {
        if(!gpio_get_level(WIRELESS_UART_RTS_PIN))
        {
            uart_write_byte(WIRELESS_UART_INDEX, data);                         // ��������
            break;
        }
        time_count --;
        system_delay_ms(1);
    }
    return (0 < time_count);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ת����ģ�� �������ݿ�
// ����˵��     *buff           ���ͻ�����
// ����˵��     len             �������ݳ���
// ���ز���     uint32          ʣ�෢�ͳ���
// ʹ��ʾ��     wireless_uart_send_buff(buff, 64);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_send_buff (const uint8 *buff, uint32 len)
{
    zf_assert(buff != NULL);
    uint16 time_count = 0;
    while(0 != len)
    {
        if(!gpio_get_level(WIRELESS_UART_RTS_PIN))                                    // ���RTSΪ�͵�ƽ �������������
        {
            if(30 <= len)                                                       // ���ݷ� 30byte ÿ������
            {
                uart_write_buffer(WIRELESS_UART_INDEX, buff, 30);                    // ��������
                buff += 30;                                                     // ��ַƫ��
                len -= 30;                                                      // ����
                time_count = 0;
            }
            else                                                                // ���� 30byte ������һ���Է������
            {
                uart_write_buffer(WIRELESS_UART_INDEX, buff, len);                   // ��������
                len = 0;
                break;
            }
        }
        else                                                                    // ���RTSΪ�ߵ�ƽ ��ģ��æ
        {
            if(WIRELESS_UART_TIMEOUT_COUNT <= (++ time_count))                  // ���������ȴ�ʱ��
            {
                break;                                                          // �˳�����
            }
            system_delay_ms(1);
        }
    }
    return len;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ת����ģ�� �����ַ���
// ����˵��     *str            Ҫ���͵��ַ�����ַ
// ���ز���     uint32          ʣ�෢�ͳ���
// ʹ��ʾ��     wireless_uart_send_string("Believe in yourself.");
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_send_string (const char *str)
{
    zf_assert(str != NULL);
    uint16 time_count = 0;
    uint32 len = strlen(str);
    while(0 != len)
    {
        if(!gpio_get_level(WIRELESS_UART_RTS_PIN))                                    // ���RTSΪ�͵�ƽ �������������
        {
            if(30 <= len)                                                       // ���ݷ� 30byte ÿ������
            {
                uart_write_buffer(WIRELESS_UART_INDEX, (const uint8 *)str, 30);      // ��������
                str += 30;                                                      // ��ַƫ��
                len -= 30;                                                      // ����
                time_count = 0;
            }
            else                                                                // ���� 30byte ������һ���Է������
            {
                uart_write_buffer(WIRELESS_UART_INDEX, (const uint8 *)str, len);     // ��������
                len = 0;
                break;
            }
        }
        else                                                                    // ���RTSΪ�ߵ�ƽ ��ģ��æ
        {
            if(WIRELESS_UART_TIMEOUT_COUNT <= (++ time_count))                  // ���������ȴ�ʱ��
            {
                break;                                                          // �˳�����
            }
            system_delay_ms(1);
        }
    }
    return len;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ת����ģ�� ��������ͷͼ������λ���鿴ͼ��
// ����˵��     *image_addr     ��Ҫ���͵�ͼ���ַ
// ����˵��     image_size      ͼ��Ĵ�С
// ���ز���     void
// ʹ��ʾ��     wireless_uart_send_image(&mt9v03x_image[0][0], MT9V03X_IMAGE_SIZE);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void wireless_uart_send_image (const uint8 *image_addr, uint32 image_size)
{
    zf_assert(image_addr != NULL);
    extern uint8 camera_send_image_frame_header[4];
    wireless_uart_send_buff(camera_send_image_frame_header, 4);
    wireless_uart_send_buff((uint8 *)image_addr, image_size);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ת����ģ�� ��ȡ����
// ����˵��     *buff           ���ջ�����
// ����˵��     len             ��ȡ���ݳ���
// ���ز���     uint32          ʵ�ʶ�ȡ���ݳ���
// ʹ��ʾ��     wireless_uart_read_buff(buff, 32);
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_read_buff (uint8 *buff, uint32 len)
{
    zf_assert(buff != NULL);
    uint32 data_len = len;
    fifo_read_buffer(&wireless_uart_fifo, buff, &data_len, FIFO_READ_AND_CLEAN);
    return data_len;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ת����ģ�� �����жϻص�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     wireless_uart_callback();
// ��ע��Ϣ     �ú����� ISR �ļ� �����жϳ��򱻵���
//              �ɴ����жϷ��������� wireless_module_uart_handler() ����
//              ���� wireless_module_uart_handler() �������ñ�����
//-------------------------------------------------------------------------------------------------------------------
void wireless_uart_callback (void)
{
    uart_query_byte(WIRELESS_UART_INDEX, &wireless_uart_data);
    fifo_write_buffer(&wireless_uart_fifo, &wireless_uart_data, 1);
#if WIRELESS_UART_AUTO_BAUD_RATE                                                // �����Զ�������
    if(wireless_auto_baud_flag == 1 && fifo_used(&wireless_uart_fifo) == 3)
    {
        wireless_auto_baud_flag = 3;
        fifo_read_buffer(&wireless_uart_fifo, (uint8 *)wireless_auto_baud_data, (uint32 *)&wireless_auto_baud_flag, FIFO_READ_AND_CLEAN);
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ת����ģ�� ��ʼ��
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     wireless_uart_init();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_init (void)
{
    uint8 return_state = 0;
    set_wireless_type(WIRELESS_UART, wireless_uart_callback);

    fifo_init(&wireless_uart_fifo, FIFO_DATA_8BIT, wireless_uart_buffer, WIRELESS_UART_BUFFER_SIZE);
    gpio_init(WIRELESS_UART_RTS_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);
#if(0 == WIRELESS_UART_AUTO_BAUD_RATE)                                          // �ر��Զ�������
    // ������ʹ�õĲ�����Ϊ115200 Ϊ����ת����ģ���Ĭ�ϲ����� ������������������������ģ�鲢�޸Ĵ��ڵĲ�����
    uart_init (WIRELESS_UART_INDEX, WIRELESS_UART_BUAD_RATE, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN);   // ��ʼ������
    uart_rx_interrupt(WIRELESS_UART_INDEX, 1);
#elif(1 == WIRELESS_UART_AUTO_BAUD_RATE)                                        // �����Զ�������
    uint8 rts_init_status;
    uint16 time_count = 0;

    wireless_auto_baud_flag = 0;
    wireless_auto_baud_data[0] = 0;
    wireless_auto_baud_data[1] = 1;
    wireless_auto_baud_data[2] = 3;

    rts_init_status = gpio_get_level(WIRELESS_UART_RTS_PIN);
    gpio_init(WIRELESS_UART_RTS_PIN, GPO, rts_init_status, GPO_PUSH_PULL);      // ��ʼ����������

    uart_init (WIRELESS_UART_INDEX, WIRELESS_UART_BUAD_RATE, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN);   // ��ʼ������
    uart_rx_interrupt(WIRELESS_UART_INDEX, 1);

    system_delay_ms(5);                                                         // ģ���ϵ�֮����Ҫ��ʱ�ȴ�
    gpio_set_level(WIRELESS_UART_RTS_PIN, !rts_init_status);                    // RTS�������ߣ������Զ�������ģʽ
    system_delay_ms(100);                                                       // RTS����֮�������ʱ20ms
    gpio_toggle(WIRELESS_UART_RTS_PIN);                                         // RTS����ȡ��

    wireless_auto_baud_flag = 1;

    uart_write_byte(WIRELESS_UART_INDEX, wireless_auto_baud_data[0]);           // �����ض����� ����ģ���Զ��жϲ�����
    uart_write_byte(WIRELESS_UART_INDEX, wireless_auto_baud_data[1]);           // �����ض����� ����ģ���Զ��жϲ�����
    uart_write_byte(WIRELESS_UART_INDEX, wireless_auto_baud_data[2]);           // �����ض����� ����ģ���Զ��жϲ�����
    system_delay_ms(20);

    time_count = 0;
    do
    {
        if(3 != wireless_auto_baud_flag)                                        // �����Զ��������Ƿ����
        {
            while(time_count ++)
                system_delay_ms(1);
        }
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
        {
            return_state = 1;                                                   // ���������뵽������� ˵���Զ�������ʧ����
            break;
        }

        time_count = 0;
        if( 0xa5 != wireless_auto_baud_data[0] &&                               // �����Զ��������Ƿ���ȷ
            0xff != wireless_auto_baud_data[1] &&                               // �����Զ��������Ƿ���ȷ
            0xff != wireless_auto_baud_data[2] )                                // �����Զ��������Ƿ���ȷ
        {
            while(time_count ++)
                system_delay_ms(1);
        }
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
        {
            return_state = 1;                                                   // ���������뵽������� ˵���Զ�������ʧ����
            break;
        }
        wireless_auto_baud_flag = 0;

        gpio_init(WIRELESS_UART_RTS_PIN, GPI, 0, GPI_PULL_UP);                  // ��ʼ����������
        system_delay_ms(10);                                                    // ��ʱ�ȴ� ģ��׼������
    }while(0);
#endif
    return return_state;
}


//--------------��ͨ�Ҷ�ͼ��-------------------//
//*imageͼ���ַ widthͼ��� heightͼ���
//����sendimg(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H);
//���ݰ���С:6+width * height(ͼ��һ֡���ֽ���)
void sendimg(uint8* image, uint8 width, uint8 height)
{
    uint8 dat[6] = { 0x21, 0x7A, width, height, 0x21, 0x7A };
    sw_write_buffer(dat, 6);
    sw_write_buffer(image, width * height);
}
//--------------�����ŻҶ�ͼ��-------------------//
//����ʧ���ݵ�����£���Э�������¶�λ����ʵ��һ���̶ȿ���������
//*imageͼ���ַ widthͼ��� heightͼ���
//����sendimg(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H);
//���ݰ���С:6+��width+3�� * height(ͼ��һ֡���ֽ���)
void sendimg_A( uint8* image, uint8 width, uint8 height)
{

    sw_write_byte(0x21); sw_write_byte(0x7A);
    sw_write_byte(width);sw_write_byte(height);
    sw_write_byte((width+height)/2);sw_write_byte(0x7A);

    uint8 line=0,col=0;
    for(line=0;line<width;line++)
        {
        sw_write_byte(21);
        sw_write_byte(line);
        sw_write_byte(133);
           for(col=0;col<height;col++)
           {
               sw_write_byte(*(image+line*height+col));

           }

        }
}
//--------------ѹ���Ҷ�ͼ��-------------------//
//����ѹ��ͼ�� ���� 120x180��ͼ��̫�� �����ٶ���  ����Է��� 60x90��ͼ��������ٶ�
//*imageͼ���ַ widthͼ��� heightͼ���dis_widthѹ�����ͼ��� dis_heightѹ�����ͼ���
//����sendimg(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H,MT9V03X_DVP_W/2,MT9V03X_DVP_H/2);
//���ݰ���С:6+dis_width * dis_height(ͼ��һ֡���ֽ���)
void sendimg_zoom(uint8* image, uint8 width, uint8 height, uint8 dis_width, uint8 dis_height)
{
    uint8 dat[6] = { 0x21, 0x7A, dis_width, dis_height, 0x21, 0x7A };
    sw_write_buffer(dat, 6);
    uint8 i,j;
    for(j=0;j<dis_height;j++)
    {
    for(i=0;i<dis_width;i++)
    {
       sw_write_byte(*(image+(j*height/dis_height)*width+i*width/dis_width));//��ȡ���ص�

    }
    }
}

//--------------��ֵ��ͼ��-------------------//
//imageͼ���ַ widthͼ��� heightͼ��� otu��ֵ����ֵ
//����sendimg_binary(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H,100);
void sendimg_binary( uint8* image, uint8 width, uint8 height,uint8 otu)
{

    uint8 dat[6]={0x7A,0x21,width,height,0x7A,0x21};
    sw_write_buffer(dat,6);
    int databool=255;uint8 lon=1;int data=255;
    uint8 line=0,col=0;

    for(line=0;line<width;line++)
    {
        for(col=0;col<height;col++)
        {
            if(*(image+line*height+col)>otu)data=255;
            else data=0;
            if(data==databool)
            {lon++;}else{sw_write_byte(lon);lon=1;}
            if(lon==190){sw_write_byte(lon-1);sw_write_byte(0);lon=1;}
            databool=data;
        }
    }
}
//ѹ����ֵͼ��
//uint16 dis_width, uint16 dis_height Ҫѹ��ͼ���С
void sendimg_binary_zoom( uint8* image, uint8 width, uint8 height, uint8 dis_width, uint8 dis_height,uint8 otu)
{

    uint8 dat[6]={0x7A,0x21,dis_width,dis_height,0x7A,0x21};
    sw_write_buffer(dat,6);
    int databool=255;uint8 lon=1;int data=255;
    uint8 i,j;
    for(j=0;j<dis_height;j++)
    {
    for(i=0;i<dis_width;i++)
    {
    if(*(image+(j*height/dis_height)*width+i*width/dis_width)>otu)//��ȡ���ص�
    data=255;
    else data=0;
    if(data==databool)
    {lon++;}
    else{sw_write_byte(lon);lon=1;}
    if(lon==190){sw_write_byte(lon-1);sw_write_byte(0);lon=1;}
    databool=data;
    }
    }

}
//����У��Ķ�ֵͼ��
//chkֵԽ�� ������Խǿ ֵ0-55
//�����ʵ��ʹ��������е���
void sendimg_binary_CHK(uint8* image, uint8 width, uint8 height,uint8 otu,uint8 chk)
{
    chk=chk>0?chk:0;
    chk=chk<56?chk:55;
    uint8 dat[7]={0x7A,0x21,width,height,0x7A,0x21,200+chk};
      sw_write_buffer(dat,7);
      int databool=255;uint8 lon=0;int data=255;
      uint8 line=0,col=0;
      int imglon=0;
      int imgdatlo=width*height/chk;
      uint8 CHK=0;
      for(line=0;line<width;line++)
          {
             for(col=0;col<height;col++)
             {imglon++;

                if(*(image+line*height+col)>otu)data=255;
                else data=0;
                if(data==databool)
                {lon++;}
                else{sw_write_byte(lon);lon=1;}

                if(imglon==imgdatlo)
                {CHK++;sw_write_byte(lon);data=255; databool=255;sw_write_byte(200+CHK);lon=0;imglon=0;}
                if(lon==190){sw_write_byte(lon);sw_write_byte(0);lon=0;}
               databool=data;
             }
          }
}

//--------------���ݱ�ǩ������ʾ����������ɫ�궨�����ݼ��ӣ��Ǳ��̣�-------------------//
//һ������ռһ����ַ,��ֱ�ӳ�����ͼ��ҳ������� ��һ�¿���ʾ�����Ҽ�����������ɫ�궨 �趨��
//��ֵ����ɫ�궨���� ���ټ���������û�д�����Ҳ���԰󶨱��̣��ٶȸ�ֱ�ۡ�¼��ʱ��ͬ��¼��
//�����ں�ϻ�ӣ�С��ʲô״̬һ�۱�֪
//name���ݱ�ʶ(ͨ������ַ)[0-255]  dat:����
//��:int a=0;put_int32(0,a);

//����У�������ʾ��
void put_int32(uint8 name, int dat)
{
    uint8 datc[10] = { 197, name,1,0,0,0,0,0,0,198};
    datc[3] = (uint8)(dat & 0xFF);
    datc[4] = (uint8)((dat & 0xFF00) >> 8);
    datc[5] = (uint8)((dat & 0xFF0000) >> 16);
    datc[6] = (uint8)((dat >> 24) & 0xFF);
    uint8 crc[6] = { name, 1,datc[3],datc[4],datc[5],datc[6]};
    uint16 CRC16 =  swj_CRC(crc,0,6);
    datc[7] = (uint8)(CRC16&0xff);
    datc[8] = (uint8)(CRC16>>8);
    sw_write_buffer(datc,10);
}

void put_float(uint8 name, float dat)
{
    uint8 datc[10] = { 197, name,2,0,0,0,0,0,0,198};
    char farray[4] = {0};

    *(float*)farray=dat;
    unsigned char *p = (unsigned char*)&dat + 3;
    datc[3] = *(p-3);
    datc[4] = *(p-2);
    datc[5] = *(p-1);
    datc[6] = *p;
   
    uint8 crc[6] = { name, 2,datc[3],datc[4],datc[5],datc[6]};
    uint16 CRC16 =  swj_CRC(crc,0,6);
    datc[7] = (uint8)(CRC16&0xff);
    datc[8] = (uint8)(CRC16>>8);
    sw_write_buffer(datc,10);
}


//--------------����-------------------//
/*
 * ���Խ�Ѱ�ߵõ������ұ��ߣ�������߷��͵���λ���鿴
 * ���磺
 * ���� ͼ��img[H][W];�õ����ұ��ߴ����zx[H] yx[H] �����������Ϊw[H]
 * sendline_clear(swj_BLACK,H,W);//���� ������ɫ
 * sendline(swj_WHITE,zx,H);//���������
 * sendline(swj_WHITE,yx,H);//�����ұ���
 * sendline(swj_WHITE,wx,H);//��������
 * ���磺
 * sendline_clear(swj_BLACK,H,W);//���� ������ɫ
 * sendline_xy(zx,yx,H)//�������ұ���
 * sendline(swj_WHITE,wx,H);//��������
 *
 * ������������Ч��һ�µģ��������������λ���϶�Ӧʹ��������ԭ����
 * ע�⣺
 * ��ÿ������һ֡��ͼ����� ��Ҫ����sendline_clear��������
 * ���������sendline_xy������ʹ����λ��������ԭ����ʱ�����ٵ���sendline һ������sendline_xy���� ��ֹ������
 * */
#define swj_BLACK 0
#define swj_WHITE 1
#define swj_RED 2
#define swj_GREEN 3
#define swj_BLUE 4
#define swj_PURPLE 5
#define swj_YELLOW 6
#define swj_CYAN 7
#define swj_ORANGE 8
//�������   color������ı�����ɫ  uint16 width uint16 height ͼ��Ĵ�С
void sendline_clear( uint8 color,uint8 width, uint8 height)
{
    sw_write_byte(0x21); sw_write_byte(0x7A);
    sw_write_byte(width);sw_write_byte(height);
    sw_write_byte(color);sw_write_byte(0x21);
}
//ͼ�������߽�  uint8_t *zx:��߽�   uint8_t *yx:�ұ߽�, uint32_t len���͵ı��߳���
//�ú������·ź����ֱ������������кβ�ͬ? �ú����ɶ�Ӧ��λ����ԭ�����Ĺ���*  ע���Ⱥ�˳��
void sendline_xy( uint8 *line_zx,uint8 *line_yx, uint32 len)
{
    sw_write_byte(0x21);
    sw_write_byte(9);
    sw_write_byte(len);
    sw_write_byte(255);
    sw_write_byte(255);
    sw_write_buffer(line_zx,len);
    sw_write_buffer(line_yx,len);
}
/*Ĭ��ÿ��һ����*/
//���Ʊ���   color������ɫ  uint8_t *buff ���͵ı��������ַ  len���͵ı��߳���
void sendline( uint8 color,uint8 *buff, uint32 len)
{
    sw_write_byte(0x21);
    sw_write_byte(color);
    sw_write_byte(len);
    sw_write_byte(255);
    sw_write_byte(255);
    sw_write_buffer(buff,len);
}
/*˵��:
 * ������������ a(x1,y1)b(x2,y2)c(x3,y3)
 * �� uint8 x[3]={x1,x2,x3};uint8 y[3]={y1,y2,y3};
 *  sendline2(swj_WHITE,x,y,3);
 *  sendline����ֻ�ܰ�˳��ÿ��һ�㷢�ͱ߽��
 *  sendline2��������ڰ������ ÿ�в��̶������ı߽�
  *           Ҳ�����ڷ�������� ����յ� �������
 *
 * */
//������Ʊ���  color������ɫ linex��Ӧ���x���꼯�� liney��Ӧ���y���꼯��  len���͵ı��߳���
void sendline2( uint8 color,uint8 *linex,uint8 *liney, uint32 len)
{
    sw_write_byte(0x21);
    sw_write_byte(color);
    sw_write_byte(len);
    sw_write_byte(254);
    sw_write_byte(255);
    sw_write_buffer(linex,len);
    sw_write_buffer(liney,len);

}

#define swj_point_type1 1 //Сʮ�� 3x3
#define swj_point_type2 2 //��ʮ�� 5x5
#define swj_point_type3 3 //��ʮ�� 7x7
#define swj_point_type4 4 //С  X  3x3
#define swj_point_type5 5 //��  X  5x5
#define swj_point_type6 6 //��  X  7x7
#define swj_point_type7 7 //ȫ��ʮ��

//��־�㣨����յ� ���ⲹ�ߵĵ㣩
//���� �㣨10,11��sendpoint(swj_RED,10,11,swj_point_type1);//��10��11�������ֺ�ɫСʮ��
//��ɫ ����x ����y �����ͣ�������ĺ궨�壩
void sendpoint(uint8 color,uint8 x,uint8 y, uint8 type)
{
    sw_write_byte(0x22);
    sw_write_byte(color);
    sw_write_byte(type);
    sw_write_byte(254);
    sw_write_byte(x);
    sw_write_byte(y);
    sw_write_byte(255);
}

//�ڲ�����
uint16 swj_CRC(uint8 *Buf,uint8 CRC_sta, uint8 CRC_CNT)
{
    uint16 CRC_Temp;
    uint8 i,j;
    CRC_Temp = 0xffff;

    for (i=CRC_sta;i<CRC_CNT; i++){
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}
//int32 ByteToInt(int8 *ndsk)
//{
//    int32 m = 0;
//    int8 *h = (int8 *)&m;
//    h[0] = ndsk[0];
//    h[1] = ndsk[1];
//    h[2] = ndsk[2];
//    h[3] = ndsk[3];
//
//    if(0 == m)
//    {
//        m = 1;
//    }
//
//    return m;
//}
//float ByteToFloat(unsigned char* byteArry)
//{
//return *((float*)byteArry);
//}
//--------------------------------------------------����ģ��-----------------------------------------------//
/*
 * ����ģ�� �Ƽ�ʹ����ɵ�����ģ�飨usb to nrf��*����ʱ����ͬʱ����ttl����������С��������ģ��
 * �����ʿ����������460800 *��ӦС���ϵĳ���,С��������ģ�飬�����ϵ�����ģ�飬��λ��������һ��
 * ����ģ���ٶȱ�wifi�� ���ŵ����ȶ� ���ڻҶ�ͼ���ٶȲ�̫���ã����ж�ֵ��ͼ���Ż� �ʺϱ�ͼ����ʾ����
 *
 */
