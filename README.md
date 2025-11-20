# io-multiplexing

A repository exploring I/O multiplexing techniques, including epoll, poll, and select, kqueue. Designed for education, experimentation, and performance benchmarking of different I/O models on *nix systems.

## Overview

I/O multiplexing allows a program to monitor multiple file descriptors (sockets, files, etc.) to see if I/O is possible on any of them. This is fundamental for building scalable network applications and servers.

This repo features simple, idiomatic examples of:
- **select**
- **poll**
- **epoll**
- **kqueue**

Compare their performance, understand their APIs, and learn how to implement non-blocking network servers using low-level system calls.

## Getting Started

### Prerequisites

- Linux or macOS for full API support
- GCC or Clang
- Basic C development tools (`make`, etc.)

## Usage

See integration_test

## License

MIT License

## References

- [Linux epoll documentation](https://man7.org/linux/man-pages/man7/epoll.7.html)
- [select(2) man page](https://man7.org/linux/man-pages/man2/select.2.html)
- [poll(2) man page](https://man7.org/linux/man-pages/man2/poll.2.html)
