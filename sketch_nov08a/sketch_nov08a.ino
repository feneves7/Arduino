#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9



#define LED_GREEN 7
#define LED_RED 6
#define BUZZER 8

//#define pinoBlue 4
//#define pinoGreen 3
//#define pinoRed 2

//int val;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
 
char st[20];
  
void setup() 
{
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

//  pinMode(pinoRed, OUTPUT); 
//  pinMode(pinoBlue, OUTPUT); 
//  pinMode(pinoGreen, OUTPUT); 
  
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  //Define o número de colunas e linhas do LCD:  
  lcd.begin(16, 2);  
  mensageminicial();
  
}
 
void loop() 
{
  Serial.flush();
//  for(val = 255; val > 0; val --){ //PARA val IGUAL A 255, ENQUANTO val MAIOR QUE 0, DECREMENTA val
//      analogWrite(pinoRed, val); //PINO RECEBE O VALOR
//      analogWrite(pinoBlue, 255-val); //PINO RECEBE O VALOR
//      analogWrite(pinoGreen, 128-val); //PINO RECEBE O VALOR
//      delay (10); //INTERVALO DE 10 MILISSEGUNDOS
//  }
//  for(val = 0; val < 255; val ++){ //PARA val IGUAL A 0, ENQUANTO val MENOR QUE 255, INCREMENTA val
//      analogWrite(pinoRed, val); //PINO RECEBE O VALOR
//      analogWrite(pinoBlue, 255-val); //PINO RECEBE O VALOR
//      analogWrite(pinoGreen, 128-val); //PINO RECEBE O VALOR
//      delay (10); //INTERVALO DE 10 MILISSEGUNDOS
//  }

  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;



  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  if (conteudo.substring(1) == "1A C0 4A 2B") //UID 1 - Chaveiro
  {
    Serial.println("Ola FELIPE !");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola FELIPE !");
    lcd.setCursor(0,1);
    lcd.print("Acesso liberado!");
    digitalWrite(LED_GREEN,HIGH);
    digitalWrite(LED_RED,LOW);

    tone(BUZZER,1500);   
    delay(300);
   
    //Desligando o buzzer.
    noTone(BUZZER);
    
    
    delay(3000);
    mensageminicial();
  }
  else if (conteudo.substring(1) == "EC 5C 65 55") //UID 2 - Cartao
  {
    Serial.println("Ola Cartao !");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola Cartao !");
    lcd.setCursor(0,1);
    lcd.print("Acesso Negado !");
    digitalWrite(LED_GREEN,LOW);
    digitalWrite(LED_RED,HIGH);
    tone(BUZZER,1500);   
    delay(500);
   
    //Desligando o buzzer.
    noTone(BUZZER);
    
    delay(3000);
    mensageminicial();
  }
  else{
    delay(3000);
    Serial.flush();
    mensageminicial(); 
  }

 
} 
 
void mensageminicial()
{
  lcd.clear();
  lcd.print(" Aproxime o seu");  
  lcd.setCursor(0,1);
  lcd.print("cartao do leitor");  

  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_RED,HIGH);
  
  tone(BUZZER,1500);   
  delay(200);
   
  //Desligando o buzzer.
  noTone(BUZZER);
  delay(200);
}
