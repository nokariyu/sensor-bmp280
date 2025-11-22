#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

// Pengaturan pin ini khusus untuk Esp32 devkit v1

//Pengaturan untuk koneksi pin BMP280
#define MISO (19)
#define MOSI (23)
#define SCK (18)
#define CS (5)

//pengaturan untuk module SSD1306
#define width (128)
#define height (32)

Adafruit_BMP280 sensor(CS, MOSI, MISO, SCK); // Sensor bmp280 menggunakan protokol SPI
Adafruit_SSD1306 oled(width, height, &Wire, -1); // Module oled SSD1306 menggunakan protokol I2C

const int input = 4;    // Untuk pin input dari button
int tekanan;            // Untuk menampung nilai dari pressure dalam tipe data int
bool logic1 = false;    // Untuk logika page pertama
bool logic2 = false;    // Untuk logika page kedua
int logic3 = -1;        // Untuk menampung nilai page

void setup(){
  pinMode(input, INPUT); // Menetapkan pin input sebagai INPUT
  sensor.begin(); // Memulai sensor bmp280
  sensor.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500); 

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Memulai module SSD1306

  oled.clearDisplay();       // Menghapus semua yang ditampilkan layar
  oled.setTextColor(WHITE);  // Mengatur warna tampilan text
  oled.setCursor(20, 12);    // Mengatur titik koordinat text
  oled.setTextSize(2);       // Mengatur ukuran text
  oled.print("NokariyU");    // Menampilkan teks
  oled.display();

  delay(2000);


}

void loop(){

  logic2 = logic1; // logic2 sama dengan logic1
  logic1 = digitalRead(input) == 1; // logic satu adalah nilai dari input

  if(logic1 == 0){                  // jika logic1 sama dengan 0
    logic2 = logic1;                // maka logic2 sama dengan logic1
  }

  if(logic1 != logic2){            // jika logic1 tidak sama dengan logic2
    logic3++;                    // maka logic3 akan bertambah 1

    if(logic3 > 2){               // jika logic tiga lebih dari 2
      logic3 = 0;                // maka logic3 kembali ke 0
    }

  }
  
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);

  switch (logic3){          // jika logic3 bernilai......

    case 0:                  // 0
      oled.println("======== suhu =======");
      oled.setTextSize(2);
      oled.setCursor(28, 12);
      oled.print(sensor.readTemperature());  // Menampilkan hasil pembacaan temperrature
      oled.println(" C");
      oled.display();
      break;

    case 1:             // 1
      tekanan = sensor.readPressure() /1;  // mengubah tipe data float menjadi int
      oled.println("====== tekanan ======");
      oled.setTextSize(2);
      oled.setCursor(20, 12);
      oled.print(tekanan);              // menampilkan pressure 
      oled.println(" Pa");
      oled.display();
      break;

    case 2:
      oled.println("==== ketinggian ====");
      oled.setTextSize(2);
      oled.setCursor(20, 12);
      oled.print(sensor.readAltitude(1013.25));       // menampilkan ketinggian
      oled.println(" M");
      oled.display();
      break;

    default:
      break; 
  }
  oled.display();
  delay(50);
}
