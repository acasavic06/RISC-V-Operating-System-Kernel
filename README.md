# RISC-V Operating System Kernel

A simplified operating system kernel developed in **C++ and RISC-V Assembly** as part of the **Operating Systems 1** course at the School of Electrical Engineering, University of Belgrade.

The project implements the core mechanisms of a multitasking operating system, including dynamic memory allocation, thread management, scheduling, semaphores, system calls, context switching, interrupt handling and console I/O.

---

## Overview

This project represents a small but fully functional operating system kernel running on a **RISC-V architecture** inside a QEMU emulator.

The implementation was developed completely from scratch without relying on any operating system services for:

- memory management
- thread scheduling
- synchronization
- context switching
- interrupt handling

The kernel communicates with user programs through multiple API layers and executes inside an emulated RISC-V environment.

---

## Features

### Memory Management

- Custom dynamic memory allocator
- Memory allocation (`mem_alloc`)
- Memory deallocation (`mem_free`)
- Block-based heap management

### Thread Management

- Thread creation
- Thread termination
- Context switching
- Cooperative dispatch
- Time sharing
- Sleeping threads

### Scheduler

- Ready queue implementation
- Context switching
- Thread dispatch
- Time slice management

### Synchronization

- Kernel semaphores
- wait()
- signal()
- wait(n)
- signal(n)

### System Calls

Implementation of:

- Memory allocation
- Thread API
- Semaphore API
- Sleeping threads
- Console input/output

using

- ABI layer
- C API
- C++ Object-Oriented API

### Interrupt Handling

- Software interrupts
- Timer interrupts
- Console interrupts
- Trap handling
- RISC-V privilege mode switching

### Console

- Character input
- Character output
- Interrupt-driven console communication

---

## Project Architecture

```
User Program
      │
      ▼
 C++ API
      │
      ▼
  C API
      │
      ▼
   ABI Layer
      │
      ▼
 Operating System Kernel
      │
      ▼
 Hardware Interface
```

The kernel is organized into multiple abstraction layers following the project specification.

---

## Project Structure

```
RISC-V-Operating-System-Kernel
│
├── include/          # Header files
├── src/              # Kernel implementation
├── test/             # Test programs
├── docs/             # Project specification
├── screenshots/      # Project screenshots
├── README.md
└── LICENSE
```

---

## Technologies

- C++
- RISC-V Assembly
- QEMU Emulator
- Make
- CLion
- Git

---

## Core Concepts Implemented

- Dynamic Memory Allocation
- Thread Management
- Cooperative Scheduling
- Context Switching
- Semaphores
- Synchronization
- Sleeping Threads
- System Calls
- Trap Handling
- Interrupt Processing
- Console Driver

---

## Screenshots

### Project Structure

(Add screenshots here if available.)

---

## Building

The project is intended to be built using the provided Makefile inside the development environment supplied with the course.

Typical commands:

```bash
make qemu
```

Run in debug mode:

```bash
make qemu-gdb
```

Clean project:

```bash
make clean
```

---

## Learning Outcomes

During this project the following concepts were implemented and understood:

- Operating system architecture
- Low-level memory management
- Thread scheduling
- Synchronization primitives
- Interrupt handling
- Context switching
- RISC-V privilege modes
- System call implementation
- Assembly and C++ interoperability

---

## Course Information

**Course:** Operating Systems 1

**Institution:** School of Electrical Engineering, University of Belgrade

---

## License

This repository is licensed under the MIT License.
