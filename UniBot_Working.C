#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "things.ubidots.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to configure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("POST /api/v1.6/variables/577545bc762542307a8f88e9/values HTTP/1.1");
    Serial.println("POST /api/v1.6/variables/577545bc762542307a8f88e9/values HTTP/1.1");
    client.println("X-Auth-Token: CxZhDjXndh4p7GkM9JwIIj9c10SnxX");
    Serial.println("X-Auth-Token: CxZhDjXndh4p7GkM9JwIIj9c10SnxX");
    client.println("Host: things.ubidots.com");
    Serial.println("Host: things.ubidots.com");
    client.println("Connection: close");
    Serial.println("Connection: close");
    client.println("Content-Type: application/json");
    Serial.println("Content-Type: application/json");
    client.println("Content-Length: 11");
    Serial.println("Content-Length: 11");
    client.println();
    Serial.println();
    client.println("{\"value\":0}");
    Serial.println("{\"value\":0}");
    client.println();
    Serial.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}

