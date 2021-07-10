// Plays a RAW (16-bit signed) PCM audio file at slower or faster rate
// this example requires an uSD-card inserted to teensy 3.6 with a file called DEMO.RAW
#include <Arduino.h>
#include <AudioStream.h>
#include <Audio.h>
#include "SD.h"
#include <TeensyVariablePlayback.h>

// GUItool: begin automatically generated code
AudioControlSGTL5000 audioShield;
AudioPlaySdRawResmp  playSdRaw1;     //xy=324,457
AudioOutputI2S           i2s2;           //xy=840.8571472167969,445.5714416503906
AudioConnection          patchCord1(playSdRaw1, 0, i2s2, 0);
AudioConnection          patchCord2(playSdRaw1, 0, i2s2, 1);
// GUItool: end automatically generated code

#define A14 10
#define BUILTIN_SDCARD 10

const char* _filename = "DEMO.RAW";
const int analogInPin = A14;
unsigned long lastSamplePlayed = 0;

double getPlaybackRate(int16_t analog) { //analog: 0..1023
    return  (analog - 512.0) / 512.0;
}

void setup() {
    analogReference(0);
    pinMode(analogInPin, INPUT_DISABLE);   // i.e. Analog

    Serial.begin(57600);

    if (!(SD.begin(BUILTIN_SDCARD))) {
        // stop here if no SD card, but print a message
        while (1) {
            Serial.println("Unable to access the SD card");
            delay(500);
        }
    }
    audioShield.enable();
    audioShield.volume(0.5);

    playSdRaw1.enableInterpolation(true);
    int newsensorValue = analogRead(analogInPin);
    playSdRaw1.setPlaybackRate(getPlaybackRate(newsensorValue));

    AudioMemory(24);
}

void loop() {

    int newsensorValue = analogRead(analogInPin);
    playSdRaw1.setPlaybackRate(getPlaybackRate(newsensorValue));
    
    unsigned currentMillis = millis();
    if (currentMillis > lastSamplePlayed + 500) {
        if (!playSdRaw1.isPlaying()) {
            playSdRaw1.play(_filename);
            lastSamplePlayed = currentMillis;

            Serial.print("Memory: ");
            Serial.print(AudioMemoryUsage());
            Serial.print(",");
            Serial.print(AudioMemoryUsageMax());
            Serial.println();
        }
    }
    delay(10);
}

void std::__throw_length_error(char const* message) {
    Serial.printf("length error, %s\n", message);
}
