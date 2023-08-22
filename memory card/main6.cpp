
#include <ESPAsyncWebServer.h>
#include <WiFi.h> 
#include <SPI.h> 
#include <mySD.h>

String serverIndex =  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>" " 
<form method='POST' action ='#' enctype='multipart/form-data' id='upload_form'>" " 
    <input type='file' name='update'>" 
    "<input type='submit' value='Upload'>" 
"</form>" 
"<div id='prg'>progresso: 0%</div>" " 
<script>" 
"$('form').submit(function(e){" 
    "e.preventDefault( );" 
      "var form = $('#upload_form')[0];" 
      "var data = new FormData(form);" 
      " $.ajax({" 
            "url: '/update'," 
            "type: 'POST',"                
            "data: data," 
            "contentType: false,"                   
            "processData:false,"   
            "xhr: function() {"
                "var xhr = nova janela.XMLHttpRequest();" 
                "xhr.upload.addEventListener('progress', function(evt) {" 
                    "if (evt.lengthComputable) {" 
                        "var per = evt.loaded / evt.total;" 
                        "$('#prg').html( 'progresso: ' + Math.round(per*100) + '%');" 
                    "}" 
               "}, false);" 
               "retornar xhr;" 
            "},"                                 
            "success:function(d, s) {"     
                "console.log('success!')" 
           "}," 
            "error: function (a, b, c) {" 
            "}" 
          "}); " 
"});" 
"</script>" ;

const  char * ssid =  "TRUONG AN" ;
const  char * senha =  "0909505150" ;
servidor 
ESP32WebServer ( 80 );
Raiz do arquivo;
bool aberto =  false ;

String printDirectory (arquivo dir, int numTabs) {
  String resposta =  "" ;
  dir.rewindDirectory();
  
  enquanto ( verdadeiro ) {
     Entrada de arquivo =   dir.openNextFile();
     if ( ! entrada) {
        // sem mais arquivos 
       //Serial.println("**nomorefiles**"); 
       quebrar ;
     }
     for ( uint8_t i = 0 ; i < numTabs; i ++ ) {
       Serial.print( '\t' );   // teremos uma boa indentação
     }
     // Recurse para diretórios, senão imprime o tamanho do arquivo 
     if (entry.isDirectory()) {
       printDirectory(entrada, numTabs + 1 );
     } senão {
       resposta += String( " <a href='" ) + String(entry.name()) + String( "'> " ) + String(entry.name()) + String( "</a>" ) + String( "</br>" );
     }
     entrada.fechar();
   }
   return String( "Listar arquivos:</br>" ) + resposta + String( "</br></br> Carregar arquivo:" ) + serverIndex;
}

void  handleRoot () {
  root = SD.open( "/" );
  String res = printDirectory(raiz, 0 );
  server.send( 200 , "text/html" , res);
}

bool  loadFromSDCARD (caminho da string){
  path.toLowerCase();
  String dataType =  "texto/simples" ;
  if (path.endsWith( "/" )) path +=  "index.htm" ;

  if (path.endsWith( ".src" )) path = path.substring( 0 , path.lastIndexOf( "." ));
  else  if (path.endsWith( ".jpg" )) dataType =  "image/jpeg" ;
  else  if (path.endsWith( ".txt" )) dataType =  "text/plain" ;
  else  if (path.endsWith( ".zip" )) dataType =  "aplicativo/zip" ;  
  Serial.println(dataType);
  Arquivo dataFile = SD.open(path.c_str());

  if ( ! dataFile)
     return  false ;

  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    Serial.println( "Enviou menos dados do que o esperado!" );
  }

  dataFile.close();
  retorna  verdadeiro ;
}

void  handleNotFound (){
   if (loadFromSDCARD(server.uri())) return ;
  String message =  "SDCARD não detectado \n\n " ;
  mensagem +=  "URI: " ;
  mensagem += servidor.uri();
  mensagem +=  " \n Método: " ;
  mensagem += (server.method() == HTTP_GET) ? "GET" : "POST" ;
  mensagem +=  " \n Argumentos: " ;
  mensagem += server.args();
  mensagem +=  " \n " ;
  for ( uint8_t i = 0 ; i < server.args(); i ++ ){
    mensagem +=  " NOME:" + servidor.argNome(i) +  " \n VALOR:"  + servidor.arg(i) +  " \n " ;
  }
  server.send( 404 , "texto/simples" , mensagem);
  Serial.println(mensagem);
}

 configuração nula ( void ){
  Serial.begin( 115200 );
  WiFi.begin(ssid, senha);
  Serial.println( "" );

  // Espera conexão 
  while (WiFi.status() != WL_CONNECTED) {
    atraso( 500 );
    Serial.print( "." );
  }
  Serial.println( "" );
  Serial.print( "Conectado a " );
  Serial.println(ssid);
  Serial.print( "Endereço IP: " );
  Serial.println(WiFi.localIP());
  
  //use IP ou iotsharing.local para acessar o servidor web 
  if (MDNS.begin( "iotsharing" )) {
    Serial.println( "Responder MDNS iniciado" );
  }
  if ( ! SD.begin( 26 , 14 , 13 , 27 )) {
    Serial.println( "falha na inicialização!" );
    retorno ;
  }
  Serial.println( "inicialização concluída." );
  //manipular uri   
  server.on( "/" , handleRoot);
  server.onNotFound(handleNotFound);

  /*manipulando o upload do arquivo */ 
  server.on( "/update" , HTTP_POST, [](){
    server.sendHeader( "Conexão" , "fechar" );
  },[](){
    HTTPUpload & upload = server.upload();
    if (aberto ==  false ){
      aberto =  verdadeiro ;
      root = SD.open((String("/") + upload.filename).c_str(), FILE_WRITE);  
      if ( ! raiz){
        Serial.println( "- falha ao abrir arquivo para gravação" );
        retorno ;
      }
    }
    if (upload.status == UPLOAD_FILE_WRITE){
       if (root.write(upload.buf, upload.currentSize) != upload.currentSize){
        Serial.println( "- falhou ao escrever" );
        retorno ;
      }
    } else  if (upload.status == UPLOAD_FILE_END){
      root.close();
      Serial.println( "UPLOAD_FILE_END" );
      aberto =  falso ;
    }
  });
  server.begin();
  Serial.println( "Servidor HTTP iniciado" );
}

 loop vazio ( void ) {
  server.handleClient();
}