#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "leitor_rfid.h"
#include "display_oled.h"  // <- Mantido

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

void verifica_usuario(char* id) {
    for (int i = 0; i < numUsuarios; i++) {
        if (strcmp(id, usuarios[i].uid) == 0) {
            printf("Acesso liberado para %s\n", usuarios[i].nome);
            display_show_message("Acesso liberado!");
            return;
        }
    }
    printf("Acesso negado para UID: %s\n", id);
    display_show_message("Acesso negado!");
}

int main() {
    stdio_init_all();
    sleep_ms(500);

    display_init();
    leitor_rfid_inicializar(&mfrc);

    display_show_message("Aproxime o cartao");

    while (true) {
        char uid_str[13];
        if (leitor_rfid_ler_cartao(mfrc, uid_str)) {
            printf("UID lido: %s\n", uid_str);
            verifica_usuario(uid_str);
            sleep_ms(2000);
            display_show_message("Aproxime o cartao");
        }

        sleep_ms(300);
    }

    return 0;
}
