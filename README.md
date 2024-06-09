## Sinkronisasi Waktu RTC Menggunakan NTP untuk ESP8266 dan ESP32

- Kode berikut berfungsi untuk menyeting waktu di RTC(DS3231 dan DS1307) menggunakan NTP(Network Time Protocol).

---

## Konfigurasi

Sebelum melalukukan upload ke mikrokontroller harap merubah beberapa parameter berikut:

- WiFi parameter

```cpp
const char *ssid = "xxxxxxxxxxxxxxxx";
const char *password = "xxxxxxxxxxxx";
```

Ganti/ubah SSID dan Password WiFi yang digunakan. Ini penting agar device(ESP) bisa terkoneksi dengan internet.

- Timezone

```cpp
timeClient.setTimeOffset(25200);
```

ganti angka `25200` sesuai zona waktu kalian. contoh zona waktu WIB adalah 7 maka `3600 x 7 = 25200`. Sesuaikan saja sesuai kebutuhan.

---

#### [Suka minum kopi?](https://www.buymeacoffee.com/thoriktk)

#### [Like drinking coffee?](https://www.buymeacoffee.com/thoriktk)
