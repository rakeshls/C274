#include <SD.h>
#include <ezButton.h>
#include <LiquidCrystal_I2C.h>
#include "lcd_custom_character.h"
File file;

int book_num = 0;
int book_state = 0;
int prev_book_num = -1;

ezButton next(2);
ezButton prev(3);
ezButton select_book(4);
ezButton open_book(5);
LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  Serial.begin(9600);
  SD.begin();

  next.setDebounceTime(50);
  prev.setDebounceTime(50);
  select_book.setDebounceTime(50);
  open_book.setDebounceTime(50);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  welcome();
  delay(1000);
  animate();
  delay(1000);
  /*file = SD.open("harry.txt", FILE_READ);
    Serial.print("Total characters :");
    Serial.println(file.size());

    if (file) {
    Serial.println("HARRY POTTER : ");
    delay(2000);
    while (file.available()) {
      char data = file.read();
      Serial.print(data);
    }
    Serial.println();
    file.close();
    }
    Serial.println("File read successfully");*/
}

void welcome(){
  lcd_print(1,0,"welcome");
  delay(1000);
  lcd_print(8,1,"to");
  delay(1000);
  lcd_print(10,2,"digibook");
  delay(1000);
}
void lcd_print(int x, int y, String message){
  lcd.setCursor(x,y);
  lcd.print(message);
}
void display_char(int x, int y , int id){
  lcd.setCursor(x,y);
  lcd.write(id);
}
void animate(){
  lcd.createChar(0,left_stick);
  lcd.createChar(1,right_stick);
  lcd.createChar(2,open_eye);
  lcd.createChar(3,middle_stick);
  lcd.createChar(4,closed_eye);
  for(int i=0;i<5;i++){
  display_char(11,3,1);
  display_char(9,3,3);
  display_char(7,3,0);
  if(i%2==0){
  display_char(8,3,2);
  display_char(10,3,2);
  delay(1000);
  }
  else{
  display_char(8,3,4);
  display_char(10,3,4);
  delay(100);
  }
  }
  lcd.clear();
}

void loop() {

  next.loop();
  prev.loop();
  select_book.loop();
  open_book.loop();

  if (next.isPressed()) {
    book_num++;
  }
  else if (prev.isPressed()) {
    book_num--;
  }
  else if (select_book.isPressed()) {
    book_state=0;
    prev_book_num=-1;
  }
  else if (open_book.isPressed()) {
    book_state=1;
  }

  book_num = constrain(book_num, 0, 1);
  check_book();
  
  read_book();

  //  for better working of simulator
  delay(10);
}

void check_book() {
  if (prev_book_num != book_num) {
    prev_book_num = book_num;
    if (book_num) {
      Serial.println("Think and Grow Rich");
    }
    else {
      Serial.println("Harry Potter");
    }
  }
}

void read_book(){
 if(book_state){
   if(book_num){
     file= SD.open("think.txt",FILE_READ);
   }
   else{
     file= SD.open("harry.txt",FILE_READ);
   }

   String display_text="";
   if(file){
     while(file.available()){
       char data=file.read();
       Serial.print(data);
     }
   }
 } 
 file.close();
}