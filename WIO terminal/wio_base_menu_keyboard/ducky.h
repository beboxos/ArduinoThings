// ducky script interpreter module by BeBox
// 04/2021
// 

String KeyLayout = "FR"; // default keyboard Layout (FR or US)


//--------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Clavier Azerty by Nico78
// (Octobre 2018) V1.01 -- amélioration, correction du code, ajout de caractères spéciaux gérés directement
// (Juin 2018) V1.00
// 
// POUR AVOIR LE RETOUR DES NOTIFICATIONS DES LED, IL FAUT PATCHER, VOIR ICI
// https://forum.arduino.cc/index.php?topic=173583.15
// 
// Ce code source utilise la bibliothèque Keyboard standard de l'Arduino
// Tester avec un Arduino pro micro 5V 16Mhz et l'IDE arduino 1.8.5 
// 
//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Fonctions:
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
// Source info:
// http://www.zem.fr/utiliser-mouse-keyboard-azerty-arduino-pro-micro-teensy/ pour la compréhension conversion qwerty <--> azerty
// http://forum.arduino.cc/index.php?topic=179548.0 pour l'utilisation du pad numérique, servira pour l'unicode alt+xxxx
//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Valeur des constantes des leds pour info et présentation de la fonction permettant de récupérer l'état des leds du clavier
// #define LED_CAPS_LOCK      0x02
// #define LED_NUM_LOCK       0x01
// #define LED_SCROLL_LOCK    0x04
// bool getLedStatus(uint8_t led);
//--------------------------------------------------------------------------------------------------------------------------------------------------------
#include <Keyboard.h>
#include "Mouse.h"
// Tous les scancodes de keyQuerty et keyDead correspondent a des caractères ASCII qui tiennent sur un octet
                  
         //  Alt Gr azerty                   €                                                                    ~  #  {  [  |  `  \  ^  @    ]  }     ¤       
         //   Shift azerty       Q  B  C  D  E  F  G  H  I  J  K  L  ?  N  O  P  A  R  S  T  U  V  Z  X  Y  Z  1  2  3  4  5  6  7  8  9  0    °  +  ¨  £  µ  No fr  M  %  NONE  .  /  §    >
         //         azerty       q  b  c  d  e  f  g  h  i  j  k  l  ,  n  o  p  a  r  s  t  u  v  z  x  y  z  &  é  "  '  (  -  è  _  ç  à    )  =  ^  $  *  No fr  m  ù   ²    ;  :  !    <
         //         qwerty       a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z  1  2  3  4  5  6  7  8  9  0    -  =  [  ]  \  No US  ;  '   `    ,  .  /   No US      
const byte scancode[] PROGMEM = {4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,  45,46,47,48,49,  50,  51,52, 53,  54,55,56,  100}; 
                    
// Dead key (combinaison de deux touches produisant un ou deux caractères), pour les voyelles, traitement dans cette chaîne
                     // azerty   a   e   i   o   u   y         
const byte keyDead[] PROGMEM = {20,  8, 12, 18, 24, 28};
int first=0;  
void sendkeycommand(byte key){
  //#define KEY_UP_ARROW    0xDA
  //#define KEY_DOWN_ARROW    0xD9
  //#define KEY_LEFT_ARROW    0xD8
  //#define KEY_RIGHT_ARROW   0xD7
  //#define KEY_BACKSPACE   0xB2
  //#define KEY_TAB       0xB3
  //#define KEY_RETURN      0xB0
  //#define KEY_ESC       0xB1
  //#define KEY_INSERT      0xD1
  //#define KEY_DELETE      0xD4
  //#define KEY_PAGE_UP     0xD3
  //#define KEY_PAGE_DOWN   0xD6
  //#define KEY_HOME      0xD2
  //#define KEY_END       0xD5
  //#define KEY_CAPS_LOCK   0xC1
  //#define KEY_F1        0xC2
  //#define KEY_F2        0xC3
  //#define KEY_F3        0xC4
  //#define KEY_F4        0xC5
  //#define KEY_F5        0xC6
  //#define KEY_F6        0xC7
  //#define KEY_F7        0xC8
  //#define KEY_F8        0xC9
  //#define KEY_F9        0xCA
  //#define KEY_F10       0xCB
  //#define KEY_F11       0xCC
  //#define KEY_F12       0xCD
    Keyboard.write(key);
    delay(5);
}

