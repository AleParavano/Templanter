#ifndef PLANT_H
#define PLANT_H

#include <string>
#include <ctime>
#include "Core/GameData.h"
#include "PlantTypes.h"
#include "Patterns/State/PlantState.h"
#include "Patterns/Observer/PlantSubject.h"
#include "Patterns/Template/GrowthCycle.h"

// Forward declarations
class PlantState;
class GrowthCycle;
class Observer;

// ============================================
// PLANT CLASS
// ============================================
// Combines multiple design patterns:
// - State Pattern: Plant growth states
// - Observer Pattern: Plant is an observable subject
// - Template Method: Growth cycle algorithm
// - Flyweight: Shares PlantTypeInfo data
class Plant {
private:
    // ============================================
    // UNIQUE PLANT DATA (Not shared)
    // ============================================
    int id;
    int posX, posY;
    
    // ============================================
    // FLYWEIGHT PATTERN: Reference to shared data
    // ============================================
    PlantType plantType;
    
    // ============================================
    // STATE PATTERN: Current state
    // ============================================
    PlantState* currentState;
    
    // ============================================
    // TEMPLATE METHOD PATTERN: Growth algorithm
    // ============================================
    GrowthCycle* growthCycle;
    
    // ============================================
    // GROWTH TRACKING
    // ============================================
    float growthProgress;      // 0-100%
    int waterLevel;            // 0-100%
    std::time_t lastWateredTime;
    float decayTimer;          // Minutes in current state
    
    // Growth modifiers
    float growthModifier;
    float pendingGrowth;
    
    // ============================================
    // OBSERVER PATTERN: This plant is a subject
    // ============================================
    PlantSubject* subject;
    
    // Notification flags (prevent spam)
    bool needsWaterNotified;
    bool ripeNotified;
    bool decayingNotified;
    bool deadNotified;

public:
    // ============================================
    // CONSTRUCTOR & DESTRUCTOR
    // ============================================
    Plant(int plantId, PlantType type, int x, int y);
    ~Plant();
    
    // Delete copy constructor and assignment (prevent accidental copying)
    Plant(const Plant&) = delete;
    Plant& operator=(const Plant&) = delete;
    
    // ============================================
    // MAIN UPDATE LOOP
    // ============================================
    void update(float deltaTime);
    
    // ============================================
    // PLAYER ACTIONS
    // ============================================
    void water();
    bool harvest(int& value);
    void applyNutrient();
    
    // ============================================
    // STATE PATTERN: State management
    // ============================================
    void setState(PlantState* newState);
    PlantState* getState() const { return currentState; }
    std::string getStateName() const;
    
    // ============================================
    // OBSERVER PATTERN: Attach/detach observers
    // ============================================
    void attach(Observer* observer);
    void detach(Observer* observer);
    PlantSubject* getSubject() const { return subject; }
    
    // ============================================
    // GETTERS: Basic info
    // ============================================
    int getId() const { return id; }
    int getPosX() const { return posX; }
    int getPosY() const { return posY; }
    PlantType getPlantType() const { return plantType; }
    const PlantTypeInfo& getPlantTypeInfo() const;
    
    // ============================================
    // GETTERS: Growth state
    // ============================================
    float getGrowthProgress() const { return growthProgress; }
    int getWaterLevel() const { return waterLevel; }
    std::time_t getLastWateredTime() const { return lastWateredTime; }
    float getDecayTimer() const { return decayTimer; }
    float getGrowthModifier() const { return growthModifier; }
    float getPendingGrowth() const { return pendingGrowth; }
    
    bool canHarvest() const;
    int getSpriteIndex() const;
    
    // ============================================
    // SETTERS: Used by states and growth cycles
    // ============================================
    void setGrowthProgress(float progress);
    void addGrowthProgress(float progress);
    void setWaterLevel(int level);
    void setLastWateredTime(std::time_t time);
    void addDecayTimer(float minutes);
    void setGrowthModifier(float modifier) { growthModifier = modifier; }
    void setPendingGrowth(float growth) { pendingGrowth = growth; }
    
    // ============================================
    // WATER MANAGEMENT
    // ============================================
    void updateWaterLevel(float deltaTime);
    bool needsWater() const;
    
    // ============================================
    // SERIALIZATION: For save/load via Memento
    // ============================================
    PlantData serialize() const;
    static Plant* deserialize(const PlantData& data);
};

#endif // PLANT_H