#include <mega16.h>  
#include <delay.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h>
#include <common.h>
#include <n3310lcd.c>

#define xtal 8000000L
#define p1 PORTD.0
#define p2 PORTD.1
#define p3 PORTD.2
#define p4 PORTD.3
#define p5 PORTD.4
#define p6 PORTD.5
#define p7 PORTD.6
#define p8 PORTD.7
#define p9 PORTC.0
#define p10 PORTC.1
#define p11 PORTC.2
#define p12 PORTC.3
#define p13 PORTC.4
#define p14 PORTC.5
#define p15 PORTC.6
#define p16 PORTC.7
#define p17 PORTA.7



/*объявляем прерменные***********************************************************************************************************/
unsigned char ped1,ped2,ped3,ped4,ped5,ped6,ped7,ped8,ped9,ped10,ped11,ped12,ped13,ped14,ped15,ped16,ped17;       //переменные, для изменения скважности ШИМ в программе
unsigned char p1_b,p2_b,p3_b,p4_b,p5_b,p6_b,p7_b,p8_b,p9_b,p10_b,p11_b,p12_b,p13_b,p14_b,p15_b,p16_b,p17_b;    //переменные, для буферизации значений скважности ШИМ
unsigned char count;                            //переменная- счетчик вызовов обработчика прерываний
unsigned char Buf[17];
unsigned char state,old_state;
unsigned char s;
unsigned char q; 
unsigned char r; 
unsigned char g;
char i; 
char EditMode=0;
char strIDX=0;
char lastIDX=0;
char backlightfix=1;
char pedSyncCounter = 0;
int pedArraySyncCounter = 0;


//TrueЪ links
unsigned char pedCounter;
unsigned char* pedArrayup[17] = {&ped1, &ped2, &ped3, &ped4, &ped5, &ped6, &ped7, &ped8, &ped9, &ped10, &ped11, &ped12, &ped13, &ped14, &ped15, &ped16};
//------
                           //переменная для работы алгоритма смены цветов




             
void Save();
void Home();
void HomeUp();
void HomeDown();
void HomeTest();
void flashLeds();
void doSettings(char Key);
void processsensors();
/********************************************************************************************************************************/ 
#ifdef _EEPROM 
  eeprom char effect           = 1;      //Эффект 
  eeprom char railef           = 0;      //Режим поручня
  eeprom char dimable          = 100;     //Диммер
  eeprom char steps            = 5;     //Время задержки перед выключением всех ступенек
  eeprom char step_onoff       = 1;      //Время задержки каждой ступеньки вверх
 eeprom char sensmod           = 1;      //Время задержки каждой ступеньки вниз    
  eeprom char stairs           = 16;     //Количество ступенек   
  eeprom char skip             = 0;      //Пропуск порта A 
  eeprom char skip2            = 0;      //Пропуск порта B
  eeprom char skip3            = 0;      //Пропуск порта C   
  eeprom char back             = 0;      //Эффект фоновой подсветки


#else
  char  effect              = 1;        //Эффект  
  char  railef              = 0;       //Режим поручня
  char  dimable             = 100;       //Диммер
  char  steps               = 1;       //Время задержки перед выключением всех ступенек
  char step_onoff           = 1;       //Время задержки каждой ступеньки вверх
  char sensmod              = 1;       //Время задержки каждой ступеньки вниз   
  char  stairs              = 16;      //Колличество ступенек
  char skip                 = 0;      //Пропуск порта A
  char skip2                = 0;      //Пропуск порта B
  char skip3                = 0;      //Пропуск порта C
  char back                 = 0;      //Эффект фоновой подсветки

#endif
#ifdef _EEPROM
 //  eeprom char *sEM            = "     Setup     ";
 // eeprom char *sLR            = "<-PREV    NEXT->";
  eeprom char *StairsW        = "Delay before off";
 eeprom char *sensM           = "Sensor function";
  eeprom char *Stair_onoff     = " Delay on/off ";
  eeprom char *EffectM        = "    Effect    "; 
  eeprom char *BacklightX     = "  Backlight FL   "; 
  eeprom char *railR          = "    Handrail    ";
  eeprom char *StairsN        = "   Max Stairs   "; 
  eeprom char *DimmN          = "    Dimmable    ";  
  eeprom char *skipN          = "Skip A channel";
  eeprom char *skipN2        = "Skip B channel";
  eeprom char *skipN3        = "Skip C channel";


