#include <xc.h>
#define _XTAL_FREQ 6000000
void lcd_init(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);
void delay(unsigned int);
//void lcdOutput(unsigned int);
void lcdOutput(unsigned char i);
void lcd_print(const char *str);
void keyscane();
unsigned char array[15]={"BATT VOLT:  17.5"};
unsigned char x,m,n,value;
unsigned int i,j,d1,d2,d3,d4;
void main(void) {
    lcd_init();
    while(1){
        keyscane();
    }
}
void lcd_init(){
	// lcd init
	//TRISC = 0x00; //all pins are op
    TRISB &= 0x00; //RB0(EN) and RB3 as ouptut
	TRISD = 0x00;  
	PORTD = 0x00;
	PORTB = 0xF0; //RB3 and RB0 as low
	lcd_command(0x30);
	__delay_ms(100);
	lcd_command(0x30);
	__delay_ms(100);
	lcd_command(0x30);
	__delay_ms(100);
	lcd_command(0x38);
	__delay_ms(100);
	lcd_command(0x06);
	__delay_ms(100);
	lcd_command(0x0C);
	__delay_ms(100);
	lcd_command(0x01);
	__delay_ms(100);
}
void keyscane(){
    value = PORTB & 0xF0;
    switch(value){
        //initial condition for batt value
        case 0xE0:
        j=17.5;
        lcd_command(0x80);
        for(x=0;x<8;x++){
            lcd_data(array[x]);
        }
        lcd_command(0x8A);
        for(x=10;x<14;x++){
            lcd_data(array[x]);
        }
        lcd_command(0x88);
        lcdOutput(j);
        break;
        //increment by 1
        case 0xD0:
            j+=0.1;
            if(j>30.0){
                j=30.0;
            }
            lcd_command(0x88);
            lcdOutput(j);
            break;
        
        //decrement by 1
        case 0xB0:
            j-=0.1;
            if(j<0.0){
                j=0.0;
            }
            lcd_command(0x88);
            lcdOutput(j);
            break;
            
        //Resetting the value to similar to first input
        case 0x70:
            j=17.5;
            lcd_command(0x88);
            lcdOutput(j);
            break;
    }
    lcd_command(0xC0);
    if(j<17.5){
        lcd_print("BATTERY LOW    ");
    }
    else if(j>20.5){
        lcd_print("BATTERY HIGH   ");
    }
    else{
        lcd_print("BATTERY NORMAL   ");
    }
}
void lcdOutput(unsigned char i){
    //j=17.5
    i=(int)(j*10+0.5);
    d4=(unsigned char)(i/1000); //extracts the thousand place
    d3=(unsigned char)((i/100)%10);//extracts the hundered place
    d2=(unsigned char)((i/10)%10); //extracts the tens place
    d1=(unsigned char)(i%10);//extracts the ones place
    
     lcd_command(0x88);//moving to 8th position in first row of lcd
     lcd_data(':');
      lcd_command(0x8A);//moving to the 10th position in first row of lcd
      lcd_data(48+d3);
       lcd_command(0x8B);//moving to the 11th position in first row of lcd
      lcd_data(48+d2);
       lcd_command(0x8C);//moving to the 12th position in first row of lcd
      lcd_data('.');
       lcd_command(0x8D);//moving to the 13th position in first row of lcd
      lcd_data(48+d1);
}
void lcd_print(const char *str){
    while(*str){
        lcd_data(*str++);
    }
}
void lcd_command(unsigned char i){
	PORTB &= 0xF7; //RS=>RB3 //
	PORTD=i;
	PORTB |= 0x01;  //RB0 =1
	PORTB &= 0xFE;  //RC0=0
	__delay_ms(100);
}
void lcd_data(unsigned char i){
	PORTB |= 0x08; //RS->RC3->1
	PORTD=i;
	PORTB |= 0x01;  //RC0 =1
	PORTB &= 0xFE; //~0x01;  //RC0=0
	__delay_ms(100);
}
