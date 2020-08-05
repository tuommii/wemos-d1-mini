<h1 align="center">
	Learning IoT
</h1>

<p align="center">
	<br>
	<img width=400 src="/wemos.jpeg">
</p>

> Playing with IoT, MQTT and HTTP

## Commands

Listen messages

```sh
mosquitto_sub -d -u USERNAME -P PASSWORD -t wemos/out
```

Change LED blink rate (ms)

```sh
mosquitto_pub -d -u USERNAME -P PASSWORD -t wemos/in -m "100"
```
