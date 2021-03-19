#ifndef COMMON_H
#define COMMON_H

// *** Раскоментировать, есль используется EEPROM !!!  ***
//#define  _EEPROM
                        



//Маски кнопок
#define btEnter 0b00001100 //Кнопка вкл/выкл настроек
#define btUp    0b00100000 //Кнопра вверх
#define btDown  0b00010000 //Кнопка вниз  
#define btRight 0b00001000 //Кнопка вправо 
#define btLeft  0b00000100 //Кнопка влево  
#define datDown PINA.0 //Датчик вверху
#define datUp   PINA.1 //датчик внизу
#define datAll   PINA.6 //датчик выключатель         