#else 
 // char *sEM            = "     Setup     ";
 // char *sLR            = "<-PREV    NEXT->";
  char *StairsW          = "Delay before off";
   char *sensM          = "Sensor function";
  char *Stair_onoff       = " Delay on/off ";
  char *EffectM        = "    Effect    "; 
  char *BacklightX     = "  Backlight FL   "; 
  char *railR        = "    Handrail    ";
  char *StairsN        = "   Max Stairs   ";   
  char *DimmN          = "    Dimmable    ";
  char *skipN          = "Skip A channel";
  char *skipN2        = "Skip B channel";
  char *skipN3        = "Skip C channel";

#endif

/*обработчик прерывания**********************************************************************************************************/
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
count++;
if (count == 0){                                //если счетчик переполнился и принял значение 0
        p1_b   = ped1;                          //сохранием значения в буфер
        p2_b = ped2;
        p3_b  = ped3;
        p4_b  = ped4;
        p5_b  = ped5;
        p6_b  = ped6;
        p7_b  = ped7;
        p8_b  = ped8;
        p9_b  = ped9;
        p10_b  = ped10;
        p11_b  = ped11;
        p12_b  = ped12;
        p13_b  = ped13;
        p14_b  = ped14;
        p15_b  = ped15;
        p16_b  = ped16;
        p17_b  = ped17; 
     
        p1  = 1;                               //выставляем ноги, отвечающие за ШИМ в логическую 1
        p2  = 1;
        p3  = 1;
        p4  = 1;
        p5  = 1;
        p6  = 1;
        p7  = 1;
        p8  = 1;
        p9  = 1;
        p10  = 1;
        p11  = 1;
        p12  = 1;
        p13  = 1;
        p14  = 1;
        p15  = 1;
        p16  = 1;
         p17  = 1;
      
        }

if (p1_b  == count) { p1  = 0;}             //по достижении заданной скважности выводим логический 0 в ножку МК
if (p2_b  == count) { p2  = 0;}
if (p3_b  == count) { p3  = 0;}
if (p4_b  == count) { p4  = 0;}
if (p5_b  == count) { p5  = 0;}
if (p6_b  == count) { p6  = 0;}
if (p7_b  == count) { p7  = 0;}
if (p8_b  == count) { p8  = 0;}
if (p9_b  == count) { p9  = 0;}
if (p10_b  == count) { p10  = 0;}
if (p11_b  == count) { p11  = 0;}
if (p12_b  == count) { p12  = 0;}
if (p13_b  == count) { p13  = 0;}
if (p14_b  == count) { p14  = 0;}
if (p15_b  == count) { p15  = 0;}
if (p16_b  == count) { p16  = 0;}
if (p17_b  == count) { p17  = 0;}

}
/********************************************************************************************************************************/


 void wait_step_onoff(void)
{ 
  TCNT1=1;
  while (TCNT1<257*step_onoff*q) {};
}  


void flashLeds() {                       // Мигание всеми светодиодами, применяем как визуальное оповещение (например о сохранении настроек)
   PORTC = 1;
   PORTD = 1; 
   while ((PORTC += PORTC + 1) < 0xff);
   while ((PORTD += PORTD + 1) < 0xff);
   delay_ms(100);  
   PORTC = 0;
   PORTD = 0;     
}       

void dimables(){
if (dimable  == 100) { s  = 255 , q = 2;}
if (dimable  == 90) { s  = 230 , q = 3;}
if (dimable  == 80) { s  = 200 , q = 4;}
if (dimable  == 70) { s  = 170 , q = 5;}
if (dimable  == 60) { s  = 150 , q = 6;}
if (dimable  == 50) { s  = 125 , q = 7;}
if (dimable  == 40) { s  = 90 , q = 9;}
if (dimable  == 30) { s  = 50 , q = 20;}
if (dimable  == 20) { s  = 30 , q = 30;}
if (dimable  == 10) { s  = 15 , q = 40;}
}


