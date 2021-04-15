// system base by BeBoX
// 04/2021
// Azerty keyboard support with Nico78 code
// functions : 
//--------------------------------------------------------------------------------------------------------------------------------------------------------
// sendtext("abcd...") --> pour envoyer du texte (longueur maxi 254 caractères à la fois)
// sendtextln("abcd...") --> pour envoyer du texte avec retour chariot (longueur maxi 254 caractères à la fois)
// sendkeypad(0123456789/*-+E) pour simuler le clavier numérique, E pour la touche Entrée
// altunicode("0169"); 2 à 4 chiffres en argument pour afficher d'autres caractères unicodes dans les éditeurs qui les acceptent
// ÇÉÈ æ œ øØ ↨↑↓→←↔ ►◄▲▼ ☺☻ ♥♦♣♠ ♂♀ ♪♫ ☼ “” ™©® --> caractères spéciaux que l'on peut utiliser directement (utilise altunicode automatiquement)
// sendkeycombi(KEY_LEFT_GUI, 'r') permet de taper deux touches en même temps (une touche system + une touche ascii), ici 'window + r'
// selectall() permet de sélectionner tout le texte
// cut() pour couper du texte
// copy() pour copier du texte
// paste() pour coller du texte
// homekey() pour déplacer le curseur vers le début du texte lors d'une sélection
// endkey() pour déplacer le curseur vers la fin du texte lors d'une sélection
// leftkey(nb) déplace le curseur vers la gauche (nb = nombre de fois)
// rightkey(nb) déplace le curseur vers la droite (nb = nombre de fois)
// upkey(nb) déplace le curseur vers le haut (nb = nombre de fois)
// downkey(nb) déplace le curseur vers le bas (nb = nombre de fois)
// tabkey(nb) permet de faire une tabulation (nb = nombre de fois)
// returnkey(nb) permet de revenir à la ligne (nb = nombre de fois)
// backspacekey(nb) touche Retour Arrière du clavier (nb = nombre de fois)
// deletekey(nb) touche Suppr du clavier (nb = nombre de fois)
// sendkeycommand(command) permet d'envoyer des commandes qui ne sont pas utiliser dans les fonctions précédentes comme 
// KEY_ESC , KEY_INSERT , KEY_PAGE_UP , KEY_PAGE_DOWN , KEY_CAPS_LOCK 
//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Valeur des constantes des leds pour info et présentation de la fonction permettant de récupérer l'état des leds du clavier
// #define LED_CAPS_LOCK      0x02
// #define LED_NUM_LOCK       0x01
// #define LED_SCROLL_LOCK    0x04
// bool getLedStatus(uint8_t led);
//--------------------------------------------------------------------------------------------------------------------------------------------------------
// experimental unfinished Hak5 ducky script implementation by BeBoX 
//
// if KeyLayout == "FR" then azerty keyboard is used if KeyLayout != "FR" then use US keyboard layout (Arduino Keyboard library)
// Usage DuckyScript(String comm)
// Example : (type Window + r, type notepad.exe and Return key to launch notepad on windows)
//  DuckyScript("GUI r");               // shortcut for running command
//  DuckyScript("DELAY 500");           // wait 500 ms to let windows show and focus on window
//  DuckyScript("STRING notepad.exe");  // type notepad.ex
//  DuckyScript("ENTER");               // press Return key
//
//--------------------------------------------------------------------------------------------------------------------------------------------------------
// 
//
//--------------------------------------------------------------------------------------------------------------------------------------------------------

#include "ducky.h" // My Ducky Script interpreter module and Azerty support
#include "menu.h" 

const char* menu[] = {
  "00 title",
  "01",
  "02",
  "03",
  "04",
  "05",
  "06",
  "07",
  "08",
  "09",
  "10",
  "11 title",
  "12",
  "13",
  "14",
  "15",
  "16",
  "17",
  "18",
  "19",
  "20",
  "21",
  "22 title",
  "23",
  "24",
  "25",
  "26",
  "27",
  "28",
  "29",
  "30",
  "31",
  "32",
  "33 title",
  "34",
  "35",
  "36",
  "37",
  "38",
  "39",
  "40",
  "41",
  "42",
  "43",
  "44 title",
  "45",
  "46",
  "47",
  "48",
  "49",
  "50",
  "51",
  "52",
  "53",
  "54",
  "55 title",
  "56",
  "57",
  "58",
  "59",
  "60",
  "61",
  "62",
  "63",
  "64",
  "65",
  "66 title",
  "67",
  "68",
  "69",
  "70",
  "71",
  "72",
  "73",
  "74",
  "75",
  "76",
  "77 title",
  "78",
  "79",
  "80",
  "81",
  "82",
  "83",
  "84",
  "85",
  "86",
  "87",  
  "88 title",
  "89",
  "90",
  "91",
  "92",
  "93",
  "94",
  "95",
  "96",
  "97",
  "98",
  "99 title",
  "100",
  "101",
  "102",
  "103",
  "104",
  "105",
  "106",
  "107",
  "108",
  "109",
  "110 title",
  "111",
  "112",
  "113",
  "114",
  "115",
  "116",
  "117",
  "118",
  "119",
  "110"    
  };
  
