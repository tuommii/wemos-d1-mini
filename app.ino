#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

const char	*ssid = "EDIT";
const char	*pass = "EDIT";
int			status = WL_IDLE_STATUS;
int			code;
String		payload;
HTTPClient	http;

void setup()
{
	Serial.begin(115200);
	while (!Serial)
	{
		;
	}

	if (WiFi.status() == WL_NO_SHIELD)
	{
		Serial.println("No WiFi shield!");
		while (true)
		{
			;
		}
	}

	WiFi.mode(WIFI_STA);
	while (status != WL_CONNECTED)
	{
		Serial.println("Connecting...");
		status = WiFi.begin(ssid, pass);
		delay(5000);
	}

	Serial.println("\nWifi connected!");
	Serial.println(WiFi.localIP());
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
	http.begin("http://192.168.1.105:8000/hello");
	http.addHeader("Content-Type", "text/plain");

	code = http.POST("Hello from Wemos D1 Mini!");
	payload = http.getString();
	http.end();

	Serial.println(code);
	Serial.println(payload);

	digitalWrite(LED_BUILTIN, HIGH);
	delay(5000);
	digitalWrite(LED_BUILTIN, LOW);
	delay(5000);
}
