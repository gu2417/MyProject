# 빌드 및 실행 방법

## 빌드 방법(make 경로 path 경로 연동 가능시)

```powershell
make
```

## 실행 방법(터미널 분활 해야합니다)

Terminal 1:

```powershell
.\server.exe
```

Terminal 2:

```powershell
.\client.exe
```

## 빌드 방법2(make 경로 path 경로 연동 가능시)

```powershell(server)
gcc -std=c11 -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8 -Iinclude -o server.exe src/server/server_main.c src/server/request_handler.c src/common/protocol.c src/common/save_config.c src/common/scheduler.c src/data/linked_list.c src/data/user.c src/storage/file_io.c -lws2_32
```

```powershell(client)
gcc -std=c11 -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8 -Iinclude -o client.exe src/client/client_main.c src/common/protocol.c src/common/save_config.c -lws2_32
```

## 실행 방법2(터미널 분활 해야합니다)

Terminal 1:

```powershell
.\server.exe
```

Terminal 2:

```powershell
.\client.exe
```
