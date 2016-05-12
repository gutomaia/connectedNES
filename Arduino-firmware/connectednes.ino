#include "SPI.h"

// Remember that Ethernet Shield uses pins 10, 11, 12 and 13

#define NES_CLOCK 2                                 // Red wire
#define NES_LATCH 3                                 // Orange wire
#define NES_DATA  4                                 // Yellow wire

// Also that not all pins are available for attach interrupt
// You will need interrupt events on CLOCK and LATCH
// https://www.arduino.cc/en/Reference/AttachInterrupt

#define NES_CLOCK_INT 0                        // Red wire
#define NES_LATCH_INT 1                        // Orange wire


volatile unsigned char latchedByte;                 // Controller press byte value = one letter in tweet
volatile unsigned char bitCount;                    // A single LDA $4017 (get one bit from "controller press")
volatile unsigned char byteCount;                   // How many bytes have already been printed
volatile unsigned char bytesToTransfer;             // How many bytes are left to print

unsigned char tweetData[192];                       // Array that will hold 192 hex values representing tweet data

void ClockNES() {
    digitalWrite(NES_DATA, latchedByte & 0x01);
    latchedByte >>= 1;
    bitCount++;
}

void LatchNES() {
    if (byteCount == bytesToTransfer) {
        latchedByte = 0xFF;
        digitalWrite(NES_DATA, latchedByte & 0x01);
        latchedByte >>= 1;
        bitCount = 0;
    } else {
        latchedByte = tweetData[byteCount] ^ 0xFF;
        digitalWrite(NES_DATA, latchedByte & 0x01);
        latchedByte >>= 1;
        bitCount = 0;
        byteCount++;
    }
}

void OnData(String event, String data) {
    Serial.println(data);
    static int length = (data.length() > 192)?192:data.length();
    char inputStr[length];
    data.toCharArray(inputStr, length);
    tweetData[0] = 0xE8;
    static int i=1;
    for(i=1; i<length; i++) { tweetData[i] = inputStr[i]; }
    memset(&inputStr[0], 0, sizeof(inputStr));
    bytesToTransfer = length;
    byteCount = 0;
}

void setup() {
    Serial.begin(9600);
    Serial.println("Started ConnectedNES");

    pinMode(NES_CLOCK, INPUT);                      // Set NES controller red wire (clock) as an input
    pinMode(NES_LATCH, INPUT);                      // Set NES controller orange wire (latch) as an input
    pinMode(NES_DATA, OUTPUT);                      // Set NES controller yellow wire (data) as an output

    attachInterrupt(NES_CLOCK_INT, ClockNES, FALLING);  // When NES clock ends, execute ClockNES
    attachInterrupt(NES_LATCH_INT, LatchNES, RISING);   // When NES latch fires, execure LatchNES

    byteCount = 0;                                  // Initialize byteCount at zero, no letters printed to screen
    bytesToTransfer = 0;                            // Initialize bytesToTransfer at zero, no letters waiting to print to screen
    OnData("any", "Hello World testing");
}

void loop() {
}