#ifndef GAMEMEMENTO_H
#define GAMEMEMENTO_H

#include "Core/GameData.h"

// Memento Pattern: Immutable snapshot of game state
// Only the Originator (GameManager) can create mementos
class GameMemento {
private:
    GameData state;
    
    // Private constructor - only friends can create
    GameMemento(const GameData& data) : state(data) {}
    
    friend class GameManager;  // Originator can create mementos
    
public:
    // Read-only access to state (for Caretaker and restoration)
    const GameData& getState() const { return state; }
    
    // Convenience accessors for metadata
    std::string getSaveName() const { return state.saveName; }
    std::string getTimestamp() const { return state.timestamp; }
    int getDay() const { return state.time.day; }
    double getCurrency() const { return state.economy.currency; }
    
    // Display
    void printSummary() const { state.printSummary(); }
};

#endif // GAMEMEMENTO_H