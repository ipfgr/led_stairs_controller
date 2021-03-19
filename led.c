#include <common.h>
#include <delay.h> 
#include <mega16.h>  

// LCD � ��� ����������� �� LCD_PORT
void LCD_func(unsigned char CH, unsigned char isCMD){ 
unsigned char TEMP=0; 
 TEMP |= OE;        // OE=1
 TEMP &= RS;        // RS=0
 delay_us(100);
 
 TEMP=CH ;   
 
 TEMP &= 0xF0;      //�������� � 0 ������� 4 ���� 

 TEMP |= OE;        // OE=1
 if (!isCMD) TEMP |= RS;        // ���� ��� �������, �� RS=1
 LCD_PORT = TEMP;      //��������� ������� 4 ���� � LCD
 delay_us(100);
 LCD_PORT &= ~OE;      //OE=0 
 TEMP=CH; 

 TEMP=TEMP<<4;      //����������� ������� ���� �� ����� �������
 
 TEMP |= OE;        // OE=1
 if (!isCMD) TEMP |= RS;        // ���� ��� �������, �� RS=1
 LCD_PORT = TEMP;      //��������� ������� 4 ���� � LCD
 delay_us(10);
 LCD_PORT &= ~OE;      //OE=0 
 
 delay_us(50);      // ��������� 
}  

void LCD_CMD(unsigned char CMD){ 
 LCD_func(CMD,1); 
}
void LCD_CHAR(unsigned char CH){ 
 LCD_func(CH,0); 
}
//���������������� �������
void LCD_POS(unsigned char pos){
 pos = pos | 0x80;
 LCD_CMD(pos);
 delay_us(350);
} 
//����� ������ �� �������
#ifdef _EEPROM  //���� ������ � EEPROM
void LCD_stre(eeprom char *str){ 
eeprom char *ptr;
ptr=str;
while (*ptr!='\0')LCD_CHAR(*ptr++);
}
#endif

void LCD_str(char *str){ 
char *ptr;
ptr=str;
while (*ptr!='\0')LCD_CHAR(*ptr++);
}

void LCD_clear(){
 LCD_CMD(0b00000001);   // �������� �������
 delay_ms(100);           // ���������
}  

//������������� LCD
void init_LCD(){  
 delay_ms(100); 
 // ���������� 4-������ ����� �� 2-� �������� ���������� (���. 17 DV-16230 PDF) 
 //�.�. ������ ����� ����������, �� ������ ��� �� ������ ������
 LCD_PORT = 0b00100000;
 delay_ms(20); 
  LCD_PORT = 0b00100000;
 delay_ms(20);           
  LCD_PORT = 0b00100000;
                                          
 LCD_PORT &= ~RS;
 LCD_PORT |= OE; //OE=1
 //���������� OE=0 � �����
 LCD_PORT &= ~OE;
 delay_us(100);
 
 //������ ���  - 
 LCD_CMD(0b00101000);   // ���������� 4-������ ����� �� 2-� �������� ���������� (���. 17 DV-16230 PDF) 
  delay_ms(20);
 LCD_CMD(0b00001100);   // LCD=on, ������=off �������� �������=off
                delay_ms(20);
}