void SendKey(byte key, byte control, byte alt, byte shift){
  
    if (control==1){
      Keyboard.press(KEY_LEFT_CTRL);
    }

    if (alt==1){
      Keyboard.press(KEY_LEFT_ALT);
    }

    if (shift==1){
        Keyboard.press(KEY_LEFT_SHIFT);
    }
   
    Keyboard.press(key+136);
    delay(5);
    Keyboard.release(key+136);

    if (shift==1){
      Keyboard.release(KEY_LEFT_SHIFT);
    }
    
    if (alt==1){
      Keyboard.release(KEY_LEFT_ALT);
    }
    
    if (control==1){
      Keyboard.release(KEY_LEFT_CTRL);
    }
    delay(5);
}



byte indexOfCharacter(const String &Character, byte &index, byte &sequence){
    // Les chaines de caractères UTF-8 sont composées de 1 ou plusieurs octets
    // C'est pourquoi la variable Character est transmise comme une chaîne et non comme un char
    // La fonction calcul la position du caractère comme si tous les caractères tenaient sur 1 seul octet 
    // pour une comparaison future avec une autre chaine de référence ASCII dont tous les caractères tiennent par nature sur 1 octet

    const String keyAzertyAltGr      = F("0000€0000000000000000000000~#{[|`\\^@]}0¤");
    const String keyAzertyShift      = F("QBCDEFGHIJKL?NOPARSTUVZXYZ1234567890°+¨£µ0M%0./§>");
    const String keyAzerty           = F("qbcdefghijkl,noparstuvzxyz&é\"'(-è_çà)=^$*0mù²;:!<");
    const String circomflexNoShift   = F("âêîôû");
    const String circomflexShift     = F("ÂÊÎÔÛ");
    const String tremaNoShift        = F("äëïöüÿ");
    const String tremaShift          = F("ÄËÏÖÜ");
    
    // il y a un doublon avec le caractère ^ qui peut être soit interprété comme un accent ou soit comme une puissance
    // il apparait dans la chaine keyAzertyAltGr et dans la chaine keyAzerty
    // si le caractère ^ apparait seul, il sera traité comme une puissance, cas le plus intéressant pour nous
    // par conséquent le chaîne keyAzertyAltGr sera traité avant la chaîne keyAzerty

    // \ est un caractère d'échappement, donc \\ sera transformé en \ et  \" sera transformé en "
     
    String r="", compare="";
    byte c, align, indice=0;

    compare.reserve(50);
    r.reserve(4);
    
    if (keyAzertyShift.indexOf(Character)>-1){ 
      compare=keyAzertyShift;
      sequence=2;
    }else if (keyAzertyAltGr.indexOf(Character)>-1){ 
      compare=keyAzertyAltGr;
      sequence=3;
    }else if (keyAzerty.indexOf(Character)>-1){
      compare=keyAzerty;
      sequence=1;
    }else if (circomflexNoShift.indexOf(Character)>-1){ 
      compare=circomflexNoShift;
      sequence=4;
    }else if (circomflexShift.indexOf(Character)>-1){ 
      compare=circomflexShift;
      sequence=5;
    }else if (tremaNoShift.indexOf(Character)>-1){ 
      compare=tremaNoShift;
      sequence=6;
    }else if (tremaShift.indexOf(Character)>-1){ 
      compare=tremaShift;
      sequence=7;
    }

    if (compare!=""){
        for (unsigned int i=0; i<compare.length(); i++){
            align=0;
            c=compare.charAt(i);
            if (c<128){                  // 1 octet
              align=1;
            }else if (c>191 && c<224){   // 2 octets
              align=2;
            }else if (c>223 && c<240){   // 3 octets
              align=3;
            }else if (c>239 && c<248){   // 4 octets (section qui ne sera pas utilisé dans notre cas)
              align=4;
            }
    
            if (align!=0){     
              r=compare.substring(i, i+align);
              if (r==Character){
                index=indice;
                return 1;
              }
            }else{
              return 0;
            } 
            i=i+align-1;
            indice+=1;
        } 
    }else{
        return 0;
    } 
    return 0;  
}

