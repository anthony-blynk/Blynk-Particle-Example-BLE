#pragma once
#include "Particle.h"
#include <BlynkInject.h>
#include <BlynkSysUtils.h>
#include <httpc.h>

#ifndef BLYNK_VENDOR_PREFIX
#define BLYNK_VENDOR_PREFIX "Blynk"
#endif
#ifndef BLYNK_DEFAULT_SERVER
#define BLYNK_DEFAULT_SERVER "blynk.cloud"
#endif
// Cellular can take much longer than WiFi to associate (poor coverage, SIM
// activation, carrier registration) - bound the wait so a device that never
// gets online during provisioning reports a failure and retries instead of
// hanging forever with no feedback.
#ifndef PARTICLE_CONNECT_TIMEOUT_MS
#define PARTICLE_CONNECT_TIMEOUT_MS (180000UL) // 3 minutes
#endif


/*
 * 4096 bit, ISRG Root X1,            expires: Mon, 04 Jun 2035 11:04:38 GMT
 * 2048 bit, Amazon Root CA 1,        expires: Sun, 17 Jan 2038 00:00:00 GMT
 * 2048 bit, DigiCert Global Root G2, expires: Fri, 15 Jan 2038 12:00:00 GMT
 */
const char* blynk_root_ca_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
-----BEGIN CERTIFICATE-----
MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH
MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI
2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx
1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ
q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz
tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ
vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP
BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV
5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY
1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4
NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG
Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91
8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe
pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl
MrY=
-----END CERTIFICATE-----
)EOF";

static bool headerNameIs(const char* name, const char* expected) {
   while (*name && *expected) {
      char a = *name++, b = *expected++;
      if (a >= 'a' && a <= 'z') a -= 32;
      if (b >= 'a' && b <= 'z') b -= 32;
      if (a != b) return false;
   }
   return *name == 0 && *expected == 0;
}

// Splits an absolute "https://host[:port]/path?query" URL into host and
// path+query. Returns false if it's not an absolute https:// URL (in which
// case it's a path-only/relative redirect, to be used against the same host).
static bool splitAbsoluteUrl(const String& url, String& host, String& path) {
   if (!url.startsWith("https://")) return false;
   int hostStart = 8; // strlen("https://")
   int slash = url.indexOf('/', hostStart);
   if (slash < 0) {
      host = url.substring(hostStart);
      path = "/";
   } else {
      host = url.substring(hostStart, slash);
      path = url.substring(slash);
   }
   return host.length() > 0;
}

