#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include "FS.h"
#include "SPIFFS.h"

String serverIndex = "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
    "<input type='file' name='update'>"
    "<input type='submit' value='Upload'>"
    
"</form>"
"<form method='POST' action='/date' enctype='multipart/form-data' id='download_form'>"
    "<input type='file' name='download'>"
    "<input type='submit' value='Donwload'>"
"</form>"
"<div id='prg'>progress: 0%</div>"
"<script>"
"$('form').submit(function(e){"
    "e.preventDefault();"
      "var form = $('#upload_form')[0];"
      "var data = new FormData(form);"
      " $.ajax({"
            "url: '/update',"
            "type: 'POST',"               
            "data: data,"
            "contentType: false,"                  
            "processData:false,"  
            "xhr: function() {"
                "var xhr = new window.XMLHttpRequest();"
                "xhr.upload.addEventListener('progress', function(evt) {"
                    "if (evt.lengthComputable) {"
                        "var per = evt.loaded / evt.total;"
                        "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
                    "}"
               "}, false);"
               "return xhr;"
            "},"                                
            "success:function(d, s) {"    
                "console.log('success!')"
           "},"
            "error: function (a, b, c) {"
            "}"
          "});"
"});"
"</script>";

const char* ssid = "teste";
const char* password = "123456789";
bool opened = false;
File file;

#define FORMAT_SPIFFS_IF_FAILED true
ESP32WebServer server(80);

String listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    String response = "";
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return "";
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return "";
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
            response += String("<a href='") + String(file.name()) + String("'>") + String(file.name()) + String("</a>") + String("</br>");
        }
        file = root.openNextFile();
    }
    return String("List files:</br>") + response + String("</br></br> Upload file:") + serverIndex;
}

void handleRoot() {
  String res = listDir(SPIFFS, "/", 0);
  server.send(200, "text/html", res);
}

bool loadFromSPIFFS(String path){
  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.htm";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".txt")) dataType = "text/plain";
  else if(path.endsWith(".zip")) dataType = "application/zip";  

  File dataFile = SPIFFS.open(path.c_str());

  if (!dataFile)
    return false;

  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    Serial.println("Sent less data than expected!");
  }

  dataFile.close();
  return true;
}

void handleNotFound(){
  if(loadFromSPIFFS(server.uri())) return;
  String message = "SDCARD Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}

void setup(void){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  //use IP or iotsharing.local to access webserver
  if (MDNS.begin("iotsharing")) {
    Serial.println("MDNS responder started");
  }
  //init spiffs
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  //handle uri  
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  /*handling uploading file */
  server.on("/update", HTTP_POST, [](){
    server.sendHeader("Connection", "close");
    opened = false;
  },[](){
    HTTPUpload& upload = server.upload();
    if(opened == false){
        opened = true;
        file = SPIFFS.open(String("/") + upload.filename, FILE_WRITE);
        if(!file){
            Serial.println("- failed to open file for writing");
            return;
        }
    } 
    if(upload.status == UPLOAD_FILE_WRITE){
      if(file.write(upload.buf, upload.currentSize) != upload.currentSize){
        Serial.println("- failed to write");
        return;
      }
    } else if(upload.status == UPLOAD_FILE_END){
        opened = false;
    }
  });
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}