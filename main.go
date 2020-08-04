package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"net/http"
	"os"
	"time"
)

const errResponse = "Stop that!\n"

func hello(w http.ResponseWriter, r *http.Request) {
	now := time.Now()

	if r.Header.Get("User-Agent") != "ESP8266HTTPClient" || r.Header.Get("Secret") != "Jing Yang" {
		w.Write([]byte(errResponse))
		log.Println("Header not valid")
		return
	}

	if r.Method != "POST" {
		w.Write([]byte(errResponse))
		log.Println("Method not valid")
		return
	}

	body := io.LimitReader(r.Body, 4096)
	_, err := io.Copy(os.Stdout, body)
	fmt.Println()

	if err != nil {
		w.Write([]byte(errResponse))
		log.Println("Body not valid")
		return
	}

	log.Println("Took: ", time.Since(now))
	w.Write([]byte("Got It!"))
}

func main() {
	port := flag.String("port", "8000", "port to listen")
	flag.Parse()
	http.HandleFunc("/hello", hello)
	if err := http.ListenAndServe(":"+*port, nil); err != nil {
		log.Fatal(err)
	}
}
