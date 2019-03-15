CC=gcc
CFLAGS=-Wall -g -Og

all: server client

server: ds_server.c
	$(CC) $(CFLAGS) `xml2-config --cflags` -o $@ $< `xml2-config --libs`

client: ds_client.c
	$(CC) $(CFLAGS) `xml2-config --cflags` -o $@ $< `xml2-config --libs`

clean:
	rm server client
 
