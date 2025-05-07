#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "ARDUINO-CAR";
WiFiServer server(80);

// Définition des broches pour les moteurs
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

void setup() {
  Serial.begin(9600);
  
  // Configuration des broches moteur
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Démarrer le point d'accès
  if (WiFi.beginAP(ssid) != WL_AP_LISTENING) {
    Serial.println("Erreur lors du démarrage de l'AP");
    while (true);
  }
  
  Serial.println("AP démarré !");
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Nouveau client !");
    String req = "";
    boolean currentLineIsBlank = true;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        req += c;
        Serial.write(c);
        
        // Fin de la requête
        if (c == '\n' && currentLineIsBlank) {
          // TRAITEMENT DE LA REQUÊTE
          if (req.indexOf("GET /forward") >= 0) {
            avancer();
          } else if (req.indexOf("GET /backward") >= 0) {
            reculer();
          } else if (req.indexOf("GET /left") >= 0) {
            tournerGauche();
          } else if (req.indexOf("GET /right") >= 0) {
            tournerDroite();
          } else if (req.indexOf("GET /stop") >= 0) {
            arreter();
          }
          
          // Réponse HTML
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          
          // HTML de l'interface améliorée
          client.println("<!DOCTYPE html>");
          client.println("<html lang='fr'>");
          client.println("<head>");
          client.println("  <meta charset='UTF-8'>");
          client.println("  <meta name='viewport' content='width=device-width, initial-scale=1.0'>");
          client.println("  <title>Contrôle Voiture RC</title>");
          client.println("  <style>");
          client.println("    body {");
          client.println("      font-family: Arial, sans-serif;");
          client.println("      margin: 0;");
          client.println("      padding: 0;");
          client.println("      background-color: #f5f5f5;");
          client.println("      display: flex;");
          client.println("      flex-direction: column;");
          client.println("      align-items: center;");
          client.println("      justify-content: center;");
          client.println("      min-height: 100vh;");
          client.println("    }");
          client.println("    .container {");
          client.println("      max-width: 500px;");
          client.println("      width: 90%;");
          client.println("      background-color: white;");
          client.println("      border-radius: 15px;");
          client.println("      box-shadow: 0 4px 8px rgba(0,0,0,0.1);");
          client.println("      padding: 20px;");
          client.println("      margin: 20px;");
          client.println("    }");
          client.println("    h1 {");
          client.println("      color: #333;");
          client.println("      text-align: center;");
          client.println("      margin-bottom: 20px;");
          client.println("    }");
          client.println("    .status {");
          client.println("      text-align: center;");
          client.println("      margin: 15px 0;");
          client.println("      font-weight: bold;");
          client.println("      height: 30px;");
          client.println("    }");
          client.println("    .controls {");
          client.println("      display: grid;");
          client.println("      grid-template-columns: repeat(3, 1fr);");
          client.println("      grid-template-rows: repeat(3, auto);");
          client.println("      gap: 10px;");
          client.println("      margin: 20px 0;");
          client.println("    }");
          client.println("    .btn {");
          client.println("      background-color: #4CAF50;");
          client.println("      color: white;");
          client.println("      border: none;");
          client.println("      border-radius: 12px;");
          client.println("      padding: 15px 0;");
          client.println("      font-size: 18px;");
          client.println("      cursor: pointer;");
          client.println("      transition: all 0.3s ease;");
          client.println("      display: flex;");
          client.println("      align-items: center;");
          client.println("      justify-content: center;");
          client.println("    }");
          client.println("    .btn:hover {");
          client.println("      transform: scale(1.05);");
          client.println("      box-shadow: 0 2px 4px rgba(0,0,0,0.2);");
          client.println("    }");
          client.println("    .btn:active {");
          client.println("      transform: scale(0.95);");
          client.println("    }");
          client.println("    .btn-forward {");
          client.println("      grid-column: 2;");
          client.println("      grid-row: 1;");
          client.println("    }");
          client.println("    .btn-left {");
          client.println("      grid-column: 1;");
          client.println("      grid-row: 2;");
          client.println("    }");
          client.println("    .btn-stop {");
          client.println("      grid-column: 2;");
          client.println("      grid-row: 2;");
          client.println("      background-color: #f44336;");
          client.println("    }");
          client.println("    .btn-right {");
          client.println("      grid-column: 3;");
          client.println("      grid-row: 2;");
          client.println("    }");
          client.println("    .btn-backward {");
          client.println("      grid-column: 2;");
          client.println("      grid-row: 3;");
          client.println("    }");
          client.println("    .info {");
          client.println("      text-align: center;");
          client.println("      margin-top: 20px;");
          client.println("      font-size: 14px;");
          client.println("      color: #666;");
          client.println("    }");
          client.println("    .triangle-up {");
          client.println("      width: 0;");
          client.println("      height: 0;");
          client.println("      border-left: 10px solid transparent;");
          client.println("      border-right: 10px solid transparent;");
          client.println("      border-bottom: 18px solid white;");
          client.println("    }");
          client.println("    .triangle-down {");
          client.println("      width: 0;");
          client.println("      height: 0;");
          client.println("      border-left: 10px solid transparent;");
          client.println("      border-right: 10px solid transparent;");
          client.println("      border-top: 18px solid white;");
          client.println("    }");
          client.println("    .triangle-left {");
          client.println("      width: 0;");
          client.println("      height: 0;");
          client.println("      border-top: 10px solid transparent;");
          client.println("      border-bottom: 10px solid transparent;");
          client.println("      border-right: 18px solid white;");
          client.println("    }");
          client.println("    .triangle-right {");
          client.println("      width: 0;");
          client.println("      height: 0;");
          client.println("      border-top: 10px solid transparent;");
          client.println("      border-bottom: 10px solid transparent;");
          client.println("      border-left: 18px solid white;");
          client.println("    }");
          client.println("    .square {");
          client.println("      width: 16px;");
          client.println("      height: 16px;");
          client.println("      background-color: white;");
          client.println("    }");
          client.println("  </style>");
          client.println("</head>");
          client.println("<body>");
          client.println("  <div class='container'>");
          client.println("    <h1>Contrôle Voiture RC</h1>");
          client.println("    <div id='status' class='status'>Prêt</div>");
          client.println("    <div class='controls'>");
          client.println("      <button id='forward' class='btn btn-forward' ontouchstart='drive(\"forward\")' ontouchend='drive(\"stop\")' onmousedown='drive(\"forward\")' onmouseup='drive(\"stop\")'><div class='triangle-up'></div></button>");
          client.println("      <button id='left' class='btn btn-left' ontouchstart='drive(\"left\")' ontouchend='drive(\"stop\")' onmousedown='drive(\"left\")' onmouseup='drive(\"stop\")'><div class='triangle-left'></div></button>");
          client.println("      <button id='stop' class='btn btn-stop' onclick='drive(\"stop\")'><div class='square'></div></button>");
          client.println("      <button id='right' class='btn btn-right' ontouchstart='drive(\"right\")' ontouchend='drive(\"stop\")' onmousedown='drive(\"right\")' onmouseup='drive(\"stop\")'><div class='triangle-right'></div></button>");
          client.println("      <button id='backward' class='btn btn-backward' ontouchstart='drive(\"backward\")' ontouchend='drive(\"stop\")' onmousedown='drive(\"backward\")' onmouseup='drive(\"stop\")'><div class='triangle-down'></div></button>");
          client.println("    </div>");
          client.println("    <div class='info'>");
          client.println("      <p>Connecté au réseau: ARDUINO-CAR</p>");
          client.println("      <p>Utilisez les touches flèches du clavier ou les boutons ci-dessus pour contrôler la voiture</p>");
          client.println("    </div>");
          client.println("  </div>");
          client.println("  <script>");
          client.println("    function drive(direction) {");
          client.println("      fetch('/' + direction)");
          client.println("        .then(response => {");
          client.println("          document.getElementById('status').innerText = 'Action: ' + direction;");
          client.println("        })");
          client.println("        .catch(error => {");
          client.println("          document.getElementById('status').innerText = 'Erreur: ' + error;");
          client.println("        });");
          client.println("    }");
          client.println("    // Support du clavier");
          client.println("    document.addEventListener('keydown', function(event) {");
          client.println("      if (event.repeat) return;");
          client.println("      switch(event.key) {");
          client.println("        case 'ArrowUp':    drive('forward'); break;");
          client.println("        case 'ArrowDown':  drive('backward'); break;");
          client.println("        case 'ArrowLeft':  drive('left'); break;");
          client.println("        case 'ArrowRight': drive('right'); break;");
          client.println("        case ' ':          drive('stop'); break;");
          client.println("      }");
          client.println("    });");
          client.println("    document.addEventListener('keyup', function(event) {");
          client.println("      if (['ArrowUp', 'ArrowDown', 'ArrowLeft', 'ArrowRight'].includes(event.key)) {");
          client.println("        drive('stop');");
          client.println("      }");
          client.println("    });");
          client.println("  </script>");
          client.println("</body>");
          client.println("</html>");
          
          break;
        }
        
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    
    delay(1);
    client.stop();
    Serial.println("Client déconnecté.");
  }
}

