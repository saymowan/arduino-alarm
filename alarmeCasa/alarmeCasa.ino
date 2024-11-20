#include <LiquidCrystal.h> // Biblioteca LCD

LiquidCrystal lcd(12, 13, 5, 4, 3, 2); // Pinos do LCD
int vermelhoLedPin = 8; // Pino para cor vermelha do LED RGB
int verdeLedPin = 9;    // Pino para cor verde do LED RGB
int azulLedPin = 10;    // Pino para cor azul do LED RGB
int buzzerPin = 6;      // Pino do buzzer
int trigPin = 11;       // Pino do Trigger do sensor ultrassônico
int echoPin = 7;        // Pino do Echo do sensor ultrassônico

long duracao;
int distancia;

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial
  lcd.begin(16, 2);   // Inicializa o LCD de 16x2
  escreveLCD("ALARME CASA     ", 0);
  escreveLCD("PROJETO ROBOTICA", 1);

  // Configurações dos pinos
  pinMode(vermelhoLedPin, OUTPUT); 
  pinMode(verdeLedPin, OUTPUT); 
  pinMode(azulLedPin, OUTPUT); 
  pinMode(buzzerPin, OUTPUT); 
  pinMode(trigPin, OUTPUT);  // Configura o pino de Trigger como saída
  pinMode(echoPin, INPUT);   // Configura o pino de Echo como entrada
}

void loop() {
  // Envia um pulso para o Trigger do sensor ultrassônico
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2);  
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);   
  digitalWrite(trigPin, LOW);  
  
  // Lê a duração do pulso de Echo
  duracao = pulseIn(echoPin, HIGH);
  
  // Calcula a distância em centímetros (velocidade do som = 0,0343 cm/us)
  distancia = duracao * 0.0343 / 2;
  delay(250);

  // Exibe a distância no Monitor Serial para depuração (opcional)
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // Atualiza a distância no LCD
  lcd.setCursor(0, 0);
  lcd.print("Distancia: ");
  lcd.print(distancia);
  lcd.print(" cm    "); // Adiciona espaços para limpar qualquer valor residual

  // Se a distância for menor que 10 cm, consideramos que há algo perto
  if (distancia < 7) {
    digitalWrite(vermelhoLedPin, HIGH); // Acende a cor vermelha do LED
    digitalWrite(verdeLedPin, LOW);     // Desliga a cor verde do LED
    digitalWrite(azulLedPin, LOW);      // Desliga a cor azul do LED
    digitalWrite(buzzerPin, HIGH);      // Emite o som do buzzer
    escreveLCD("Intruso entrou", 1);     // Exibe no LCD
    delay(3000);  // Ação dura 5 segundos
    digitalWrite(buzzerPin, LOW);       // Desliga o buzzer
  } 
  // Se a distância for maior que 10 cm, não há intruso
  else {
    digitalWrite(vermelhoLedPin, LOW);  // Desliga a cor vermelha do LED
    digitalWrite(verdeLedPin, HIGH);    // Acende a cor verde do LED
    digitalWrite(azulLedPin, LOW);      // Desliga a cor azul do LED
    escreveLCD("Ninguem na casa", 1);   // Exibe no LCD
  }

  delay(200);  // Atraso para evitar leituras excessivas do sensor ultrassônico
}

// Função para escrever no LCD
void escreveLCD(String texto, int linha) {
  lcd.setCursor(0, linha);
  lcd.print("                ");  // Limpa a linha
  lcd.setCursor(0, linha);
  lcd.print(texto);  // Escreve o texto na linha do LCD
}
