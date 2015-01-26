#include "stm32f10x.h"
#include "TFT_Driver.h"
#include "delay.h"
#include "usart.h"
#include "MMC_SD.h"
#include "spi.h"
#include "timer.h"
#include "rtc.h"
#include "ff.h"  

FATFS fs;		 /* Work area (file system object) for logical drive */
FIL fsrc, fdst;	  /* file objects */
FRESULT res;
UINT br,bw;
uint16_t bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;
char path0[20]="0:";
//char path0[512]="0:";
//char buffer[4096];   /* file copy buffer */
char buffer[10];   /* file copy buffer */
uint8_t textFileBuffer[] = "ABCDE";

GPIO_InitTypeDef GPIO_InitStructure;

//void Delayms(__IO uint32_t nCount);
int SDTest(void);
int FTTest(void);
int testRTC(void);
unsigned char Num[10]={0,1,2,3,4,5,6,7,8,9};

void Redraw_Mainmenu(void)
{
	TFT_Clear(GRAY0);
	TFT_DrawFont_GBK16(16,0,BLUE,GRAY0,"ȫ�����Ӽ���");
	TFT_DrawFont_GBK16(16,20,RED,GRAY0,"Һ�����Գ���");
	DisplayButtonDown(15,38,113,58); //x1,y1,x2,y2
	TFT_DrawFont_GBK16(16,40,YELLOW,GRAY0,"��ɫ������");
	DisplayButtonUp(15,68,113,88); //x1,y1,x2,y2
	TFT_DrawFont_GBK16(16,70,BLUE,GRAY0,"������ʾ����");
	DisplayButtonUp(15,98,113,118); //x1,y1,x2,y2
	TFT_DrawFont_GBK16(16,100,RED,GRAY0,"ͼƬ��ʾ����");

	TFT_DrawFont_GBK16(16,120,BLUE,GRAY0,"S1:Move.	");
	TFT_DrawFont_GBK16(16,140,RED,GRAY0, "S2:Sellect  ");
	//delay_ms(1000);
	//delay_ms(1000);

	TFT_DrawFont_Num32(100,125,RED,GRAY0,Num[5]);
	delay_ms(1000);
	TFT_DrawFont_Num32(100,125,RED,GRAY0,Num[4]);
	delay_ms(1000);
	TFT_DrawFont_Num32(100,125,RED,GRAY0,Num[3]);
	delay_ms(1000);
	TFT_DrawFont_Num32(100,125,RED,GRAY0,Num[2]);
	delay_ms(1000);
	TFT_DrawFont_Num32(100,125,RED,GRAY0,Num[1]);
	delay_ms(1000);
	TFT_DrawFont_Num32(100,125,RED,GRAY0,Num[0]);	
}

void Num_Test(void)
{
	u8 i=0;
	TFT_Clear(GRAY0);
	TFT_DrawFont_GBK16(16,20,RED,GRAY0,"Num Test");
	delay_ms(1000);
	TFT_Clear(GRAY0);

	for(i=0;i<10;i++)
	{
	TFT_DrawFont_Num32((i%3)*40,32*(i/3)+30,RED,GRAY0,Num[i+1]);
	delay_ms(100);
	}
	delay_ms(1000);
	
}
//������ʾ����ȫ�����Ӽ����̳���Ϊרע����רҵ
void Font_Test(void)
{
	u8 x_offset;
#ifdef H_VIEW  //��������˺�����ʾ
	x_offset=40;
#else
	x_offset=0;
#endif
	TFT_Clear(GRAY0);
	TFT_DrawFont_GBK16(16,10,BLUE,GRAY0,  "   ������ʾ����   ");

	delay_ms(1000);
	TFT_Clear(WHITE);
	TFT_DrawFont_GBK16(x_offset,10,RED,WHITE,  "	 ȫ�����Ӽ������޹�˾	  ");
	TFT_DrawFont_GBK16(x_offset,30,RED,WHITE,  "		 QDtech .,LTD		 ");
	TFT_DrawFont_GBK24(x_offset,50,BLUE,WHITE, "		   ��ӭ��			");
	TFT_DrawFont_GBK16(x_offset,100,GREEN,WHITE,   "	 ȫ�̼���֧��	 ");	
	TFT_DrawFont_GBK16(x_offset,120,RED,YELLOW,   "http://qdtech.taobao.com");
	TFT_DrawFont_GBK16(x_offset,140,RED,YELLOW,   "E-Mail:QDtech2008@gmail.com");
	TFT_DrawFont_GBK16(x_offset,160,RED,YELLOW,   "��������Ⱥ:324828016");	
	TFT_DrawFont_GBK16(x_offset,180,BLUE,WHITE,   "Tel:15989313508 ");
	delay_ms(3000);	
	

}

