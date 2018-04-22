// Baseado em trinketloader.ino
// https://learn.adafruit.com/introducing-trinket/repairing-bootloader
//
// Traduzido para português por
// Mauricio Jabur
// mau (arroba) maumaker.com.br
// 20/03/2017
//
// Carregue no Arduino e abra o terminal serial para programar
// seu chip AVR.
//
// O programa espera que você digite G (maiúsculo) no terminal
// 
// Esta versão programa o bootloader do Arduino Gemma com
// o programa BLINK em um ATTINY85
//
// TRADUÇÃO DOS COMENTÁRIOS:
// Este sketch permite que o Arduino envie um programa para
// a memória flash de qualquer AVR se for possível fazer o
// arquivo HEX caber na memória de programa
// Não é necessário um computador. Dois LEDs para notificação
// de status
// Pressione um botão para programar um novo chip.
// (esta parte parece não estar implamentada)
// Um transdutor piezo indica erro ou sucesso.
// (esta parte parece não estar implamentada)
// É ideal para programar muitos chips de forma rápida!
// Baseado no AVRISP
//
// Usa os seguintes pinos:
// 10: slave reset
// 11: MOSI
// 12: MISO
// 13: SCK
// 8: LED_ERR
// 7: LED_PROGMODE



// Standalone AVR ISP programmer
// August 2011 by Limor Fried / Ladyada / Adafruit
// Jan 2011 by Bill Westfield ("WestfW")
//
// this sketch allows an Arduino to program a flash program
// into any AVR if you can fit the HEX file into program memory
// No computer is necessary. Two LEDs for status notification
// Press button to program a new chip. Piezo beeper for error/success
// This is ideal for very fast mass-programming of chips!
//
// It is based on AVRISP
//
// using the following pins:
// 10: slave reset
// 11: MOSI
// 12: MISO
// 13: SCK


#include "optiLoader.h"
#include "SPI.h"

// Global Variables
int pmode = 0;
byte pageBuffer[128];		       /* One page of flash */


/*
   Pins to target
*/
#define SCK 13
#define MISO 12
#define MOSI 11
#define RESET 10
#define CLOCK 9     // self-generate 8mhz clock - handy
// gera simais de 8MHz - conveniente!

#define BUTTON 10
// usar o mesmo pino para o botão e para o sinal de reset?

void setup () {
  Serial.begin(9600);			/* Initialize serial for status msgs */
  // Inicializa a serial para mensagens de status
  //  Serial.println("\nTrinket loader!");
  Serial.println("\nProgramador trinket!");
}

