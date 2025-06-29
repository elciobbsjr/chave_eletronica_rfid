// display_oled.c
#include <string.h>
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "ssd1306_i2c.h"
#include "display_oled.h"

#define SDA_PIN 14
#define SCL_PIN 15
#define I2C_PORT i2c1
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

static ssd1306_t oled;

void display_init() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    ssd1306_init_bm(&oled, OLED_WIDTH, OLED_HEIGHT, false, ssd1306_i2c_address, I2C_PORT);
    ssd1306_config(&oled);
    ssd1306_init();
    ssd1306_clear(&oled);
    ssd1306_draw_string(oled.ram_buffer + 1, 10, 20, "Sistema iniciado");
    ssd1306_show(&oled);
}

void display_show_message(const char *mensagem) {
    memset(oled.ram_buffer + 1, 0, oled.bufsize - 1);
    ssd1306_draw_string(oled.ram_buffer + 1, 0, 20, (char *)mensagem);
    ssd1306_show(&oled);
}
