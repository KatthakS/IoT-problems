//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555


const char *ssid = "MyESP32AP";
const char *password = "testpassword";
/* Put IP Address details */
WiFiServer server(80);
Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
String led_value;
String moisture_value;
String temp_value;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("Awesome! startHere: Received from %u msg=%s\n", from, msg.c_str());
  String str = msg.c_str();
  //Serial.println(recv_string);
  int occ = 0, i;
  char ch = ',';
  int count = 2;
  for (i = 0; i < str.length(); i++) { 
  
        // Increment occ if current char is equal 
        // to given character 
        if (str[i] == ch) 
            occ++; 
  
        // Break the loop if given character has 
        // been occurred given no. of times 
        if (occ == count) 
            break; 
    } 
    //Serial.println(str.substring(i+1, str.length()));
    if(str.startsWith("G")) {
      temp_value = str.substring(i+1, str.length());
      //Serial.print("temp_value = ");
      //Serial.println(temp_value);
    } else if (str.startsWith("P")) {
      led_value = str.substring(i+1, str.length());
      //Serial.print("led_value = ");
      //Serial.println(led_value);
    } else if (str.startsWith("M")) {
      moisture_value = str.substring(i+1, str.length());
     // Serial.print("moisture_value = ");
     // Serial.println(moisture_value);
    }

}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  // it will run the user scheduler as well
   WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    
    while (client.connected()) {
      //Serial.println(WiFi.softAPgetStationNum());
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
         if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("</html>");
          client.print("PIR:");
          client.println(led_value);
          client.print("\nBME680:");
          client.println(temp_value);
          client.print("\nMoisture:");
          client.println(moisture_value);
          break;
        }
        
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
        
      }
    }
  }
  mesh.update();
}


  