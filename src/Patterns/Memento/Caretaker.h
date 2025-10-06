#ifndef CARETAKER_H
#define CARETAKER_H

#include "GameMemento.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

// Caretaker Pattern: Manages memento history in memory
// Stores snapshots but never modifies them
class Caretaker {
private:
    std::vector<GameMemento> mementos;
    int maxSnapshots;
    
public:
    explicit Caretaker(int maxSaves = 10) : maxSnapshots(maxSaves) {}
    
    // Save a snapshot
    void saveMemento(const GameMemento& memento) {
        // Remove oldest if at capacity
        if (static_cast<int>(mementos.size()) >= maxSnapshots) {
            mementos.erase(mementos.begin());
            std::cout << "🗑️  Oldest snapshot removed (max capacity)" << std::endl;
        }
        
        mementos.push_back(memento);
        std::cout << "💾 Snapshot saved: " << memento.getSaveName() 
                  << " (" << mementos.size() << "/" << maxSnapshots << ")" << std::endl;
    }
    
    // Get specific snapshot by index
    GameMemento getMemento(int index) const {
        if (index < 0 || index >= static_cast<int>(mementos.size())) {
            throw std::out_of_range("Invalid snapshot index: " + std::to_string(index));
        }
        return mementos[index];
    }
    
    // Get most recent snapshot
    GameMemento getLatestMemento() const {
        if (mementos.empty()) {
            throw std::runtime_error("No snapshots available");
        }
        return mementos.back();
    }
    
    // Undo: Remove current and return previous
    GameMemento undo() {
        if (mementos.size() <= 1) {
            throw std::runtime_error("Cannot undo: no previous state available");
        }
        
        GameMemento removed = mementos.back();
        mementos.pop_back();
        
        std::cout << "⏪ Undoing: " << removed.getSaveName() << std::endl;
        std::cout << "   Restoring: " << mementos.back().getSaveName() << std::endl;
        
        return mementos.back();
    }
    
    // Query methods
    int getSnapshotCount() const { return static_cast<int>(mementos.size()); }
    bool hasSnapshots() const { return !mementos.empty(); }
    bool canUndo() const { return mementos.size() > 1; }
    int getMaxSnapshots() const { return maxSnapshots; }
    
    // Clear all snapshots
    void clear() {
        int count = mementos.size();
        mementos.clear();
        std::cout << "🗑️  Cleared " << count << " snapshot(s)" << std::endl;
    }
    
    // List all snapshots
    void listSnapshots() const {
        if (mementos.empty()) {
            std::cout << "\n📋 No snapshots available" << std::endl;
            return;
        }
        
        std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
        std::cout << "║       AVAILABLE SNAPSHOTS              ║" << std::endl;
        std::cout << "╚════════════════════════════════════════╝" << std::endl;
        std::cout << "Total: " << mementos.size() << "/" << maxSnapshots << std::endl;
        
        for (size_t i = 0; i < mementos.size(); i++) {
            const GameMemento& m = mementos[i];
            std::cout << "\n[" << i << "] " << m.getSaveName() << std::endl;
            std::cout << "    Time: " << m.getTimestamp() << std::endl;
            std::cout << "    Day " << m.getDay() << " | $" << m.getCurrency() << std::endl;
        }
    }
};

#endif // CARETAKER_H