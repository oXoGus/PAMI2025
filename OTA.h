#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char *ssid = "iPhone de Mathis D.";
const char *password = "testtest"; 

// Dimensions du logo
const int logoWidth = 40;
const int logoHeight = 15;

// Déclaration de la variable externe pour l'écran OLED
extern Adafruit_SSD1306 display;

//convert to hexa : https://javl.github.io/image2cpp/?pseSrc=pgEcranOledArduino
// 'wifi_logo'
const unsigned char Wifi_logo[] PROGMEM = {
    0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xff, 0xff, 0xc0, 0x1f,
    0xf0, 0x00, 0x0f, 0xf8, 0x3f, 0x07, 0xff, 0xe0, 0xfc, 0x3c, 0x7f, 0xff, 0xfe, 0x3c, 0x01, 0xff,
    0x00, 0xff, 0x80, 0x03, 0xf0, 0xff, 0x0f, 0xc0, 0x00, 0x87, 0xff, 0xe1, 0x00, 0x00, 0x1f, 0xff,
    0xf8, 0x00, 0x00, 0x1e, 0x00, 0x78, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00
};

void wifi_start() {
  display.clearDisplay();
  // Afficher le logo wifi au centre
  int x = (128 - logoWidth) / 2;  // Centre horizontal
  int y = (32 - logoHeight) / 2;  // Centre vertical
  display.drawBitmap(x, y, Wifi_logo, logoWidth, logoHeight, WHITE);
  display.display();

  //debut initialisation OTAN
  Serial.println("Booting");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //Animation erreur de connexion au reseau wifi
  if (WiFi.waitForConnectResult() != WL_CONNECTED){
    while (y > 0) { //animation logo en haut de l'ecran
      // Efface l'écran
      display.clearDisplay();
      // Dessine le logo à la nouvelle position
      display.drawBitmap(x, y, Wifi_logo, logoWidth, logoHeight, WHITE);
      // Met à jour l'écran
      display.display();
      // Réduire les coordonnées pour déplacer le logo vers le haut à gauche
      if (y > 0) y--;
    }
    display.setTextSize(1); //chaque lettre = 3px de large
    display.setTextColor(WHITE);
    display.setCursor(20, 20);
    display.println("ERREUR CONNEXION");
    display.display();
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(5000);
      //ESP.restart();
    }
  }

  // Animation : déplacement vers le coin supérieur gauche
  while (x > 0 || y > 0) {
    // Efface l'écran
    display.clearDisplay();
    // Dessine le logo à la nouvelle position
    display.drawBitmap(x, y, Wifi_logo, logoWidth, logoHeight, WHITE);
    // Met à jour l'écran
    display.display();

    // Réduire les coordonnées pour déplacer le logo vers le haut à gauche
    if (x > 0) x--;
    if (y > 0) y--;
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);
  ArduinoOTA.begin();
  //mot de passe wifi par defaut : 1234
  Serial.println("Ready");
  Serial.print("adresse IP :");
  Serial.println(WiFi.localIP());

  /////////////////affichage adresse IP
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(45, 0);
  // Display static text
  display.println("Adresse IP :");
  display.setCursor(45, 10);
  display.println(WiFi.localIP());
  display.display();
}

void OTA() {
  ArduinoOTA.handle();
}

void wifi_end(){
  WiFi.disconnect(); // Déconnecte du réseau
  WiFi.mode(WIFI_OFF); // Désactive le module WiFi
}
