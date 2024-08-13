// Conditional compilation to include WiFiMulti library for ESP32 or ESP8266
#if defined(ESP32)
#include <WiFiMulti.h>  // Include WiFiMulti library for ESP32
WiFiMulti wifiMulti;    // Create a WiFiMulti object for managing WiFi connections
#define DEVICE "ESP32"  // Define a macro for the device type as ESP32
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>  // Include WiFiMulti library for ESP8266
ESP8266WiFiMulti wifiMulti;    // Create a WiFiMulti object for managing WiFi connections
#define DEVICE "ESP8266"        // Define a macro for the device type as ESP8266
#endif

// Include libraries for the PZEM004T energy monitor and software serial communication
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

#if defined(ESP32)
#error "Software Serial is not supported on the ESP32"  // Error message if Software Serial is used on ESP32
#endif

// Include libraries for InfluxDB client
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

// Define WiFi credentials
#define WIFI_SSID "xxxx"         // WiFi SSID (network name)
#define WIFI_PASSWORD "xxxx"     // WiFi password

// Define InfluxDB connection details
#define INFLUXDB_URL "https://xxxx.aws.cloud2.influxdata.com"  // InfluxDB URL
#define INFLUXDB_TOKEN "xxxx"  // InfluxDB authentication token
#define INFLUXDB_ORG "xxxx"    // InfluxDB organization name
#define INFLUXDB_BUCKET "energy_monitor"  // InfluxDB bucket (database) name

// Define the time zone information
#define TZ_INFO "UTC7"  // Time zone setting (UTC+7)

// Define RX and TX pins for PZEM communication if not already defined
#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN D1  // Define RX pin for PZEM communication
#define PZEM_TX_PIN D0  // Define TX pin for PZEM communication
#endif

// Create a SoftwareSerial object for PZEM communication using the defined pins
SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
// Create a PZEM004Tv30 object for interfacing with the PZEM energy monitor
PZEM004Tv30 pzem(pzemSWSerial);

// Create an InfluxDB client object with preconfigured cloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Create a data point object named "sensor" for sending data to InfluxDB
Point sensor("ecm");

void setup() {
  Serial.begin(115200);  // Start the serial communication at a baud rate of 115200

  // Initialize WiFi in station mode and add the WiFi network credentials
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");  // Print a message indicating the start of WiFi connection
  while (wifiMulti.run() != WL_CONNECTED) {  // Wait until the device is connected to WiFi
    Serial.print(".");  // Print a dot for each attempt
    delay(100);  // Delay 100ms between attempts
  }
  Serial.println();  // Print a newline after connection

  // Synchronize the time using NTP servers
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Validate connection to the InfluxDB server
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");  // Print successful connection message
    Serial.println(client.getServerUrl());  // Print the InfluxDB server URL
  } else {
    Serial.print("InfluxDB connection failed: ");  // Print failure message if connection fails
    Serial.println(client.getLastErrorMessage());  // Print the last error message
  }

  // Add tags to the data point
  sensor.addTag("Device", DEVICE);  // Add a tag for the device type (ESP32 or ESP8266)
  sensor.addTag("Sensor", "PZEM");  // Add a tag for the sensor type (PZEM)
}

void loop() {
  sensor.clearFields();  // Clear previous data fields in the sensor data point

  // Read measurements from the PZEM sensor
  float voltage = pzem.voltage();      // Get the voltage reading
  float current = pzem.current();      // Get the current reading
  float power = pzem.power();          // Get the power reading
  float energy = pzem.energy();        // Get the energy reading
  float frequency = pzem.frequency();  // Get the frequency reading
  float pf = pzem.pf();                // Get the power factor reading

  // Calculate apparent power (VA) and reactive power (VAR)
  float va = power / pf;
  float var = power / pf * sqrt(1 - sq(pf));

  // Check if any readings are not a number (NaN)
  if (isnan(voltage) || isnan(current) || isnan(power) || isnan(energy) || isnan(frequency) || isnan(pf)) {
    Serial.println("Error reading sensor data");  // Print error message if any readings are invalid
  } else {

    // Add the sensor readings to the InfluxDB data point
    sensor.addField("voltage", voltage);
    sensor.addField("current", current);
    sensor.addField("power", power);
    sensor.addField("energy", energy);
    sensor.addField("frequency", frequency);
    sensor.addField("power_factor", pf);
    sensor.addField("VA", va);  // Apparent power
    sensor.addField("VAR", var);  // Reactive power

    // Print the data being sent to InfluxDB
    Serial.print("Writing: ");
    Serial.println(sensor.toLineProtocol());  // Convert data to InfluxDB line protocol format
    Serial.println("Success to send data to InfluxDB");  // Confirm successful data send
  }

  // Check if the WiFi connection is still active
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi connection lost");  // Print message if WiFi connection is lost
  }

  // Attempt to write the data point to InfluxDB
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");  // Print failure message if data write fails
    Serial.println(client.getLastErrorMessage());  // Print the last error message
  }

  Serial.println("Waiting 1 second");  // Print a message indicating a delay
  delay(500);  // Wait for 500ms before the next loop iteration
}
