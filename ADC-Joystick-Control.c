#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "libs/font.h"
#include "libs/ssd1306.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define VRX 27
#define VRY 26
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define SW 22
#define LED_GREEN 11
#define LED_BLUE 12
#define LED_RED 13
#define BUTTON_A 5
#define DEAD_ZONE 200 // Tamanho da zona morta
#define SQUARE_SIZE 8 //tamanho do quadrado

uint slice_led_b, slice_led_r;
const float DIVIDER_PWM = 16.0;
const uint16_t PERIOD = 4096;
bool cor_display = false;
ssd1306_t ssd;

uint32_t volatile last_time = 0; // variável que auxilia no debounce
bool volatile borda_display = false;
bool volatile pwm_ativado = true;

void setup_I2C()
{
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

void setup_joystick()
{
    adc_init();
    adc_gpio_init(VRX);
    adc_gpio_init(VRY);

    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);
}

void setup_led_pwm(uint led, uint *slice)
{
    gpio_set_function(led, GPIO_FUNC_PWM);
    *slice = pwm_gpio_to_slice_num(led);
    pwm_set_clkdiv(*slice, DIVIDER_PWM);
    pwm_set_wrap(*slice, PERIOD);
    pwm_set_gpio_level(led, 0);
    pwm_set_enabled(*slice, true);
}

void setup_button_and_ledGreen()
{
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
}

void setup_display()
{
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    ssd1306_send_data(&ssd);                                      // Envia os dados para o display
    ssd1306_fill(&ssd, false);                                    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_send_data(&ssd);                                      // Atualiza display
}

void setup()
{
    stdio_init_all();
    setup_I2C();
    setup_joystick();
    setup_led_pwm(LED_BLUE, &slice_led_b);
    setup_led_pwm(LED_RED, &slice_led_r);
    setup_button_and_ledGreen();
    setup_display();
}

void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos para debounce
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se o tempo mínimo de debounce passou (300ms)
    if (current_time - last_time > 300000)
    {
        if (gpio == SW)
        {
            gpio_put(LED_GREEN, !gpio_get(LED_GREEN));
            borda_display = !borda_display;
        }
        if(gpio == BUTTON_A){
            pwm_ativado = !pwm_ativado;
        }

        last_time = current_time;
    }
}

// Função para ler os valores dos eixos do joystick (X e Y)
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value)
{
    // Leitura do valor do eixo X do joystick
    adc_select_input(ADC_CHANNEL_1); // Seleciona o canal ADC para o eixo X
    sleep_us(2);                     // Pequeno delay para estabilidade
    *vrx_value = adc_read();         // Lê o valor do eixo X (0-4095)

    // Leitura do valor do eixo Y do joystick
    adc_select_input(ADC_CHANNEL_0); // Seleciona o canal ADC para o eixo Y
    sleep_us(2);                     // Pequeno delay para estabilidade
    *vry_value = adc_read();         // Lê o valor do eixo Y (0-4095)
}

void level_pwm(int value_x, int value_y)
{
    int offset_x = value_x - 2048;
    int offset_y = value_y - 2048;

    // Aplica zona morta
    if (abs(offset_x) < DEAD_ZONE)
        offset_x = 0;
    if (abs(offset_y) < DEAD_ZONE)
        offset_y = 0;

    // Normaliza o valor para PWM (remapeia para o intervalo 0 - 4095)
    int pwm_x = (offset_x != 0) ? (abs(offset_x) * PERIOD) / (2048 - DEAD_ZONE) : 0;
    int pwm_y = (offset_y != 0) ? (abs(offset_y) * PERIOD) / (2048 - DEAD_ZONE) : 0;

    // Define a intensidade do LED proporcionalmente
    pwm_set_gpio_level(LED_RED, pwm_x);
    pwm_set_gpio_level(LED_BLUE, pwm_y);
}

void move_square_with_joystick(ssd1306_t *ssd, uint16_t value_x, uint16_t value_y) {

    // Mapear os valores do joystick para as coordenadas do display
    int square_x = (value_x * (WIDTH - SQUARE_SIZE)) / PERIOD;
    int square_y = ((PERIOD - value_y) * (HEIGHT - SQUARE_SIZE)) / PERIOD; // Inverte o eixo Y

    // Desenhar o quadrado na nova posição
    draw_filled_square(ssd, square_x, square_y);
}

int main()
{
    uint16_t value_x, value_y;
    setup();

    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(SW, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    while (true)
    {
        joystick_read_axis(&value_x, &value_y);

        printf("valor x: %d\tvalor y; %d\n", value_x, value_y);

        if(pwm_ativado) level_pwm(value_x, value_y);


        ssd1306_fill(&ssd, !cor_display); // Limpa o display
        (borda_display) ? ssd1306_rect(&ssd, 5, 5, 118, 54, cor_display, !cor_display) : 
                          ssd1306_rect(&ssd, 1, 1, 126, 62, cor_display, !cor_display);
        move_square_with_joystick(&ssd, value_x, value_y);
        

        ssd1306_send_data(&ssd); // Atualiza o display
        sleep_ms(50);
    }
}
