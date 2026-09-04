# Blynk-Particle-Example-BLE

Provision a Particle device to Blynk using the Blynk app's BLE "Inject"
flow, instead of pre-generating a static token/QR code per device.

This is a follow-on to
[Blynk-Particle-Example](https://github.com/anthony-blynk/Blynk-Particle-Example),
which covers the static-tokens approach (see
[this article](https://www.blynk.io/blog/connect-a-particle-fleet-to-blynk-without-per-device-tokens)) -
useful when you're printing/scanning QR codes as part of manufacturing. BLE
provisioning is the better fit when a person is setting the device up in
person with the Blynk app, and there's nothing to pre-generate or print.

## How it works

```
Blynk app --BLE-->  Particle device --HTTPS--> Blynk (claim token)
Particle device --Particle.publish(JSON)--> Particle Cloud --Webhook--> Blynk HTTP Data Converter
```

On first boot the device advertises over BLE. Open it in the Blynk app, and
the app hands over a fresh auth token there and then - no token needs to be
pre-generated or scanned from a QR code. The device claims that token itself
over HTTPS and writes its Particle Device ID into a Blynk device metadata
field, then falls back to plain `Particle.publish()` for everything else -
same webhook + Blynk HTTP Data Converter pattern as Blynk-Particle-Example.
There's no library on the device keeping a live connection to Blynk running.

Provisioning only runs once - a flag is saved once it succeeds, so
subsequent boots skip straight to normal operation.

## Setup

1. **Blynk**: same as Blynk-Particle-Example - create a Datastream and an
   HTTP Data Converter on your Blynk template/device, and copy the
   converter's URL.
2. Fill in `BLYNK_TEMPLATE_ID` / `BLYNK_TEMPLATE_NAME` in `src/main.cpp` for
   your Blynk template. `BLYNK_TEMPLATE_NAME` (slugified - lowercase,
   hyphenated) is used to build the Particle event name, so **avoid names
   starting with `particle` or `spark`** - those prefixes are reserved for
   Particle's own system events, and events using them get silently
   dropped: `Particle.publish()` still returns `true` on the device, but
   the event never reaches your event stream in the Particle console.
3. Update `BLYNK_DEVICE_ID_METAFIELD_NAME` in `src/BlynkInjectHelper.h` to
   the exact (case-sensitive) name of the metadata field (not a datastream)
   you want the Particle Device ID written to.
4. **Particle**: set up the webhook integration exactly as in
   Blynk-Particle-Example (*Integrations > New Integration > Webhook*,
   pointed at the Data Converter URL from step 1, Event Name
   `<BLYNK_TEMPLATE_NAME>/data`).
5. Flash the firmware, then open the device in the Blynk app to provision
   it over BLE.
6. Check the device's metadata in the Blynk console - the field from step 3
   should now hold this device's Particle Device ID - and watch values
   arrive in Blynk from `publishSensorData()`, same as
   Blynk-Particle-Example.

## Blynk Enterprise / your own branded app

If you're a Blynk Enterprise client with your own server and a
white-labeled app, uncomment and fill in `BLYNK_VENDOR_PREFIX` and
`BLYNK_DEFAULT_SERVER` near the top of `src/main.cpp`.

## Cellular-only, no WiFi

This example targets cellular Particle devices. WiFi is explicitly turned
off (`WiFi.off()` in `setup()`), and this project's vendored copy of
`NetMgr` (`lib/NetMgr/src/NetMgr.h`) has WiFi support compiled out
entirely - leaving it in would turn the WiFi radio on during BLE
provisioning, which shares a radio chip with BLE on this hardware and makes
advertising flaky. If you need WiFi, undo the `#if Wiring_WiFi && 0` change
in that file and remove the `WiFi.off()` call.

## Re-provisioning

Call the `clearProvisioning` cloud function, e.g.
`particle call <device> clearProvisioning ""` - it clears the saved
provisioning flag and any stored WiFi credentials, then reboots the device
straight back into BLE provisioning.

## Using real sensors

Same as Blynk-Particle-Example: replace the `random()` calls in
`publishSensorData()` with real sensor readings, and add any extra fields
to the JSON payload and to the webhook mapping in Blynk.