void sendkeypad(const String &keys){
  // Trick by johnwasser
  // add 136 to the keycode
  byte test,keyc;
  String key; 
  
  if (keys.length()<255){
     for (byte i=0; i < keys.length(); i++){
        key = keys.substring(i, i + 1);
        
        test=1;
        if (key=="1"){
          keyc=225;
        }else if (key=="2"){
          keyc=226;
        }else if (key=="3"){
          keyc=227;
        }else if (key=="4"){
          keyc=228;
        }else if (key=="5"){
          keyc=229;
        }else if (key=="6"){
          keyc=230;
        }else if (key=="7"){
          keyc=231;
        }else if (key=="8"){
          keyc=232;
        }else if (key=="9"){
          keyc=233;
        }else if (key=="0"){
          keyc=234; 
        }else if (key=="/"){
          keyc=220;
        }else if (key=="*"){
          keyc=221;
        }else if (key=="-"){
          keyc=222;
        }else if (key=="+"){
          keyc=223;
        }else if (key=="E"){ // touche enter du clavier
          keyc=224;
        }else{
          test=0;
        }
    
        if (test==1){
          Keyboard.press(keyc);
          delay(5);
          Keyboard.release(keyc);
        }
     }
  }
}

void altunicode(const String &keys){
  if (keys.length()<5){
    Keyboard.press(KEY_LEFT_ALT);
    sendkeypad(keys);
    Keyboard.release(KEY_LEFT_ALT);
    delay(5);
  }
}

void altspecialkey(unsigned long specialkey){
  if (specialkey==0xC387){ 
      altunicode("128"); // Ç
  } else if (specialkey==0xC389){
      altunicode("144"); // É
  } else if (specialkey==0xC3A6){
      altunicode("145"); // æ
  } else if (specialkey==0xC3B8){
      altunicode("155"); // ø
  } else if (specialkey==0xC398){
      altunicode("157"); // Ø
  } else if (specialkey==0xC593){
      altunicode("0156"); // œ
  } else if (specialkey==0xC388){
      altunicode("212"); // È
  } else if (specialkey==0xE296BA){
      altunicode("16"); // ►
  } else if (specialkey==0xE29784){
      altunicode("17"); // ◄
  } else if (specialkey==0xE286A8){
      altunicode("23"); // ↨
  } else if (specialkey==0xE28691){
      altunicode("24"); // ↑
  } else if (specialkey==0xE28693){
      altunicode("25"); // ↓
  } else if (specialkey==0xE28692){
      altunicode("26"); // →
  } else if (specialkey==0xE28690){
      altunicode("27"); // ←
  } else if (specialkey==0xE28694){
      altunicode("29"); // ↔
  } else if (specialkey==0xE296B2){
      altunicode("30"); // ▲
  } else if (specialkey==0xE296BC){
      altunicode("31"); // ▼
  } else if (specialkey==0xE298BA){
      altunicode("1"); // ☺
  } else if (specialkey==0xE298BB){
      altunicode("2"); // ☻
  } else if (specialkey==0xE299A5){
      altunicode("3"); // ♥
  } else if (specialkey==0xE299A6){
      altunicode("4"); // ♦
  } else if (specialkey==0xE299A3){
      altunicode("5"); // ♣
  } else if (specialkey==0xE299A0){
      altunicode("6"); // ♠
  } else if (specialkey==0xE29982){
      altunicode("11"); // ♂
  } else if (specialkey==0xE29980 ){
      altunicode("12"); // ♀
  } else if (specialkey==0xE299AA){
      altunicode("13"); // ♪
  } else if (specialkey==0xE299AB ){
      altunicode("14"); // ♫
  } else if (specialkey==0xE298BC){
      altunicode("15"); // ☼
  } else if (specialkey==0xE2809C ){
      altunicode("0147"); // “
  } else if (specialkey==0xE2809D){
      altunicode("0148"); // ”
  } else if (specialkey==0xE284A2){
      altunicode("0153"); // ™
  } else if (specialkey==0xC2A9){
      altunicode("0169"); // ©
  } else if (specialkey==0xC2AE ){
      altunicode("0174"); // ®
  }
}

