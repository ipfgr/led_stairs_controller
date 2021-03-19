#include <common.h>
#include <delay.h> 
#include <mega16.h>  

// LCD у нас подсоединен на LCD_PORT
void LCD_func(unsigned char CH, unsigned char isCMD){ 
unsigned char TEMP=0; 
 TEMP |= OE;        // OE=1
 TEMP &= RS;        // RS=0
 delay_us(100);
 
 TEMP=CH ;   
 
 TEMP &= 0xF0;      //Сбросить в 0 младшие 4 бита 

 TEMP |= OE;        // OE=1
 if (!isCMD) TEMP |= RS;        // Если это команда, то RS=1
 LCD_PORT = TEMP;      //Переслать старшие 4 бита в LCD
 delay_us(100);
 LCD_PORT &= ~OE;      //OE=0 
 TEMP=CH; 

 TEMP=TEMP<<4;      //Переместить младшие биты на место старших
 
 TEMP |= OE;        // OE=1
 if (!isCMD) TEMP |= RS;        // Если это команда, то RS=1
 LCD_PORT = TEMP;      //Переслать старшие 4 бита в LCD
 delay_us(10);
 LCD_PORT &= ~OE;      //OE=0 
 
 delay_us(50);      // подождать 
}  

void LCD_CMD(unsigned char CMD){ 
 LCD_func(CMD,1); 
}
void LCD_CHAR(unsigned char CH){ 
 LCD_func(CH,0); 
}
//Позиционирование курсора
void LCD_POS(unsigned char pos){
 pos = pos | 0x80;
 LCD_CMD(pos);
 delay_us(350);
} 
//Вывод строки на дисплей
#ifdef _EEPROM  //если храним в EEPROM
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
 LCD_CMD(0b00000001);   // Очистить дисплей
 delay_ms(100);           // подождать
}  

//Инициализация LCD
void init_LCD(){  
 delay_ms(100); 
 // Установить 4-битный режим на 2-х строчном индикаторе (стр. 17 DV-16230 PDF) 
 //т.к. сейчас режим неизвестен, то делаем это на низком уровне
 LCD_PORT = 0b00100000;
 delay_ms(20); 
  LCD_PORT = 0b00100000;
 delay_ms(20);           
  LCD_PORT = 0b00100000;
                                          
 LCD_PORT &= ~RS;
 LCD_PORT |= OE; //OE=1
 //Установить OE=0 и ждать
 LCD_PORT &= ~OE;
 delay_us(100);
 
 //Второй раз  - 
 LCD_CMD(0b00101000);   // Установить 4-битный режим на 2-х строчном индикаторе (стр. 17 DV-16230 PDF) 
  delay_ms(20);
 LCD_CMD(0b00001100);   // LCD=on, Курсор=off Мерцание курсора=off
                delay_ms(20);
}