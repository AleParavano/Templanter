#include "SaveManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

// Initialize static instance
SaveManager* SaveManager::instance = nullptr;

SaveManager::SaveManager() 
    : saveDirectory(Config::SAVE_DIRECTORY) {
    ensureSaveDirectoryExists();
}

SaveManager* SaveManager::getInstance() {
    if (instance == nullptr) {
        instance = new SaveManager();
    }
    return instance;
}

void SaveManager::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

std::string SaveManager::getSaveFilePath(const std::string& slotName) const {
    return saveDirectory + slotName + Config::SAVE_EXTENSION;
}

bool SaveManager::ensureSaveDirectoryExists() {
    try {
        if (!fs::exists(saveDirectory)) {
            fs::create_directories(saveDirectory);
            std::cout << "Created save directory: " << saveDirectory << std::endl;
        }
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error creating save directory: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::saveGame(const GameMemento& memento, const std::string& slotName) {
    if (!ensureSaveDirectoryExists()) {
        std::cerr << "Failed to ensure save directory exists" << std::endl;
        return false;
    }
    
    std::string filePath = getSaveFilePath(slotName);
    std::string tempPath = filePath + ".tmp";
    
    try {
        // Write to temporary file first
        std::ofstream file(tempPath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << tempPath << std::endl;
            return false;
        }
        
        std::string jsonData = memento.toJSON();
        file << jsonData;
        file.close();
        
        // Verify file was written
        if (!fs::exists(tempPath)) {
            std::cerr << "Temporary save file was not created" << std::endl;
            return false;
        }
        
        // Backup old save if it exists
        if (fs::exists(filePath)) {
            std::string backupPath = filePath + ".backup";
            fs::copy_file(filePath, backupPath, fs::copy_options::overwrite_existing);
        }
        
        // Replace old save with new save
        fs::rename(tempPath, filePath);
        
        std::cout << "Game saved successfully to: " << filePath << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error saving game: " << e.what() << std::endl;
        
        // Clean up temp file if it exists
        if (fs::exists(tempPath)) {
            fs::remove(tempPath);
        }
        
        return false;
    }
}

GameMemento SaveManager::loadGame(const std::string& slotName) {
    std::string filePath = getSaveFilePath(slotName);
    
    try {
        if (!fs::exists(filePath)) {
            throw std::runtime_error("Save file does not exist: " + filePath);
        }
        
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open save file: " + filePath);
        }
        
        std::string jsonData((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
        file.close();
        
        GameMemento memento = GameMemento::fromJSON(jsonData);
        
        std::cout << "Game loaded successfully from: " << filePath << std::endl;
        return memento;
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
        
        // Try to load from backup
        std::string backupPath = filePath + ".backup";
        if (fs::exists(backupPath)) {
            std::cout << "Attempting to load from backup..." << std::endl;
            
            try {
                std::ifstream backupFile(backupPath);
                std::string jsonData((std::istreambuf_iterator<char>(backupFile)),
                                     std::istreambuf_iterator<char>());
                backupFile.close();
                
                GameMemento memento = GameMemento::fromJSON(jsonData);
                std::cout << "Loaded from backup successfully!" << std::endl;
                return memento;
                
            } catch (const std::exception& backupError) {
                std::cerr << "Backup load also failed: " << backupError.what() << std::endl;
            }
        }
        
        throw;  // Re-throw exception if both loads failed
    }
}

bool SaveManager::saveExists(const std::string& slotName) const {
    std::string filePath = getSaveFilePath(slotName);
    return fs::exists(filePath);
}

std::vector<std::string> SaveManager::listSaves() const {
    std::vector<std::string> saves;
    
    try {
        if (!fs::exists(saveDirectory)) {
            return saves;
        }
        
        for (const auto& entry : fs::directory_iterator(saveDirectory)) {
            if (entry.is_regular_file() && 
                entry.path().extension() == Config::SAVE_EXTENSION) {
                std::string filename = entry.path().stem().string();
                saves.push_back(filename);
            }
        }
        
        // Sort alphabetically
        std::sort(saves.begin(), saves.end());
        
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error listing saves: " << e.what() << std::endl;
    }
    
    return saves;
}

bool SaveManager::deleteSave(const std::string& slotName) {
    std::string filePath = getSaveFilePath(slotName);
    
    try {
        if (fs::exists(filePath)) {
            fs::remove(filePath);
            std::cout << "Save deleted: " << filePath << std::endl;
            
            // Also delete backup if it exists
            std::string backupPath = filePath + ".backup";
            if (fs::exists(backupPath)) {
                fs::remove(backupPath);
            }
            
            return true;
        }
        return false;
        
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error deleting save: " << e.what() << std::endl;
        return false;
    }
}

std::string SaveManager::getSlotFileName(int slotNumber) const {
    return "slot" + std::to_string(slotNumber);
}

std::vector<GameMemento> SaveManager::loadAllSaveSlots() const {
    std::vector<GameMemento> saves;
    
    for (int i = 1; i <= Config::MAX_SAVE_SLOTS; i++) {
        std::string slotName = getSlotFileName(i);
        if (saveExists(slotName)) {
            try {
                GameMemento memento = const_cast<SaveManager*>(this)->loadGame(slotName);
                saves.push_back(memento);
            } catch (const std::exception& e) {
                std::cerr << "Error loading slot " << i << ": " << e.what() << std::endl;
            }
        }
    }
    
    return saves;
}