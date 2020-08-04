<h1 align="center">
	IoT - (Learning)
</h1>

<p align="center">
	<br>
	<img width=400 src="/wemos.jpeg">
</p>

> Playing with IoT, MQTT and HTTP

## Commands

Listen messages

`mosquitto_sub -d -u USERNAME -P PASSWORD -t wemos/out`

Change LED Blink rate (ms)

`mosquitto_pub -d -u USERNAME -P PASSWORD -t wemos/in -m "100"`
