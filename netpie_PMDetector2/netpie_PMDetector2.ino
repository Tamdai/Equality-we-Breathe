#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SDS011.h>

const char* ssid = "RBM_2.4GHz";
const char* password = "0649506228";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "c7056364-5a68-4ca2-b918-15261f334e84";
const char* mqtt_username = "HgWReAEF21r6cpSUKUWDntiWGxH8THRm";
const char* mqtt_password = "T3yY09d0s$KDAKxnGUjSAugLdHGnmXyU";
WiFiClient espClient;
PubSubClient client(espClient);
char msg[100];

float p = 3.1415926;
float p10,p25;
int value;
SDS011 my_sds;
int relay = D0; //relay>>D0

void reconnect() {
  while (!client.connected()) {
    Serial.print("Sensor MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to "); 
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  my_sds.begin(D3,D4); // TX D3 , RX D4
  pinMode(relay,OUTPUT);
  digitalWrite(relay,HIGH);
}

void loop() {
    value = my_sds.read(&p25,&p10);
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    String data = "{\"data\": {\"p25\":" + String(p25) + "}}";
    Serial.println(data);
    data.toCharArray(msg, (data.length() + 1));
    client.publish("@shadow/data/update", msg);

    if((p25) >= 20){
      digitalWrite(relay,LOW);
    } else
    {
      digitalWrite(relay,HIGH);
    }
    delay(2000);
}
