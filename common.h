#ifndef COMMON_H
#define COMMON_H

// *** ����������������, ���� ������������ EEPROM !!!  ***
//#define  _EEPROM
                        



//����� ������
#define btEnter 0b00001100 //������ ���/���� ��������
#define btUp    0b00100000 //������ �����
#define btDown  0b00010000 //������ ����  
#define btRight 0b00001000 //������ ������ 
#define btLeft  0b00000100 //������ �����  
#define datDown PINA.0 //������ ������
#define datUp   PINA.1 //������ �����
#define datAll   PINA.6 //������ �����������         
