# Redis CLI Client (C++)

A lightweight Redis command-line client built entirely from scratch in C++. This project communicates directly with Redis using raw POSIX sockets, manually implements the Redis Serialization Protocol (RESP2), supports Redis authentication, Redis URLs, and TLS-encrypted connections without relying on external Redis client libraries.

The client supports both interactive shell mode and one-shot command execution.

---

## Platform Support

This project currently targets Linux-based environments and has been tested on:

* Ubuntu
* Debian-based distributions
* Windows Subsystem for Linux (WSL)

The implementation depends on POSIX networking APIs, GNU Readline, and OpenSSL for TLS-encrypted Redis connections.

Native Windows support is not currently provided.

---

## Features

* Raw POSIX socket communication
* IPv4 / IPv6 hostname resolution via `getaddrinfo`
* Manual RESP2 serialization and deserialization
* Interactive shell powered by GNU Readline
* Command history support
* Makefile-based build workflow
* One-shot command execution
* Redis AUTH support
* ACL username + password authentication
* Redis URL parsing
* TLS support using OpenSSL (`rediss://`)
* Quoted string parsing
* Configurable host and port
* Local Redis support
* Cloud Redis support (tested with Upstash)

---

## Architecture

```text
┌───────────────────────────────────────────────────┐
│                      main.cpp                     │
│      Parses startup arguments and launch mode     │
└─────────────────────────┬─────────────────────────┘
                          │
                          ▼
┌───────────────────────────────────────────────────┐
│                     CLI Class                     │
│  Handles interactive mode and one-shot execution  │
└───────┬───────────────────────────┬───────────────┘
        │                           │
        ▼                           ▼
┌───────────────────────┐  ┌─────────────────────────────┐
│    CommandHandler     │  │         RedisClient         │
│ Tokenizes commands    │  │ TCP / TLS Connection Layer  │
│ Builds RESP payloads  │  │ Socket Communication        │
└──────────┬────────────┘  └───────────┬─────────────────┘
           │                           │
           └─────────────┬─────────────┘
                         ▼
                 ┌───────────────┐
                 │ Redis Server  │
                 └───────┬───────┘
                         │
                         ▼
                 ┌───────────────┐
                 │ ResponseParser│
                 │ RESP2 Decoder │
                 └───────────────┘
```

---

## Authentication

### Password Authentication

```bash
./main -pass mysecret
```

### ACL Authentication

```bash
./main -u default -pass mysecret
```

The client automatically issues:

```redis
AUTH password
```

or:

```redis
AUTH username password
```

depending on the provided credentials.

---

## Redis URL Support

The client supports both standard Redis URLs and TLS Redis URLs.

### Standard Redis

```bash
./main -url "redis://:password@localhost:6379"
```

### ACL Authentication

```bash
./main -url "redis://default:password@localhost:6379"
```

### TLS Connection

```bash
./main -url "rediss://default:password@host:6379"
```

Supported URL components:

```text
scheme://[username[:password]@]host[:port]
```

Examples:

```text
redis://localhost
redis://localhost:6379
redis://:password@localhost:6379
redis://default:password@localhost:6379
rediss://default:password@host:6379
```

---

## TLS Support

The client supports encrypted Redis connections using OpenSSL.

Connection flow:

```text
TCP Connect
     ↓
TLS Handshake
     ↓
AUTH (optional)
     ↓
RESP2 Commands
```

When a URL uses:

```text
rediss://
```

the client automatically establishes a TLS session using:

```cpp
SSL_connect()
SSL_write()
SSL_read()
```

allowing secure communication with cloud-hosted Redis providers such as Upstash.

---

## Building

### Install Dependencies

Ubuntu / Debian:

```bash
sudo apt update

sudo apt install \
    g++ \
    make \
    libreadline-dev \
    libssl-dev
```

### Manual Compilation

```bash
g++ -std=c++17 \
    main.cpp \
    RedisClient.cpp \
    CommandHandler.cpp \
    ResponseParser.cpp \
    RedisUrlParser.cpp \
    CLI.cpp \
    -o main \
    -lreadline \
    -lssl \
    -lcrypto
```

### Using the Makefile

The project includes a Makefile for common development tasks.

Compile the project:

```bash
make com
```

Compile and immediately launch the client:

```bash
make crun
```

Run an already compiled executable:

```bash
make run
```

Merge source files into a single file:

```bash
make merge
```

Remove the compiled executable:

```bash
make clean
```

Current Makefile:

```make
com:
	g++ *.cpp -lreadline -lssl -lcrypto -o main

run:
	./main

crun:
	g++ *.cpp -lreadline -lssl -lcrypto -o main && ./main

merge:
	python3 merge.py

clean:
	rm -f main
```

---

## Usage

### Interactive Mode

Launch the Redis shell:

```bash
./main
```

Example:

```text
Connected to Redis server

127.0.0.1:6379> SET name Soumik
OK

127.0.0.1:6379> GET name
Soumik

127.0.0.1:6379> INCR visits
(integer) 1

127.0.0.1:6379> exit
Goodbye from your client.
```

---

### One-Shot Mode

Execute a single Redis command and exit:

```bash
./main PING
```

Output:

```text
PONG
```

Example:

```bash
./main GET name
```

Output:

```text
Soumik
```

---

### Custom Host and Port

```bash
./main -h localhost -p 6379 PING
```

---

### Password Authentication

```bash
./main -pass mysecret
```

---

### ACL Authentication

```bash
./main -u default -pass mysecret
```

---

### Redis URL

```bash
./main -url "redis://:password@localhost:6379"
```

---

### TLS Redis URL

```bash
./main -url "rediss://default:password@host:6379"
```

---

## RESP2 Support

Supported response types:

| Type             | Supported |
| ---------------- | --------- |
| Simple String    | ✅         |
| Error            | ✅         |
| Integer          | ✅         |
| Bulk String      | ✅         |
| Array            | ✅         |
| Null Bulk String | ✅         |
| Null Array       | ✅         |

---

## Tested Commands

Examples of commands successfully tested:

```redis
PING
SET
GET
DEL
INCR
MGET
KEYS
TYPE
AUTH
```

---

## Future Improvements

* RESP3 support
* Redis-style array formatting
* Command auto-completion
* TLS certificate verification
* Connection pooling
* Pipelining support

---

## License

This project is intended for learning, experimentation, and educational purposes.