void decodeUTF8(const String & Texte){
    String r="";
    byte c, ctrl, alt, shift;
    byte indice, index, sequence, align;
    unsigned long specialkey;
      
    for (byte i=0; i<Texte.length(); i++)
    {
        ctrl=0, alt=0, shift=0;
        align=0;
        specialkey=0;
        c = Texte.charAt(i);
        if (c<128){
          // 1 octet
          align=1;
        }else if (c>191 && c<224){
          // 2 octets
          align=2;
        }else if (c>223 && c<240){
          // 3 octets
          align=3;
        }else if (c>239 && c<248){ // section qui ne sera pas utilisé dans notre cas
          // 4 octets 
          align=4;
        }

        if (align!=0){
            indice = i;
            i=i+align-1;  
            r=Texte.substring(indice, indice + align);
         
            // traitement particulier de certaines commandes et caractères
            if (r==" "){
              sendkeycommand(0x2c+136);
            }else if (c==9){
              sendkeycommand(KEY_TAB);
            }else if (c==13){
              sendkeycommand(KEY_RETURN);
            }else{
                if (indexOfCharacter(r, index, sequence)>0){  
                    if (sequence==1){         // normal
                        shift=0;
                    }else if (sequence==2){   // shift
                        shift=1;
                    }else if (sequence==3){   // altgr  
                        ctrl=1, alt=1;
                        // delay supplémentaire pour dead key simple avec Alt Gr
                        if (index==31 || index==36){
                          // delay supplémentaire
                          //delay(5);
                        }
                    // à partir de là, traitement des dead keys ^ et ¨
                    }else if (sequence==4){   // circomflexe
                        SendKey(47, 0, 0, 0);
                       
                    }else if (sequence==5){  // circomflexe + shift caractère                   
                        SendKey(47, 0, 0, 0);
                        shift=1;

                    }else if (sequence==6){  // tréma     
                        SendKey(47, 0, 0, 1); 

                    }else if (sequence==7){  // tréma + shift caractère
                        SendKey(47, 0, 0, 1); 
                        shift=1;  
                    }
                    
                    if (sequence<4){
                        //c=scancode[index];
                        c=pgm_read_byte_near(scancode+index);
                        SendKey(c, ctrl, alt, shift); 
                    }else{
                        //c=keyDead[index];
                        c=pgm_read_byte_near(keyDead+index);
                        SendKey(c, ctrl, alt, shift);          
                    }
                }else{
                  if (align>1){
                    for (byte j=0; j<align; j++){
                      c=Texte.charAt(indice+j);
                      specialkey=(specialkey<<8);
                      specialkey+=c;
                    }
                    altspecialkey(specialkey);
                  }
                }
            }   
        }  
    }       
}




void sendtext(const String &Texte){
  if (Texte.length()<255){
    decodeUTF8(Texte);
  }
}

void sendtextln(const String &Texte){
  if (Texte.length()<255){
    decodeUTF8(Texte);
    sendkeycommand(KEY_RETURN);
  }
}

void sendkeycombi(byte key1, char key){
  String keys;
  keys+=key;
  Keyboard.press(key1);
  sendtext(keys);
  Keyboard.release(key1);
  delay(280);
}

void selectall(){
  sendkeycombi(KEY_LEFT_CTRL,'a');
  delay(280);
}
void cut(){
  sendkeycombi(KEY_LEFT_CTRL, 'x');
  delay(280);
}
void copy(){
  sendkeycombi(KEY_LEFT_CTRL, 'c');
  delay(280);
}
void paste(){
  sendkeycombi(KEY_LEFT_CTRL, 'v');
  delay(280);
}
void homekey(){
    sendkeycommand(KEY_HOME);
}
void endkey(){
    sendkeycommand(KEY_END);
}
void leftkey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_LEFT_ARROW);
  }
}

void rightkey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_RIGHT_ARROW);
  }
}
void upkey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_UP_ARROW);
  }
}
void downkey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_DOWN_ARROW);
  }
}

void returnkey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_RETURN);
  }
}

void backspacekey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_BACKSPACE);
  }
}

void deletekey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_DELETE);
  }
}