// Percent-encodes a query parameter (RFC 3986 unreserved chars pass
// through as-is; everything else, including spaces, is escaped).
static String urlEncode(const String &s) {
    String out;
    for (unsigned i = 0; i < s.length(); i++) {
        char c = s.charAt(i);
        bool unreserved = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
                           (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~';
        if (unreserved) {
            out += c;
        } else {
            char buf[4];
            snprintf(buf, sizeof(buf), "%%%02X", (unsigned char)c);
            out += buf;
        }
    }
    return out;
}

// Blynk's external API redirects the generic "blynk.cloud" host to the
// actual regional server (e.g. via HTTP 308), so this follows redirects
// rather than treating them as failures. Returns 0 on a final HTTP 200,
// -1 otherwise.
static int do_https_get(const char* host, const int port, const char* path) {
   String curHost = host;
   String curPath = path;

   for (int redirects = 0; redirects < 3; redirects++) {
      HttpsClient client;
      int ret = client.initTls(blynk_root_ca_cert, strlen(blynk_root_ca_cert) + 1);
      if (ret != 0) {
         LOG_E("initTls failed: %d", ret);
         return ret;
      }

      ret = client.connect((char*)curHost.c_str(), port);
      if (ret != 0) {
         LOG_E("connect failed: %d", ret);
         return ret;
      }

      ret = client.get(curPath.c_str());

      uint16_t status = client.resp->status_code;
      LOG_I("HTTP status %d, %d byte(s) received", status, ret);
      if (ret > 0) {
         LOG_I("body: %.*s", ret, client.resp->body); // (dynamic length)
      }

      if (status == 301 || status == 302 || status == 307 || status == 308) {
         String location;
         bool found = false;
         for (int i = 0; i < client.resp->headers.size(); i++) {
            if (headerNameIs(client.resp->headers.at(i).name, "Location")) {
               location = client.resp->headers.at(i).value;
               found = true;
               break;
            }
         }
         client.disconnect();

         if (!found) {
            LOG_E("Redirect (%d) with no Location header", status);
            return -1;
         }

         String newHost, newPath;
         if (splitAbsoluteUrl(location, newHost, newPath)) {
            LOG_I("Redirected to host %s path %s", newHost.c_str(), newPath.c_str());
            curHost = newHost;
            curPath = newPath;
         } else {
            LOG_I("Redirected to path %s", location.c_str());
            curPath = location;
         }
         continue;
      }

      client.disconnect();
      return status == 200 ? 0 : -1;
   }

   LOG_E("Too many redirects");
   return -1;
}

// All data comms happen via Particle (Particle.publish() + webhook), so the
// device never needs the auth token or host again after provisioning - only
// a flag recording that Blynk now has this device's Particle Device ID.
#define PROV_EEPROM_ADDR 0
#define PROV_MAGIC       0x5A // arbitrary marker distinguishing "provisioned" from erased flash

bool isAlreadyProvisioned() {
    return EEPROM.read(PROV_EEPROM_ADDR) == PROV_MAGIC;
}

void saveProvisioned() {
    EEPROM.write(PROV_EEPROM_ADDR, PROV_MAGIC);
}

// Set by clearProvisioning() and acted on from loop() - resetting directly
// inside a cloud function handler tears the device down before the RPC
// return value can be sent back to the caller, so every call would time
// out even though the underlying work already succeeded.
bool resetRequested = false;

int clearProvisioning(String args) {
    LOG_W("clearProvisioning() called (args=\"%s\")", args.c_str());
    EEPROM.write(PROV_EEPROM_ADDR, 0);
    // Also drop Device OS's own stored WiFi credentials - otherwise its
    // Connection Manager will keep preferring WiFi over cellular on its
    // own, independent of anything the BLE provisioning flow does, making
    // it impossible to test a genuinely fresh cellular-only provision.
    WiFi.clearCredentials();
    resetRequested = true;
    return 1;
}

BlynkInject   _inject;

// Returns false if the app disconnected before completing provisioning
// (i.e. before sending "connect"). BLE is left open either way - the
// caller decides when to end() it.
bool doBlynkInject() {
    LOG_I("Starting BLE provisioning");

    static bool injectDone = false;
    injectDone = false;
    _inject.setProvisionCallback([]() {
        injectDone = true;
    });

    _inject._config.host = BLYNK_DEFAULT_SERVER;

    systemInit(BLYNK_VENDOR_PREFIX, "Device");
    _inject.begin(systemGetDeviceName(), BLYNK_VENDOR_PREFIX, BLYNK_TEMPLATE_ID, "001", "0.0.0");

    while (!injectDone) {
        _inject.run();
        if (_inject.isAppDisconnected()) {
            _inject.setUserFinishedConfiguring();
            LOG_W("App disconnected before completing provisioning");
            return false;
        }
        // clearProvisioning() can be called (and its handler run) while
        // still stuck in this loop, well before setup() ever returns and
        // loop() gets a chance to run - check for it here too, not just in
        // loop(), otherwise the reset request just sits unnoticed.
        if (resetRequested) {
            LOG_W("Reset requested while waiting for BLE provisioning");
            System.reset();
        }
        delay(10);
    }

    LOG_I("BLE provisioning complete");
    return true;
}

// Name of the Blynk device metadata field to store the Particle Device ID
// in (case-sensitive, must match your Blynk template exactly). This is NOT
// the same thing as a datastream/virtual pin.
#ifndef BLYNK_DEVICE_ID_METAFIELD_NAME
#define BLYNK_DEVICE_ID_METAFIELD_NAME "ParticleDeviceId"
#endif

// Claims the auth token the app handed over via BLE. This is what makes
// the device show as online/provisioned in the Blynk app.
//  curl -v "https://fra.blynk-qa.com/external/api/provision?token=...&templateId=..."
int provisionToken() {
    String path = String::format(
        "/external/api/provision?token=%s&templateId=%s",
        _inject._config.auth.c_str(), BLYNK_TEMPLATE_ID);

    LOG_I("Claiming token: host=%s path=%s", _inject._config.host.c_str(), path.c_str());

    return do_https_get(_inject._config.host.c_str(), 443, path.c_str());
}

// Stores this device's Particle Device ID in a Blynk device metadata
// field, so the Particle Integration webhook -> Blynk Data Converter can
// authenticate it.
int setDeviceIdMetaField() {
    String path = String::format(
        "/external/api/device/meta/update?token=%s&metaFieldName=%s&value=%s",
        _inject._config.auth.c_str(), urlEncode(BLYNK_DEVICE_ID_METAFIELD_NAME).c_str(),
        System.deviceID().c_str());

    LOG_I("Setting device ID meta field: host=%s path=%s", _inject._config.host.c_str(), path.c_str());

    return do_https_get(_inject._config.host.c_str(), 443, path.c_str());
}

// Returns true once the device is provisioned and ready for normal
// operation (either just now, or already, previously). Returns false if
// provisioning failed or was abandoned - caller should not proceed into
// normal operation in that case.
bool doBlynkProvisioning() {
    Particle.connect();

    if (isAlreadyProvisioned()) {
        return true;
    }

    if (!doBlynkInject()) {
        _inject.end();
        return false;
    }

    _inject.reportStatus(BlynkInject::STATUS_CONNECTING_CLOUD);
    LOG_I("Connecting to Particle...");
    unsigned long connectStart = millis();
    while (!Particle.connected()) {
        if (millis() - connectStart >= PARTICLE_CONNECT_TIMEOUT_MS) {
            LOG_E("Timed out waiting for Particle cloud connection");
            _inject.reportFailure(BlynkInject::ERROR_NETWORK_TIMEOUT, "Timed out connecting to Particle");
            _inject.end();
            return false;
        }
        if (resetRequested) {
            LOG_W("Reset requested while waiting for Particle cloud connection");
            System.reset();
        }
        delay(100);
    }

    if (provisionToken() != 0) {
        _inject.reportFailure(BlynkInject::ERROR_CLOUD_TOKEN, "Blynk provisioning failed");
        _inject.end();
        return false;
    }

    if (setDeviceIdMetaField() != 0) {
        _inject.reportFailure(BlynkInject::ERROR_CLOUD_GENERIC, "Failed to set device metadata");
        _inject.end();
        return false;
    }

    saveProvisioned();
    _inject.reportStatus(BlynkInject::STATUS_CONNECTED);
    _inject.end();
    return true;
}

