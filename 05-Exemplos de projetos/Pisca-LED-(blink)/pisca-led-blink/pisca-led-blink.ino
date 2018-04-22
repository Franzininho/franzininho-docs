/*
  Franzininho
  Exemplo: Pisca LED - Blink

  Esse exemplo exibe como piscar o LED presente na placa ou um LED externo ligado ao pino 1 da Franzininho em intervalos de 1 segundo.
    
*/

const int LED = 1; //pino digital conectado ao LED

void setup(){
  pinMode(LED,OUTPUT); //pino digital de saída
}

void loop(){
  digitalWrite(LED,HIGH);   //acende o led da placa
  delay(1000);             //aguarda um segundo
  digitalWrite(LED,LOW);  //apaga o led da placa
  delay(1000);           //aguarda um segundo
}
