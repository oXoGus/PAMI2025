#include <Adafruit_NeoPixel.h>


// Variable ring LED
#define ledRingPin 6 // Broche de la bague LED
#define ledCount 12   // Nombre de LEDs dans la bague
#define choixCote 40

// pour l'affichage dynamique 
extern bool isObjectDetected();

// Objet pour contrôler la bague LED
Adafruit_NeoPixel ring = Adafruit_NeoPixel(ledCount, ledRingPin, NEO_GRB + NEO_KHZ800);

void changeColLED(uint32_t couleur){
  // debug
  for (int i = 0; i < ledCount; i++) {
    ring.setPixelColor(i,  couleur);
  }
  ring.show();
}

void affichageChoixCote(){
  uint32_t col = digitalRead(choixCote) ? ring.Color(0, 0, 139) : ring.Color(255, 255, 0);
  changeColLED(col);
}

// Fonction d'initialisation des boutons et de la bague LED
void setupBoutons_ringLED() {
  pinMode(choixCote, INPUT_PULLDOWN);
  ring.begin();      // Initialise l'aneau de LED
  ring.setBrightness(128); // Luminosité des leds
  
  // test couleur par défaut
  
  affichageChoixCote();
}



// Fonction pour changer la couleur de la bague LED
void affichageDynamiqueLED() {
  uint32_t couleur = isObjectDetected() ? ring.Color(139, 0, 0) : (digitalRead(choixCote) ? ring.Color(0, 0, 139) : ring.Color(255, 255, 0)); // Bleu profond ou Jaune

  for (int i = 0; i < ledCount; i++) {
    ring.setPixelColor(i, couleur);
  }
  ring.show();
}