// Print the header for a display screen
void header(const char* string, uint16_t color) {
    tft.fillScreen(color);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    tft.fillRect(0, 0, 320, 30, TFT_DARKGREY);
    tft.setTextDatum(TC_DATUM);
    tft.drawString(string, 160, 2, 4); // Font 4 for fast drawing with background
}  
// show menu routine
void ShowMenu( int MenuIndex, int nblines) {
  // first is header
  //Serial.print("Menu num");
  //Serial.println(MenuIndex);
  header(menu[MenuIndex],TFT_BLACK); // Fix header
  int y=0;
  for (int i=MenuIndex+1; i<=MenuIndex+nblines; i++) {
    tft.setTextColor(TFT_WHITE);    
    tft.setCursor(20, start+y); // curent line
    Serial.println(i);
    tft.print(menu[i]);
    //tft.setCursor(300, start+y); // curent line
    //tft.print(i);    
    y=y+20;
  }
}

void setup() {
  // put your setup code here, to run once:
  Mouse.begin();
  Keyboard.begin();
  tft.begin();
  tft.setRotation(3);
  tft.setFreeFont(FSB9);   // Select Free Serif 9 point font
  // Physical controls init
  pinMode(WIO_5S_UP, INPUT_PULLUP); 
  pinMode(WIO_5S_DOWN, INPUT_PULLUP); 
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);  
  ShowMenu(page,10);
  ShowCursor(0, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
   if (digitalRead(WIO_5S_UP) == LOW) {
    //Serial.println("5 Way Up");
    if (curseur>0){
      curseur--;
      ShowCursor(curseur+1, curseur);
    }else { 
      curseur=9;
      ShowCursor(0, curseur);
    }
   }
   else if (digitalRead(WIO_5S_DOWN) == LOW) {
    //Serial.println("5 Way Down");
    if (curseur<9) {
      curseur++;
      ShowCursor(curseur-1, curseur);
    }else { 
      curseur=0;
      ShowCursor(9, curseur);
    }
   }
   else if (digitalRead(WIO_5S_LEFT) == LOW) {
    //Serial.println("5 Way Left");
   }
   else if (digitalRead(WIO_5S_RIGHT) == LOW) {
    //Serial.println("5 Way Right");
   }
   else if (digitalRead(WIO_5S_PRESS) == LOW) {
    // traitement validation dans menu
    switch (page) {
      case 0:
        // Main page
        page=(curseur+1)*11;
        ShowMenu(page,10);       
        ShowCursor(0, 0); // init cursor to 0
        curseur=0;
        break;
      case 11:
        // page 1 
        switch (curseur) {
          case 0:
            // 1er option
            break;
          case 1:
            // 2e option
            break;
          case 2:
            // 3e option
            break;
          case 3:
            // 4e option
            break;
          case 4:
            // 5e option
            break;
          case 5:
            // 6e option
            break;
          case 6:
            // 7e option
            break;
          case 7:
            // 8e option
            break;
          case 8:
            // 9e option
            break;
          case 9:
            // 10e option
            break;
          } 
        break;
      case 22:
        // page 2
        switch (curseur) {
          case 0:
            // 1er option
            break;
          case 1:
            // 2e option
            break;
          case 2:
            // 3e option
            break;
          case 3:
            // 4e option
            break;
          case 4:
            // 5e option
            break;
          case 5:
            // 6e option
            break;
          case 6:
            // 7e option
            break;
          case 7:
            // 8e option
            break;
          case 8:
            // 9e option
            break;
          case 9:
            // 10e option
            break;
          }         
        break;
      case 33:
        // page 3
        switch (curseur) {
          case 0:
            // 1er option
            break;
          case 1:
            // 2e option
            break;
          case 2:
            // 3e option
            break;
          case 3:
            // 4e option
            break;
          case 4:
            // 5e option
            break;
          case 5:
            // 6e option
            break;
          case 6:
            // 7e option
            break;
          case 7:
            // 8e option
            break;
          case 8:
            // 9e option
            break;
          case 9:
            // 10e option
            break;
          }         
        break;
      case 44:
        // page 4
        switch (curseur) {
          case 0:
            // 1er option
            break;
          case 1:
            // 2e option
            break;
          case 2:
            // 3e option
            break;
          case 3:
            // 4e option
            break;
          case 4:
            // 5e option
            break;
          case 5:
            // 6e option
            break;
          case 6:
            // 7e option
            break;
          case 7:
            // 8e option
            break;
          case 8:
            // 9e option
            break;
          case 9:
            // 10e option
            break;
          }         
        break;              
      case 55:
        // page 5
        switch (curseur) {
          case 0:
            // 1er option
            break;
          case 1:
            // 2e option
            break;
          case 2:
            // 3e option
            break;
          case 3:
            // 4e option
            break;
          case 4:
            // 5e option
            break;
          case 5:
            // 6e option
            break;
          case 6:
            // 7e option
            break;
          case 7:
            // 8e option
            break;
          case 8:
            // 9e option
            break;
          case 9:
            // 10e option
            break;
          }         
        break;
      case 66:
        // page 6
        switch (curseur) {
          case 0:
            // 1er option
            break;
          case 1:
            // 2e option
            break;
          case 2:
            // 3e option
            break;
          case 3:
            // 4e option
            break;
          case 4:
            // 5e option
            break;
          case 5:
            // 6e option
            break;
          case 6:
            // 7e option
            break;
          case 7:
            // 8e option
            break;
          case 8:
            // 9e option
            break;
          case 9:
            // 10e option
            break;
          }         
        break;
      case 77:
        // page 7
        switch (curseur) {
          case 0:
            // 1er option
            break;
          case 1:
            // 2e option
            break;
          case 2:
            // 3e option
            break;
          case 3:
            // 4e option
            break;
          case 4:
            // 5e option
            break;
          case 5:
            // 6e option
            break;
          case 6:
            // 7e option
            break;
          case 7:
            // 8e option
            break;
          case 8:
            // 9e option
            break;
          case 9:
            // 10e option
            break;
          }         
        break;
      case 88:
        // page 8
        switch (curseur) {
          case 0:
            // 1er option
            break;
          case 1:
            // 2e option
            break;
          case 2:
            // 3e option
            break;
          case 3:
            // 4e option
            break;
          case 4:
            // 5e option
            break;
          case 5:
            // 6e option
            break;
          case 6:
            // 7e option
            break;
          case 7:
            // 8e option
            break;
          case 8:
            // 9e option
            break;
          case 9:
            // 10e option
            break;
          }         
        break;
      case 99:
        // page 9
        switch (curseur) {
          case 0:
            // 1er option
            break;
          case 1:
            // 2e option
            break;
          case 2:
            // 3e option
            break;
          case 3:
            // 4e option
            break;
          case 4:
            // 5e option
            break;
          case 5:
            // 6e option
            break;
          case 6:
            // 7e option
            break;
          case 7:
            // 8e option
            break;
          case 8:
            // 9e option
            break;
          case 9:
            // 10e option
            break;
          }         
        break;
      case 110:
        // page 10
        switch (curseur) {
          case 0:
            // 1er option
            break;
          case 1:
            // 2e option
            break;
          case 2:
            // 3e option
            break;
          case 3:
            // 4e option
            break;
          case 4:
            // 5e option
            break;
          case 5:
            // 6e option
            break;
          case 6:
            // 7e option
            break;
          case 7:
            // 8e option
            break;
          case 8:
            // 9e option
            break;
          case 9:
            // 10e option
            break;
          }         
        break;                                              
    }
    // wait for release button
    while (digitalRead(WIO_5S_PRESS) == LOW){
          delay(100);
        }
   }  // end ---- button press 
   if (digitalRead(WIO_KEY_A) == LOW) {
    //page plus
    if (page <110) {
      page=page+11;

    } else {
      page=0;
    }
    ShowMenu(page,10);
    ShowCursor(0, 0);
    curseur=0;
   }
   else if (digitalRead(WIO_KEY_B) == LOW) {
    //Page moins
    if (page >=11) {
      page=page-11;
    } else {
      page=110;   
    }
    ShowMenu(page,10);
    ShowCursor(0, 0);
    curseur=0;
   }
   else if (digitalRead(WIO_KEY_C) == LOW) {
    page=0;
    ShowMenu(page,10);
    ShowCursor(0, 0);
    curseur=0;
   }
   delay(100);      

}
