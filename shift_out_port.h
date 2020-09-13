#ifndef shift_out_port_h
#define shift_out_port_h

#include <Arduino.h>  // импортируем библитеки
#include <avr/io.h>
#include <util/delay.h>

  const int maxR = 2;// count of reg.  
  const int cop = 8;
  const int high = 255;



class sop
{
  private:

    unsigned char exitA[maxR];// выходной массив
    unsigned char cm[maxR * cop]; // счетный массив
    unsigned char value1[maxR * cop]; // массив коефициентов // ========
    void writePin2(bool ifg){
      if (ifg) PORTD |= (1 << PD2); else PORTD &= ~(1 << PD2) ;// вспомогательная функция для загрузки в регистры
    }
    public:
      sop();
      void SetValue(int index, unsigned char value);
      void SetExitBit(int index, unsigned char bit, unsigned char value);
      void PWM(int nop, unsigned char nm);
      void mainloop();
      void initRegAndTimer0(byte mode, byte value);
};

#endif