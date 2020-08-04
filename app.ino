#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char		*ssid = "";
const char		*password = "";

const char		*server = "";
const char		*topic = "";
const char		*user = "";
const char		*pswd = "";
int				port = 0;

WiFiClient		wifi;
PubSubClient	client(wifi);

int				status = WL_IDLE_STATUS;
int				ledState = LOW;
int				ledInterval = 1000;

unsigned long	prev = 0;
unsigned long	elapsed = 0;

String			msg = String("MSG #");
unsigned int	counter = 1;
String			payload;


void blinkLed(int interval) {
	elapsed = millis();
	if (elapsed - prev < interval)
		return ;
	prev = elapsed;
	if (ledState == LOW)
		ledState = HIGH;
	else
		ledState = LOW;
	digitalWrite(LED_BUILTIN, ledState);
}

void reconnect(int interval) {
	while (!client.connected()) {
		Serial.println("Making MQTT connection...");
		if (client.connect("ESP8266Client", user, pswd))
		{
			Serial.println("Connected");
			client.publish(topic, "Wemos D1 Mini Connected!", true);
			client.subscribe("wemos/in");
		}
		else
		{
			Serial.println("Failed");
			Serial.println(client.state());
			Serial.println(WiFi.status());
			delay(interval);
		}
	}
}

void on_receive_cb(char *topic, byte *payload, unsigned int length) {
	Serial.print("Message arrived: [");
	Serial.print(topic);
	Serial.print("]");
	Serial.println();

	int i = -1;
	while (++i < length)
		Serial.print((char)payload[i]);
	Serial.println();

	int newLedInterval = String((char *)payload).toInt();
	if (newLedInterval)
		ledInterval = newLedInterval;
}

void setup()
{
	Serial.begin(115200);
	while (!Serial)
		;

	if (WiFi.status() == WL_NO_SHIELD)
	{
		Serial.println("No WiFi shield!");
		while (true)
			;
	}

	WiFi.mode(WIFI_STA);
	while (status != WL_CONNECTED)
	{
		Serial.println("Connecting...");
		status = WiFi.begin(ssid, password);
		delay(5000);
	}

	Serial.println("\nWifi connected!");
	Serial.println(WiFi.localIP());

	client.setServer(server, port);
	client.setCallback(on_receive_cb);

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, ledState);
}

void loop()
{
	blinkLed(ledInterval);

	if (!client.connected()) {
		reconnect(5000);
	}
	client.loop();

	payload = msg + counter;
	if (++counter >= 1000)
		counter = 0;

	client.publish("wemos/out", (char *)payload.c_str(), true);
	delay(33);
}
