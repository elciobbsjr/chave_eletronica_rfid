#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "leitor_rfid.h"

#define PIN_CS   5
#define PIN_RST  6

void leitor_rfid_inicializar(MFRC522Ptr_t *mfrc) {
    spi_init(spi0, 1 * 1000 * 1000);
    gpio_set_function(2, GPIO_FUNC_SPI); // SCK
    gpio_set_function(3, GPIO_FUNC_SPI); // MOSI
    gpio_set_function(4, GPIO_FUNC_SPI); // MISO
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);

    *mfrc = MFRC522_Init();
    PCD_Init(*mfrc, spi0);
}

bool leitor_rfid_ler_cartao(MFRC522Ptr_t mfrc, char* id_buffer) {
    if (!PICC_IsNewCardPresent(mfrc) || !PICC_ReadCardSerial(mfrc)) {
        return false;
    }

    for (int i = 0; i < 4; i++) {
        sprintf(&id_buffer[i * 3], "%02X:", mfrc->uid.uidByte[i]);
    }
    id_buffer[11] = '\0';

    PICC_HaltA(mfrc);
    PCD_StopCrypto1(mfrc);
    return true;
}
