//версия для карантина
#include <etherShield.h>// модуль интернета
#include <ETHER_28J60.h>// модуль интернета
#include <DS3231.h>// модуль часов реального времени

static uint8_t mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};// задаем мак адрес
static uint8_t ip[4] = {192, 168, 100, 245};// задаем IP адрес
static uint16_t port = 80;// задаем порт
ETHER_28J60 ethernet;
DS3231  rtc(SDA, SCL);

const int sensor01 = 2;//   гидрапоника - датчик верхнего уровня btnUp
const int dev01 = 3;//      гидрапоника - клапан сброса воздуха по умолчанию открыт, 2.7 вольта
const int dev02 = 4;//      гидрапоника - помпа нагнетающая давление, активна совместно с клапоном сброса воздуха но не по его расписанию, 12 вольт
const int dev03 = 5;//      гидрапоника - помпа аэрации, 12 вольт
const int dev04 = 6;//      гидрапоника - 220 вольт (лампа теплого света)
const int dev05 = 7;//      зал - 220 вольт (ночник)
const int dev06 = 8;//      зал - девайс для софтбокса
const int dev07 = 9;//      зал - девайс для софтбокса
const int dev08 = 10;//     хол - обход счетчика
const int dev13 = 13;//     L светодиод  активируеться в случаи неисправности

boolean swDev01 = true; //бинарный переключатель состояния 
boolean swDev02 = true; //бинарный переключатель состояния
boolean swDev03 = true; //бинарный переключатель состояния
boolean swDev04 = false; //бинарный переключатель состояния
boolean swDev05 = false; //бинарный переключатель состояния
boolean swDev06 = false; //бинарный переключатель состояния
boolean swDev07 = false; //бинарный переключатель состояния
boolean swDev08 = false; //бинарный переключатель состояния
boolean swDev13 = false; //бинарный переключатель состояния
boolean swStream = false; //бинарный переключатель состояния отключает все несвязанное со стримом мешаюшие внешне.

boolean statSensor01 = true;//переменная статуса false-устройство не активно, true-устройство активно (в данном случае инициализируем состоянием сработки сенсора)
boolean statDev01 = false;
boolean statDev02 = false;
boolean statDev03 = false;

void setup()
{ 
  ethernet.setup(mac, ip, port);
  Serial.begin(9600);
  rtc.begin();

  pinMode(sensor01, INPUT);
  digitalWrite(sensor01, HIGH); //подача высокого уровня напряжения на вход (сенсор замыкает GND и pin на котором 5 вольт)
  pinMode(dev01, OUTPUT);
  pinMode(dev02, OUTPUT);
  pinMode(dev03, OUTPUT);
  pinMode(dev04, OUTPUT);
  pinMode(dev05, OUTPUT);
  pinMode(dev06, OUTPUT);
  pinMode(dev07, OUTPUT);
  pinMode(dev08, OUTPUT);
  pinMode(dev13, OUTPUT);
}



void loop()
{
  GetEthernet();
  fTimeDev();
  fStatSensor();
  fRunDev();
}



