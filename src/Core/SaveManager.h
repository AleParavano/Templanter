#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>
#include <vector>
#include "../Patterns/Memento/GameMemento.h"
#include "Config.h"

// Singleton Pattern: Manages save/load operations
// Caretaker in Memento Pattern: Stores and retrieves mementos
class SaveManager {
private:
    static SaveManager* instance;
    
    std::string saveDirectory;
    
    // Private constructor for Singleton
    SaveManager();
    
    // Helper methods
    std::string getSaveFilePath(const std::string& slotName) const;
    bool ensureSaveDirectoryExists();

public:
    // Singleton access
    static SaveManager* getInstance();
    static void destroyInstance();
    
    // Delete copy constructor and assignment
    SaveManager(const SaveManager&) = delete;
    SaveManager& operator=(const SaveManager&) = delete;
    
    // Save operations
    bool saveGame(const GameMemento& memento, const std::string& slotName);
    GameMemento loadGame(const std::string& slotName);
    
    // Utility
    bool saveExists(const std::string& slotName) const;
    std::vector<std::string> listSaves() const;
    bool deleteSave(const std::string& slotName);
    
    // Slot management
    std::string getSlotFileName(int slotNumber) const;
    std::vector<GameMemento> loadAllSaveSlots() const;
};

#endif // SAVEMANAGER_H