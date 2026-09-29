package main

import (
	"fmt"
	"net"
)

func handleConnection(conn net.Conn) {
	var content []byte

	_, err := conn.Read(content)
	if err != nil {
		panic(err)
	}

	fmt.Println(content)
}

func main() {
	listener, err := net.Listen("tcp", ":8080")
	if err != nil {
		panic(err)
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			fmt.Println("Unable to accept connection.")
		}

		go handleConnection(conn)
	}
}
