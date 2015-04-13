/*
 Servidor web con ESP8266
 Programado con Arduino IDE 1.6.1 sobre el chip ESP8266
 */


#include <ESP8266WiFi.h>



int temperatura;
int val=0;
int estado=0;

// TCP server at port 80 will respond to HTTP requests
WiFiServer server(80);

void setup(void)
{  
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP8266");
  WiFi.printDiag(Serial);
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  
  //server.on("/", handle_root);
  
  /*
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });
  */
  server.begin();
  Serial.println("HTTP server started");
  

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  //Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  //Serial.println(req);
  client.flush();
  
  // Match the request

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
    //Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
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

  // Prepare the response
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
    s += "</html>\n";
      
  }
  // Send the response to the client
  client.print(s);
  delay(1);
  //Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

