#include <Arduino.h>  // импортируем библитеки
#include <avr/io.h>
#include <util/delay.h>
#include "shift_out_port.h"

 sop :: sop(){
     for (int f = 0; f  < ((maxR * cop) - 1); f++){ // предварительная инициализация упр. массивов ===
        cm[f] = 0;
        value1[f] = 0; 
      };
      for(int f = 0; f < maxR - 1; f++){
        exitA[f] = 0b00000000;
      }//=========================
}
void sop :: SetValue(int index, unsigned char value ){
      value1[index] = value; 
    }
void sop :: SetExitBit(int index, unsigned char bit,unsigned char value ){
      if (value==1) exitA[index] |= (1 << bit); 
      else exitA[index] &= ~(1 << bit);
    }
void sop :: PWM(int nop, unsigned char nm){
    if (value1[nop] > 0){
      if((nm >= 0) && (nm <= value1[nop])){
        exitA[static_cast<int>(nop / cop)] |= (1 << (nop % cop));
      } else if(( nm > value1[nop]) && (nm < high)){
        exitA[static_cast<int>(nop / cop)] &= ~(1 << (nop % cop));
      } else {
        nm = 0;
      }
    }
  }
void sop :: initRegAndTimer0(byte mode, byte value){
  switch (mode)
  {
    case 0:
    OCR0A = value;
    break;
    case 1:
    TCCR0A = (1<<WGM01);
    TCCR0B = (1<<CS00);
    OCR0A = value;
    break;
    case 2:
    TCCR0A = (1<<WGM01);
    TCCR0B = (1<<CS01);
    OCR0A = value;
    break;
    case 3:
    TCCR0A = (1<<WGM01);
    TCCR0B = (1<<CS00)|(1<<CS01);
    OCR0A = value;
    break;
    case 4:
    TCCR0A = (1<<WGM01);
    TCCR0B = (1<<CS02);
    OCR0A = value;
    break;
    case 5:
    TCCR0A = (1<<WGM01);
    TCCR0B = (1<<CS00)|(1<<CS02);
    OCR0A = value;
  }
    // TCCR0A = (1<<WGM01); // Режим CTC (сброс по совпадению)
    // // TCCR0B = (1<<CS00); // Тактирование от CLK.
    // // Если нужен предделитель :
    //  TCCR0B = (1<<CS01); // CLK/8
    //TCCR0B = (1<<CS00)|(1<<CS01); // CLK/64
    //  TCCR0B = (1<<CS02); // CLK/256
   //   TCCR0B = (1<<CS00)|(1<<CS02); // CLK/1024

    // //OCR0A = 123; // Верхняя граница счета. Диапазон от 0 до 255.
    // // Частота прерываний будет = Fclk/(N*(1+OCR0A))
    // // где N - коэф. предделителя (1, 8, 64, 256 или 1024)
    // //TIMSK0 = (1<<OCIE0A); // Разрешить прерывание по совпадению
    // OCR0A = 5;
    TIMSK0 |= (1 << OCIE0A);
    DDRD |= (1 << DDD2); // pin 2 - data 
    DDRD |= (1 << DDD3); // pin 3 - clock
    DDRD |= (1 << DDD4); // pin 4 - lauch
  }
void sop :: mainloop(){
    
    for (int i = (maxR * cop) - 1; i >= 0; i--){ 
        cm[i]++;
    }
    for (int f = maxR-1; f>= 0; f--){
      for (int i = cop-1; i >= 0; i--){
        PORTD &= ~(1 << PD3); 
         writePin2(exitA[f] & (1 << i));
        PORTD |= (1 << PD3);
      }//==============
    }
    PORTD |= (1 << PD4);
    PORTD &= ~(1 << PD4);
    // шим 
    for (int p = 0; p < (maxR * cop); p++){
      PWM(p, cm[p]);
    } //==========
  }