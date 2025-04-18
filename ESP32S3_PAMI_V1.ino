#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "logo.h" // Données et fonction pour afficher le logo
#include "bouton.h" //Lecture des bouton, affichage oled et ring led
#include "ultrason.h"  // Inclure le fichier ultrason.h
#include "L298N.h"

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

//temps de lecture entre 2 milis
unsigned long previousMillis = 0; // Variable pour stocker le temps précédent
const unsigned long interval = 200; // Intervalle de 200 ms

unsigned long beginMillis = 0;


void setup() {
  Serial.begin(921600);
  
  setup_ultrason();
  setup_moteur();
  setupBoutons_ringLED();
  // Initialisation de l'écran OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Échec de l'initialisation de l'écran OLED !"));
    while (true); // Boucle infinie en cas d'échec
  }

  display.clearDisplay();
  display.setRotation(2); // Rotation à 180°
  
  displayLogo(display); // Affiche le logo
  delay(2000);          // Pause pour visualiser le logo

  //Attendre l'activation de la tirette
  while(etaTirette()){
    afficherEtatBoutons();
  }

  //affichage
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Demarrage Moteurs");
  display.display();
  
  activer_moteur();

  beginMillis = millis();
}

void loop() {
  if(etaTirette()){
    stop();
    ESP.restart();
  }

  avancerToutDroit(255, 10);
  
  delay(4000);

  stop();

  while (1);



}
