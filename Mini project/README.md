```markdown
# Peer-to-Peer Chat Application

A command-line peer-to-peer (P2P) chat application that allows users to connect over TCP, exchange messages, and manage connections. Built in C with a focus on reliability, security, and clean code practices.

---

## Features

### Functional Requirements
- **CLI Commands**:
  - `help`: Show available commands
  - `myip`: Display local IP address
  - `myport`: Show listening port
  - `connect <IP> <port>`: Establish TCP connection to another peer
  - `list`: List all active connections
  - `terminate <ID>`: Close a connection by ID
  - `send <ID> <message>`: Send message to a peer (max 100 chars)
  - `exit`: Close all connections and exit
- Connection management with error handling for:
  - Invalid IP/port
  - Self-connections
  - Duplicate connections
  - Sudden disconnections

### Non-Functional Requirements
- Multi-threaded architecture using `pthread`
- Memory leak prevention with resource cleanup
- Input validation and error handling
- Modular code structure with separation of concerns
- Valgrind-compatible memory management

---

## Usage

Start the application on a port:
```bash
./peerchat <port>
```

**Example Session**:
```
> myip
192.168.1.6

> myport
5000

> connect 192.168.1.6 5001
Connected to 192.168.1.6:5001

> list
1: 192.168.1.6 5001

> send 1 Hello World!
Message sent to 192.168.1.6:5001

[Peer] Message from 192.168.1.6:5001 -> Hi there!

> exit
Closing all connections...
```

---

## Design

### Architecture
```
.
├── inc
│   ├── cli.h
│   ├── common.h
│   ├── connection.h
│   ├── message.h
│   └── utils.h
├── Makefile
├── peer_app
├── README.md
└── src
    ├── cli.c
    ├── cli.o
    ├── common.c
    ├── common.o
    ├── connection.c
    ├── connection.o
    ├── main.c
    ├── main.o
    ├── message.c
    ├── message.o
    ├── utils.c
    └── utils.o

```

### Key Components
- **Multi-threading**: Uses `select()` for async I/O and `pthread` for message receiving
- **Memory Safety**: Cleanup functions with `shutdown()`/`close()` for sockets
- **Validation**:
  - IP format checking
  - Port range verification
  - Message length enforcement

---

## Testing

1. **Memory Leak Check**:
   ```bash
   valgrind --leak-check=full ./peerchat 1234
   ```

2. **Connection Test**:
   - Run two instances on different ports
   - Use `connect` to establish bidirectional links
   - Verify message delivery and connection listing

---
