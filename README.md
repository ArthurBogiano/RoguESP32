# RogueESP32 - Rogue Access Point com Captive Portal para Coleta de Credenciais

## Introdução

O **RogueESP32** é um projeto de Rogue Access Point (AP) que utiliza o microcontrolador ESP32 para criar um ponto de acesso aberto, simulando redes Wi-Fi legítimas. Este sistema intercepta conexões de dispositivos próximos, redirecionando-os para uma página de login personalizada onde é possível capturar credenciais de forma simplificada.

**Objetivo**: Este projeto foi desenvolvido com fins educacionais e de pesquisa, buscando demonstrar como funcionam captive portals e técnicas de segurança em redes sem fio. Ele permite observar o comportamento de dispositivos conectados em redes abertas, servindo como uma ferramenta poderosa para estudantes e profissionais de segurança. Lembre-se: **com grandes poderes vêm grandes responsabilidades**.

## Endpoints e Como Usar

### Principais Endpoints

1. **Página de Login (`/`)**:
   - Exibe uma página de login baseada no modo de operação configurado (ex: `instagram.html` ou `google.html`).
   - O arquivo HTML é carregado do SPIFFS, com base na configuração atual.

2. **Captura de Credenciais (`/get`)**:
   - Método: `GET`
   - Parâmetros: `email` e `password`
   - Processa e armazena as credenciais fornecidas pelos usuários na página de login.
   - Após o envio dos dados, o dispositivo exibe uma mensagem de erro simulada.

3. **Página de Credenciais Capturadas (`/creds`)**:
   - Exibe uma tabela HTML com todas as credenciais capturadas, permitindo visualizar rapidamente os dados enviados pelos usuários.

4. **Configuração do Dispositivo (`/settings`)**:
   - Permite alterar o `SSID`, `password` e `mode` do Access Point.
   - Salva as novas configurações de rede e reinicia o ESP32 automaticamente para aplicá-las.

### Como Usar

1. **Configuração Inicial**:
   - Compile e faça o upload do código para o ESP32.
   - Configure a rede Wi-Fi com o SSID e a senha desejados através do endpoint `/settings` ou diretamente no código.

2. **Conectar-se ao AP**:
   - O ESP32 cria um ponto de acesso com o SSID configurado (por padrão, “Wifi Free”).
   - Qualquer dispositivo que se conecta ao AP será automaticamente redirecionado para a página de login.

3. **Captura de Credenciais**:
   - A página de login solicita credenciais, que são armazenadas e podem ser visualizadas na rota `/creds`.

4. **Alteração de Configurações**:
   - Para modificar `SSID`, `password` ou `mode`, acesse o endpoint `/settings` e faça as alterações desejadas. O dispositivo reiniciará automaticamente para aplicar as novas configurações.

## Referências e Inspiração

Este projeto foi inspirado em outros trabalhos de Rogue Access Point e Phishing utilizando dispositivos ESP32 e Flipper Zero:

- [ESPhishing](https://github.com/penegui/ESPhishing) - Um projeto focado em Phishing com ESP32.
- [Flipper Zero Evil Portal](https://github.com/RogueMaster/flipperzero-evil-portal) - Projeto que utiliza o Flipper Zero para criar um portal malicioso em redes Wi-Fi.

## Agradecimentos

Agradecimentos especiais ao ChatGPT, que foi de grande ajuda na elaboração deste projeto, contribuindo com ideias, soluções e melhorias durante o desenvolvimento.
