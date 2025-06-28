#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "mfrc522.h"

// Define os pinos do Raspberry Pi Pico
#define LED_BRANCO 6
#define LED_VERMELHO 7
#define BOTAO 2
#define SERVO_PIN 4

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

uint slice_num;

void setup_servo() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    pwm_set_wrap(slice_num, 20000); // período de 20ms
    pwm_set_clkdiv(slice_num, 64.0f);
    pwm_set_enabled(slice_num, true);
}

void mover_servo(uint duty_us) {
    // duty_us entre 500 e 2500
    uint level = (duty_us * 20000) / 20000;
    pwm_set_gpio_level(SERVO_PIN, level);
}

void piscar_led(int led, int tempo_ms) {
    gpio_put(led, 1);
    sleep_ms(tempo_ms);
    gpio_put(led, 0);
}

void verifica_usuario(char* id) {
    for (int i = 0; i < numUsuarios; i++) {
        if (strcmp(id, usuarios[i].uid) == 0) {
            printf("Acesso liberado para %s\n", usuarios[i].nome);
            piscar_led(LED_BRANCO, 500);
            mover_servo(2400);
            sleep_ms(1000);
            mover_servo(500);
            return;
        }
    }
    printf("Acesso negado para UID: %s\n", id);
    piscar_led(LED_VERMELHO, 1000);
}

void le_cartao(char* id_buffer) {
    for (int i = 0; i < 4; i++) {
        sprintf(&id_buffer[i * 3], "%02X:", mfrc->uid.uidByte[i]);
    }
    id_buffer[11] = '\0';
    PICC_HaltA(mfrc);       // Corrigido: função correta da biblioteca
    PCD_StopCrypto1(mfrc);
}

int main() {
    stdio_init_all();
    gpio_init(LED_BRANCO);
    gpio_set_dir(LED_BRANCO, GPIO_OUT);
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(BOTAO);
    gpio_set_dir(BOTAO, GPIO_IN);
    gpio_pull_up(BOTAO);

    setup_servo();
    mover_servo(500);

    mfrc = MFRC522_Init();
    PCD_Init(mfrc, spi0);

    while (true) {
        if (!PICC_IsNewCardPresent(mfrc) || !PICC_ReadCardSerial(mfrc)) {
            sleep_ms(100);
            continue;
        }

        char uid_str[13];
        le_cartao(uid_str);
        printf("UID lido: %s\n", uid_str);
        verifica_usuario(uid_str);
        sleep_ms(2000);
    }

    return 0;
}
