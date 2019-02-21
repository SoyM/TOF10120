#include <SoftwareSerial.h>

String rx_a = "";
String rx_b = "";
bool check_a = false;
bool check_b = false;

char rx_char;
uint8_t double_m = 0;

uint16_t last_a = 0;
uint16_t last_b = 0;
bool newLine_a = false;
bool newLine_b = false;
uint8_t errCheck_a = 0;
uint8_t errCheck_b = 0;

// software serial #1: RX = digital pin 10, TX = digital pin 11
SoftwareSerial portOne(10, 11);

// software serial #2: RX = digital pin 8, TX = digital pin 9
SoftwareSerial portTwo(8, 9);


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }


  // Start each software serial port
  portOne.begin(9600);
  portTwo.begin(9600);
  Serial.println("start..");
  delay(22);
  portOne.write("s2-20#");
  portTwo.write("s2-20#");
}

void loop() {
  rx_a = "";
  rx_b = "";
  
  portOne.listen();
  delay(19);
  
  while(1){
    if(portOne.available() > 0){
      rx_char = portOne.read();
      if(rx_char == 10){
        newLine_a = true;
        continue;
      }
      if(newLine_a == true){
        if(rx_char == 'm'){
          double_m = double_m + 1;  
        }
        else{
          rx_a.concat(rx_char);
        }
        if(double_m == 2){
          newLine_a = false;
          check_a = true;
          break;
        }
      }else{
        errCheck_a++;
      }
      
    }else{
      if( (errCheck_a++) > 100){
        break;  
      }
    }
  }
  errCheck_a = 0;
  
  if(check_a==false){
    rx_a="";
  }else{
    if( (rx_a.toInt()-last_a>50) || (rx_a.toInt()-last_a<-50) ){
      last_a = rx_a.toInt();
      rx_a="";     
    }else{
      last_a = rx_a.toInt();
    }
  }
  
  check_a = false;
  double_m = 0;
  

/////////////////////////////////////////
// part B
//////////////////////////////////////////

  
//  Serial.println("Read B");/
  
  portTwo.listen();
  delay(19);
  while (portTwo.available() > 0) {
    rx_char = portTwo.read();

    if(rx_char == 'm'){
      double_m = double_m + 1;  
    }
    else if(rx_char==13 || rx_char==10){
      
    }
    else{
      rx_b.concat(rx_char);
    }
    if(double_m == 2){
        check_b = true;
        break;
    }
  }
  if(check_b==false){
    rx_b="";
  }else{
    if( (rx_b.toInt()-last_b>50) || (rx_b.toInt()-last_b<-50) ){
      last_b = rx_b.toInt();
      rx_b = "";     
    }else{
      last_b = rx_b.toInt();
    }
  }
  
  check_b = false;
  double_m = 0;
  
  if(rx_a!="" || rx_b != ""){
    Serial.print("A: ");
    Serial.print(rx_a);
    Serial.print("  ");
    Serial.print("B: ");
    Serial.println(rx_b);
  }
}
