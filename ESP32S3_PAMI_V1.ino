#include <Wire.h>
#include "ultrason.h"  // Inclure le fichier ultrason.h
#include "L298N.h"
#include <Adafruit_NeoPixel.h>

#define choixCote 5
#define PERIM 326.725635973
#define IMP_PAR_TOUR 11
#define ratioReducteur 40
#define PIN_ENCODEUR_G 13
#define PIN_ENCODEUR_D 14

// Pins des boutons et des LEDs
#define bouton1 5
#define boutonWifi 6
#define boutonTirette 7

// Variable ring LED
#define ledRingPin 10 // Broche de la bague LED
#define ledCount 12   // Nombre de LEDs dans la bague

// le robot s'arrete s'il détetecte un objet a moins de distStop
#define distStop 140

// Objet pour contrôler la bague LED
Adafruit_NeoPixel ring = Adafruit_NeoPixel(ledCount, ledRingPin, NEO_GRB + NEO_KHZ800);

// Fonction d'initialisation des boutons et de la bague LED
void setupBoutons_ringLED() {
  pinMode(choixCote, INPUT);
  ring.begin();      // Initialise l'aneau de LED
  ring.setBrightness(128); // Luminosité des leds
  
  // test couleur par défaut
  
  affichageChoixCote();
}

void changeColLED(uint32_t couleur){
  // debug
  for (int i = 0; i < ledCount; i++) {
    ring.setPixelColor(i,  couleur);
  }
  ring.show();
}

//return l'etat du bouton tirette
bool etaTirette(){
  return digitalRead(boutonTirette);
}



// Fonction pour changer la couleur de la bague LED
void affichageDynamiqueLED() {
  uint32_t couleur = mesurerDistance() < distStop ? ring.Color(139, 0, 0) : (digitalRead(choixCote) ? ring.Color(0, 0, 139) : ring.Color(255, 255, 0)); // Bleu profond ou Jaune

    for (int i = 0; i < ledCount; i++) {
      ring.setPixelColor(i, couleur);
    }
    ring.show();
}

void affichageChoixCote(){
  uint32_t col = digitalRead(choixCote) ? ring.Color(0, 0, 139) : ring.Color(255, 255, 0);
  changeColLED(col);
}

bool isObjectDetected(){
  long dist = mesurerDistance(); 
  return dist < distStop;
}




// Compteur d'impulsions pour le moteur gauche
volatile unsigned long impulsionsGauche = 0; 
volatile unsigned long impulsionsDroite = 0; 


// Fonction pour incrémenter le compteur du moteur gauche
void IRAM_ATTR compteurImpulsionGauche() {
  impulsionsGauche++;
}
void IRAM_ATTR compteurImpulsionDroite() {
  impulsionsDroite++;
}

int distParcouruGauche(){
  // calcul la distance en mm parcouru à partir du nombre d'implulsion 
  return impulsionsGauche/IMP_PAR_TOUR/ratioReducteur * PERIM;
}
int distParcourDroite(){
  return impulsionsDroite/IMP_PAR_TOUR/ratioReducteur * PERIM;
}

// distance à parcourir en mm
int distAParcourir = 1050;


// calcul du nombre d'impultion pour parcourir la distance dist
// l'encodeur envoie 12 impultion par tour 
// nombre de tours moteur pour parcourir dist
double nbRotaMoteur = distAParcourir/PERIM;
long impulsionsCible = IMP_PAR_TOUR * nbRotaMoteur;


//temps de lecture entre 2 milis
unsigned long previousMillis = 0; // Variable pour stocker le temps précédent
const unsigned long interval = 200; // Intervalle de 200 ms

unsigned long beginMillis = 0;


void setup() {
  Serial.begin(921600);
  setup_ultrason();
  setup_moteur();

  pinMode(choixCote, INPUT);
  pinMode(boutonTirette, INPUT_PULLDOWN);
  setupBoutons_ringLED();

  // odométrie setup
  pinMode(PIN_ENCODEUR_G, INPUT_PULLUP);  // Encodeur moteur gauche
  pinMode(PIN_ENCODEUR_D, INPUT_PULLUP);  // Encodeur moteur gauche

  // Attacher l'interruption pour l'encodeur
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODEUR_G), compteurImpulsionGauche, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODEUR_D), compteurImpulsionDroite, RISING);

  // affichageDynamiqueLED();
  //Attendre l'activation de la tirette
  while(etaTirette()){
    //affichageDynamiqueLED();
    affichageChoixCote();
  }
  
  activer_moteur();

  beginMillis = millis();
}

void loop() {
  

  // on attends 90 sec
  //while (millis() - beginMillis < 90000);

  // 9,5 sec pour faire avancer
  while(millis() - beginMillis < 9500){
    
    // obstacle détecté à moins de  distStop
    if (isObjectDetected()){
      stop();

      changeColLED(ring.Color(139, 0, 0));
      while (isObjectDetected());
      affichageChoixCote();

    }

    // on avance tout droit sur dist
    if (distParcouruGauche() < distAParcourir){
      avancerToutDroit(255);
    }
    else {
      stop();
      changeColLED(ring.Color(255, 255, 255));
      // faire bouger les servo pour danser
      while(1); // on stop le prog
    }
  }
  stop();
  changeColLED(ring.Color(255, 255, 255));
  while(1); // on stop le prog
}