void tabkey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_TAB);
  }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void DuckyScript(String comm) {
  // Ducky Script Hack5 commands interpreter Routine
  // getValue(String data, char separator, int index)
  String Dcom = getValue(comm, ' ',0); // lit la la commande
  Serial.println(Dcom + " command ...");
  Serial.print("parameters : ");
    if (Dcom=="REM") {
      // do nothing
       }
    if (Dcom=="DELAY") {
      // DELAY COMMAND
      String param = comm;
      param.remove(0,6);
      Serial.println(param);
      int temps=param.toInt();
      delay(temps);
    }
    if (Dcom=="STRING") {
      // STRING COMMAND
      comm.remove(0,7);
      Serial.println(comm);
      if (KeyLayout == "FR") {
        sendtext(comm);
      } else {
       Keyboard.print(comm); 
      }
    }    
    if (Dcom=="GUI" or Dcom=="WINDOWS") {
      // GUI COMMAND
      if (Dcom=="GUI") {
        comm.remove(0,4);
      } else {
        comm.remove(0,8);
      }
      Keyboard.press(KEY_LEFT_GUI);
      if (comm!="") {

         if (KeyLayout == "FR") {
            sendtext(comm);
          } else { 
            Serial.println("complement : " + comm);
            char buff[10];
            comm.toCharArray(buff,10);
            Keyboard.press(buff[0]);
          }
        
      }
      Keyboard.releaseAll(); 
    }  
    if (Dcom=="MENU" or Dcom=="APP") {
      // MENU COMMAND SHIFT+F10
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_F10);
      Keyboard.releaseAll(); 
    }         
    if (Dcom=="UP" or Dcom=="UPARROW") {
      // UP COMMAND
      Keyboard.press(KEY_UP_ARROW);
      Keyboard.releaseAll(); 
    }               
    if (Dcom=="DOWN" or Dcom=="DOWNARROW") {
      // DOWN COMMAND
      Keyboard.press(KEY_DOWN_ARROW);
      Keyboard.releaseAll(); 
    }   
    if (Dcom=="LEFT" or Dcom=="LEFTARROW") {
      // LEFT COMMAND
      Keyboard.press(KEY_LEFT_ARROW);
      Keyboard.releaseAll(); 
    }
    if (Dcom=="RIGHT" or Dcom=="RIGHTARROW") {
      // RIGHT COMMAND
      Keyboard.press(KEY_RIGHT_ARROW);
      Keyboard.releaseAll(); 
    }   
    if (Dcom=="ENTER" or Dcom=="RETURN") {
      // ENTER COMMAND
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll(); 
    }      
    if (Dcom=="CAPSLOCK") {
      // CAPSLOCK COMMAND
      Keyboard.press(KEY_CAPS_LOCK);
      Keyboard.releaseAll(); 
    }   
    if (Dcom=="DELETE") {
      // DELETE COMMAND
      Keyboard.press(KEY_DELETE);
      Keyboard.releaseAll(); 
    }   
    if (Dcom=="END") {
      // END COMMAND
      Keyboard.press(KEY_END);
      Keyboard.releaseAll(); 
    }     
    if (Dcom=="ESC" or Dcom=="ESCAPE") {
      // ESC COMMAND
      Keyboard.press(KEY_ESC);
      Keyboard.releaseAll(); 
    } 
    if (Dcom=="HOME") {
      // HOME COMMAND
      Keyboard.press(KEY_HOME);
      Keyboard.releaseAll(); 
    }   
    if (Dcom=="INSERT") {
      // INSERT COMMAND
      Keyboard.press(KEY_INSERT);
      Keyboard.releaseAll(); 
    }    
    if (Dcom=="PAGEUP") {
      // PAGEUP COMMAND
      Keyboard.press(KEY_PAGE_UP);
      Keyboard.releaseAll(); 
    }   
    if (Dcom=="PAGEDOWN") {
      // PAGEDOWN COMMAND
      Keyboard.press(KEY_PAGE_DOWN);
      Keyboard.releaseAll(); 
    }         
    if (Dcom=="SPACE") {
      // SPACE COMMAND
      Keyboard.press(' ');
      Keyboard.releaseAll(); 
    }       
    if (Dcom=="TAB") {
      // TAB COMMAND
      Keyboard.press(KEY_TAB);
      Keyboard.releaseAll(); 
    } 
         
}
