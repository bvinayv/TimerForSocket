#include <Time.h>
#include <TimeLib.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 13, d5 = 10, d6 = 9, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int clockH = 12, clockM = 12, clockS = 00;
int clockDay = 1, clockMonth = 1, clockYear = 2020;
int hrs=0, mins=0, hrRef=0, minRef=0;

char load;// test commands instead of the push buttons.
int stateValue = 0;// states while setting time
int flg = 0; // this is for the main menu change 
char timerStatus[3] ="OFF";
int nval, val; // reading the pot for setting the values

int plug = 4; // for the digital pin to plug
int joyX = A0; // for adjusting the values
int buttonRed = 5;//for funtionality change 
int buttonBlack = 6;// for setting different values

// the functions used in the module
void setClockTime();
void setOffMins();
void setOffTime();
void setOnMins();
void setOnTime();
void switchOffIn();
void switchOffAt(int hrs, int mins);
void switchOnIN(int mins);
void switchOnAt(int hrs, int mins);
void ClockLEDDisplay();


void setup() {
  Serial.begin(9600);
  pinMode(plug,OUTPUT);
  pinMode(joyX,INPUT);
  pinMode(buttonRed,INPUT_PULLUP);
  pinMode(buttonBlack,INPUT_PULLUP); 
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Set Clock First"); // Print a message to the LCD.
  lcd.setCursor(0,1);
  lcd.print("          T :OFF");
  delay(1000);
}

void loop() {
  // reading the joyStick
  val = analogRead(joyX);
  nval = map( val , 0 , 1023 , 0 , 180 );
  //Serial.print("X Pot : ");
  //Serial.println(nval);
 
  if(!digitalRead(buttonRed)){
    if(flg < 5){
      flg = flg + 1;
      stateValue = 0;
    }
    else flg = 0;
  }
  // serial display when connected to the PC
  //digitalClockDisplay();
  if(!digitalRead(buttonBlack)){
    int noParameters;
    switch(flg){
    case 0: noParameters = 3+1;
            break;
    case 1: noParameters = 2+1;
            break;
    case 2: noParameters = 3+1;
            break;
    case 3: noParameters = 2+1;
            break;
    case 4: noParameters = 3+1;
            break;
    default : flg = flg;
    }
    if(stateValue < noParameters)
      stateValue = stateValue + 1;
    else stateValue = 0;
  }
  switch(flg){
    case 0: setClockTime();
            break;
    case 1: setOffMins();
            break;
    case 2: setOffTime();
            break;
    case 3: setOnMins();
            break;
    case 4: setOnTime();
            break;
    default : flg = flg;
  }
  delay(500);
}

void setClockTime(){
  int change = 0;
  if(stateValue == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time: ");
    ClockLEDDisplay();
    lcd.setCursor(0,1);
    lcd.print("          T :");
    lcd.print(timerStatus);
  }
  else if(stateValue == 1){// setting hours
    lcd.clear();
    lcd.setCursor(0,0);    
    lcd.print(" Hr   Min   Sec ");
    lcd.setCursor(1,1);
    if(clockH < 10)
    lcd.print('0');
    lcd.print(clockH);
    if((nval>100)&(clockH<24)){
      clockH = clockH + 1;
      change = 1;}
    else if((nval<70)&(clockH>0)){
      clockH = clockH - 1;
      change = 1;}
    }
  else if(stateValue == 2){// setting minutes
    lcd.setCursor(7,1);
    if(clockM < 10)
    lcd.print('0');
    lcd.print(clockM);
    if((nval>100)&(clockM<60)){
      clockM = clockM + 1;
      change = 1;}
    else if((nval<70)&(clockM>0)){
      clockM = clockM - 1;
      change = 1;}
    }
  else if(stateValue == 3){// setting seconds
    lcd.setCursor(13,1);
    if(clockS < 10)
    lcd.print('0');
    lcd.print(clockS);
    if((nval>100)&(clockS<60)){
      clockS = clockS + 1;
      change = 1;}
    else if((nval<70)&(clockS>0)){
      clockS = clockS - 1;
      change = 1;}
    }
  if(change==1){
    //Serial.print("Change : ");
    //Serial.print(change);
    setTime(clockH,clockM,clockS,clockDay,clockMonth,clockYear);
    change = 0;
    } 
}

