// === GEREKLİ KÜTÜPHANELER ===
#include <ESP8266WiFi.h>      // Wi-Fi bağlantısı için
#include "DHT.h"              // DHT11 sıcaklık ve nem sensörü için
#include <SFE_BMP180.h>       // BMP180 basınç sensörü için
#include <Wire.h>             // I2C haberleşme için

// === TANIMLAMALAR ===
#define YUKSEKLIK 875.0       // Ankara'nın rakımı (deniz seviyesinden yükseklik)
#define DHTPIN D3             // DHT sensörünün bağlı olduğu pin
#define DHTTYPE DHT11         // Kullanılan DHT sensör modeli

// === SENSÖR NESNELERİ ===
SFE_BMP180 basinc;            // BMP180 nesnesi
DHT dht(DHTPIN, DHTTYPE);     // DHT nesnesi

// === Wi-Fi AYARLARI ===
const char* ssid = "A";             // Wi-Fi ağ adı
const char* password = "1234567890"; // Wi-Fi şifresi

WiFiServer server(80);        // 80 numaralı portta web sunucusu

// === BAŞLATMA (setup) FONKSİYONU ===
void setup() {
  Serial.begin(9600);         // Seri haberleşme başlat
  delay(1000);
  dht.begin();                // DHT sensörünü başlat

  // Wi-Fi'ye bağlan
  Serial.print("Bağlanıyor...");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Bağlantı başarılıysa IP adresini yazdır
  Serial.println("");
  Serial.println("Kablosuz ağa (Wi-Fi) bağlandı.");
  Serial.print("IP Adresi: ");
  Serial.println(WiFi.localIP());

  server.begin();             // Web sunucusunu başlat

  // BMP180 sensörünü başlat
  if (basinc.begin()) {
    Serial.println("BMP180 sensörü çalışmaya başladı.");
  } else {
    Serial.println("BMP180 sensör hatası!");
    while (1); // Kalıcı hata durumunda dur
  }

  delay(1000); // Stabilizasyon beklemesi
}

// === ANA DÖNGÜ (loop) FONKSİYONU ===
void loop() {
  // Sensör verilerini oku
  float nem = dht.readHumidity();
  float sicaklikC = dht.readTemperature();
  float sicaklikF = dht.readTemperature(true);

  char status;
  double T, P, basinc0 = 0;

  // BMP180 ile sıcaklık ve basınç ölçümü
  status = basinc.startTemperature();
  if (status != 0) {
    delay(status);
    basinc.getTemperature(T);

    status = basinc.startPressure(3);
    if (status != 0) {
      delay(status);
      basinc.getPressure(P, T);
      basinc0 = basinc.sealevel(P, YUKSEKLIK); // Deniz seviyesine göre basınç

      // Seri port çıktısı (debug amaçlı)
      Serial.print("Nem: ");
      Serial.print(nem);
      Serial.print(" %, Sıcaklık: ");
      Serial.print(sicaklikC);
      Serial.print(" °C / ");
      Serial.print(sicaklikF);
      Serial.print(" °F, Basınç: ");
      Serial.print(basinc0, 2);
      Serial.println(" mb");
    }
  }

  // Web istemcisi bağlantısı varsa HTML sayfasını gönder
  WiFiClient client = server.available();
  if (client) {
    // === HTTP BAŞLIKLARI ===
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=UTF-8");
    client.println("Connection: close");
    client.println("Refresh: 10"); // Sayfa 10 saniyede bir yenilenir
    client.println();

    // === HTML SAYFA BAŞLANGICI ===
    client.println("<!DOCTYPE HTML><html>");
    client.println("<head>");
    client.println("<meta charset=\"UTF-8\">");
    client.println("<style>body { text-align: center; font-family: 
    Arial, sans-serif; background-color: #f0f8ff; }</style>");
    client.println("</head>");
    client.println("<body>");

    // Başlık
    client.println("<h2 style='color:red;'>NodeMCU METEOROLOJİ SUNUCU SAYFASI</h2>");

    // Nem bilgisi
    client.print("<p><h2>Nem: ");
    client.print(nem);
    client.println(" %</h2></p>");

    // Sıcaklık bilgisi
    client.print("<p><h2>Sıcaklık: ");
    client.print(sicaklikC);
    client.print(" °C / ");
    client.print(sicaklikF);
    client.println(" °F</h2></p>");

    // Basınç bilgisi
    client.print("<p><h2>Basınç: ");
    client.print(basinc0, 2);
    client.println(" mb</h2></p>");

    client.println("</body></html>");

    client.stop();  // Bağlantıyı sonlandır
  }
}
