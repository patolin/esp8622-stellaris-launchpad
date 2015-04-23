/*
 Servidor web con ESP8266
 Programado con Arduino IDE 1.6.1 sobre el chip ESP8266
 */


#include <ESP8266WiFi.h>



int temperatura;
int val=0;
int estado=0;

// Requests al puerto 80
WiFiServer server(80);

void setup(void)
{  
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP8266");
  WiFi.printDiag(Serial);
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  server.begin();
  Serial.println("HTTP server started");
  

}

void loop() {
  // Chequeamos si se ha conectado un cliente
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  while(!client.available()){
    delay(1);
  }
  
  String req = client.readStringUntil('\r');
  client.flush();
  
  // verificamos la opcion
  estado=0;
  if (req.indexOf("/led/off") != -1)
    val = 0;
  else if (req.indexOf("/led/rojo") != -1)
    val = 1;
  else if (req.indexOf("/led/verde") != -1)
    val = 2;
  else if (req.indexOf("/led/azul") != -1)
    val = 3;
  else if (req.indexOf("/status") != -1)
    estado = 1;
  else {
    client.stop();
    return;
  }

  
  if (estado==0) {
    // enviamos por el serial el cambio del led
    Serial.print(val);  
  } else {
    Serial.print('s');  
    delay(250);
    if (Serial.available()) {
      char c=Serial.read();  
      temperatura=c;
    }
  }

  
  client.flush();

  // Generamos la respuesta
  String s;
  if (estado==0) {
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nEl Led esta: ";
    switch (val) {
      case 0:
        s+="Apagado";
        break;
      case 1:
        s+="Rojo";
        break;  
      case 2:
        s+="Verde";
        break;
      case 3:
        s+="Azul";
        break;
    }
    s += "</html>\n";
  } else {
     s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nLed: ";
     switch (val) {
      case 0:
        s+="Apagado";
        break;
      case 1:
        s+="Rojo";
        break;  
      case 2:
        s+="Verde";
        break;
      case 3:
        s+="Azul";
        break;
    }
    s+="<br/>\r\n";
    s+="Temp: ";
    s+=temperatura;
    s+=" grados";
    s += "</html>\n";
      
  }
  // Enviamos la respuesta al cliente
  client.print(s);
  delay(1);
}

