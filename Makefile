CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8 -Iinclude
LDFLAGS := -lws2_32

COMMON_SRC := src/common/protocol.c src/common/save_config.c src/common/scheduler.c src/data/linked_list.c src/data/user.c src/storage/file_io.c
SERVER_SRC := src/server/server_main.c src/server/request_handler.c $(COMMON_SRC)
CLIENT_SRC := src/client/client_main.c src/common/protocol.c src/common/save_config.c
HEADERS := $(wildcard include/*.h)

.PHONY: all clean

all: server.exe client.exe

server.exe: $(SERVER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_SRC) $(LDFLAGS)

client.exe: $(CLIENT_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_SRC) $(LDFLAGS)

clean:
	rm -f server.exe client.exe