void GetEthernet(){
  String vol;
  if (vol = ethernet.serviceRequest())
  {

    if(vol == ""){
      ethernet.print("unixk0d_for_mega_2560_v.2.2.4");
      ethernet.respond();
    }

    if(vol == "stat"){ 
      Time RTC_T;
      RTC_T = rtc.getTime();
      int volTimeDow = RTC_T.dow;
      int volTimeHour = RTC_T.hour;
      int volTimeMin = RTC_T.min;
      int volTimeSec = RTC_T.sec;     
      ethernet.print(volTimeDow); 
      ethernet.print(";");  
      ethernet.print(volTimeHour); 
      ethernet.print(";");   
      ethernet.print(volTimeMin); 
      ethernet.print(";");   
      ethernet.print(volTimeSec);
      ethernet.print(";");  
      ethernet.print(swDev01);
      ethernet.print(";");    
      ethernet.print(swDev02);
      ethernet.print(";"); 
      ethernet.print(swDev03);
      ethernet.print(";");
      ethernet.print(swDev04);
      ethernet.print(";");
      ethernet.print(swDev05);
      ethernet.print(";");
      ethernet.print(swDev06);
      ethernet.print(";");
      ethernet.print(swDev07);
      ethernet.print(";");
      ethernet.print(swDev08);
      ethernet.print(";");
      ethernet.print(swDev13);
      ethernet.print(";");
      ethernet.print(swStream);
      ethernet.print(";");
      ethernet.print(statSensor01);
      ethernet.print(";");
      ethernet.print(statDev01);
      ethernet.print(";");
      ethernet.print(statDev02);
      ethernet.print(";");
      ethernet.print(statDev03);
      ethernet.print(";");
      ethernet.respond();
    }
    
        if(vol == "don123")
            {
                swDev01=true;
                swDev02=true;
                swDev03=true;
                ethernet.print(swDev01);
                ethernet.print(";");
                ethernet.print(swDev02);
                ethernet.print(";");
                ethernet.print(swDev03);
                ethernet.respond();
            } 
        if(vol == "doff123")
            {
                swDev01=false;
                swDev02=false;
                swDev03=false;
                ethernet.print(swDev01);
                ethernet.print(";");
                ethernet.print(swDev02);
                ethernet.print(";");
                ethernet.print(swDev03);
                ethernet.respond();
            }  

        if(vol == "don4")
            {
                swDev04=true;
                ethernet.print(swDev04);
                ethernet.print(";");
                ethernet.respond();
            } 
        if(vol == "doff4")
            {
                swDev04=false;
                ethernet.print(swDev04);
                ethernet.print(";");
                ethernet.respond();
            }  
            
        if(vol == "don5")
            {
                swDev05=true;
                ethernet.print(swDev05);
                ethernet.print(";");
                ethernet.respond();
            } 
        if(vol == "doff5")
            {
                swDev05=false;
                ethernet.print(swDev05);
                ethernet.print(";");
                ethernet.respond();
            } 
            
       if(vol == "don6")
            {
                swDev06=true;
                ethernet.print(swDev06);
                ethernet.print(";");
                ethernet.respond();
            } 
        if(vol == "doff6")
            {
                swDev06=false;
                ethernet.print(swDev06);
                ethernet.print(";");
                ethernet.respond();
            }
            
       if(vol == "don7")
            {
                swDev07=true;
                ethernet.print(swDev07);
                ethernet.print(";");
                ethernet.respond();
            } 
        if(vol == "doff7")
            {
                swDev07=false;
                ethernet.print(swDev07);
                ethernet.print(";");
                ethernet.respond();
            }
        if(vol == "don8")
            {
                swDev08=true;
                ethernet.print(swDev08);
                ethernet.print(";");
                ethernet.respond();
            } 
        if(vol == "doff8")
            {
                swDev08=false;
                ethernet.print(swDev08);
                ethernet.print(";");
                ethernet.respond();
            } 
       if(vol == "onstream")
            {
                swStream=true;
                swDev06=true;
                swDev07=true;
                ethernet.print(swStream);
                ethernet.print(";");
                ethernet.respond();
            } 
        if(vol == "offstream")
            {
                swStream=false;
                swDev06=false;
                swDev07=false;
                ethernet.print(swStream);
                ethernet.print(";");
                ethernet.respond();
            }   

        if(vol == "dev-1-2-3"){
      swDev01=!swDev01;
      swDev02=!swDev02;
      swDev03=!swDev03;
      ethernet.print(swDev01);
      ethernet.print(";");
      ethernet.print(swDev02);
      ethernet.print(";");
      ethernet.print(swDev03);
      ethernet.respond();
      delay(4000); // отсечка пакетов производяших паразитный инкремент
    }  

        if(vol == "dev-4"){
      swDev04=!swDev04;
      ethernet.print(swDev04);
      ethernet.respond();
      delay(4000); // отсечка пакетов производяших паразитный инкремент
    }    

        if(vol == "dev-5"){
      swDev05=!swDev05;
      ethernet.print(swDev05);
      ethernet.respond();
      delay(4000); // отсечка пакетов производяших паразитный инкремент
    }

        if(vol == "dev-6"){
      swDev06=!swDev06;
      ethernet.print(swDev06);
      ethernet.respond();
      delay(4000); // отсечка пакетов производяших паразитный инкремент
    }

        if(vol == "dev-7"){
      swDev07=!swDev07;
      ethernet.print(swDev07);
      ethernet.respond();
      delay(4000); // отсечка пакетов производяших паразитный инкремент
    }
    
        if(vol == "stream"){
      swStream=!swStream;
      swDev06=!swDev06;
      swDev07=!swDev07;
      ethernet.print(swStream);
      ethernet.respond();
      delay(4000); // отсечка пакетов производяших паразитный инкремент
    }

  }
}



