// WiFi Configuration
#define WIFI_SSID "xxxxxxx"       // <= Edit WiFi SSID.
#define WIFI_PASSWORD "xxxxxxxx" // <= Edit WiFi password.

// Amazon Certificate
#define THING_GROUP "esp32dkc"
#define AWS_IOT_SUB_TOPIC "cmd/esp32dkc"
#define AWS_IOT_PUB_TOPIC "tele/esp32dkc"
#define AWS_IOT_ENDPOINT "xxxxxxx.iot.ap-southeast-1.amazonaws.com" // <= Edit AWS IoT Endpoint.
extern const char AWS_CERT_CA[] asm("_binary_certs_ca_pem_start");
extern const char AWS_CERT_CRT[] asm("_binary_certs_cert_pem_start");
extern const char AWS_CERT_PRIVATE[] asm("_binary_certs_private_pem_start");
