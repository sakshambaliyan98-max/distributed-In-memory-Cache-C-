# Distributed In-Memory Cache

A production-inspired distributed in-memory key-value cache built from scratch in **C++17** to explore systems programming, concurrent programming, networking, storage systems, and distributed systems.

> **Status:** 🚧 In Progress

---

## Motivation

Modern applications rely heavily on distributed caches to reduce database load, decrease latency, and improve scalability.

The objective of this project is **not** to clone Redis, but to understand and implement the core ideas behind production-grade caching systems from first principles.

This project is being developed incrementally, following the same design-first approach used in large engineering organizations.

---

# Features

## Cache Core

- In-memory key-value storage
- Modular cache engine
- Configurable cache capacity
- Storage abstraction layer
- Interface-driven architecture (SOLID)

---

## Eviction Policies

- LRU (Least Recently Used)
- LFU (Least Frequently Used)
- FIFO
- TTL-based expiration

---

## Networking

- TCP Cache Server
- Text-based command protocol
- Multi-client support

Example:

```
SET user Saksham
GET user
DELETE user
EXISTS user
```

---

## Concurrency

- Thread Pool
- Producer-Consumer Request Queue
- Mutexes
- Condition Variables
- Reader-Writer Locks
- Thread-safe Cache Operations

---

## Distributed System

- Consistent Hashing
- Virtual Nodes
- Replication
- Heartbeat-based Failure Detection
- Automatic Request Routing

---

## Persistence

- Write Ahead Log (WAL)
- Snapshot-based Recovery
- Crash Recovery
- Configurable Persistence Strategy

---

## Observability

- Cache Statistics
- Hit/Miss Ratio
- Structured Logging
- Health Checks
- Performance Metrics

---

## Production Features

- API Key Authentication
- Rate Limiting
- Graceful Shutdown
- Docker Deployment
- CI/CD Pipeline

---

# Project Architecture

```
                 Clients
                     │
               TCP Server
                     │
              Thread Pool
                     │
              Cache Engine
        ┌────────┼─────────┐
        │        │         │
   Eviction   Storage   Persistence
        │        │         │
        │   HashMapStorage │
        │                  │
        └────────┬─────────┘
                 │
          Metrics & Logging
```

---

# Project Structure

```
DistributedCache/

├── include/
│   ├── cache/
│   ├── common/
│   ├── storage/
│   ├── eviction/
│   ├── network/
│   ├── protocol/
│   └── server/
│
├── src/
│
├── tests/
│
├── benchmark/
│
├── docs/
│
├── docker/
│
├── scripts/
│
└── CMakeLists.txt
```

---

# Build

```bash
git clone <repository-url>

cd DistributedCache

mkdir build

cd build

cmake ..

make
```

---

# Run

```bash
./DistributedCache
```

---

# Design Principles

- SOLID Principles
- Dependency Injection
- Strategy Pattern
- Interface-based Architecture
- Separation of Concerns
- Modular Design

---

# Technologies

- C++17
- STL
- CMake
- TCP Sockets
- Multithreading
- Docker
- GitHub Actions

---

# Development Roadmap

- [x] Project Structure
- [x] Cache Models
- [ ] Cache Engine
- [ ] Storage Layer
- [ ] Eviction Framework
- [ ] LRU Cache
- [ ] LFU Cache
- [ ] TTL Support
- [ ] TCP Server
- [ ] Command Protocol
- [ ] Thread Pool
- [ ] Persistence
- [ ] Consistent Hashing
- [ ] Replication
- [ ] Metrics
- [ ] Authentication
- [ ] Docker
- [ ] Benchmarks
- [ ] CI/CD

---

# Future Improvements

- Async Networking (epoll)
- Lock Striping
- Object Pool Allocator
- Slab Allocation
- Cluster Auto-Rebalancing
- gRPC Support
- Prometheus Metrics
- Kubernetes Deployment

---

# Learning Objectives

This project explores concepts from:

- Data Structures
- Object-Oriented Design
- Design Patterns
- Concurrent Programming
- Operating Systems
- Computer Networks
- Distributed Systems
- Storage Systems
- Performance Engineering
- Backend Infrastructure

---

# License

MIT License
