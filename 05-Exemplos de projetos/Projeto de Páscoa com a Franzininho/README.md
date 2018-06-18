# Projeto de Páscoa 

O propósito desse projeto é de estimular a criatividade das crianças através da pintura, recorte, colagem,montagem e
animação com programação.

O  cenário é de páscoa, onde temos um coelho movimentado sua cabeça e olhos de piscantes, o coelho é feito de papel sulfite e para que fique firme foi usado papel de caixa de bombom, os olhos foram colocados 2 leds, e para que a cabeça faça o movimento foi usado um servo motor, para isso foi usado jumpers e a placa Franzininho. Logo temos mais informações:

A seguir vamos apresentar uma aplicação que você pode fazer com o seu filho ou alunos.

# Atividade será dividida em duas etapas:

- Pintura do coelho,  recorte e colagem.
- Programação para a aminação.


# Insumos de papelaria:
- Lápis de cor/Giz/Canetinha;
- Tesoura sem ponta;
- Papelão;
- Cola líquida;
- Cola quente;
- Palito de sorvete/churrasco;

# Insumos eletrônico:
- 1 Franzininho;
- 1 servo motor;
- 2 leds vermelhos;
- Jumpers;

Após separar os itens a serem usados para a montagem do coelho, vamos dar início a primeira etapa, seja a seguir:

# Primeira etapa: Recorte e pintura do coelho


Passo 1 -  Deve imprimir os desenhos em uma folha A4.(Está Aqui)

Passo 2 - Com o cenário em mãos, recorte o coelho separados por cabeça e corpo.

Passo 3 -  Agora recorte os ovos e as cenouras.

Passo 4 - Após isso, deve-se colar os desenhos em um papel mais firme, pode ser ter usado caixa de bombom como eu fiz ou então caixa de papelão.

Passo 5 - Agora você deve colar a cabeça do coelho no servo motor com cola quente.

Passo 6 - Com a cola quente você deve colar o corpo do coelho no palito de churrasco.


# Segunda etapada: Ligação do circuito 

Passo 1: A ligação deve ser feita no seguintes fios.

Fio preto do servo motor no GND dos pinos da Franzininho

Fio Marrom do servo motor no VCC dos pinos da Franzininho
Fio Vermelho

# Circuito

![circuito-pascoa_bb.png](https://github.com/Franzininho/franzininho-docs/blob/master/05-Exemplos%20de%20projetos/Projeto%20de%20P%C3%A1scoa%20com%20a%20Franzininho/circuito-pascoa_bb.png)

# Sketch

```c++
/*

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


Veja o infográfico abaixo:

![infografico-pascoa-A4.png](https://github.com/Franzininho/franzininho-docs/blob/master/05-Exemplos%20de%20projetos/Projeto%20de%20P%C3%A1scoa%20com%20a%20Franzininho/infografico-pascoa-A4.png)


