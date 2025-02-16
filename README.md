# 🎮 Controle de Joystick e LEDs com PWM no Raspberry Pi Pico W  

Este projeto explora o uso do **Conversor Analógico-Digital (ADC)** do **RP2040**, permitindo controlar a intensidade de **LEDs RGB** e a movimentação de um **quadrado no display SSD1306** usando um **joystick analógico**.  

A aplicação foi desenvolvida com o **Pico SDK** utilizando a placa **BitDogLab**.  

## 🚀 Funcionalidades  

### 🕹️ Controle via Joystick  
O **joystick analógico** permite:  
- **Controlar a intensidade dos LEDs RGB** de forma suave usando **PWM**:  
  - **Eixo X** ajusta o brilho do **LED Vermelho**.  
  - **Eixo Y** ajusta o brilho do **LED Azul**.  
- **Mover um quadrado de 8x8 pixels no display SSD1306** proporcionalmente à posição do joystick.  
- **Aplicação de zona morta** para eliminar pequenas variações quando o joystick está parado.  

### 🔘 Botões com Interrupções e Debounce  
1. **Botão do Joystick (GPIO 22):**  
   - **Alterna o estado do LED Verde**.  
   - **Modifica o estilo da borda do display**, alternando entre dois estilos.  

2. **Botão A (GPIO 5):**  
   - **Ativa ou desativa o controle PWM** dos LEDs RGB.  

🔄 **Todas as funções dos botões são tratadas com interrupções e debounce via software**, garantindo **alta responsividade e evitando múltiplos acionamentos**.  

---

## 🛠️ Tecnologias Utilizadas  

- **Linguagem de Programação**: C.  
- **Placa Microcontroladora**: Raspberry Pi Pico W.  
- **Bibliotecas**:  
  - **Pico SDK** para manipulação de GPIO, PWM e ADC.  
  - **ssd1306** para controle do display OLED.  
- **Ferramentas**:  
  - **VS Code** (Ambiente de Desenvolvimento).  

---

## 📟 Display SSD1306 e I2C  

O **display SSD1306** é utilizado para:  
- **Mostrar um quadrado móvel de 8x8 pixels** que segue a posição do joystick.  
- **Alterar o estilo da borda do display** quando o botão do joystick é pressionado.  
- **Comunicação via I2C** (GPIO 14 - SDA / GPIO 15 - SCL).  

---

## 💡 Como Funciona  

### 🎮 Joystick e Controle de LEDs  
- **Leitura Analógica do Joystick:** Utilizando o **ADC do RP2040** nas GPIOs 26 (Y) e 27 (X).  
- **Intensidade dos LEDs RGB** é ajustada dinamicamente com base na posição do joystick:  
  - **Centro (2048):** LED apagado.  
  - **Extremos (0 ou 4095):** LED com brilho máximo.  
- **Zona Morta de ±200** aplicada para estabilidade na posição central.  

### 🔘 Interrupções e Debounce  
- As **interrupções** garantem alta responsividade na leitura dos botões.  
- **Debounce via software** com atraso de 300ms para evitar acionamentos múltiplos.  

---

## 🔌 Configuração do Hardware  

| Componente           | Pino do Pico W |  
|----------------------|----------------|  
| Joystick Eixo X      | GP27 (ADC1)    |  
| Joystick Eixo Y      | GP26 (ADC0)    |  
| Botão do Joystick    | GP22           |  
| Botão A              | GP5            |  
| LED Vermelho (PWM)   | GP13           |  
| LED Verde (Digital)  | GP11           |  
| LED Azul (PWM)       | GP12           |  
| Display SSD1306 (I2C)| SDA (GP14) / SCL (GP15) |  

---

## 🗂️ Estrutura do Repositório  

- `ADC-Joystick-Control.c`: Código-fonte principal contendo a implementação do controle de joystick, PWM e display.  
- `libs/ssd1306.h` e `libs/font.h`: Bibliotecas para o controle do display OLED SSD1306.  
- `README.md`: Documentação detalhada do projeto.  

---

## 📹 Demonstração do Projeto  

- **[Ver Vídeo](#)**   

---

## 🎯 Objetivos Alcançados  

- **Leitura analógica precisa** utilizando o **Conversor A/D** do RP2040.  
- **Controle de intensidade luminosa** dos LEDs RGB via **PWM**.  
- **Movimentação suave** de um objeto no display proporcional à posição do joystick.  
- **Comunicação I2C** com o **display SSD1306**.  
- **Debounce via software** e **interrupções** para botões responsivos.  

---

## 🔥 Desafios e Soluções  

- **Debounce via software:** Implementado com um **delay de 300ms**, eliminando leituras duplicadas.  
- **Movimentação suave do quadrado no display:** Utilizando **mapeamento proporcional** dos valores do joystick para as coordenadas do display.  
- **Controle preciso de intensidade dos LEDs RGB:** Com **PWM de 12 bits (4096 níveis)**, garantindo **transições suaves de brilho**.  

---

## 📚 Conceitos Aplicados  

- **Conversor Analógico-Digital (ADC)** para leitura de joystick analógico.  
- **PWM (Modulação por Largura de Pulso)** para controle de intensidade luminosa.  
- **I2C (Inter-Integrated Circuit)** para comunicação com o display OLED.  
- **Interrupções e Debounce** para captura de eventos dos botões.  

---

## 🔗 Links Úteis  

- [Documentação do Pico SDK](https://raspberrypi.github.io/pico-sdk-doxygen/index.html)  
- [Datasheet do Display SSD1306](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)  

---

## 💡 Desenvolvido por  

- **Henrique Santos**  
- **[LinkedIn](https://www.linkedin.com/in/dev-henriqueo-santos/)**  
