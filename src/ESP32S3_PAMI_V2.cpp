#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
#include "ultrason.h"  // Inclure le fichier ultrason.h
#include "L298N.h"
#include "servo.h"
#include "ringLED.h"


#define PERIM 163.362817987 
#define IMP_PAR_TOUR 11
#define ratioReducteur 40
#define PIN_ENCODEUR_G 3
#define PIN_ENCODEUR_D 1

// Pins des boutons et des LEDs
#define bouton1 41
#define boutonTirette 42

// le robot s'arrete s'il détetecte un objet a moins de distStop
#define distStop 45

#define VAR_ASSERV 1
#define COEF_ASSERV 0.5


//return l'etat du bouton tirette
bool etaTirette(){
  return digitalRead(boutonTirette);
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

double distParcouruGauche(){
  // calcul la distance en mm parcouru à partir du nombre d'implulsion 
  return (double) impulsionsGauche/IMP_PAR_TOUR/ratioReducteur * PERIM;
}
double distParcourDroite(){
  return  (double)  impulsionsDroite/IMP_PAR_TOUR/ratioReducteur * PERIM;
}

double distParcouru(){
  return (distParcourDroite() + distParcouruGauche())/2.0;
}

typedef struct ConsignesMoteurs {
  int consigneMG;
  int consigneMD;
} ConsignesMoteurs;

// compense la différence du nombres d'impultions entre les deux moteurs 
// en modifiant les consignes moteurs
void asservToutDroit(ConsignesMoteurs *consignesMoteurs){
  
  // calcule du délai a mettre pour ne pas faire d'accou 
  // 50 ms quand la diff = 0
  long pause = 50 + COEF_ASSERV*abs((long) (impulsionsDroite - impulsionsGauche));

  if (pause < 0) {
    pause = 10;
  }

  // 2 cas possible 
  // on va a gauche donc impMD > impMG
  if (impulsionsDroite > impulsionsGauche){
    
    // on baisse la consigne moteur du MD et on augmente la consigne du MG


    if (consignesMoteurs->consigneMD > 125){
      consignesMoteurs->consigneMD -= VAR_ASSERV;
    } 
    if (consignesMoteurs->consigneMG < 255){
      consignesMoteurs->consigneMG += VAR_ASSERV;
    }
    
  } else if (impulsionsGauche > impulsionsDroite){
    
    // on baisse la consigne moteur du MD et on augmente la consigne du MG

    if (consignesMoteurs->consigneMD < 255){
      consignesMoteurs->consigneMD += VAR_ASSERV;
    } 
    if (consignesMoteurs->consigneMG > 125){
      consignesMoteurs->consigneMG -= VAR_ASSERV;
    }
  }

  delay(pause);
}

// pour appliquer l'asserv
void avancer(ConsignesMoteurs consignesMoteurs){
  avancerMD(consignesMoteurs.consigneMD);
  avancerMG(consignesMoteurs.consigneMG);
}

long beginMillis;

void setup() {
  Serial.begin(115200);

  // setup de tout les composants
  setup_moteur();
  setupTOF();
  setupServo();
  
  pinMode(boutonTirette, INPUT_PULLDOWN);
  setupBoutons_ringLED();

  // odométrie setup
  pinMode(PIN_ENCODEUR_G, INPUT_PULLUP);  // Encodeur moteur gauche
  pinMode(PIN_ENCODEUR_D, INPUT_PULLUP);  // Encodeur moteur droit

  // Attacher l'interruption pour l'encodeur
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODEUR_G), compteurImpulsionGauche, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODEUR_D), compteurImpulsionDroite, RISING);
  
  // tant qu'on a pas retirer la tirette 
  while (etaTirette()){

    // on affiche le choix de coté sur le ringLED
    affichageChoixCote();
  }

  activer_moteur();

  beginMillis = millis();
}

void loop() {
  
  
  // on attend 85 secondes
  while (millis() - beginMillis < 85000);

  // on reset le timer pour les 15s d'actions
  beginMillis = millis();

  // initialisation des consignes moteurs 
  ConsignesMoteurs consignesMoteurs;
  consignesMoteurs.consigneMD = 240;
  consignesMoteurs.consigneMG = 240;

  // on démare avec une petite courbe d'accélération
  rampeAccelToutDroit(consignesMoteurs.consigneMD, 500);
  
  // 13s d'actions (on prend large, pour ne pas dépasser)
  // pas d'objet détecté
  while (millis() - beginMillis < 13000 && !isObjectDetected()){

    // on avance tout droit avec l'asserv

    // on change les consignes moteurs
    asservToutDroit(&consignesMoteurs);
    
    // on appliques les nouvelles consignes
    avancer(consignesMoteurs);


  } 
  
  // une fois l'action terminé on s'arrete et on dance 
  stop();
  dance();
} 
  
