#include <CAN.h>
#include <Arduino.h>
#include "lvgl.h"
#include "ui/ui.h"


void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Receiver");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Inicializacao CAN Falhou!");
    while (1);
  }
 
  //const int pino do botao aqui
  int EstadoBotao;
  int UltimoEstadoBotao = LOW;

  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 100;
}

//Decodificando a mensagem CAN (até onde eu entendi, a tela só vai ser reciever),
//Pode-se pegar as informações recebidas dos sensores;
//e traduzir em atualizações pros dados apresentados na tela
//Variáveis criadas de acordo (ex: float para Voltagem e pressão, int para kmh e RPM).
void updateLVGL(){
  //Para atualizar dados em texto:
  lv_label_set_text(nome_label, val);

  //Para atualizar valores em Gauge:
  lv_arc_set_value(nome_arc, val);

  //Para atualizar valores em barras:
  lv_bar_set_value(nome_bar, val);

}


void loop() {
  int packetSize = CAN.parsePacket();

  if (packetSize || CAN.packetId() != -1) {
    // recebeu pacote CAN
    Serial.print("Received ");

    if (CAN.packetExtended()) {
      Serial.print("extended ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, pacote sem dados
      Serial.print("RTR ");
    }

    Serial.print("packet with id 0x");
    Serial.print(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print(" and requested length ");
      Serial.println(CAN.packetDlc());
    } else {
      Serial.print(" and length ");
      Serial.println(packetSize);

      // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print((char)CAN.read());
      }
      Serial.println();
    }

    Serial.println();
  }

  updateLVGL(parâmetros)

  //O Outro código possúi as duas telas diferentes armazenadas como "static lv_obj_t *NomeTelaX", 
  //Assumindo um botão inserido em algum pino do ESP32-S3 da tela, dá pra fazer a troca entre telas
  //E também utilizar debounce time pra não ter troca descontrolada de telas

  int leitura = digitalRead(Pino_do_Botao);

  if (leitura != UltimoEstadoBotao) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    // if the button state has changed:
    if (leitura != EstadoBotao) {
      EstadoBotao = reading;

      if (EstadoBotao == HIGH) { //Se entramos nesse IF, é porque clicamos no botão (leitura HIGH) e estamos dentro do Debounce (millis -last)
        //OBS: "lv_scr_act" retorna um ponteiro para a tela atualmente ativa
        //NÃO tenho certeza se esse é o jeito certo de trocar entre telas com um botão externo
        if (lv_scr_act() == Tela1){ //Se estamos na tela 1:
          lv_scr_load(Tela2); //Carrega Tela 1
          if (Tela1 != NULL) {
            lv_obj_del(Tela1); // APaga tela 1 da memória (tipo um free() do malloc)
            Tela1 = NULL;
          }
        }
        else if (lv_scr_act() == Tela2){ // Se estamos na tela 2:
          lv_scr_load(Tela1);
          if (Tela2 != NULL){
            lv_obj_del(Tela2); // APaga tela 2
            Tela2 = NULL;
          }
        }
      }
    }
  } // Essa feiura de bracket é uma combinação do final do código do debounce time com o código de trocar tela

}
