# HTTPWebserver

A high-performance, lightweight HTTP web server implemented in C++20.

![C++20](https://img.shields.io/badge/std-c%2B%2B20-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Status](https://img.shields.io/badge/status-active-success)

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)


## 📖 Overview

HTTPWebserver is a custom implementation of the HTTP protocol designed to handle concurrent client connections efficiently. It serves static files and handles basic HTTP requests, showcasing modern C++ features

## ✨ Features

*   **Modern C++:** Built entirely using C++20 standards.
*   **Concurrent Handling:** Supports multiple simultaneous client connections.
*   **HTTP/1.1 Support:** Handles standard GET, POST, and HEAD requests.
*   **Static File Serving:** Efficiently serves HTML, CSS, JS, and image files.
*   **Configurable:** Port and root directory can be configured.
*   **Logging:** Comprehensive request and error logging.

## 🧩 Architecture

*   **Server Class:** Manages the socket creation, binding, and listening loop.
*   **Request Parser:** Parses raw TCP streams into HTTP request objects.
*   **Response Builder:** Constructs valid HTTP responses based on resource availability.
*   **Connection Manager:** Handles threading/async operations for client connections.
