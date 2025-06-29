#ifndef LEITOR_RFID_H
#define LEITOR_RFID_H

#include "mfrc522.h"

// Inicializa o leitor RFID com SPI e configura os pinos
void leitor_rfid_inicializar(MFRC522Ptr_t *mfrc);

// Lê o cartão e armazena a UID formatada em string
bool leitor_rfid_ler_cartao(MFRC522Ptr_t mfrc, char* id_buffer);

#endif
