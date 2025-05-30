# 🌤️ NodeMCU Meteoroloji İstasyonu

Bu proje, **NodeMCU ESP8266** kartı ile sıcaklık, nem ve basınç verilerini toplayarak bir **web arayüzü** üzerinden gösteren basit bir meteoroloji istasyonudur.

## 🔧 Kullanılan Bileşenler

- NodeMCU ESP8266
- DHT11 (Sıcaklık ve Nem Sensörü)
- BMP180 (Basınç Sensörü)
- Wi-Fi bağlantısı

## 📷 Proje Görünümü

Yan yana önceki ve sonraki hali için:

<p float="left">
  <img src="once.png" width="45%" />
  <img src="sonra.png" width="45%" />
</p>

## 📡 Özellikler

- Wi-Fi üzerinden bağlantı kurar
- Web sunucusu üzerinden veri yayınlar
- Sıcaklık, nem ve deniz seviyesine göre düzeltilmiş basınç ölçer
- Web sayfası her 10 saniyede bir otomatik yenilenir

## 🧠 Kod Açıklamaları

- `DHT11` sensörüyle sıcaklık ve nem verisi alınır
- `BMP180` ile sıcaklık ve basınç verileri ölçülür
- `WiFiServer` ile yerel web sunucusu kurulur
- Kullanıcı tarayıcıdan ESP'nin IP adresine giderek verileri canlı takip edebilir

## 📲 Wi-Fi Ayarları

Kod içinde şu satırları kendi Wi-Fi bilgilerinle değiştir:

```cpp
const char* ssid = "A";             // Wi-Fi ağ adı
const char* password = "1234567890"; // Wi-Fi şifresi
