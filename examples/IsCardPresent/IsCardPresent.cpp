#include "MFRC522.h"
#include <string>
#include <chrono>
#include <thread>


// The GPIO pin that's connected to the MFRC522's reset pin
#define RST_PIN RPI_V2_GPIO_P1_15
// The GPIO pin that's connected to the MFRC522's SDA pin,
// sometimes labeled SS or CE or CS.
// Doesn't have to be one of the CE pins on the pi
#define SS_PIN RPI_V2_GPIO_P1_38

/**
 * Helper routine to dump a byte array as hex values.
 */
void printHex(uint8_t *buffer, size_t bufferSize) {
  for (size_t i = 0; i < bufferSize; i++) {
    printf(" %02X", buffer[i]);
  }
}

int main() {
    MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);  // Instance of the class
    rfid.PCD_Init();

    bool card_present = false;

    while (1) {
        if (rfid.PICC_IsCardPresent()) {
            if (!card_present) {
                if (rfid.PICC_ReadCardSerial()) {
                    printf("card present: ");
                    printHex(rfid.uid.uidByte, rfid.uid.size);
                    printf("\n");
                    card_present = true;
                }
            }
            rfid.PICC_HaltA();
            rfid.PCD_StopCrypto1();
        } else {
            if (card_present) {
                printf("card removed\n");
                card_present = false;
            }
        }
    }

    return 0;
}