#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "mfrc522.h"
#include "ssd1306_i2c.h"
#include "ssd1306.h"

#define PIN_CS   5
#define PIN_RST  6
#define SDA_PIN  14
#define SCL_PIN  15
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define I2C_PORT i2c1

ssd1306_t oled;

MFRC522Ptr_t mfrc;

typedef struct {
    const char* nome;
    const char* uid;
} Usuario;

Usuario usuarios[] = {
    {"Victor", "02:DC:A7:34"},
    {"Elcio", ""},
    {"Filipe", ""}
};
const int numUsuarios = sizeof(usuarios) / sizeof(usuarios[0]);

void inicializa_oled() {
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

void mostrar_mensagem_oled(const char *mensagem) {
    memset(oled.ram_buffer + 1, 0, oled.bufsize - 1);
    ssd1306_draw_string(oled.ram_buffer + 1, 0, 20, (char *)mensagem);
    ssd1306_show(&oled);
}

void le_cartao(char* id_buffer) {
    for (int i = 0; i < 4; i++) {
        sprintf(&id_buffer[i * 3], "%02X:", mfrc->uid.uidByte[i]);
    }
    id_buffer[11] = '\0';
    PICC_HaltA(mfrc);
    PCD_StopCrypto1(mfrc);
}

void verifica_usuario(char* id) {
    for (int i = 0; i < numUsuarios; i++) {
        if (strcmp(id, usuarios[i].uid) == 0) {
            printf("Acesso liberado para %s\n", usuarios[i].nome);
            mostrar_mensagem_oled("Acesso liberado!");
            return;
        }
    }
    printf("Acesso negado para UID: %s\n", id);
    mostrar_mensagem_oled("Acesso negado!");
}

int main() {
    stdio_init_all();
    sleep_ms(500);
    inicializa_oled();

    // Inicializa SPI0
    spi_init(spi0, 1 * 1000 * 1000);
    gpio_set_function(2, GPIO_FUNC_SPI); // SPI0_SCK
    gpio_set_function(3, GPIO_FUNC_SPI); // SPI0_TX (MOSI)
    gpio_set_function(4, GPIO_FUNC_SPI); // SPI0_RX (MISO)
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);

    mfrc = MFRC522_Init();
    PCD_Init(mfrc, spi0);

    mostrar_mensagem_oled("Aproxime o cartao");

    while (true) {
        if (!PICC_IsNewCardPresent(mfrc) || !PICC_ReadCardSerial(mfrc)) {
            sleep_ms(300);
            continue;
        }

        char uid_str[13];
        le_cartao(uid_str);
        printf("UID lido: %s\n", uid_str);
        verifica_usuario(uid_str);
        sleep_ms(2000);
        mostrar_mensagem_oled("Aproxime o cartao");
    }

    return 0;
}