void handrail(){
if (railef  == 0) { r  = 0;}
if (railef  == 10) { r  = 15;}
if (railef  == 20) { r  = 30;}
if (railef  == 30) { r  = 50;}
if (railef  == 40) { r  = 90;}
if (railef  == 50) { r  = 125;}
if (railef  == 60) { r  = 150;}
if (railef  == 70) { r  = 170;}
if (railef  == 80) { r  = 200;}
if (railef  == 90) { r  = 230;}
if (railef  == 100) { r  = 255;}

}

void backlight(){
if (back  == 0) { g  = 0,backlightfix  = 1;}
if (back  == 10) { g  = 15,backlightfix  = 2;}
if (back  == 20) { g  = 30,backlightfix  = 2;}
if (back  == 30) { g  = 50,backlightfix  = 2;}
if (back  == 40) { g  = 90,backlightfix  = 2;}
if (back  == 50) { g  = 125,backlightfix  = 2;}
if (back  == 60) { g  = 150,backlightfix  = 2;}
if (back  == 70) { g  = 170,backlightfix  = 2;}
if (back  == 80) { g  = 200,backlightfix  = 2;}
if (back  == 90) { g  = 230,backlightfix  = 2;}
if (back  == 100) { g  = 255,backlightfix  = 2;}

}

