# Redis CLI Client (C++)

A lightweight Redis command-line client built from scratch in C++. This project communicates directly with Redis using raw POSIX sockets and implements the Redis Serialization Protocol (RESP2) manually without relying on external Redis client libraries.

The client supports both interactive shell mode and one-shot command execution.

---

## Platform Support

This project currently targets Linux-based environments and has been tested on:

* Ubuntu
* Debian-based distributions
* Windows Subsystem for Linux (WSL)

The implementation depends on POSIX networking APIs (`socket`, `connect`, `send`, `recv`, `close`) and GNU Readline. Native Windows support is not currently provided.

---

## Features

* Built using raw POSIX sockets (`socket`, `connect`, `send`, `recv`)
* Manual RESP2 serialization and deserialization
* Interactive shell powered by GNU Readline
* Command history support
* One-shot command execution
* IPv4 / IPv6 hostname resolution via `getaddrinfo`
* Quoted string parsing
* Configurable host and port

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
┌───────────────────────┐  ┌────────────────────────┐
│    CommandHandler     │  │      RedisClient       │
│ Tokenizes commands    │  │ TCP connection layer   │
│ Builds RESP payloads  │  │ Socket communication   │
└──────────┬────────────┘  └───────────┬────────────┘
           │                           │
           └─────────────┬─────────────┘
                         ▼
                ┌────────────────┐
                │ Redis Server   │
                └───────┬────────┘
                        │
                        ▼
                ┌────────────────┐
                │ ResponseParser │
                │ RESP2 Decoder  │
                └────────────────┘
```

---

## Components

### RedisClient

Responsible for:

* Hostname resolution using `getaddrinfo()`
* TCP socket creation
* Connection management
* Reliable transmission of commands
* Socket cleanup

### CommandHandler

Responsible for:

* Splitting user input into arguments
* Supporting quoted strings
* Encoding commands into RESP2 format

Example:

```text
SET user:name "Soumik Majumder"
```

becomes:

```text
*3
$3
SET
$9
user:name
$16
Soumik Majumder
```

### ResponseParser

Parses Redis RESP2 responses directly from the socket stream.

Supported types:

| Prefix | Type          | Example                    |
| ------ | ------------- | -------------------------- |
| `+`    | Simple String | `+OK\r\n`                  |
| `-`    | Error         | `-ERR unknown command\r\n` |
| `:`    | Integer       | `:1000\r\n`                |
| `$`    | Bulk String   | `$5\r\nhello\r\n`          |
| `*`    | Array         | `*2\r\n...`                |

### CLI

Responsible for:

* Interactive shell mode
* One-shot execution mode
* Command history via GNU Readline
* User interaction flow

---

## Building

### Install Dependencies

Ubuntu / Debian:

```bash
sudo apt update
sudo apt install g++ make libreadline-dev
```

### Manual Compilation

```bash
g++ -std=c++11 \
    main.cpp \
    RedisClient.cpp \
    CommandHandler.cpp \
    ResponseParser.cpp \
    CLI.cpp \
    -o main \
    -lreadline
```

### Using the Makefile

Compile and run:

```bash
make crun
```

Run an already compiled executable:

```bash
make run
```

Remove the executable:

```bash
make clean
```

---

## Usage

After compilation, the executable generated is:

```bash
./main
```

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

127.0.0.1:6379> MGET name visits
Soumik
1

127.0.0.1:6379> exit
Goodbye from your client.
```

---

### One-Shot Mode

Execute a single Redis command and exit:

```bash
./main GET name
```

Output:

```text
Soumik
```

Example:

```bash
./main SET status active
```

Output:

```text
OK
```

Another example:

```bash
./main PING
```

Output:

```text
PONG
```

---

### Custom Host and Port

Override the default host (`127.0.0.1`) and port (`6379`):

```bash
./main -h 192.168.1.50 -p 7000 GET status
```

Example:

```bash
./main -h localhost -p 6379 PING
```

---

## RESP2 Support

The client currently supports the following RESP2 response types:

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

## Example: Reliable TCP Send Loop

TCP does not guarantee that every byte is transmitted in a single call to `send()`. The client therefore continues sending until the entire payload has been delivered.

```cpp
size_t totalSent = 0;

while (totalSent < command.size())
{
    ssize_t sent = send(
        sockfd,
        command.c_str() + totalSent,
        command.size() - totalSent,
        0
    );

    if (sent <= 0) {
        return false;
    }

    totalSent += sent;
}
```

---

## Example: RESP Dispatch

Incoming Redis replies are dispatched based on the RESP prefix byte.

```cpp
switch (prefix)
{
    case '+': return parseSimpleString(sockfd);
    case '-': return parseSimpleError(sockfd);
    case ':': return parseInteger(sockfd);
    case '$': return parseBulkString(sockfd);
    case '*': return parseArrays(sockfd);
    default:  return "(Error) unknown reply type.";
}
```

---

## Future Improvements

* RESP3 support
* TLS (`rediss://`) support
* Redis AUTH support
* Command auto-completion
* Redis-style array formatting
* Connection URL parsing

---

## License

This project is intended for learning, experimentation and educational purposes.