void setOffMins(){
  if(stateValue == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Plug OFF in  *");
  }
  else if(stateValue == 1){// setting hours
    lcd.setCursor(13,0);
    lcd.print(" *");
    if((nval>100)&(mins<160)){
      mins = mins + 5;}
    else if((nval<70)&(mins>0)){
      mins = mins - 5;}
    }
  else if(stateValue == 2){// setting minutes
    lcd.setCursor(13,0);
    lcd.print("  *");
    lcd.setCursor(13,1);
    lcd.print(timerStatus);
    if((nval>100)){
      timerStatus[0] = 'O';
      timerStatus[1] = 'N';
      timerStatus[2] = ' ';
      digitalWrite(plug , HIGH );
      minRef = minute();
      hrRef = 0;
      }
    else if((nval<70)){
      timerStatus[0] = 'O';
      timerStatus[1] = 'F';
      timerStatus[2] = 'F';
      digitalWrite(plug , LOW );
      }
    }
  lcd.setCursor(0,1);
  if(clockH < 90)
    lcd.print('0');
  if(clockH < 10)
   lcd.print('0');    
  lcd.print(mins);
  lcd.setCursor(3,1);
  lcd.print("mins   T1:");
  lcd.print(timerStatus);
  Serial.print(mins);
  Serial.println("  going in");
  switchOffIn();
}

void setOffTime(){
  if(stateValue == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Plug OFF at Hr:M");
    lcd.setCursor(0,1);
    lcd.print(" __:__    T :");
    lcd.print(timerStatus);
  }
  else if(stateValue == 1){// setting hours
    lcd.setCursor(1,1);
    lcd.print(hrs);
    if((nval>100)&(hrs<12)){
      hrs = hrs + 1;}
    else if((nval<70)&(hrs>0)){
      hrs = hrs - 1;}
    }
  else if(stateValue == 2){// setting hours
    lcd.setCursor(4,1);
    lcd.print(mins);
    if((nval>100)&(mins<60)){
      mins = mins + 5;}
    else if((nval<70)&(mins>0)){
      mins = mins - 5;}
    }
  else if(stateValue == 3){// setting minutes
    lcd.setCursor(13,1);
    lcd.print(timerStatus);
    if((nval>100)){
      timerStatus[0] = 'O';
      timerStatus[1] = 'N';
      timerStatus[2] = ' ';}
    else if((nval<70)){
      timerStatus[0] = 'O';
      timerStatus[1] = 'F';
      timerStatus[2] = 'F';}
    }
}

void setOnMins(){
  if(stateValue == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Plug ON  in ");
    lcd.setCursor(0,1);
    lcd.print("___mins   T :");
    lcd.print(timerStatus);
  }
  else if(stateValue == 1){// setting hours
    lcd.setCursor(0,1);
    lcd.print(mins);
    if((nval>100)&(mins<120)){
      mins = mins + 10;}
    else if((nval<70)&(mins>0)){
      mins = mins - 10;}
    }
  else if(stateValue == 2){// setting minutes
    lcd.setCursor(13,1);
    lcd.print(timerStatus);
    if((nval>100)){
      timerStatus[0] = 'O';
      timerStatus[1] = 'N';
      timerStatus[2] = ' ';}
    else if((nval<70)){
      timerStatus[0] = 'O';
      timerStatus[1] = 'F';
      timerStatus[2] = 'F';}
    }
}

