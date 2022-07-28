<h1 align="center">Welcome to AWS IoT Device Provisioning with ESP32 👋</h1>
<p>
</p>

> AWS IoT device provisioning with Fleet provisioning templates, The hardware is ESP32 that built-in a WiFi and Bluetooth module.

## Prerequisites

- [Platform IO CLI](https://docs.platformio.org/en/latest/core/index.html)

## Configuration

```c++
// include/config.h
-------------------
// Edit WiFi SSID and Password.
#define WIFI_SSID "xxxxxxx"
#define WIFI_PASSWORD "xxxxxxxx"
// Change a AWS_IOT_ENDPOINT follow you organize.
#define AWS_IOT_ENDPOINT "xxxxxxx.iot.ap-southeast-1.amazonaws.com"

// certs directory
------------------
// ca.pem => `RootCA` from AWS.
// cert.pem => `Device certification` from claim certificate.
// private.pem => `Private key` from claim certificate.

```

## Usage

```sh
make
```

You can use PlatformIO GUI in Visual Studio Code also.
First step you should run `Build Filesystem Image` then `Upload Filesystem Image` and the finally `Upload and Monitor` your device.

## Author

👤 **Thanapon Tapala**

- Website: https://thanapon.info/

## Show your support

Give a ⭐️ if this project helped you!

---

_This README was generated with ❤️ by [readme-md-generator](https://github.com/kefranabg/readme-md-generator)_
