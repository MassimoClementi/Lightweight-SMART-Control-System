// Author: Massimo Clementi
// Date: 26 August 2019
// Project: Lightweight SMART Control System  |   MIT License

#include <LiquidCrystal.h>

// LCD pins
#define LCD_RS 53
#define LCD_RW 52
#define LCD_E 51
#define LCD_D4 50
#define LCD_D5 49
#define LCD_D6 48
#define LCD_D7 47
#define LCD_CONTRAST 2
#define LCD_BACKLIGHT 3

// RGB Led pins
#define LED_R 5
#define LED_G 6
#define LED_B 7

// Buttons pins
#define BUTTON_1 37
#define BUTTON_2 36

// Potentiometer pin
#define POTENTIOMETER A0

// Time for "button long press"
#define BUTTON_TIME 300

LiquidCrystal lcd(LCD_RS, LCD_RW, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
int main_menu_i = 0;
int user_choice = 0;
char* main_menu_index[] = {"Red comp.       ","Green comp.     ","Blue comp.       ",\
                            "Contrast        ","Backlight       "};
int tab_index=0;
int values[]= {0,0,0,95,255}; // Default values to set when turned on

void setup(){
  pinMode(LCD_BACKLIGHT,OUTPUT);
  pinMode(LCD_CONTRAST,OUTPUT);
  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(LED_B,OUTPUT);

  pinMode(BUTTON_1,INPUT);
  pinMode(BUTTON_2,INPUT);

  pinMode(POTENTIOMETER,INPUT);

  digitalWrite(LCD_BACKLIGHT,HIGH);
  analogWrite(LCD_CONTRAST, values[3]);
  lcd.begin(16,2);

  start();
  delay(2000);
}

void start(){
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.write("= RGB  LED =");
  lcd.setCursor(0,1);
  lcd.write("SMART MANAGEMENT");
}

int button_input_wait(){
  //return 0 is BUTTON_1 quick press
  //return 1 is BUTTON_1 long press
  //return 2 is BUTTON_2 quick press

  while(1){
    if(digitalRead(BUTTON_1)){
      int i = 0;
      while(digitalRead(BUTTON_1)&&i<BUTTON_TIME){
        i++;delay(1);
      }
      if(i<BUTTON_TIME) return 0;
      while(digitalRead(BUTTON_1)){}
      return 1;
    }
    if(digitalRead(BUTTON_2)){
      while((digitalRead(BUTTON_2))){}
      return 2;
    }
  }
}

int potentiometer_in(){
  return analogRead(POTENTIOMETER)*0.25;
}

void set_out(){
  analogWrite(LED_R,values[0]);
  analogWrite(LED_G,values[1]);
  analogWrite(LED_B,values[2]);
  analogWrite(LCD_CONTRAST, values[3]);
  analogWrite(LCD_BACKLIGHT, values[4]);
}

//========================================================================

void loop() {
  while(tab_index==0){
    lcd.clear();
    lcd.write("== MAIN  MENU ==");
    lcd.setCursor(0,1);
    lcd.print(main_menu_i+1);
    lcd.print(" - ");
    lcd.print(main_menu_index[main_menu_i]);

    user_choice = button_input_wait();
    switch (user_choice) {
      case 0: main_menu_i = (main_menu_i+1)%5; break;
      case 2: tab_index=1; break;
      default: break;
    }
    set_out();
    delay(20);
  }

  while(tab_index==1){
    lcd.clear();
    lcd.print("= ");
    lcd.print(main_menu_index[main_menu_i]);
    lcd.setCursor(15,0);
    lcd.print("=");
    lcd.setCursor(0,1);
    lcd.write("Value:     ");
    lcd.setCursor(7,1);
    lcd.print(values[main_menu_i]);

    user_choice = button_input_wait();
    switch (user_choice) {
      case 1: tab_index=0; break;
      case 2:
        while(!digitalRead(BUTTON_2)){
          values[main_menu_i] = potentiometer_in();
          lcd.setCursor(0,1);
          lcd.write("Value:       MOD");
          lcd.setCursor(7,1);
          lcd.print(values[main_menu_i]);
          set_out();
          delay(150);
        }
        while(digitalRead(BUTTON_2)){}
      default: break;
    }
    set_out();
    delay(20);
  }
}