void fTimeDev(){
  
  Time RTC_T;
  RTC_T = rtc.getTime();
  int volTimeDow = RTC_T.dow;
  int volTimeHour = RTC_T.hour;
  int volTimeMin = RTC_T.min;
  int volTimeSec = RTC_T.sec;

// Time Dev 01
  if ((volTimeHour == 11||
       volTimeHour == 13||
       volTimeHour == 16||
       volTimeHour == 19||
       volTimeHour == 21) && volTimeMin < 20)
       {
           statDev01 = true;
       } 
       
  if ((volTimeHour == 0||
       volTimeHour == 1||
       volTimeHour == 2||
       volTimeHour == 3||
       volTimeHour == 4||
       volTimeHour == 5||
       volTimeHour == 6||
       volTimeHour == 7||
       volTimeHour == 8||
       volTimeHour == 9||
       volTimeHour == 10||
       volTimeHour == 12||
       volTimeHour == 14||
       volTimeHour == 15||
       volTimeHour == 17||
       volTimeHour == 18||
       volTimeHour == 20||
       volTimeHour == 22||
       volTimeHour == 23) || volTimeMin > 20)
       {
           statDev01 = false;
       }
// Time Dev 01

// Time Dev 02
  if ((volTimeHour == 11||
       volTimeHour == 13||
       volTimeHour == 16||
       volTimeHour == 19||
       volTimeHour == 21) 
      
      && (((volTimeMin == 1||
            volTimeMin == 2||
            volTimeMin == 18) && (volTimeSec == 10|| 
                                  volTimeSec == 11|| 
                                  volTimeSec == 12|| 
                                  volTimeSec == 13|| 
                                  volTimeSec == 14|| 
                                  volTimeSec == 15|| 
                                  volTimeSec == 16|| 
                                  volTimeSec == 17|| 
                                  volTimeSec == 18|| 
                                  volTimeSec == 19||
                                  volTimeSec == 40|| 
                                  volTimeSec == 41|| 
                                  volTimeSec == 42|| 
                                  volTimeSec == 43|| 
                                  volTimeSec == 44|| 
                                  volTimeSec == 45|| 
                                  volTimeSec == 46|| 
                                  volTimeSec == 47|| 
                                  volTimeSec == 48|| 
                                  volTimeSec == 49)) 
          ||  ((volTimeMin == 3||
                volTimeMin == 6||
                volTimeMin == 7||
                volTimeMin == 8||
                volTimeMin == 9||
                volTimeMin == 15||
                volTimeMin == 16||
                volTimeMin == 17) && (volTimeSec == 10|| 
                                      volTimeSec == 11|| 
                                      volTimeSec == 12|| 
                                      volTimeSec == 13|| 
                                      volTimeSec == 14|| 
                                      volTimeSec == 15|| 
                                      volTimeSec == 16|| 
                                      volTimeSec == 17|| 
                                      volTimeSec == 18|| 
                                      volTimeSec == 19))                 
          ||  ((volTimeMin == 4||
                volTimeMin == 10||
                volTimeMin == 14) && (volTimeSec == 40|| 
                                      volTimeSec == 41|| 
                                      volTimeSec == 42|| 
                                      volTimeSec == 43|| 
                                      volTimeSec == 44|| 
                                      volTimeSec == 45|| 
                                      volTimeSec == 46|| 
                                      volTimeSec == 47|| 
                                      volTimeSec == 48|| 
                                      volTimeSec == 49))))
                                      {
                                      statDev02 = true;
                                      }                                                                                                       
  if ((volTimeHour == 0||
       volTimeHour == 1||
       volTimeHour == 2||
       volTimeHour == 3||
       volTimeHour == 4||
       volTimeHour == 5||
       volTimeHour == 6||
       volTimeHour == 7||
       volTimeHour == 8||
       volTimeHour == 9||
       volTimeHour == 10||
       volTimeHour == 12||
       volTimeHour == 14||
       volTimeHour == 15||
       volTimeHour == 17||
       volTimeHour == 18||
       volTimeHour == 20||
       volTimeHour == 22||
       volTimeHour == 23) 
      
      || volTimeMin > 18
      
      ||  ((volTimeMin == 1||
            volTimeMin == 2||
            volTimeMin == 18) && (volTimeSec < 10|| 
                                 volTimeSec == 20||
                                 volTimeSec == 21||
                                 volTimeSec == 22||
                                 volTimeSec == 23||
                                 volTimeSec == 24||
                                 volTimeSec == 25||
                                 volTimeSec == 26||
                                 volTimeSec == 27||
                                 volTimeSec == 28||
                                 volTimeSec == 29||
                                 volTimeSec == 30||
                                 volTimeSec == 31||
                                 volTimeSec == 32||
                                 volTimeSec == 33||
                                 volTimeSec == 34||
                                 volTimeSec == 35||
                                 volTimeSec == 36||
                                 volTimeSec == 37||
                                 volTimeSec == 38||                               
                                 volTimeSec == 39|| 
                                  volTimeSec > 49)) 
          ||  ((volTimeMin == 3||
                volTimeMin == 6||
                volTimeMin == 7||
                volTimeMin == 8||
                volTimeMin == 9||
                volTimeMin == 15||
                volTimeMin == 16||
                volTimeMin == 17) && (volTimeSec < 10||  
                                      volTimeSec > 19))                 
          ||  ((volTimeMin == 4||
                volTimeMin == 10||
                volTimeMin == 14) && (volTimeSec < 40|| 
                                      volTimeSec > 49)))
                                      {
                                      statDev02 = false;
                                      }
// Time Dev 02

// Time Dev 03
    if ((volTimeHour == 11
       ||volTimeHour == 13
       ||volTimeHour == 16
       ||volTimeHour == 19
       ||volTimeHour == 21)&&(volTimeMin == 10
                            ||volTimeMin == 12
                            ||volTimeMin == 14
                            ||volTimeMin == 16
                            ||volTimeMin == 18))
                            {
                              statDev03 = true;
                            }
    
    if ((volTimeHour == 0||
         volTimeHour == 1||
         volTimeHour == 2||
         volTimeHour == 3||
         volTimeHour == 4||
         volTimeHour == 5||
         volTimeHour == 6||
         volTimeHour == 7||
         volTimeHour == 8||
         volTimeHour == 9||
         volTimeHour == 10||
         volTimeHour == 12||
         volTimeHour == 14||
         volTimeHour == 15||
         volTimeHour == 17||
         volTimeHour == 18||
         volTimeHour == 20||
         volTimeHour == 22||
         volTimeHour == 23)||(volTimeMin <10)||(volTimeMin == 11
                                              ||volTimeMin == 13
                                              ||volTimeMin == 15
                                              ||volTimeMin == 17)||(volTimeMin >18))
                                              {
                                                 statDev03 = false;
                                              }
// Time Dev 03
  
// Time Dev 04
//  && volTimeMin == 1 && volTimeSec == 1 в конце необходимо для отключения в первую минуту, иначе первая минута вся будет активна,
// ,также для устронения постоянной переинициализации переменной времени.
  if ((volTimeDow == 1
     ||volTimeDow == 2
     ||volTimeDow == 3
     ||volTimeDow == 4
     ||volTimeDow == 5)&&(//(volTimeHour == 8 && volTimeMin == 30 && volTimeSec == 1)||
                                                                                     ((//volTimeHour == 9
                                                                                     //||volTimeHour == 10
                                                                                     //||volTimeHour == 11||
                                                                                       volTimeHour == 15
                                                                                     ||volTimeHour == 16
                                                                                     ||volTimeHour == 17
                                                                                     ||volTimeHour == 18
                                                                                     ||volTimeHour == 19) && volTimeMin == 1 && volTimeSec == 1)))
                                                                                     {
                                                                                        swDev04  = true;
                                                                                     }
  if ((volTimeDow == 1
     ||volTimeDow == 2
     ||volTimeDow == 3
     ||volTimeDow == 4
     ||volTimeDow == 5)&&((//volTimeHour == 0||
                           volTimeHour == 1
                         ||volTimeHour == 2
                         ||volTimeHour == 3
                         ||volTimeHour == 4
                         ||volTimeHour == 5
                         ||volTimeHour == 6
                         ||volTimeHour == 7
                         ||volTimeHour == 12
                         ||volTimeHour == 13
                         ||volTimeHour == 14
                         ||volTimeHour == 20
                         ||volTimeHour == 21
                         //||volTimeHour == 22
                         //||volTimeHour == 23
                                            ) && volTimeMin == 1 && volTimeSec == 1))
                         {
                            swDev04  = false;
                         }
  if ((volTimeDow == 6
     ||volTimeDow == 7)&&((volTimeHour == 15
                         ||volTimeHour == 16
                         ||volTimeHour == 17
                         ||volTimeHour == 18
                         ||volTimeHour == 19) && volTimeMin == 1 && volTimeSec == 1))
                         {
                            swDev04  = true;
                         }
  if ((volTimeDow == 6
     ||volTimeDow == 7)&&((//volTimeHour == 0
                         //||volTimeHour == 1
                         //||volTimeHour == 2||
                           volTimeHour == 3
                         ||volTimeHour == 4
                         ||volTimeHour == 5
                         ||volTimeHour == 6
                         ||volTimeHour == 7
                         ||volTimeHour == 8
                         ||volTimeHour == 9
                         ||volTimeHour == 10
                         ||volTimeHour == 11
                         ||volTimeHour == 12
                         ||volTimeHour == 13
                         ||volTimeHour == 14
                         ||volTimeHour == 20
                         ||volTimeHour == 21
                         //||volTimeHour == 22
                         //||volTimeHour == 23
                                            ) && volTimeMin == 1 && volTimeSec == 1))
                         {
                            swDev04  = false;
                         }    
// Time Dev 04

// Time Dev 05
//  && volTimeMin == 1 && volTimeSec == 1 в конце необходимо для отключения в первую минуту, иначе первая минута вся будет активна,
// ,также для устронения постоянной переинициализации переменной времени.
  if ((volTimeDow == 1
     ||volTimeDow == 2
     ||volTimeDow == 3
     ||volTimeDow == 4
     ||volTimeDow == 5)&&(//(volTimeHour == 7 && volTimeMin == 30 && volTimeSec == 1)||
                                                                                     ((//volTimeHour == 8||
                                                                                       volTimeHour == 9
                                                                                     ||volTimeHour == 20
                                                                                     ||volTimeHour == 21
                                                                                     ||volTimeHour == 22
                                                                                     ||volTimeHour == 23) && volTimeMin == 1 && volTimeSec == 1)))
                                                                                     {
                                                                                        swDev05  = true;
                                                                                     }
  if ((volTimeDow == 1
     ||volTimeDow == 2
     ||volTimeDow == 3
     ||volTimeDow == 4
     ||volTimeDow == 5)&&((//volTimeHour == 0||
                           volTimeHour == 1
                         ||volTimeHour == 2
                         ||volTimeHour == 3
                         ||volTimeHour == 4
                         ||volTimeHour == 5
                         ||volTimeHour == 6
                         ||volTimeHour == 7
                         ||volTimeHour == 8
                         ||volTimeHour == 10
                         ||volTimeHour == 11
                         ||volTimeHour == 12
                         ||volTimeHour == 13    
                         ||volTimeHour == 14
                         ||volTimeHour == 15
                         ||volTimeHour == 16
                         ||volTimeHour == 17
                         ||volTimeHour == 18
                         ||volTimeHour == 19) && volTimeMin == 1 && volTimeSec == 1))
                         {
                            swDev05  = false;
                         }
  if ((volTimeDow == 6
     ||volTimeDow == 7)&&((volTimeHour == 20
                         ||volTimeHour == 21
                         ||volTimeHour == 22
                         ||volTimeHour == 23) && volTimeMin == 1 && volTimeSec == 1))
                         {
                            swDev05  = true;
                         }
  if ((volTimeDow == 6
     ||volTimeDow == 7)&&((//volTimeHour == 0
                         //||volTimeHour == 1||
                           volTimeHour == 2
                         ||volTimeHour == 3
                         ||volTimeHour == 4
                         ||volTimeHour == 5
                         ||volTimeHour == 6
                         ||volTimeHour == 7
                         ||volTimeHour == 8
                         ||volTimeHour == 9
                         ||volTimeHour == 10
                         ||volTimeHour == 11
                         ||volTimeHour == 12
                         ||volTimeHour == 13
                         ||volTimeHour == 14
                         ||volTimeHour == 15
                         ||volTimeHour == 16
                         ||volTimeHour == 17
                         ||volTimeHour == 18
                         ||volTimeHour == 19) && volTimeMin == 1 && volTimeSec == 1))
                         {
                            swDev05  = false;
                         }    
// Time Dev 05

// Time reset sw 1-2-3
//  && volTimeMin == 1 && volTimeSec == 1 в конце необходимо для отключения в первую минуту, иначе первая минута вся будет активна,
// ,также для устронения постоянной переинициализации переменной времени.
  if ((volTimeDow == 1
     ||volTimeDow == 2
     ||volTimeDow == 3
     ||volTimeDow == 4
     ||volTimeDow == 5)&&((//volTimeHour == 10
                         //||volTimeHour == 12
                         //||volTimeHour == 15
                         //||volTimeHour == 18||
                             volTimeHour == 20) && volTimeMin == 59 && volTimeSec == 59))
                         {
                           swDev01  = true;
                           swDev02  = true;
                           swDev03  = true;
                         }
      
  if ((volTimeDow == 5
     ||volTimeDow == 6)&&((volTimeHour == 23) && volTimeMin == 59 && volTimeSec == 59))//подготавлеваем гидропонику для выходных дней
                         {
                           swDev01  = false;
                           swDev02  = false;
                           swDev03  = false;
                         } 
                   
  if ((volTimeDow == 6
     ||volTimeDow == 7)&&((volTimeHour == 15
                         ||volTimeHour == 18
                         ||volTimeHour == 20) && volTimeMin == 59 && volTimeSec == 59))
                         {
                           swDev01  = true;
                           swDev02  = true;
                           swDev03  = true;
                         }   
// Time reset sw 1-2-3

// Time Dev 13
  if (volTimeSec % 2)
  {
    swDev13 = true;
  }
  else
  {
    swDev13 = false;
  }
// Time Dev 13

}



