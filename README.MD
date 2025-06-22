## 🚀 Tech Stack

### Language
- **C++17** — Modern C++ for memory-safe and performant backend systems.

### Core Features
- **In-Memory Storage** — Hash table with chaining via linked lists.
- **Key-Value Commands** — Supports `SET`, `GET`, `DEL` operations.
- **Command Pattern** — Each command implements a unified interface for flexibility.
- **Factory Pattern** — Commands are dynamically created using a factory registry.
- **Strategy Pattern** — Pluggable hash function interface for custom hash algorithms.

### Future Extensions
- TTL support (`EXPIRE`)
- Additional data types (List, Set, Hash)
- Pub/Sub model
- AOF and RDB persistence
- Replication and clustering
