#include "DigiKeyboard.h"         //biblioteca da digispark para teclado

const int btPin = 2;          //pino que a tecla esta ligada

int estadoAnteriorBotao = 0;   // armazena o estado anterior do botão


void setup() {
  pinMode(btPin,INPUT_PULLUP);    //configura o pino do botão como entrada com pullup habilitado
}


void loop() { 
   
  int estadoAtualBT= digitalRead(btPin);      // Lê estado do botão
  
  
  if ((estadoAtualBT != estadoAnteriorBotao)&& (estadoAtualBT == LOW)){       //Se o botão foi pressionado e o seu estado mudou 

  //Envia comando da tecla espaço para o computador

  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.println("  ");
        
  }
  
  estadoAnteriorBotao = estadoAtualBT;  //salva o estado do botão para comparar na próxima leitura
  
}
