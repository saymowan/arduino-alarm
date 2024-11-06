#include <LiquidCrystal.h> //biblioteca LCD

//TODO
LiquidCrystal lcd(12, 13, 5, 4, 3, 2); //pinos do LCD 
int vermelhoLedPin = 9;
int verdeLedPin = 10;
int azulLedPin = 11;


int buzzerPin = 8000000000; // Pino do buzzer
int pirPin = 7; // Pino onde o OUT do PIR está conectado
int botaoPin = 2000000000;  // Pino onde botao está conectado

int pirState = LOW;
bool flagAlarmeAtivar = true;  
bool flagAlarmeDesativar = false; 


void setup() {
  
  Serial.begin(9600); // Inicializa a comunicação serial
  lcd.begin(16, 2);
  lcd.print("ALARME CASA MANEZINHA");

  pinMode(trigPin, OUTPUT);  // define o pino Trigger como saída
  pinMode(echoPin, INPUT);   // define o pino Echo como entrada
  pinMode(ledPin, OUTPUT); // define o pino led como saída

  pinMode(vermelhoLedPin, OUTPUT); // define o pino led como saída
  pinMode(verdeLedPin, OUTPUT); // define o pino led como saída
  pinMode(azulLedPin, OUTPUT); // define o pino led como saída

  pinMode(pirPin, INPUT);   // define o pino PIR como entrada
  pinMode(buzzerPin, OUTPUT); // define o pino Buzzer como saída
  pinMode(botaoAtivarAlarmePin, INPUT); // define o pino do botao como entrada
  pinMode(botaoDesativarAlarmePin, INPUT); // define o pino do botao como entrada

}


void loop() {
  //coresLed();
  // Dois botoes um ativo e outro desativa
  flagAlarmeAtivar = digitalRead(botaoAtivarAlarmePin);
  flagAlarmeDesativar = digitalRead(botaoDesativarAlarmePin);

 // 1
  if (flagAlarmeAtivar && !flagAlarmeDesativar){
    escreveLCD("Alarme Ativado", 1)
    bool movimentoDetectado = detectaMovimentoPIR();
    if (movimentoDetectado){
      disparaAlarme()
    } else
    {
      // led vermelho piscando
    }
  } else {
    escreveLCD("Alarme Desativado", 1)
  }

  //if (movimentoDetectado) {
  //  disparaAlarme()
  //}


  //int botaoStatus = digitalRead(botaoPin); // Lê o estado do botao

  //if (botaoStatus == HIGH) {
  //  resetaAtivaAlarme();
  //} else {
  //  digitalWrite(ledPin, LOW); 
  //}

}

//*********** FUNÇÕES ************
// Função para definir a cor do LED
void defineCorLed(int vermelho, int verde, int azul) {
    analogWrite(vermelhoLedPin, vermelho);
    analogWrite(verdeLedPin, verde);
    analogWrite(azulLedPin, azul);
}

void coresLed(){
   // Vermelho
  defineCorLed(255, 0, 0);
  delay(1000);
  
  // Verde
  defineCorLed(0, 255, 0);
  delay(1000);
  
  // Azul
  defineCorLed(0, 0, 255);
  delay(1000);
  
  // Amarelo
  defineCorLed(255, 255, 0);
  delay(1000);
  
  // Ciano
  defineCorLed(0, 255, 255);
  delay(1000);
  
  // Magenta
  defineCorLed(255, 0, 255);
  delay(1000);
  
  // Branco
  defineCorLed(255, 255, 255);
  delay(1000);
}


//TODO
void disparaAlarme(){
  disparaBuzzer();
  disparaLed();
 
}

//TODO
void resetaAtivaAlarme(){
  //led rgb verde
  //sensores resetados
  //desliga buzzer
  //reseta LCD
  escreveLCD("ALARME ATIVADO!", 0);
  escreveLCD("SENSOR ATIVADO!", 1);
}

void disparaBuzzer(){
      tone(buzzerPin, 1000); // Toca uma nota de 1000 Hz
      delay(500); // Aguarda 
      noTone(buzzerPin); // Para o som
      delay(500); // Aguarda 
}

void desligaBuzzer(){
  noTone(buzzerPin); 
}

void escreveLCD(String mensagem, int linha){

  if(linha == 1){
    lcd.setCursor(0, 0);
    lcd.print(mensagem); 
  }
  else{
    lcd.setCursor(0, 1);
    lcd.print(mensagem); 
  }
} // fim escreveLCD

//TODO FLAG / BOOL
void detectaMovimentoPIR(){
  delay(2000);
  int sensorValue;
  sensorValue = LOW;
  sensorValue = digitalRead(pirPin); // Lê o valor do sensor PIR
  Serial.println(sensorValue);
  if (sensorValue == HIGH) { // Movimento detectado
      escreveLCD("Casa invadida!       ", 1);
      return true;
      //DISPARA ALARME (FUNÇÕES)
  } else {
    // Nenhum movimento
    sensorValue = digitalRead(pirPin); // Lê o valor do sensor PIR
    escreveLCD("Sem movimento           ", 1);
    sensorValue = LOW;
    return false;
  } // fim else
} // fim detectaMovimento

// TODO FLAG BOOL
void 



