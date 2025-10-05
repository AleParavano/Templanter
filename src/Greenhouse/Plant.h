#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <ctime>
#include "Core/GameData.h"                     // ← Should be this
#include "PlantTypes.h"                        // ← This is fine (same directory)
#include "Patterns/State/PlantState.h"         // ← Fixed
#include "Patterns/Observer/Observer.h"        // ← Fixed
#include "Patterns/Template/GrowthCycle.h"     // ← Fixed

// ... rest of file

// Forward declarations
class PlantState;
class GrowthCycle;

// Main Plant class - uses State, Observer, Template, and Flyweight patterns
class Plant {
private:
    // Unique plant data (not shared)
    int id;
    int posX, posY;
    
    // Flyweight: Reference to shared data
    PlantType plantType;
    
    // State Pattern
    PlantState* currentState;
    
    // Template Method Pattern
    GrowthCycle* growthCycle;
    
    // Growth tracking
    float growthProgress;      // 0-100%
    int waterLevel;            // 0-100%
    std::time_t lastWateredTime;
    float decayTimer;          // Minutes in current state
    
    // Growth modifiers
    float growthModifier;
    float pendingGrowth;
    
    // Observer subject
    PlantSubject* subject;
    
    // Flags
    bool needsWaterNotified;
    bool ripeNotified;

public:
    Plant(int plantId, PlantType type, int x, int y);
    ~Plant();
    
    // Main update
    void update(float deltaTime);
    
    // Actions
    void water();
    bool harvest(int& value);  // Returns harvest value
    void applyNutrient();      // Switch to boosted growth
    
    // State management
    void setState(PlantState* newState);
    PlantState* getState() const { return currentState; }
    std::string getStateName() const;
    
    // Observer management
    void addObserver(PlantObserver* observer);
    void removeObserver(PlantObserver* observer);
    void notifyObservers(const std::string& event);
    
    // Getters
    int getId() const { return id; }
    int getPosX() const { return posX; }
    int getPosY() const { return posY; }
    PlantType getPlantType() const { return plantType; }
    const PlantTypeInfo& getPlantTypeInfo() const;
    
    float getGrowthProgress() const { return growthProgress; }
    int getWaterLevel() const { return waterLevel; }
    std::time_t getLastWateredTime() const { return lastWateredTime; }
    float getDecayTimer() const { return decayTimer; }
    float getGrowthModifier() const { return growthModifier; }
    float getPendingGrowth() const { return pendingGrowth; }
    
    bool canHarvest() const;
    int getSpriteIndex() const;
    
    // Setters (used by states and growth cycles)
    void setGrowthProgress(float progress);
    void addGrowthProgress(float progress);
    void setWaterLevel(int level);
    void setLastWateredTime(std::time_t time);
    void addDecayTimer(float minutes);
    void setGrowthModifier(float modifier) { growthModifier = modifier; }
    void setPendingGrowth(float growth) { pendingGrowth = growth; }
    
    // Water level management
    void updateWaterLevel(float deltaTime);
    bool needsWater() const;
    
    // Serialization (for save/load)
    PlantData serialize() const;
    static Plant* deserialize(const PlantData& data);
};

#endif // PLANT_H