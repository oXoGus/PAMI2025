#ifndef BOUTON_H
#define BOUTON_H

#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include "ultrason.h"  // Inclure le fichier ultrason.h
#include "OTA.h"

// Pins des boutons et des LEDs
#define bouton1 5
#define boutonWifi 6
#define boutonTirette 7

// Variable ring LED
#define ledRingPin 10 // Broche de la bague LED
#define ledCount 12   // Nombre de LEDs dans la bague

// Déclaration de la variable externe pour l'écran OLED
extern Adafruit_SSD1306 display;

// Objet pour contrôler la bague LED
Adafruit_NeoPixel ring = Adafruit_NeoPixel(ledCount, ledRingPin, NEO_GRB + NEO_KHZ800);

//return l'etat du bouton tirette
bool etaTirette(){
  bool etatboutonTirette = digitalRead(boutonTirette);
  return etatboutonTirette;
}

// Fonction d'initialisation des boutons et de la bague LED
void setupBoutons_ringLED() {
  pinMode(bouton1, INPUT);
  pinMode(boutonWifi, INPUT);
  pinMode(boutonTirette, INPUT);

  ring.begin();      // Initialise l'aneau de LED
  ring.show();       // Éteint toutes les LEDs par défaut
  ring.setBrightness(128); // Luminosité des leds
}

// Fonction pour changer la couleur de la bague LED
void changerCouleurLED(bool bouton1Etat) {
  uint32_t couleur = bouton1Etat ? ring.Color(0, 0, 139) : ring.Color(255, 255, 0); // Bleu profond ou Jaune

  for (int i = 0; i < ledCount; i++) {
    ring.setPixelColor(i, couleur);
  }
  ring.show();
}

void changerCouleurLEDRouge(){
  for (int i = 0; i < ledCount; i++) {
    ring.setPixelColor(i, ring.Color(139, 0, 0));
  }
  ring.show();
}

// Fonction pour afficher l'état des boutons
void afficherEtatBoutons() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // Lire les états des boutons
  bool etatBouton1 = digitalRead(bouton1);
  bool etatboutonWifi = digitalRead(boutonWifi);
  bool etatboutonTirette = digitalRead(boutonTirette);

  // Afficher l'état des boutons sur l'écran
  //Bouton 1
  display.print("Couleur : ");
  display.println(etatBouton1 ? "Bleu" : "Jaune");
  //Bouton 2
  display.print("Wifi Mode : ");
  display.println(etatboutonWifi ? "On" : "Off");
  //Bouton 3
  display.print("Tirette : ");
  display.println(etatboutonTirette ? "0" : "1");

  // Affichage de la distance mesurée par le capteur ultrason
  long distance = mesurerDistance();  // Récupérer la distance en cm
  //display.setCursor(0, 30);  // Déplacer le curseur en dessous des boutons
  if(distance >= 0){
    display.print("Distance : ");
    display.print(distance/10);
    display.println(" cm");
    display.display();
    // Changer la couleur de la bague LED selon l'état du bouton 1
    //changerCouleurLED(etatBouton1);
    mesurerDistance() < 40 ? changerCouleurLEDRouge() : changerCouleurLED(etatBouton1);
  }

  //Bouton 2 : Wifi ON
  if(etatboutonWifi){
    wifi_start();
    while(etatboutonWifi){
      OTA();
      etatboutonWifi = digitalRead(boutonWifi);
    }
    wifi_end();
  }
}

#endif // BOUTON_H
