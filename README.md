# HTTPWebserver

A minimal HTTP server implemented in modern C++20.

This project demonstrates low-level networking, HTTP parsing, multithreading with a threadpool, C++20 modules, and manual server architecture without external libraries.

## Table of Contents
- **[Overview](#overview)**
- **[Project Features](#project-features)**
  - **[Networking](#networking)**
  - **[HTTP Handling](#http-handling)**
  - **[C++20 Modules](#c20-modules)**
  - **[Concurrency](#concurrency)**
  - **[Coroutine Support (Minimal)](#coroutine-support-minimal)**
  - **[Architecture](#architecture)**
- **[Detailed Technical Breakdown](#detailed-technical-breakdown)**
- **[Concurrency Model](#concurrency-model)**
  - **[Thread Pool (Implemented)](#thread-pool-implemented)**
  - **[Coroutines (Available but Minimal)](#coroutines-available-but-minimal)**
- **[Not Implemented](#not-implemented)**
- **[Build and Run](#build-and-run)**
- **[HTTP Pipeline Diagram](#http-pipeline-diagram)**
- **[Limitations](#limitations)**
- **[Future Improvements](#future-improvements)**
- **[Summary](#summary)**

## Overview

This server is built from scratch using raw POSIX sockets and modern C++20 features.

It serves static HTML pages, static assets, and basic routes.

**Key technologies:**

- **C++20 modules (.ixx)**
- **POSIX TCP sockets**
- **Thread-pool concurrency**
- **Manual HTTP parsing and response generation**
- **CMake multi-file project structure**

The goal of this project is to deeply understand how real web servers operate at the lowest level.

## Project Features

### Networking

- **IPv4 TCP socket creation**
- **Bind, listen, accept loop**
- **Read/write raw HTTP requests and responses**

### HTTP Handling

- **GET request parsing**
- **Path extraction**
- **Static file routing** (`/`, `/about`, `/contact`, `/public/...`)
- **MIME detection** for HTML and CSS
- **404 handling**

### C++20 Modules

The codebase uses `export module ...` structure for:

- **`http.threadpool`**
- **`http.coroutines`**
- **`http.tcpserver`** (server core)

This demonstrates real-world modular C++ application layout.

### Concurrency

- **Functional thread-pool with worker threads**
- **Task queue protected by `std::mutex` + `std::condition_variable`**
- **Futures for task results** via `std::future` / `std::packaged_task`

### Coroutine Support (Minimal)

- **A minimal coroutine promise type is implemented**
- **Enables coroutine syntax, but does not suspend (synchronous only)**
- **Included as a foundation for future async development**

### Architecture

- **`src/`**
  - `threadpool.ixx` – Worker threads, task queue
  - `coroutines.ixx` – Minimal coroutine type (`suspend_never`)
  - `tcpserver.ixx` – Socket logic and request handling
- **`include/`** – Public headers (if any)
- **`templates/`** – HTML pages
- **`public/`** – CSS and static assets
- **`main.cpp`** – Program entry
- **`CMakeLists.txt`** – Build configuration

## Detailed Technical Breakdown

### 1. TCP Server

The TCP server manages:

- **Creating the listening socket**
- **Binding to `0.0.0.0:8080`**
- **Running the accept loop**
- **Reading raw HTTP request text**
- **Passing requests to the router**
- **Sending the final HTTP response**

This is a raw POSIX socket implementation.

No frameworks, no wrappers.

### 2. Request Parsing

The incoming request is plain text. The server extracts, for example:

```text
GET /about HTTP/1.1
Host: localhost
```

Parsing extracts:

- **Method**: `GET`
- **Path**: `/about`

### 3. Routing

The path is mapped to files:

- **Path** `→` **File**
- `/` → `templates/index.html`
- `/about` → `templates/about.html`
- `/contact` → `templates/contact.html`
- `/public/*` → Directly served static files

If the file doesn’t exist:

- **Return a generated `404 Not Found` response.**

### 4. Static File Serving

The server loads:

- **HTML**
- **CSS**
- **Images or other static assets**

MIME types are determined by file extension.

The entire file is read, inserted into the HTTP body, and sent in one response.

## Concurrency Model

### Thread Pool (Implemented)

The `ThreadPool` class includes:

- **Worker threads created at startup**
- **A `std::queue<std::function<void()>>` of tasks**
- **`std::mutex` + `std::condition_variable` for synchronization**
- **Safe enqueueing via `enqueue()`**
- **Future-based results with `std::packaged_task`**

This is full true concurrency.

### Coroutines (Available but Minimal)

`Task` in `http.coroutines` uses:

- **`std::suspend_never`**

Meaning:

- **Coroutines do not suspend**
- **No asynchronous behavior**
- **No event-loop**
- **Not used for HTTP request handling**

It exists as an educational demonstration of how coroutine promises work.

## Not Implemented

To be technically accurate, the following are **not** part of the server:

- **No asynchronous socket I/O**
- **No `epoll`/`kqueue`/`select` loop**
- **No coroutine scheduler**
- **No awaitable networking**

## Build and Run

### Requirements

- **C++20 capable compiler** (Clang, GCC, MSVC)
- **CMake 3.20+**

### Commands

```bash
git clone https://github.com/NicholasAli1/HTTPWebserver
cd HTTPWebserver
mkdir build
cd build
cmake ..
cmake --build .
./HTTPWebserver
```

Visit:

`http://localhost:8080`

## HTTP Pipeline Diagram

`Browser → TCPServer → HTTP Parser → Router → File Loader → Response Builder → Browser`

**Step by step:**

1. Browser sends `GET /path`
2. Server accepts connection
3. Request line parsed
4. Router selects file
5. File is loaded into memory
6. HTTP response is assembled
7. Response is sent back
8. Connection closes

## Limitations

Current version does not support:

- **POST requests**
- **HTTPS**
- **JSON APIs**
- **Persistent connections**
- **Hot-reloading**
- **Logging subsystem**

This is intentional — the project focuses on foundational server mechanics.

## Future Improvements

Planned possible upgrades:

- **Add coroutine-based async I/O**
- **Add an event loop**
- **Add POST and JSON routes**
- **Support persistent HTTP/1.1 connections**
- **Add TLS via OpenSSL**
- **Add caching and compression**
- **Add routing table with handler functions**
- **Add middleware system**

## Summary

This project demonstrates:

- **Real low-level networking in C++**
- **Modern C++20 modules and organization**
- **Thread-pool concurrency**
- **Hands-on HTTP protocol handling**
- **Manual request parsing and response generation**
- **Multi-file C++ architecture with CMake**
- **Foundations for async and coroutine-based servers**

This is the type of project that shows employers strong systems-level engineering ability.
