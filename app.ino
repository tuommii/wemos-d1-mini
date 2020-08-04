#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

const char		*ssid = "EDIT";
const char		*pass = "EDIT";
int				status = WL_IDLE_STATUS;
int				code;
unsigned int	counter = 0;
int				ledState = LOW;
unsigned long	prevMillis = 0;
unsigned long	currMillis = 0;
String			msg = String("MSG #");
String			payload;
String			response;
HTTPClient		http;

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
	digitalWrite(LED_BUILTIN, ledState);
}

void blinkLed(int interval) {
	currMillis = millis();
	if (currMillis - prevMillis < interval)
		return ;
	prevMillis = currMillis;
	if (ledState == LOW)
	{
		ledState = HIGH;
	}
	else
	{
		ledState = LOW;
	}
	digitalWrite(LED_BUILTIN, ledState);
}

void loop()
{
	blinkLed(1000);
	http.begin("http://192.168.1.105:8000/hello");
	http.addHeader("Content-Type", "text/plain");
	http.addHeader("Secret", "Jing Yang");

	counter++;
	payload = msg + counter;
	if (counter >= 1000)
	{
		counter = 0;
	}

	code = http.POST(payload);
	response = http.getString();
	http.end();

	Serial.println(code);
	Serial.println(response);

	delay(420);
}