void setOnTime(){
  if(stateValue == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Plug ON  at Hr:M");
    lcd.setCursor(0,1);
    lcd.print(" __:__    T :");
    lcd.print(timerStatus);
  }
  else if(stateValue == 1){// setting hours
    lcd.setCursor(1,1);
    lcd.print(hrs);
    if((nval>100)&(hrs<12)){
      hrs = hrs + 1;}
    else if((nval<70)&(hrs>0)){
      hrs = hrs - 1;}
    }
  else if(stateValue == 2){// setting hours
    lcd.setCursor(4,1);
    lcd.print(mins);
    if((nval>100)&(mins<60)){
      mins = mins + 5;}
    else if((nval<70)&(mins>0)){
      mins = mins - 5;}
    }
  else if(stateValue == 3){// setting minutes
    lcd.setCursor(13,1);
    lcd.print(timerStatus);
    if((nval>100)){
      timerStatus[0] = 'O';
      timerStatus[1] = 'N';
      timerStatus[2] = ' ';}
    else if((nval<70)){
      timerStatus[0] = 'O';
      timerStatus[1] = 'F';
      timerStatus[2] = 'F';}
    }
}

void switchOffIn(/*int mins*/){
  int minElased = 0;
//function makes the plug work for that many minutes( countDown )
  Serial.print(minRef);
  Serial.print(" : ");
  Serial.println(minute());
  if((minRef<59)&&(minRef==minute())){
    minRef = minRef + 1;
    minElased = 1;
    }
  else if((minute()==0)&&(minRef==59)){
    minRef = 0;
    hrRef = hrRef + 1;
    minElased = 1;}
  if(minElased == 1 && mins >0){
    mins = mins - 1;
    Serial.print("min :");
    Serial.print(mins);
    minElased = 0;}
  if(mins <= 0){
    digitalWrite(plug , LOW );
    minElased = 0;
    timerStatus = "OFF";
  }
  /*
  int hrRef = clockH + mins/60;
  minRef = clockM + mins%60;
  if(minRef>=60){
  hrRef +=1;
  minRef = minRef%60;
  */
}

void switchOffAt(int hrs, int mins){
//function makes the plug work till that specific time in the day
  digitalWrite(plug , HIGH ); 
  if((clockH-hrs == 0)&(clockM-mins == 0)){
    digitalWrite(plug , LOW ); 
    }
  }

void switchOnIN(int min){
  int hr = clockH + mins/60;
  mins = clockM + mins%60;
  if(mins>=60){
    hr +=1;
    mins = mins%60;
  }
  if(hr>=24) hr = hr%24;

  switchOnAt(hr, mins);
}

void switchOnAt(int hrs, int mins){
//function makes the plug ON at that specific time in the day
  digitalWrite(plug , LOW ); 
  if((clockH-hrs == 0)&(clockM-mins == 0)){
    digitalWrite(plug , HIGH ); 
    }
  }

void ClockLEDDisplay(){
lcd.setCursor(5,0);
lcd.print(hour());
ShowDigitsLED(minute());
ShowDigitsLED(second());
}

void ShowDigitsLED(int digits){
lcd.print(":");
if(digits < 10)
lcd.print('0');
lcd.print(digits);
}

//Used only for Serial Printing on the monitor
void digitalClockDisplay(){
// digital clock display of the time
Serial.print(hour());
printDigits(minute());
printDigits(second());
Serial.print(" ");
Serial.print(day());
Serial.print(" ");
Serial.print(month());
Serial.print(" ");
Serial.print(year());
Serial.println();
}

void printDigits(int digits){
// utility function for clock display: prints preceding colon and leading 0
Serial.print(":");
if(digits < 10)
Serial.print('0');
Serial.print(digits);
}



/* the remaining parts :-
  * add the method of taking the hrs and mins.
  * add display and set the UI.
  * function of the buttns should be designed   1. for the mode and 2. selection
    logic yet to be designed.
  * still not very clear with the outline of the project
 */