void Color_Test(void)
{
	//u8 i=2;
	TFT_Clear(GRAY0);
	
	TFT_DrawFont_GBK16(20,10,BLUE,GRAY0,"Color Test");
	delay_ms(1000);

	//while(i--)
	//{
	  TFT_Clear(WHITE);
	  TFT_Clear(BLACK);
	  TFT_Clear(RED);
  	TFT_Clear(GREEN);
  	TFT_Clear(BLUE);
	//}		
}

//u16 ID=0;
u8 rdata = 255;
int main(void)
{
  //uint16_t a;
  /* System Clocks Configuration **********************************************/
  SystemInit();
  delay_init(72);//��ʱ��ʼ�� 
  uart_init(9600);
  while(1) 
  {
		TFT_Init();			
		TFT_Clear(BLACK);
		TFT_LED_SET;
		printf("Test RTC\r\n");		
		testRTC();
		printf("Test SD\r\n");
		SDTest();
		FTTest();
		rdata = PAin(3);
		printf("Get PA3 Data %d\r\n",rdata);
		PAout(3)=1;
		rdata = PAin(3);
		printf("Get PA3 After set 1 Data %d\r\n",rdata);
		PAout(3)=0;
		rdata = PAin(3);
		printf("Get PA3 After reset 0 Data %d\r\n",rdata);
		TFT_Clear(BLUE);  
		//delay_ms(1000);
		delay_ms(1000);
		//TFT_Clear(BLACK); 
		TFT_Clear(WHITE);	

		Redraw_Mainmenu();//�򵥲˵�����
		Color_Test();//��ˢ������
		Num_Test();//�����������ʾ
		Font_Test();//��Ӣ�Ĳ���  
			
		TFT_Clear(BLACK);  
		//delay_ms(1000);
		delay_ms(1000);
		TFT_Clear(BLACK); 
		//TFT_Clear(WHITE);		
		TFT_DrawFont_GBK16(10,50,BLUE,GRAY0,"���Գɹ�.");
		delay_ms(1000);
  }
 
}

u8 buf[512];//SD�����ݻ�����
int SDTest(void)
{	
	u32 sd_size;
	u8 t=0;						
//  	Stm32_Clock_Init(9);//ϵͳʱ������
//	delay_init(72);		//��ʱ��ʼ��
//	uart_init(72,9600); //����1��ʼ��  	  
//	LCD_Init();			//��ʼ��Һ�� 
//	LED_Init();		 //LED��ʼ��		  														
//					   
// 	POINT_COLOR=RED;//��������Ϊ��ɫ	   
//	LCD_ShowString(60,50,"Mini STM32");	
//	LCD_ShowString(60,70,"SD Card TEST");	
//	LCD_ShowString(60,90,"ATOM@ALIENTEK");
//	LCD_ShowString(60,110,"2011/1/1");
	u8 retry = 3;	
	while(SD_Init()!=0 && retry-->0) //��ⲻ��SD��
	{
		printf("SD Card Failed!\r\n");
		delay_ms(500);
		printf("Please Check!	  \r\n");
		delay_ms(500);
//		LED0=!LED0;//DS0��˸
	}
	if(retry == 255)return 1;
	//���SD���ɹ� 	
	sd_size=SD_GetCapacity();											
	printf("SD Card Checked OK \r\n");
	printf("SD Card Size:  %d Mb\r\n",sd_size);	
	while(1)
	{
		if(t==30)//ÿ6s��ִ��һ��
		{
			if(SD_ReadSingleBlock(0,buf)==0)//��ȡMBR����
			{	
				printf("USART1 Sending Data...");
				printf("SECTOR 0 DATA:\n");
				for(sd_size=0;sd_size<512;sd_size++)printf("%x ",buf[sd_size]);//��ӡMBR��������		   
				printf("\nDATA ENDED\n");
				printf("USART1 Send Data Over!");
			}
			t=0;
			break;
		}   
		t++;
		delay_ms(200);
		//LED0=!LED0;
	}
	return 0;
}

