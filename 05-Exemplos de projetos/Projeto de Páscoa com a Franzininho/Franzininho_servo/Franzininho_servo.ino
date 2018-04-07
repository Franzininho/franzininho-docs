/*******************************************************************
 
  Required library is the Adafruit_SoftServo library
  available at https://github.com/adafruit/Adafruit_SoftServo
  The standard Arduino IDE servo library will not work with 8 bit
  AVR microcontrollers like Trinket and Gemma due to differences
  in available timer hardware and programming. We simply refresh
  by piggy-backing on the timer0 millis() counter

 
 *******************************************************************/

#include <Adafruit_SoftServo.h>  // SoftwareServo (works on non PWM pins)

#define SERVOPIN 0          
#define INTERVALO_PISCA     150  // tempo para inversão do led
#define INTERVALO_ATUALIZA  10    //tempo para atualizar o servo: minimo 5

#define MINIMO 30      //meno valor 0
#define MAXIMO 149    //maior valor 179

const int LED = 1;

Adafruit_SoftServo myServo1;  //create TWO servo objects

int servoPos;  // variable to convert voltage on pot to servo position
int inc;
long int ultimaInversao = 0;
long int ultimaatualizacao = 0;
   
void setup() {
  // Set up the interrupt that will refresh the servo for us automagically
  OCR0A = 0xAF;            // any number is OK
  TIMSK |= _BV(OCIE0A);    // Turn on the compare interrupt (below!)

  pinMode(LED, OUTPUT);
  
  myServo1.attach(SERVOPIN);   // Attach the servo to pin 0 on Trinket
  myServo1.write(90);           // Tell servo to go to position per quirk
  delay(15);                    // Wait 15ms for the servo to reach the position
 // while(1);
}

void loop()  {


  if ((millis() - ultimaatualizacao) > INTERVALO_ATUALIZA)  //verifica se já deu o tempo para stualizar o servo
  {
      if(servoPos<=MINIMO) inc = 1;
      else if(servoPos>=MAXIMO) inc = -1;
      
      servoPos+=inc;                                //incrementa posição do Servo
      
      myServo1.write(servoPos);                    // tell servo to go to position

      ultimaatualizacao = millis();
  }


  if ((millis() - ultimaInversao) > INTERVALO_PISCA) //verifica se já deu o tempo para inverter o LED
  {
      digitalWrite(LED, !digitalRead(LED));
      ultimaInversao = millis();
  }
}


// We'll take advantage of the built in millis() timer that goes off
// to keep track of time, and refresh the servo every 20 milliseconds
// The SIGNAL(TIMER0_COMPA_vect) function is the interrupt that will be
// Called by the microcontroller every 2 milliseconds
volatile uint8_t counter = 0;
SIGNAL(TIMER0_COMPA_vect) {
  // this gets called every 2 milliseconds
  counter += 2;
  // every 20 milliseconds, refresh the servos!
  if (counter >= 20) {
    counter = 0;
    myServo1.refresh();
  }
}
