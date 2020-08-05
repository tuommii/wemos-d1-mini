#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WIFI
const char		*primary_ssid = "";
const char		*primary_password = "";

const char		*secondary_ssid = "";
const char		*secondary_password = "";

// MQTT
const char		*server = "";
const char		*topic = "";
const char		*user = "";
const char		*pswd = "";
int				port = 0;

// LED
# define HOUR 1000 * 60 * 60

int				status = WL_IDLE_STATUS;
int				ledState = LOW;
int				ledInterval = 1000;
unsigned long	prevTime = 0;
unsigned long	elapsed = 0;

// CLIENTS
WiFiClient		wifi;
PubSubClient	client(wifi);

// MESSAGE
String			msg = String("MSG #");
unsigned int	counter = 1;
String			payload;

void setupWIFI() {
	if (WiFi.status() == WL_NO_SHIELD)
	{
		Serial.println("No WiFi shield!");
		while (true)
			;
	}

	WiFi.mode(WIFI_STA);
	while (status != WL_CONNECTED)
	{
		Serial.println("Connecting to primary WiFi...");
		status = WiFi.begin(primary_ssid, primary_password);
		if (status != WL_CONNECTED)
		{
			Serial.println("Connecting to secondary WiFi...");
			status = WiFi.begin(secondary_ssid, secondary_password);
		}
		delay(5000);
	}

	Serial.println("\nWifi connected!");
	Serial.println(WiFi.localIP());
}


void reconnectMQTT(int interval) {
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

void blinkLed(int interval) {
	elapsed = millis();
	if (elapsed - prevTime < interval)
		return ;
	prevTime = elapsed;
	if (ledState == LOW)
		ledState = HIGH;
	else
		ledState = LOW;
	digitalWrite(LED_BUILTIN, ledState);
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
	if (newLedInterval > 0 && newLedInterval < HOUR)
		ledInterval = newLedInterval;
}

/*=============================================================================
** SETUP & LOOOP
==============================================================================*/

void setup()
{
	Serial.begin(115200);
	while (!Serial)
		;

	setupWIFI();
	client.setServer(server, port);
	client.setCallback(on_receive_cb);
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, ledState);
}

void loop()
{
	blinkLed(ledInterval);

	if (!client.connected()) {
		reconnectMQTT(5000);
	}
	client.loop();

	payload = msg + counter;
	if (++counter >= 1000)
		counter = 1;

	client.publish(topic, (char *)payload.c_str(), true);
	delay(100);
}
