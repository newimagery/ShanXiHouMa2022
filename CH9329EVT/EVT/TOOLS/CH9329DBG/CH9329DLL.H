//
//****************************************
//**  Copyright  (C)  W.ch  1999-2011   **
//**  Web:  http://www.winchiphead.com  **
//****************************************
//**  DLL for HID interface chip CH9329  **
//**  C, VC6.0                          **
//****************************************
//
// HIDӦ�ò�ӿڿ�  V1.1
// �Ͼ��ߺ�������޹�˾  ����: zyw@wch.cn 2014.08
// HIDDLL  V1.1 
// 
// 
//

#ifndef		_CH9329_DLL_H
#define		_CH9329_DLL_H

#ifdef __cplusplus
extern "C" {
#endif

/*	BAUD rate setting	*/
#define B300		0x01
#define B600		0x02
#define B1200		0x03
#define B2400		0x04
#define B4800		0x05
#define B9600		0x06
#define B14400      0x07
#define B19200		0x08
#define B28800      0x09
#define B38400		0x0A
#define B57600		0x0B
#define B76800      0x0C
#define B115200 	0x0D

/* Parity define	*/
#define P_ODD		0x01        //��У��
#define P_EVEN		0x02		//żУ��
#define P_SPC		0x03		//�հ�λ
#define P_NONE		0x04		//��У��

/*	Data bits define		*/
#define BIT_5		0x01			
#define BIT_6		0x02
#define BIT_7		0x03
#define BIT_8		0x04

/* Stop bits define	*/
#define STOP_1		0x01			
#define STOP_2		0x02

//��ʼ��DLL�⡣
BOOL        WINAPI CH9329DllInt();

//CH9329GetHidGuid��ȡHID��GUID���÷��ο�CH9329DBG
//�ȼ���HidD_GetHidGuid,�ο�MSDN
void        WINAPI CH9329GetHidGuid (void *   HidGuid);

// �����豸��VID/PID�����豸
// CH9329OpenDevice�ɹ������豸�����ʧ�ܷ���INVALID_HANDLE_VALUE
HANDLE		WINAPI	CH9329OpenDevice(	unsigned short USB_VID,		        //�豸��VID
				        unsigned short USB_PID 								//�豸��PID
				        ); 
				        
// �����豸�����������豸
// CH9329OpenDevice�ɹ������豸�����ʧ�ܷ���INVALID_HANDLE_VALUE
HANDLE	WINAPI	CH9329OpenDevicePath(PCHAR DevicePath       //�豸������
                        );

BOOL	WINAPI	CH9329GetDevicePath(ULONG DevIndex,         //�豸�����ţ���ʾϵͳ�еڼ���HID�豸
                                    PCHAR DevicePath,       //���������豸������
                                    ULONG DevicePathLen     //DevicePath�Ļ���������
                                    );				
//�ȼ���HidD_GetAttributes��ȡVID,PID���汾��,�ο�MSDN���ɹ�����TRUE,ʧ�ܷ���FALSE
unsigned long	WINAPI CH9329GetAttributes (HANDLE hDeviceHandle,           //�豸���
                        unsigned short * pVID,                              //�����豸��VID
                        unsigned short * pPID,                              //�����豸��PID
                        unsigned short * pVer                               //�����豸�İ汾��
                        );
                
//��ȡCH9329ReadData��CH9329WriteDataһ�ζ�д��������ݳ���
//�ɹ�����TRUE��ʧ�ܷ���FALSE
unsigned long	WINAPI	CH9329GetBufferLen(HANDLE hDeviceHandle,	        //�豸���
				        unsigned short * InputReportLen,					//���ؽ������ݳ���
				        unsigned short * OutputReportLen					//���ط������ݳ���
				        );
				
//����CH9329ReadData��CH9329WriteData��д��ʱ�������λ����
void		WINAPI	CH9329SetTimeOut(	HANDLE hDeviceHandle,		        //�豸���
				        unsigned long ReadTimeOut,							//���ö����ݳ�ʱ���
				        unsigned long SendTimeOut							//����д���ݳ�ʱ���
				        );
				
//��һ�����ݣ���󳤶�ΪCH9329GetBufferLen���ص�InputReportLen-2����оƬ�̼�����.
//CH9329��һ��������󳤶�Ϊ31.�ɹ�����TRUE��ʧ�ܷ���FALSE
unsigned long	WINAPI	CH9329ReadData(	HANDLE hDeviceHandle,		        //�豸���
				        void * 			ReadBuffer ,						//ָ��������ݻ�����
				        unsigned long * pReadLen,							//����ʵ�ʶ��������ݳ���
				        HANDLE 			hEventObject						//�ȴ��¼����
				        );
				
//дһ�����ݣ���󳤶�ΪCH9329GetBufferLen���ص�OutputReportLen-2����оƬ�̼�����.
//CH9329дһ��������󳤶�Ϊ31.�ɹ�����TRUE��ʧ�ܷ���FALSE				
unsigned long	WINAPI	CH9329WriteData(	HANDLE hDeviceHandle,           //�豸���
				        void * 			SendBuffer,							//ָ�������ݻ�����
				        unsigned long	SentLen,							//Ҫ�������ݵĳ���
				        HANDLE 			hEventObject						//�ȴ��¼����
				        );
/*
ucRate	: 1=300(ucRateΪ1ʱ��Ӧ������300),2=600,3=1200,4=2400,5=4800,6=9600(Ĭ��ֵ),7=14400,8=19200,9=28800,10=38400,11=57600,12=76800,13=115200
ucCheck	: 1=��У��,2=żУ��,3=�հ�λ,4=��У��(Ĭ��ֵ)
ucStop	: 1=1λ(Ĭ��ֵ)��2=2λ
ucData	��1=5λ��2=6λ��3=7λ��4=8λ(Ĭ��ֵ)
*/
//���ò����ʣ�ucIntervalΪ�������ݵĳ�ʱ�����������ܵ�����������31����
//���ҳ���ucIntervalʱ������û���ܵ����ݣ�CH9329����������ϴ���PC
//�ɹ�����TRUE��ʧ�ܷ���FALSE
unsigned long	WINAPI	CH9329SetRate(	HANDLE	hDeviceHandle,	            //�豸���
					    unsigned char	ucRate,	                            //������
					    unsigned char	ucCheck,	                        //У��ģʽ
					    unsigned char	ucStop,	                            //ֹͣλ
					    unsigned char	ucData,                             //����λ
					    unsigned char	ucInterval                          //HID����ʱ��ʱ������0x10=3MS((Ĭ��ֵ)),0x20Լ����6MS��,0x30Լ����9MS
					    );	
					
/*
CH9329InitThreadData,CH9329GetThreadDataLen,CH9329ClearThreadData,
CH9329ReadThreadData,CH9329StopThread
�������麯����Ҫ��Ϊ�˷�������ݲ���������ͻ�ʹ�����ַ�ʽ�����ݡ�
CH9329InitThreadData:����һ���̺߳�˽�жѣ����߳��ڲ��ĵ���CH9329ReadData�����ݣ����������ݻ������ڲ���˽�ж��С�
CH9329GetThreadDataLen:��ȡ�ڲ������������ݵĳ��ȡ�
CH9329ReadThreadData:���ڲ������������ݡ�
CH9329ClearThreadData:��ջ������������ݡ�
CH9329StopThread:ֹͣ�ڲ��̶߳�ȡ��������ڲ���������
*/

//��ʼ���̺߳�˽�жѣ��ɹ�����TRUE��ʧ�ܷ���FALSE
unsigned long 	WINAPI	CH9329InitThreadData(HANDLE hDeviceHandle   //�豸���
                        );
                        
//��ѯ���ݣ������̻߳��������ж�������
unsigned long 	WINAPI	CH9329GetThreadDataLen(HANDLE hDeviceHandle  //�豸���
                        );
//���߳������ݣ��ɹ�����TRUE��ʧ�ܷ���FALSE
//ע�⣺ReadLen�����ڵ���֮ǰΪҪ��ȡ�����ݳ��ȣ����Ȳ��ܳ���ReadBuffer����Ĵ�С������ᷢ���ڴ�������
unsigned long 	WINAPI	CH9329ReadThreadData(HANDLE hDeviceHandle,  //HID�豸���
                        void * ReadBuffer,                          //�������׵�ַ
                        unsigned long *ReadLen                      //����֮ǰΪҪ��ȡ�����ݳ��ȣ�����ʵ�ʶ��������ݳ���
                        );
//��ջ���������
void 	        WINAPI	CH9329ClearThreadData(HANDLE hDeviceHandle  //�豸���
                        );
                        
//ֹͣ�ڲ��̶߳�ȡ��������ڲ�������
void 	        WINAPI	CH9329StopThread(HANDLE hDeviceHandle       //�豸���
                        );

//���ò�Ʒ��������Ϣ�������ʹ�ã��ر�Ҫע��������VID/PIDʱ��
//���豸ʱ��CH9329OpenDevice��Ӧ��VID/PIDҪ����Ӧ���޸ģ������ʧ�ܡ�
//�ɹ�����TRUE��ʧ�ܷ���FALSE
unsigned long 	WINAPI	CH9329SetDeviceCfg(HANDLE hDeviceHandle,    //�豸���
                        unsigned short wVID,                        //�����µ�VID
                        unsigned short wPID,                        //�����µ�PID
                        unsigned short wPower,                      //�����µ��豸����,50~150 
                        char* strManufacturer,                      //�����µĳ���������
                        unsigned long  uMSlen,                      //�����µĳ������������ȣ����29
                        char* strProduct,                           //�����µĲ�Ʒ������
                        unsigned long  uPSlen,                      //�����µĲ�Ʒ���������ȣ����29
                        char* strSerialNo,                          //�����µĲ�Ʒ���к�
                        unsigned long  uSSlen                       //�����µĲ�Ʒ���кų��ȣ����29
                        );

//����Ĭ�ϲ����ʣ��ϵ���Ĭ�ϲ����ʣ�������ʱCH9329Ĭ�ϲ�������9600����������ο�CH9329SetRate
//�ɹ�����TRUE��ʧ�ܷ���FALSE
unsigned long	WINAPI	CH9329SetDefaultRate(	HANDLE	hDeviceHandle,	    //�豸���
					    unsigned char	ucRate,	                            //������
					    unsigned char	ucCheck,	                        //У��ģʽ
					    unsigned char	ucStop,	                            //ֹͣλ
					    unsigned char	ucData,                             //����λ
					    unsigned char	ucInterval                          //HID����ʱ��ʱ������0x10=3MS((Ĭ��ֵ)),0x20Լ����6MS��,0x30Լ����9MS
					    );	

//�ָ�����Ĭ�����ã�ʹCH9329SetDeviceCfg��CH9329SetDefaultRate��������Ч��
//�ɹ�����TRUE��ʧ�ܷ���FALSE
unsigned long	WINAPI	CH9329ResetToDefault(	HANDLE hDeviceHandle 	        //�豸���
                        );
					
//����16λIO�ڷ��򣬳ɹ�����TRUE��ʧ�ܷ���FALSE
unsigned long 	WINAPI	CH9329SetIODir(HANDLE hDeviceHandle,                //�豸���
                        unsigned short usDir                                //16λIO�ڵ�����������ã���Ӧλ��1�������0������                           
                        );
                        
//����16λIO�ڵĵ�ƽ���ɹ�����TRUE��ʧ�ܷ���FALSE
unsigned long 	WINAPI	CH9329WriteIOData(HANDLE hDeviceHandle,             //�豸���
                        unsigned short usData                               //16λIO������ĸߵ͵�ƽ���ã���Ӧλ��1������ߵ�ƽ��0������ص�ƽ
                        );

//��ȡ16λIO�ڵ����ŵ�ƽ���ɹ�����TRUE��ʧ�ܷ���FALSE                        
unsigned long 	WINAPI	CH9329ReadIOData(HANDLE hDeviceHandle,              //�豸���
                        unsigned short* pData                               //��ȡ����16λIO�ڵ����ŵ�ƽ
                        );   

//��ȡ�����ַ������������ɹ�����TRUE��ʧ�ܷ���FALSE 
//�ȼ���HidD_GetManufacturerString���ο�MSDN 
unsigned long 	WINAPI  CH9329GetManufacturerString (HANDLE hDeviceHandle,  //�豸���
                        void * Buffer,                                      //�������׵�ַ
                        unsigned long BufferLength                          //��������С
                        );

//��ȡ��Ʒ�ַ������������ɹ�����TRUE��ʧ�ܷ���FALSE 
//�ȼ���HidD_GetProductString���ο�MSDN 
unsigned long 	WINAPI  CH9329GetProductString(HANDLE hDeviceHandle,        //�豸���
                        void * Buffer,                                      //�������׵�ַ
                        unsigned long BufferLength                          //��������С
                        );

//��ȡ���к��ַ������������ɹ�����TRUE��ʧ�ܷ���FALSE
//�ȼ���HidD_GetSerialNumberString���ο�MSDN
unsigned long 	WINAPI  CH9329GetSerialNumberString(HANDLE hDeviceHandle,   //�豸���
                        void * Buffer,                                      //�������׵�ַ
                        unsigned long BufferLength                          //��������С
                        );                                                 


//�ر��豸
void 		    WINAPI	CH9329CloseDevice(HANDLE hDeviceHandle              //�豸���
                        );

#ifdef __cplusplus
}
#endif

#endif		// _CH9329_DLL_H