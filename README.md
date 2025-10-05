# 🌱 TEMPLANTER

A 2D plant management simulation game built with C++ and SFML, featuring 14 software design patterns.

## 🎮 Game Description

Manage your greenhouse empire! Plant seeds, hire workers, serve customers, and grow your business from a small 3x3 plot to a thriving botanical empire.

## 🎯 Features

- **10 Plant Types** with tiered difficulty (Basil to Rare Cactus)
- **Worker Skill System** - Train workers from rookies to 5-star experts
- **Real-Time Gameplay** - Dynamic day/night cycle
- **Customer System** - Serve customers, manage ratings, defend against robbers
- **Achievement System** - 30+ achievements to unlock
- **Save System** - Manual save/load with 3 save slots

## 🛠️ Technical Features

Implements 14 design patterns:
1. **Singleton** - Game Manager, Time Manager, Save Manager
2. **Observer** - Plant watering notifications
3. **Mediator** - Work scheduler coordination
4. **Memento** - Save/load game state
5. **Factory** - Plant/customer/product creation
6. **Prototype** - Clone plant configurations
7. **Template Method** - Plant growth cycles
8. **Decorator** - Worker role assignment
9. **Strategy** - Growth strategies with nutrients
10. **State** - Plant growth states
11. **Composite** - Storage hierarchy
12. **Iterator** - Time progression
13. **Command** - Worker task queue
14. **Visitor** - Customer behavior
15. **Flyweight** - Memory optimization for plants

## 📋 Requirements

- C++17 or higher
- SFML 2.5+
- CMake 3.10+ (optional)
- nlohmann/json library

## 🚀 Installation

### Windows
```bash
# Install SFML via vcpkg
vcpkg install sfml

# Build
mkdir build && cd build
cmake ..
cmake --build .