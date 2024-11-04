#include <LiquidCrystal.h> //biblioteca LCD

//TODO
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //pinos do LCD 
int trigPin = 11;  // Pino do Trigger
int echoPin = 10; // Pino do Echo
int ledPin = 9; // pino  do LED
int buzzerPin = 8; // Pino do buzzer
int pirPin = 7; // Pino onde o OUT do PIR está conectado
int botaoPin = 2;  // Pino onde botao está conectado



void setup() {
//  Serial.begin(9600); // Inicializa a comunicação serial
  lcd.begin(16, 2);
  lcd.print("Olá humanos!");

  pinMode(trigPin, OUTPUT);  // define o pino Trigger como saída
  pinMode(echoPin, INPUT);   // define o pino Echo como entrada
  pinMode(ledPin, OUTPUT); // define o pino led como saída
  pinMode(pirPin, INPUT);   // define o pino PIR como entrada
  pinMode(buzzerPin, OUTPUT); // define o pino Buzzer como saída
  pinMode(botaoPin, INPUT); // define o pino do botao como entrada
}


void loop() {
  int botaoStatus = digitalRead(botaoPin); // Lê o estado do botao

  if (switchState == HIGH) {
    resetaAtivaAlarme();
  } else {
    digitalWrite(ledPin, LOW); 
  }

}

//*********** FUNÇÕES ************
//TODO
void disparaAlarme(){
  escreveLCD("ALARME DISPARADO!!!",0);
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

void escreveLCD(mensagem, linha){

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

  int sensorValue;
  sensorValue = digitalRead(pirPin); // Lê o valor do sensor PIR
  if (sensorValue == HIGH) { // Movimento detectado
      Serial.println("Movimento detectado!");
      //DISPARA ALARME (FUNÇÕES)
  else {
    // Nenhum movimento
    sensorValue = digitalRead(pirPin); // Lê o valor do sensor PIR
  } // fim else
  }
} // fim detectaMovimento

// TODO FLAG BOOL
void detectaMovimentoUltrassonico(){
  long duracao, distancia;

  // Envia um pulso de 10 microsegundos no Trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lê o tempo de retorno do pulso no Echo
  duracao = pulseIn(echoPin, HIGH);

  // Calcula a distância em cm
  distancia = duracao * 0.034 / 2;

  // Exibe a distância no monitor serial
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if(distancia <= 4){
    //dispara alarme
    escreveLCD("Movimento detectado!", 0);
    escreveLCD("JANELA!!", 1);
    disparaAlarme();
  }

  delay(500); // Aguarda meio segundo antes da próxima medição

}



