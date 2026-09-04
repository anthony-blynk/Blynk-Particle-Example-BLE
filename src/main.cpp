/*
 * Blynk-Particle-Example-BLE
 *
 * Uses Blynk's BLE "Inject" widget (in the Blynk app) to provision this
 * device once: the app hands over a Blynk auth token over BLE, the device
 * claims it and writes its Particle Device ID into a Blynk device metadata
 * field via the Blynk HTTP API.
 *
 * Unlike the full BlynkEdgent library (see Blynk-Particle-Helloworld), this
 * does NOT keep a live connection to Blynk running afterwards - there's no
 * clean way to stop BlynkEdgent once started, and none is needed here. Once
 * provisioning is done, the device talks only to the Particle Cloud via
 * Particle.publish() + a webhook + a Blynk HTTP Data Converter, same as
 * Blynk-Particle-Example (this project started as a copy of it).
 *
 * Fill in your Blynk Template here. Read more: https://bit.ly/BlynkInject
 */
// #define BLYNK_TEMPLATE_ID     "TMPxxxxxx"
// #define BLYNK_TEMPLATE_NAME   "Device"
#define BLYNK_TEMPLATE_ID "TMPL4fKqC2IdZ"
#define BLYNK_TEMPLATE_NAME "blynk-particle-example"

#include <Particle.h>
#include "BlynkInjectHelper.h"

SYSTEM_MODE(AUTOMATIC);

SerialLogHandler logHandler(LOG_LEVEL_INFO, {
    { "app", LOG_LEVEL_TRACE },
    { "blynk.inject", LOG_LEVEL_TRACE }
});

const char* EVENT_NAME = BLYNK_TEMPLATE_NAME "/data";
const unsigned long PUBLISH_INTERVAL_MS = 30000;

unsigned long lastPublish = 0;

void publishSensorData();

void setup() {
    Serial.begin(115200);
    waitFor(Serial.isConnected, 5000);
    Serial.println();

    // One-time BLE provisioning: no-op if already provisioned (see
    // isAlreadyProvisioned() in BlynkInjectHelper.h).
    if (!doBlynkProvisioning()) {
        Log.error("Provisioning failed, rebooting to retry");
        delay(1000); // let the log line flush over serial before reset
        System.reset();
    }

    // Cloud function to wipe the stored provisioning state, so you can
    // re-run the BLE provisioning flow for testing.
    Particle.function("clearProvisioning", clearProvisioning);
}

void loop() {
    if (millis() - lastPublish >= PUBLISH_INTERVAL_MS) {
        lastPublish = millis();
        publishSensorData();
    }
}

// Everything from here on is plain Particle - no Blynk connection involved.
// Swap this for real sensor readings; see Blynk-Particle-Example for the
// webhook/HTTP Data Converter setup that gets this data into Blynk.
void publishSensorData() {
    float temperature = 20.0 + random(0, 100) / 10.0; // 20.0 - 30.0 C
    float humidity = 40.0 + random(0, 300) / 10.0;    // 40.0 - 70.0 %

    char payload[128];
    snprintf(payload, sizeof(payload),
             "{\"temperature\":%.1f,\"humidity\":%.1f,\"uptime\":%lu}",
             temperature, humidity, millis() / 1000);

    bool published = Particle.publish(EVENT_NAME, payload, PRIVATE);
    if (published) {
        Log.info("Published %s: %s", EVENT_NAME, payload);
    } else {
        Log.error("Failed to publish %s: %s", EVENT_NAME, payload);
    }
}
