#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "Patterns/Memento/Caretaker.h"

// Singleton wrapper around Caretaker for convenient global access
class SaveManager {
private:
    static SaveManager* instance;
    Caretaker* caretaker;
    
    SaveManager() {
        caretaker = new Caretaker(10);  // Max 10 snapshots
    }
    
public:
    static SaveManager* getInstance() {
        if (instance == nullptr) {
            instance = new SaveManager();
        }
        return instance;
    }
    
    static void destroyInstance() {
        if (instance != nullptr) {
            delete instance->caretaker;
            delete instance;
            instance = nullptr;
        }
    }
    
    // Delete copy constructor and assignment
    SaveManager(const SaveManager&) = delete;
    SaveManager& operator=(const SaveManager&) = delete;
    
    // Delegate all operations to caretaker
    void saveSnapshot(const GameMemento& memento) { 
        caretaker->saveMemento(memento); 
    }
    
    GameMemento loadSnapshot(int index) { 
        return caretaker->getMemento(index); 
    }
    
    GameMemento getLatestSnapshot() { 
        return caretaker->getLatestMemento(); 
    }
    
    GameMemento undo() { 
        return caretaker->undo(); 
    }
    
    // Query methods
    int getSnapshotCount() const { 
        return caretaker->getSnapshotCount(); 
    }
    
    bool hasSnapshots() const { 
        return caretaker->hasSnapshots(); 
    }
    
    bool canUndo() const { 
        return caretaker->canUndo(); 
    }
    
    // Management
    void clearAll() { 
        caretaker->clear(); 
    }
    
    void listAll() const { 
        caretaker->listSnapshots(); 
    }
};

#endif // SAVEMANAGER_H