void loop (void) {
  pinMode(BUTTON, INPUT);     // button for next programming
  // botão para a próxima programação
  digitalWrite(BUTTON, HIGH); // pullup
  delay(10);

  //  Serial.println("\nType 'G' or hit BUTTON for next chip");
  Serial.println("\ndigite 'G' ou pressione BUTTON para o proximo chip");
  while (1) {
    if  ((! digitalRead(BUTTON)) || (Serial.read() == 'G'))
      break;
  }
  while (!digitalRead(BUTTON)) {
    delay(10);
  }
  delay(100);

  target_poweron();			/* Turn on target power */

  uint16_t signature;
  image_t *targetimage;

  // descobre qual tipo de CPU
  if (! (signature = readSignature()))		// Figure out what kind of CPU
  {
    //    error("Signature fail");
    error("Falha de assinatura");
  }
  // procura uma "imagem"
  if (! (targetimage = findImage(signature)))	// look for an image
  {
    //      error("Image fail");
    error("Falha de imagem");
  }

  eraseChip();

  // prepara os fusíveis para programação
  if (! programFuses(targetimage->image_progfuses))	// get fuses ready to program
  {
    //      error("Programming Fuses fail");
    error("Falha programando fusiveis");
  }

  if (! verifyFuses(targetimage->image_progfuses, targetimage->fusemask) )
  {
    //    error("Failed to verify fuses");
    error("Falha verificando fusiveis");
  }

  end_pmode();
  start_pmode();

  byte *hextext = targetimage->image_hexcode;
  uint16_t pageaddr = 0;
  uint8_t pagesize = pgm_read_byte(&targetimage->image_pagesize);
  uint16_t chipsize = pgm_read_word(&targetimage->chipsize);

  //Serial.println(chipsize, DEC);
  while (pageaddr < chipsize) {
    byte *hextextpos = readImagePage (hextext, pageaddr, pagesize, pageBuffer);

    boolean blankpage = true;
    for (uint8_t i = 0; i < pagesize; i++) {
      if (pageBuffer[i] != 0xFF) blankpage = false;
    }
    if (! blankpage) {
      if (! flashPage(pageBuffer, pageaddr, pagesize))
      {
        //error("Flash programming failed");
        error("Falha programando flash");
      }
    }
    hextext = hextextpos;
    pageaddr += pagesize;
  }

  // ajusta fusíveis para estado 'final'
  // Set fuses to 'final' state
  if (! programFuses(targetimage->image_normfuses))
  {
    //error("Programming Fuses fail");
    error("Falha programando fusiveis");
  }

  end_pmode();
  start_pmode();

  //  Serial.println(F("\nVerifing flash..."));
  Serial.println(F("\nVerificando flash..."));
  if (! verifyImage(targetimage->image_hexcode) ) {
    //    error("Failed to verify chip");
    error("Falha ao verificar chip");
  } else {
    //    Serial.println(F("\tFlash verified correctly!"));
    Serial.println(F("\tFlash verificada corretamente!"));
  }

  if (! verifyFuses(targetimage->image_normfuses, targetimage->fusemask) ) {
    //error("Failed to verify fuses");
    error("Falha verificando fusiveis");
  } else {
    //Serial.println(F("Fuses verified correctly!"));
    Serial.println(F("Fusiveis verificados corretamente!"));
  }
  target_poweroff();			/* turn power off */
  // desliga a alimentação
  
  Serial.println("*OK!*");

  while (1);
}


void error(char *string) {
  Serial.println(string);

  while (1) {
    //tone(PIEZOPIN, 4000, 500);
  }

}

void start_pmode () {
  pinMode(13, INPUT); // restore to default

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128);

//  debug("...spi_init done");
  debug("...spi_init completo");
  // os delays a seguir podem não funcionar em todos alvos
  // following delays may not work on all targets...
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);
  pinMode(SCK, OUTPUT);
  digitalWrite(SCK, LOW);
  delay(50);
  digitalWrite(RESET, LOW);
  delay(50);
  pinMode(MISO, INPUT);
  pinMode(MOSI, OUTPUT);
  debug("...spi_transaction");
  spi_transaction(0xAC, 0x53, 0x00, 0x00);
//  debug("...Done");
  debug("...Completo");
  pmode = 1;
}

void end_pmode () {
  SPCR = 0;				/* reset SPI */
  digitalWrite(MISO, 0);		/* Make sure pullups are off too */
  pinMode(MISO, INPUT);
  digitalWrite(MOSI, 0);
  pinMode(MOSI, INPUT);
  digitalWrite(SCK, 0);
  pinMode(SCK, INPUT);
  digitalWrite(RESET, 0);
  pinMode(RESET, INPUT);
  SPI.end();      //  need to call SPI.end() before another call to SPI.begin()  pmode = 0;
}


/*
   target_poweron
   begin programming
*/
boolean target_poweron ()
{
  pinMode(LED_PROGMODE, OUTPUT);
  digitalWrite(LED_PROGMODE, HIGH);
  digitalWrite(RESET, LOW);  // reset it right away.
  pinMode(RESET, OUTPUT);
  delay(100);
//  Serial.print(F("Starting Program Mode"));
  Serial.print(F("Iniciando Modo Programacao"));
  start_pmode();
  Serial.println(" [OK]");
  return true;
}

boolean target_poweroff ()
{
  end_pmode();
  digitalWrite(LED_PROGMODE, LOW);
  return true;
}