//ALIENTEK Mini STM32�����巶������11
//RTCʵʱʱ�� ʵ��
//����ԭ��@ALIENTEK
//������̳:www.openedv.com	  
 	
const u8 *COMPILED_DATE=__DATE__;//��ñ�������
const u8 *COMPILED_TIME=__TIME__;//��ñ���ʱ��

const u8* Week[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

 int testRTC(void)
 {
	u8 t=0;	
	RTC_Init();
	if(t!=timer.sec)
	{
		t=timer.sec;
		printf("%d-",timer.w_year);		
		printf("%d-",timer.w_month);	
		printf("%d  ",timer.w_date);	
		printf("%d:",timer.hour);	
		printf("%d:",timer.min);	
		printf("%d",timer.sec);	
		switch(timer.week)
		{
			case 0:
				printf("%s\n","Sunday   ");
				break;
			case 1:
				printf("%s\n","Monday   ");
				break;
			case 2:
				printf("%s\n","Tuesday  ");
				break;
			case 3:
				printf("%s\n","Wednesday");
				break;
			case 4:
				printf("%s\n","Thursday ");
				break;
			case 5:
				printf("%s\n","Friday   ");
				break;
			case 6:
				printf("%s\n","Saturday ");
				break;  
		}
	} 
	return 0;
 }
FRESULT scan_files (
	char* path		/* Start node to be scanned (also used as work area) */
)
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
	int i;
	char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
	static char lfn[_MAX_LFN + 1];
	fno.lfname = lfn;
	fno.lfsize = sizeof(lfn);
#endif


	res = f_opendir(&dir, path);					   /* Open the directory */
	if (res == FR_OK) {
		i = strlen(path);
		for (;;) {
			res = f_readdir(&dir, &fno);				   /* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
			if (fno.fname[0] == '.') continue;			 /* Ignore dot entry */
#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			fn = fno.fname;
#endif
			if (fno.fattrib & AM_DIR) {					/* It is a directory */
				sprintf(&path[i], "/%s", fn);
				printf("scan file - %s\n\r",path);
				res = scan_files(path);
				if (res != FR_OK) break;
				path[i] = 0;
			} else {									   /* It is a file. */
				printf("scan file - %s/%s\n\r", path, fn);
			}
		}
	}else{
		printf("scan files error : %d\n\r",res);
	}

	return res;
}
/*******************************************************************************
  * @��������	SD_TotalSize
  * @����˵��   �ļ��ռ�ռ����� 
  * @�������   �� 
  * @�������   ��
  * @���ز���   1: �ɹ� 
  				0: ʧ��
  * @ע������	��
  *****************************************************************************/
int SD_TotalSize(char *path)
{
	FATFS *fs;
	DWORD fre_clust;		

	res = f_getfree(path, &fre_clust, &fs);  /* �����Ǹ�Ŀ¼��ѡ�����0 */
	if ( res==FR_OK ) 
	{
	  printf("\n\rget %s drive space.\n\r",path);
	  /* Print free space in unit of MB (assuming 512 bytes/sector) */
	  printf("%d MB total drive space.\r\n"
		   "%d MB available.\r\n",
		   ( (fs->n_fatent - 2) * fs->csize ) / 2 /1024 , (fre_clust * fs->csize) / 2 /1024 );
		
	  return 1;
	}
	else
	{ 
	  printf("\n\rGet total drive space faild!\n\r");
	  return 0;   
	}
}

/**
  * @brief  ���ڴ�ӡ���
  * @param  None
  * @retval None
  */
