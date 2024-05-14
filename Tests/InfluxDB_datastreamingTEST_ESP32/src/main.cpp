#include <Arduino.h>

#include <WiFiMulti.h>

WiFiMulti wifiMulti;

#define DEVICE "ESP32"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>




#define WIFI_SSID "Vincent"
#define WIFI_PASS "azertyuiopq"

#define INFLUXDB_URL "https://eu-central-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "lqwJrce7qsVziJZI3th4OgYMZipK4hB4RFk93a6CYCuGDjez-H__5ahm2_XgmkcxP_8oa6PijprNba9yoyd31A=="
#define INFLUXDB_ORG "305dabac34f58eed"
#define INFLUXDB_BUCKET "robot 2022-2023"
#define TZ_INFO "UTC2"

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);



Point sensor("wifi_status");





void setup() {
  
  client.setWriteOptions(WriteOptions().writePrecision(WritePrecision::US));
  client.setWriteOptions(WriteOptions().batchSize(100));

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting to wifi AP");

  while(wifiMulti.run() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println(WiFi.localIP());
  
  Serial.println("Connected");

  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov"); //time SUNC

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  sensor.addTag("TESTS", WiFi.SSID());



}

void loop() {
  //sensor.clearFields();

  sensor.addField("sinewave", sin(2 * 3.141592 * millis() / 10000), 5);

  Serial.println(sin(2 * 3.141592 * millis() / 1000));
  
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi connection lost");
  }

  client.writePoint(sensor);

  
  //Serial.println("Waiting 1 second");
  delay(10);

}