// === Fonctions moteurs ===
void avancer() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Avancer");
}

void reculer() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Reculer");
}

void tournerGauche() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Gauche");
}

void tournerDroite() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Droite");
}

void arreter() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.println("Arrêt");
}#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "ARDUINO-CAR";
WiFiServer server(80);

// Définition des broches pour les moteurs
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

void setup() {
  Serial.begin(9600);
  
  // Configuration des broches moteur
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Démarrer le point d'accès
  if (WiFi.beginAP(ssid) != WL_AP_LISTENING) {
    Serial.println("Erreur lors du démarrage de l'AP");
    while (true);
  }
  
  Serial.println("AP démarré !");
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Nouveau client !");
    String req = "";
    boolean currentLineIsBlank = true;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        req += c;
        Serial.write(c);
        
        // Fin de la requête
        if (c == '\n' && currentLineIsBlank) {
          // TRAITEMENT DE LA REQUÊTE
          if (req.indexOf("GET /forward") >= 0) {
            avancer();
          } else if (req.indexOf("GET /backward") >= 0) {
            reculer();
          } else if (req.indexOf("GET /left") >= 0) {
            tournerGauche();
          } else if (req.indexOf("GET /right") >= 0) {
            tournerDroite();
          } else if (req.indexOf("GET /stop") >= 0) {
            arreter();
          }
          
          // Réponse HTML
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          
          // HTML de l'interface améliorée
          client.println("<!DOCTYPE html>");
          client.println("<html lang='fr'>");
          client.println("<head>");
          client.println("  <meta charset='UTF-8'>");
          client.println("  <meta name='viewport' content='width=device-width, initial-scale=1.0'>");
          client.println("  <title>Contrôle Voiture RC</title>");
          client.println("  <style>");
          client.println("    body {");
          client.println("      font-family: Arial, sans-serif;");
          client.println("      margin: 0;");
          client.println("      padding: 0;");
          client.println("      background-color: #f5f5f5;");
          client.println("      display: flex;");
          client.println("      flex-direction: column;");
          client.println("      align-items: center;");
          client.println("      justify-content: center;");
          client.println("      min-height: 100vh;");
          client.println("    }");
          client.println("    .container {");
          client.println("      max-width: 500px;");
          client.println("      width: 90%;");
          client.println("      background-color: white;");
          client.println("      border-radius: 15px;");
          client.println("      box-shadow: 0 4px 8px rgba(0,0,0,0.1);");
          client.println("      padding: 20px;");
          client.println("      margin: 20px;");
          client.println("    }");
          client.println("    h1 {");
          client.println("      color: #333;");
          client.println("      text-align: center;");
          client.println("      margin-bottom: 20px;");
          client.println("    }");
          client.println("    .status {");
          client.println("      text-align: center;");
          client.println("      margin: 15px 0;");
          client.println("      font-weight: bold;");
          client.println("      height: 30px;");
          client.println("    }");
          client.println("    .controls {");
          client.println("      display: grid;");
          client.println("      grid-template-columns: repeat(3, 1fr);");
          client.println("      grid-template-rows: repeat(3, auto);");
          client.println("      gap: 10px;");
          client.println("      margin: 20px 0;");
          client.println("    }");
          client.println("    .btn {");
          client.println("      background-color: #4CAF50;");
          client.println("      color: white;");
          client.println("      border: none;");
          client.println("      border-radius: 12px;");
          client.println("      padding: 15px 0;");
          client.println("      font-size: 18px;");
          client.println("      cursor: pointer;");
          client.println("      transition: all 0.3s ease;");
          client.println("      display: flex;");
          client.println("      align-items: center;");
          client.println("      justify-content: center;");
          client.println("    }");
          client.println("    .btn:hover {");
          client.println("      transform: scale(1.05);");
          client.println("      box-shadow: 0 2px 4px rgba(0,0,0,0.2);");
          client.println("    }");
          client.println("    .btn:active {");
          client.println("      transform: scale(0.95);");
          client.println("    }");
          client.println("    .btn-forward {");
          client.println("      grid-column: 2;");
          client.println("      grid-row: 1;");
          client.println("    }");
          client.println("    .btn-left {");
          client.println("      grid-column: 1;");
          client.println("      grid-row: 2;");
          client.println("    }");
          client.println("    .btn-stop {");
          client.println("      grid-column: 2;");
          client.println("      grid-row: 2;");
          client.println("      background-color: #f44336;");
          client.println("    }");
          client.println("    .btn-right {");
          client.println("      grid-column: 3;");
          client.println("      grid-row: 2;");
          client.println("    }");
          client.println("    .btn-backward {");
          client.println("      grid-column: 2;");
          client.println("      grid-row: 3;");
          client.println("    }");
          client.println("    .info {");
          client.println("      text-align: center;");
          client.println("      margin-top: 20px;");
          client.println("      font-size: 14px;");
          client.println("      color: #666;");
          client.println("    }");
          client.println("    .triangle-up {");
          client.println("      width: 0;");
          client.println("      height: 0;");
          client.println("      border-left: 10px solid transparent;");
          client.println("      border-right: 10px solid transparent;");
          client.println("      border-bottom: 18px solid white;");
          client.println("    }");
          client.println("    .triangle-down {");
          client.println("      width: 0;");
          client.println("      height: 0;");
          client.println("      border-left: 10px solid transparent;");
          client.println("      border-right: 10px solid transparent;");
          client.println("      border-top: 18px solid white;");
          client.println("    }");
          client.println("    .triangle-left {");
          client.println("      width: 0;");
          client.println("      height: 0;");
          client.println("      border-top: 10px solid transparent;");
          client.println("      border-bottom: 10px solid transparent;");
          client.println("      border-right: 18px solid white;");
          client.println("    }");
          client.println("    .triangle-right {");
          client.println("      width: 0;");
          client.println("      height: 0;");
          client.println("      border-top: 10px solid transparent;");
          client.println("      border-bottom: 10px solid transparent;");
          client.println("      border-left: 18px solid white;");
          client.println("    }");
          client.println("    .square {");
          client.println("      width: 16px;");
          client.println("      height: 16px;");
          client.println("      background-color: white;");
          client.println("    }");
          client.println("  </style>");
          client.println("</head>");
          client.println("<body>");
          client.println("  <div class='container'>");
          client.println("    <h1>Contrôle Voiture RC</h1>");
          client.println("    <div id='status' class='status'>Prêt</div>");
          client.println("    <div class='controls'>");
          client.println("      <button id='forward' class='btn btn-forward' ontouchstart='drive(\"forward\")' ontouchend='drive(\"stop\")' onmousedown='drive(\"forward\")' onmouseup='drive(\"stop\")'><div class='triangle-up'></div></button>");
          client.println("      <button id='left' class='btn btn-left' ontouchstart='drive(\"left\")' ontouchend='drive(\"stop\")' onmousedown='drive(\"left\")' onmouseup='drive(\"stop\")'><div class='triangle-left'></div></button>");
          client.println("      <button id='stop' class='btn btn-stop' onclick='drive(\"stop\")'><div class='square'></div></button>");
          client.println("      <button id='right' class='btn btn-right' ontouchstart='drive(\"right\")' ontouchend='drive(\"stop\")' onmousedown='drive(\"right\")' onmouseup='drive(\"stop\")'><div class='triangle-right'></div></button>");
          client.println("      <button id='backward' class='btn btn-backward' ontouchstart='drive(\"backward\")' ontouchend='drive(\"stop\")' onmousedown='drive(\"backward\")' onmouseup='drive(\"stop\")'><div class='triangle-down'></div></button>");
          client.println("    </div>");
          client.println("    <div class='info'>");
          client.println("      <p>Connecté au réseau: ARDUINO-CAR</p>");
          client.println("      <p>Utilisez les touches flèches du clavier ou les boutons ci-dessus pour contrôler la voiture</p>");
          client.println("    </div>");
          client.println("  </div>");
          client.println("  <script>");
          client.println("    function drive(direction) {");
          client.println("      fetch('/' + direction)");
          client.println("        .then(response => {");
          client.println("          document.getElementById('status').innerText = 'Action: ' + direction;");
          client.println("        })");
          client.println("        .catch(error => {");
          client.println("          document.getElementById('status').innerText = 'Erreur: ' + error;");
          client.println("        });");
          client.println("    }");
          client.println("    // Support du clavier");
          client.println("    document.addEventListener('keydown', function(event) {");
          client.println("      if (event.repeat) return;");
          client.println("      switch(event.key) {");
          client.println("        case 'ArrowUp':    drive('forward'); break;");
          client.println("        case 'ArrowDown':  drive('backward'); break;");
          client.println("        case 'ArrowLeft':  drive('left'); break;");
          client.println("        case 'ArrowRight': drive('right'); break;");
          client.println("        case ' ':          drive('stop'); break;");
          client.println("      }");
          client.println("    });");
          client.println("    document.addEventListener('keyup', function(event) {");
          client.println("      if (['ArrowUp', 'ArrowDown', 'ArrowLeft', 'ArrowRight'].includes(event.key)) {");
          client.println("        drive('stop');");
          client.println("      }");
          client.println("    });");
          client.println("  </script>");
          client.println("</body>");
          client.println("</html>");
          
          break;
        }
        
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    
    delay(1);
    client.stop();
    Serial.println("Client déconnecté.");
  }
}

// === Fonctions moteurs ===
void avancer() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Avancer");
}

void reculer() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Reculer");
}

void tournerGauche() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Gauche");
}

void tournerDroite() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Droite");
}

void arreter() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.println("Arrêt");
}