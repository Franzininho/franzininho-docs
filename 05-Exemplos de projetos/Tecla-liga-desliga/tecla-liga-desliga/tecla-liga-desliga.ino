/*
  Franzininho 
  Exemplo: Tecla liga/desliga

  Esse exemplo exibe como fazer ligar e desligar uma saída através de uma tecla
    
*/


const int LED = 1; //pino para o LED
const int BOTAO = 2; //pino para o botão 

int estadoAnteriorBotao = 0;   // armazena o estado anterior do botão

void setup(){
  pinMode(LED,OUTPUT); //o LED é uma saída
  pinMode(BOTAO,INPUT); //o BOTAO é uma entrada
}
 
void loop (){
 int estadoAtualBT= digitalRead(BOTAO);      // Lê estado do botão
 delay(10); 
  
  if ((estadoAtualBT != estadoAnteriorBotao)&& (estadoAtualBT == LOW)){       //Se o botão foi pressionado e o seu estado mudou 
  digitalWrite(LED,!digitalRead(LED)); //inverte estado do LED
  }
  
  estadoAnteriorBotao = estadoAtualBT;  //salva o estado do botão para comparar na próxima leitura
}