void fStatSensor(){
  
// sensor 01
  if ( digitalRead(sensor01) == HIGH)
  {
    statSensor01 = true;//если на пине появился высркий уровень, то  датчик ушел в обрыв, и значит ПЫЗДЕЦ
  }
  if ( digitalRead(sensor01) == LOW)
  {
    statSensor01 = false;//если на пине низкий уровень то ситуация штатная и все ОК
  }
// sensor 01

}



void fRunDev(){
  
// Run Dev 01
      if (statDev01 == true && statSensor01 == false && swDev01 == true && swStream == false)
    {
    digitalWrite(dev01, LOW);
//    Serial.print("  DEV02 ON.  ");
    }
      if (statDev01 == false || statSensor01 == true || swDev01 == false || swStream == true)
    {
    digitalWrite(dev01, HIGH);
//    Serial.print("  DEV02 OFF.  ");
    }  
// Run Dev 01

// Run Dev 02
      if (statDev02 == true && statSensor01 == false && swDev02 == true && swStream == false)
    {
    digitalWrite(dev02, LOW);
//    Serial.print("  DEV02 ON.  ");
    }
      if (statDev02 == false || statSensor01 == true || swDev02 == false || swStream == true)
    {
    digitalWrite(dev02, HIGH);
//    Serial.print("  DEV02 OFF.  ");
    }  
// Run Dev 02

// Run Dev 03
      if (statDev03 == true && swDev03 == true && swStream == false)
    {
    digitalWrite(dev03, LOW);
//    Serial.print("  DEV03 ON.  ");
    }
      if (statDev03 == false || swDev03 == false || swStream == true)
    {
    digitalWrite(dev03, HIGH);
//    Serial.print("  DEV03 OFF.  ");
    }  
// Run Dev 03

// Run Dev 04
      if (swDev04 == true && swStream == false)
    {
    digitalWrite(dev04, LOW);
//    Serial.print("  DEV04 ON.  ");
    }
      if (swDev04 == false || swStream == true)
    {
    digitalWrite(dev04, HIGH);
//    Serial.print("  DEV04 OFF.  ");
    }  
// Run Dev 04

// Run Dev 05
      if (swDev05 == true && swStream == false)
    {
    digitalWrite(dev05, LOW);
//    Serial.print("  DEV05 ON.  ");
    }
      if (swDev05 == false || swStream == true)
    {
    digitalWrite(dev05, HIGH);
//    Serial.print("  DEV05 OFF.  ");
    }  
// Run Dev 05

// Run Dev 06
      if (swDev06 == true)
    {
    digitalWrite(dev06, LOW);
//    Serial.print("  DEV06 ON.  ");
    }
      else
    {
    digitalWrite(dev06, HIGH);
//    Serial.print("  DEV06 OFF.  ");
    }  
// Run Dev 06

// Run Dev 07
      if (swDev07 == true)
    {
    digitalWrite(dev07, LOW);
//    Serial.print("  DEV07 ON.  ");
    }
      else
    {
    digitalWrite(dev07, HIGH);
//    Serial.print("  DEV07 OFF.  ");
    }  
// Run Dev 07

// Run Dev 08
      if (swDev08 == true)
    {
    digitalWrite(dev08, LOW);
//    Serial.print("  DEV08 ON.  ");
    }
      else
    {
    digitalWrite(dev08, HIGH);
//    Serial.print("  DEV08 OFF.  ");
    }  
// Run Dev 08

// Run Dev 13 (ALARM sensor1)
      if (swDev13 == true && statSensor01 == true)
    {
    digitalWrite(dev13, HIGH);
    }
      else
    {
    digitalWrite(dev13, LOW);
    }  
// Run Dev 13 (ALARM sensor1)

}
