#include "config.h"

/**************************************************
 * Description :            LCD初始化函数
 * Parameters  :                 无
 * Created By  :               Kngboy
 * Created Date:             2007-09-21
 **************************************************/
void LCD_Init(void)
{
	PINSEL1&= ~((3 << 22) | (3 << 24) | (3 << 26) | (3 << 28));
	IO0DIR |= LCD_CS | LCD_DAT | LCD_CLK | LCD_RST;
	
	IO0CLR= LCD_CLK;
	IO0CLR= LCD_RST;
	Delay(1);
	IO0SET= LCD_RST;
	IO0SET= LCD_CS;
}

/**************************************************
 * Description :           向LCD发送时序逻辑
 * Parameters  :          byte - 需要发送的内容
 * Created By  :               Kngboy
 * Created Date:             2007-09-21
 **************************************************/
void LCD_SendByte(uint8 byte)
{
	uint8 i= 0;
	for(i= 0; i< 8; i++)
	{
		(byte & 0x80) ? (IO0SET= LCD_DAT) : (IO0CLR= LCD_DAT);
		byte= byte<< 1;
		
		IO0SET= LCD_CLK;
		Delay(1);
		IO0CLR= LCD_CLK;
	}
}

/**************************************************
 * Description :          向LCD发送命令或数据
 * Parameters  :   data - 命令或数据 isdat - 0.命令 1.数据
 * Created By  :               Kngboy
 * Created Date:             2007-09-21
 **************************************************/
void LCD_SendData(uint8 data, uint8 isdat)
{
	LCD_SendByte(isdat ? 0xFA : 0xF8);
	LCD_SendByte(data & 0xF0);
	LCD_SendByte(data<< 4);
}

/**************************************************
 * Description : 在屏幕上写字符或画图之前的模式切换
 * Parameters  : TEXT - 字符 CLEAR - 清屏 其余 - 画图
 * Created By  :               Kngboy
 * Created Date:             2007-09-21
 **************************************************/
void LCD_SetMode(uint8 mode)
{
	switch(mode) {
		case LCD_MODE_TEXT:
			 LCD_SendData(0x30, 0);
			 LCD_SendData(0x0C, 0);
			 LCD_SendData(0x02, 0);
			 break;
		case LCD_MODE_CLEAR:
			 LCD_SendData(0x30, 0);
			 LCD_SendData(0x01, 0);
			 Delay(1000);
			 break;
		case LCD_MODE_HIDE:
			 LCD_SendData(0x34, 0);
			 break;
		case LCD_MODE_SHOW:
			 LCD_SendData(0x36, 0);
			 break;
	}
}

/**************************************************
 * Description :           在屏幕上写字符
 * Parameters  :  x - 0->7, X轴; y - 0->3, Y轴; *p - 字符
 * Created By  :               Kngboy
 * Created Date:             2007-09-21
 **************************************************/
void LCD_TextOut(uint8 x, uint8 y, char *p)
{
	uint8 i= 0, j= 0;
	while(*p) {
		j= (x+ i)%8;
		switch(y+ (x+ i++)/8)
		{
			case 0:	LCD_SendData(0x80+ j, 0);	break;
			case 1:	LCD_SendData(0x90+ j, 0);	break;
			case 2:	LCD_SendData(0x88+ j, 0);	break;
			case 3:	LCD_SendData(0x98+ j, 0);	break;
		}
		
		LCD_SendData(*p++, 1);
		if(*p)
			LCD_SendData(*p++, 1);
	}
}

/**************************************************
 * Description :            在屏幕上画图
 * Parameters  :                 无
 * Created By  :               Kngboy
 * Created Date:             2007-09-21
 **************************************************/
void LCD_DrawOut(void)
{
	uint8 i= 0, j= 0;
	for(i= 0; i< 64; i++)
		for(j= 0; j< 8; j++) {
			LCD_SendData(0x80+ i%32, 0);
			LCD_SendData(0x80+ j+ i/32*8, 0);
			LCD_SendData(picGRID[i][j*2], 1);
			LCD_SendData(picGRID[i][j*2+ 1], 1);
	}
}

uint8 const picGRID[64][16]= {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8C,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x80,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x5F,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE8,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xE0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x9F,0xF0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xF0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0x90,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xB0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0xF0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0xC0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,
0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x10,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x10,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x30,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x20,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x20,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x60,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x48,0xC0,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x40,0xD8,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0xE5,0xF0,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0xFE,0x80,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0xBF,0xFE,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x99,0xFF,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x3F,0xFF,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x03,0x3F,0xF9,
0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x03,0x1F,0xFB,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x06,0x07,0xEF,
0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x06,0x00,0x9C,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x0C,0x00,0x80,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x0C,0x01,0x80,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x01,0x80,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x18,0x01,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x18,0x01,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x30,0x03,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x30,0x02,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x30,0x02,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x60,0x06,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x60,0x04,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xC0,0x04,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xC0,0x0C,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0xC0,0x08,0x00,
0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF9,0x80,0x18,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x80,0x18,0x00,
0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFB,0x80,0x10,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x00,0x30,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x30,0x00,
0x00,0x00,0x08,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,
0x00,0x00,0x0C,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,
0x00,0x00,0x0E,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,
0x00,0x1F,0xFF,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,
0x00,0x1F,0xFF,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,
0x00,0x1F,0xFF,0xC8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,
0x00,0x1F,0xFF,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,
0x00,0x1F,0xFF,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,
0x00,0x00,0x0E,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,
0x00,0x00,0x0C,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,
0x00,0x00,0x08,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,
0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
