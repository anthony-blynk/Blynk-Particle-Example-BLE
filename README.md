# Blynk-Particle-Example-BLE

Started as a copy of [Blynk-Particle-Example](../Blynk-Particle-Example),
with Blynk BLE "Inject" provisioning merged in: the Blynk app hands the
device an auth token over BLE, the device claims it and writes its Particle
Device ID into a Blynk device metadata field, and from then on it talks to
Particle only - same `Particle.publish()` + webhook + Blynk HTTP Data
Converter pattern as Blynk-Particle-Example.

## How it works

Unlike [Blynk-Particle-Helloworld](../Blynk-Particle-Helloworld), this does
**not** use the full `BlynkEdgent` library to keep a live connection to
Blynk running. There are two reasons:

1. Once `BlynkEdgent` has started, there's no clean way to stop or close it.
2. It isn't needed here - once the Blynk auth token has been claimed and the
   Particle Device ID has been written to a Blynk device metadata field, the
   device doesn't need an ongoing connection to Blynk at all. Everything
   else goes via the Particle Cloud (see `publishSensorData()` in
   `src/main.cpp`).

So `main.cpp` only uses the standalone `BlynkInject` class (BLE handshake)
plus two plain HTTPS calls, all in `doBlynkProvisioning()`
(`src/BlynkInjectHelper.h`):

1. **BLE provisioning** - advertises over BLE and waits for the Blynk app to
   hand over an auth token (`BlynkInject`, same mechanism `BlynkEdgent` uses
   internally for its own onboarding).
2. **Claim the token** - `GET /external/api/provision?token=...&templateId=...`
3. **Set the device ID meta field** - `GET /external/api/device/meta/update?token=...&metaFieldName=...&value=<Particle Device ID>`

The result (auth token + host) is cached in EEPROM, so provisioning only
runs once - subsequent boots skip straight to `loop()`.

`do_https_get()` follows redirects (up to 3 hops): the generic `blynk.cloud`
host 308-redirects these calls to your account's actual regional server, and
the phone app won't see the device come online until that claim actually
succeeds.

## Setup

1. Fill in `BLYNK_TEMPLATE_ID` / `BLYNK_TEMPLATE_NAME` in `src/main.cpp` for
   your Blynk template.
2. Update `EVENT_TEMPLATE_SLUG` in `src/main.cpp` to a slugified (lowercase,
   hyphenated) version of your template name - it's kept separate from
   `BLYNK_TEMPLATE_NAME` since that one needs to be the real, human-readable
   template name for BLE provisioning. **Avoid names starting with
   `particle` or `spark`** - those prefixes are reserved for Particle's own
   system events, and events using them get silently dropped:
   `Particle.publish()` still returns `true` on the device, but the event
   never reaches your event stream in the Particle console.
3. Update `BLYNK_DEVICE_ID_METAFIELD_NAME` in `src/BlynkInjectHelper.h` to
   the exact (case-sensitive) name of the metadata field (not a datastream)
   you want the Particle Device ID written to.
4. Flash the firmware, then use the Blynk app's "Inject" / BLE provisioning
   flow to provision the device.
5. Check the device's metadata in the Blynk console - the field should now
   hold this device's Particle Device ID.
6. From here on the device just publishes dummy sensor data via
   `Particle.publish()` every 30 seconds. Wire up a webhook + Blynk HTTP
   Data Converter (see Blynk-Particle-Example) if you want that data in
   Blynk too.

## Cellular-only, no WiFi

This project's vendored copy of `NetMgr` (`lib/NetMgr/src/NetMgr.h`) has
WiFi support compiled out entirely. Leaving it in makes `BlynkInject` turn
the WiFi radio on during BLE provisioning - which shares the same radio chip
as BLE on this hardware and makes advertising flaky - and makes the Blynk
app offer WiFi as a connectivity option alongside cellular. If you need
WiFi, undo the `#if Wiring_WiFi && 0` change in that file.

## Re-provisioning

Call the `clearProvisioning` cloud function to erase the cached auth token
and host, then reset the device - it will run through BLE provisioning
again on the next boot.