void processud()
  {  
  if (backlightfix  == 1) {  TCCR0=0x00;}
  if (backlightfix  == 2 && stairs  == 16) {  TCCR0=0x01 , ped1  = g, ped16=g;} 
  if (backlightfix  == 2 && stairs  == 15) {  TCCR0=0x01 , ped1  = g, ped15=g, ped16=0;}
  if (backlightfix  == 2 && stairs  == 14) {  TCCR0=0x01 , ped1  = g, ped14=g, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 13) {  TCCR0=0x01 , ped1  = g, ped13=g, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 12) {  TCCR0=0x01 , ped1  = g, ped12=g, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 11) {  TCCR0=0x01 , ped1  = g, ped11=g, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 10) {  TCCR0=0x01 , ped1  = g, ped10=g, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 9) {  TCCR0=0x01 , ped1  = g, ped9=g, ped10=0, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 8) {  TCCR0=0x01 , ped1  = g, ped8=g, ped9=0, ped10=0, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;} 
  if (backlightfix  == 2 && stairs  == 7) {  TCCR0=0x01 , ped1  = g, ped7=g, ped8=0, ped9=0, ped10=0, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 6) {  TCCR0=0x01 , ped1  = g, ped6=g, ped7=0, ped8=0, ped9=0, ped10=0, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 5) {  TCCR0=0x01 , ped1  = g, ped5=g, ped6=0, ped7=0, ped8=0, ped9=0, ped10=0, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 4) {  TCCR0=0x01 , ped1  = g, ped4=g, ped5=0, ped6=0, ped7=0, ped8=0,ped9=0, ped10=0, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  /*
  if (backlightfix  == 1) {  TCCR0=0x00;}
  if (backlightfix  == 2 && stairs  == 16 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped16=g;} 
  if (backlightfix  == 2 && stairs  == 15 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped15=g, ped16=0;}
  if (backlightfix  == 2 && stairs  == 14 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped14=g, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 13 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped13=g, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 12 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped12=g, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 11 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped11=g, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 10 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped10=g, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 9 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped9=g, ped10=0, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 8 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped8=g, ped9=0, ped10=0, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;} 
  if (backlightfix  == 2 && stairs  == 7 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped7=g, ped8=0, ped9=0, ped10=0, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 6 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped6=g, ped7=0, ped8=0, ped9=0, ped10=0, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 5 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped5=g, ped6=0, ped7=0, ped8=0, ped9=0, ped10=0, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && stairs  == 4 && effect  == 1 || effect ==2) {  TCCR0=0x01 , ped1  = g, ped4=g, ped5=0, ped6=0, ped7=0, ped8=0,ped9=0, ped10=0, ped11=0, ped12=0, ped13=0, ped14=0, ped15=0, ped16=0;}
  if (backlightfix  == 2 && effect  == 3) {  TCCR0=0x00;}
*/

  } 
  

void processPedOffall(unsigned char *ped)
  {    ped17=0;
     for( *ped=0; *ped > 0; *ped-=1 )
     { 
    
     }
  }                                       
 void processPedOn(unsigned char *ped)
  {    ped17=r;
     for( *ped=0; *ped < s; *ped+=1 )
      
     { 
        wait_step_onoff();
     }
  }  
   void processPedOff(unsigned char *ped)
  {   ped17=0;
     for( *ped=s; *ped > 0; *ped-=1 )
     { 
        wait_step_onoff();
     } 
     
  }      

   void processPowerOnSync()
   {       ped17=r;
      for(pedSyncCounter= 0; pedSyncCounter < s; pedSyncCounter++)
      {
        for(pedArraySyncCounter = 0; pedArraySyncCounter < 16; pedArraySyncCounter++)
        {
            *(pedArrayup[pedArraySyncCounter]) = pedSyncCounter;
             wait_step_onoff();
        }
      }  
   }
   
   void processPowerOffSync()
   {      ped17=0;
            for(pedSyncCounter= s; pedSyncCounter > 0; pedSyncCounter--)
      {
        for(pedArraySyncCounter = 0; pedArraySyncCounter < 16; pedArraySyncCounter++)
        {
            *(pedArrayup[pedArraySyncCounter]) = pedSyncCounter;
             wait_step_onoff();
        }
      } 
   }
  
   
  

 
void main(void)
{


//Все порты входы
PORTA=0x7F;
DDRA=0x80;
//Выход на дисплей
PORTB=0x00;
DDRB=0xFF;
//Все порты выходы
PORTC=0x00;
DDRC=0xFF;
//Все порты выходы
PORTD=0x00;
DDRD=0xFF;

TCCR0=0x01;                                     //настраиваем таймер
TCNT0=0x00;
TCCR1A=0x00;     //Инициализация Таймера/Счетчика 1
TCCR1B=0x02;


 TCCR2 = 0x0A; //start timer
OCR2=0x19;

ASSR=0x00;
MCUCR=0x00;
MCUCSR=0x00;
TIMSK=0x81;
ACSR=0x80;

SFIOR=0x00;
// Global enable interrupts
#asm("sei")

 //Инициализация LCD

LcdInit();

  
   
while (1)   
	{

   	LcdClear();
	for (i=0; i<10; i++)
		{
		sprintf (lcd_buf, "Буфер=%u", i);
		LcdString(1,1);
		sprintf (lcd_buf, "Буфер=%u", i);
		LcdStringBold(1,2);
		sprintf (lcd_buf, "Буфер=%u/", i);
		LcdStringBig(1,4);
		sprintf (lcd_buf, "%u", i);
		LcdStringBold(9,4);
	        sprintf (lcd_buf, "Буфер=%u ", i+2);
	        LcdString(1,6);
		LcdUpdate();
		
		LcdBar(65, 27, 75, 45, 10*(i+1));
		
		delay_ms(300);
		}
	
	LcdMode(3);
	
	for (i=0; i<10; i++)
		{
		sprintf (lcd_buf, "Буфер=%u", i);
		LcdString(1,1);
		sprintf (lcd_buf, "Буфер=%u", i);
		LcdStringBold(1,2);
		sprintf (lcd_buf, "Буфер=%u/", i);
		LcdStringBig(1,4);
		sprintf (lcd_buf, "%u", i);
		LcdStringBold(9,4);
	        sprintf (lcd_buf, "Буфер=%u ", i+2);
	        LcdString(1,6);
		LcdUpdate();
		
		LcdBar(65, 27, 75, 45, 100-10*(i+1));
		
		delay_ms(300);
		}
	
	LcdMode(2);	
	};


#endif
 }