int FTTest(void)
{
	u8 res;
	u16 cnt = 1;
	printf("\r\n*******************************************************************************");
	printf("\r\n************************ Copyright 2009-2012, ViewTool ************************");
	printf("\r\n*************************** http://www.viewtool.com ***************************");
	printf("\r\n***************************** All Rights Reserved *****************************");
	printf("\r\n*******************************************************************************");
	printf("\r\n");
	//�����ļ�ϵͳ
	res = f_mount(&fs,"0:",1);
	if(res != FR_OK){
		printf("mount filesystem 0 failed : %d\n\r",res);
	}
//	//д�ļ�����
//	printf("write file test......\n\r");
//	res = f_open(&fdst, "0:/1.txt", FA_CREATE_ALWAYS | FA_WRITE);
//	if(res != FR_OK){
//		printf("open file error : %d\n\r",res);
//	}else{
//		res = f_write(&fdst, textFileBuffer, sizeof(textFileBuffer), &bw);			   /* Write it to the dst file */
//		if(res == FR_OK){
//			printf("write data ok! %d\n\r",bw);
//		}else{
//			printf("write data error : %d\n\r",res);
//		}
//		/*close file */
//		f_close(&fdst);
//	}

	//���ļ�����
	printf("read file test......\n\r");
	res = f_open(&fsrc, "0:/1.TXT", FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK){
		printf("open file error : %d\n\r",res);
	}else{
		res = f_read(&fsrc, buffer, sizeof(textFileBuffer), &br);	 /* Read a chunk of src file */
		if(res==FR_OK){
			printf("read data num : %d\n\r",br);
			printf("Cont:%s\n\r",buffer);
		}else{
			printf("read file error : %d\n\r",res);
		}
		/*close file */
		f_close(&fsrc);
	}
	
		
	//ɨ���Ѿ����ڵ��ļ�
	printf("\n\rbegin scan files path0......\n\r");
	//scan_files(path0);

	//SD_TotalSize(path0);//��ȡSD����
	
	//��BMP�ļ�����
	printf("read bmp file test......\n\r");
	
	for (;cnt<140;cnt++){
		sprintf(path0,"0:/%d.bmp",cnt);
		res = f_open(&fsrc, path0, FA_OPEN_EXISTING | FA_READ);
		if(res != FR_OK){
			printf("open file error : %d\n\r",res);
		}else{
			//bmpReadHeader(fsrc);
			/*close file */
			f_close(&fsrc);
		}
	}
	return 0;
}

/*********************************************/
// These read data from the SD card file and convert them
// to big endian(the data is stored in little endian format!)

// LITTLE ENDIAN!
uint16_t read16(FIL *f)
{
  uint16_t d;
	uint32_t rc;	
  uint8_t b,buff[1];
  f_read(f,buff,1,&rc);
	b = buff[0];
  f_read(f,buff,1,&rc);
	d=buff[0];
  d <<= 8;
  d |= b;
  return d;
}

// LITTLE ENDIAN!
uint32_t read32(FIL *f)
{
  uint32_t d;
  uint16_t b;

  b = read16(f);
  d = read16(f);
  d <<= 16;
  d |= b;
  return d;
}

int  bmpReadHeader(FIL *f) {
  // read header
  uint32_t tmp;

  if (read16(f) != 0x4D42) {
	// magic bytes missing
	return -1;
  }

  // read file size
  tmp = read32(f);
  printf("size %x\r\n",tmp);

  // read and ignore creator bytes
  read32(f);

  bmpImageoffset = read32(f);
  printf("offset %d\r\n",bmpImageoffset);

  // read DIB header
  tmp = read32(f);
  printf("header size %d\r\n",tmp);
  bmpWidth = read32(f);
  bmpHeight = read32(f);


  if (read16(f) != 1)
	return -1;

  bmpDepth = read16(f);
 printf("bitdepth %d\r\n",bmpDepth);

  if (read32(f) != 0) {
	// compression not supported!
	return -1;
  }

   printf("compression %d\r\n",tmp);

  return 1;
}



