#ifndef CREDENCIAIS_H
#define CREDENCIAIS_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <vector>

// Estrutura para armazenar cada credencial
struct Credencial {
    String user;
    String pass;
};

// Declaração do vetor global de credenciais
extern std::vector<Credencial> credenciais;

// Função para salvar uma credencial no arquivo SPIFFS
void salvarCredenciais(String user, String pass) {
    File file = SPIFFS.open("/credenciais.txt", "a");
    if (file) {
        file.printf("User: %s | Pass: %s\n", user.c_str(), pass.c_str());
        file.close();
    } else {
        Serial.println("Erro ao salvar as credenciais no SPIFFS");
    }
}

// Função para adicionar uma nova credencial ao vetor e salvar no SPIFFS
void adicionarCredencial(String user, String pass) {
    Credencial novaCredencial = {user, pass};
    credenciais.push_back(novaCredencial);
    salvarCredenciais(user, pass);
}

// Função para carregar credenciais salvas no arquivo para o vetor ao iniciar
void carregarCredenciais() {
    if (!SPIFFS.exists("/credenciais.txt")) {
        Serial.println("Nenhum arquivo de credenciais encontrado.");
        return;
    }

    File file = SPIFFS.open("/credenciais.txt", "r");
    if (!file) {
        Serial.println("Erro ao abrir o arquivo de credenciais.");
        return;
    }

    Serial.println("Carregando credenciais salvas...");
    while (file.available()) {
        String linha = file.readStringUntil('\n');
        int userIndex = linha.indexOf("User: ");
        int passIndex = linha.indexOf(" | Pass: ");

        if (userIndex != -1 && passIndex != -1) {
            String user = linha.substring(userIndex + 6, passIndex);
            String pass = linha.substring(passIndex + 9);
            credenciais.push_back({user, pass});
            Serial.printf("Credencial carregada - User: %s, Pass: %s\n", user.c_str(), pass.c_str());
        }
    }

    file.close();
}

#endif // CREDENCIAIS_H
