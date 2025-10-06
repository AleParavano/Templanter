#include "Plant.h"
#include "Patterns/Observer/Observer.h"
#include <iostream>
#include <algorithm>

// ============================================
// CONSTRUCTOR
// ============================================
Plant::Plant(int plantId, PlantType type, int x, int y)
    : id(plantId), posX(x), posY(y), plantType(type),
      currentState(nullptr), growthCycle(nullptr),
      growthProgress(0.0f), waterLevel(100), decayTimer(0.0f),
      growthModifier(1.0f), pendingGrowth(0.0f),
      subject(nullptr),
      needsWaterNotified(false), ripeNotified(false), 
      decayingNotified(false), deadNotified(false) {
    
    // Initialize with Seed state (State Pattern)
    currentState = new SeedState();
    
    // Initialize with normal growth cycle (Template Method Pattern)
    growthCycle = new NormalGrowthCycle();
    
    // Create subject (Observer Pattern - this plant is observable)
    subject = new PlantSubject(this);
    
    // Set initial watering time
    lastWateredTime = std::time(nullptr);
    
    std::cout << "🌱 Plant created: " << getPlantTypeInfo().name 
              << " at (" << posX << ", " << posY << ")" << std::endl;
}

// ============================================
// DESTRUCTOR
// ============================================
Plant::~Plant() {
    delete currentState;
    delete growthCycle;
    delete subject;
}

// ============================================
// MAIN UPDATE LOOP
// ============================================
void Plant::update(float deltaTime) {
    // ============================================
    // TEMPLATE METHOD PATTERN: Execute growth cycle
    // ============================================
    growthCycle->executeGrowthCycle(this, deltaTime);
    
    // ============================================
    // STATE PATTERN: Update current state
    // ============================================
    currentState->update(this, deltaTime);
    
    // ============================================
    // OBSERVER PATTERN: Check for events and notify observers
    // ============================================
    
    // Event 1: Plant needs water
    if (needsWater() && !needsWaterNotified) {
        subject->notifyNeedsWater();
        needsWaterNotified = true;
    }
    
    // Event 2: Plant is ripe and ready to harvest
    if (currentState->canHarvest() && !ripeNotified && getStateName() == "Ripe") {
        subject->notifyRipe();
        ripeNotified = true;
    }
    
    // Event 3: Plant is decaying
    if (getStateName() == "Decaying" && !decayingNotified) {
        subject->notifyDecaying();
        decayingNotified = true;
    }
    
    // Event 4: Plant died
    if (getStateName() == "Dead" && !deadNotified) {
        subject->notifyDead();
        deadNotified = true;
    }
}

// ============================================
// PLAYER ACTIONS
// ============================================

void Plant::water() {
    // Delegate to current state (State Pattern)
    currentState->water(this);
    
    // Reset notification flag
    needsWaterNotified = false;
    
    std::cout << "💧 Watered " << getPlantTypeInfo().name 
              << " at (" << posX << ", " << posY << ")" << std::endl;
}

bool Plant::harvest(int& value) {
    if (!currentState->canHarvest()) {
        std::cout << "❌ Cannot harvest - plant not ready" << std::endl;
        return false;
    }
    
    // Calculate harvest value
    const PlantTypeInfo& info = getPlantTypeInfo();
    value = info.sellPrice;
    
    // Reduce value if decaying
    if (getStateName() == "Decaying") {
        value = static_cast<int>(value * 0.5f);
        std::cout << "⚠️  Plant was decaying - value reduced to $" << value << std::endl;
    }
    
    std::cout << "🌾 Harvested " << info.name << " for $" << value << std::endl;
    return true;
}

void Plant::applyNutrient() {
    // Switch to boosted growth cycle (Strategy Pattern element)
    delete growthCycle;
    growthCycle = new BoostedGrowthCycle(1.5f);  // 50% faster growth
    
    std::cout << "✨ Applied nutrient to " << getPlantTypeInfo().name 
              << " - Growth boosted by 50%!" << std::endl;
}

// ============================================
// STATE PATTERN: State management
// ============================================

void Plant::setState(PlantState* newState) {
    if (currentState != nullptr) {
        std::string oldState = currentState->getStateName();
        delete currentState;
        currentState = newState;
        
        std::cout << "🔄 Plant state changed: " << oldState 
                  << " → " << newState->getStateName() << std::endl;
    } else {
        currentState = newState;
    }
    
    // Reset notification flags on state change
    if (newState->getStateName() == "Ripe") {
        ripeNotified = false;
    } else if (newState->getStateName() == "Decaying") {
        decayingNotified = false;
    } else if (newState->getStateName() == "Dead") {
        deadNotified = false;
    }
}

std::string Plant::getStateName() const {
    if (currentState == nullptr) {
        return "UNKNOWN";
    }
    return currentState->getStateName();
}

// ============================================
// OBSERVER PATTERN: Attach/detach observers
// ============================================

void Plant::attach(Observer* observer) {
    if (observer == nullptr) {
        std::cerr << "Plant::attach - null observer" << std::endl;
        return;
    }
    
    subject->attach(observer);
    std::cout << "👁️  Observer attached to plant at (" << posX << ", " << posY << ")" << std::endl;
}

void Plant::detach(Observer* observer) {
    if (observer == nullptr) {
        return;
    }
    
    subject->detach(observer);
    std::cout << "👁️  Observer detached from plant at (" << posX << ", " << posY << ")" << std::endl;
}

// ============================================
// GETTERS
// ============================================

const PlantTypeInfo& Plant::getPlantTypeInfo() const {
    return PlantTypeDatabase::getInstance()->getPlantInfo(plantType);
}

bool Plant::canHarvest() const {
    if (currentState == nullptr) {
        return false;
    }
    return currentState->canHarvest();
}

int Plant::getSpriteIndex() const {
    if (currentState == nullptr) {
        return 0;
    }
    return currentState->getSpriteIndex();
}

// ============================================
// SETTERS: Used by states and growth cycles
// ============================================

void Plant::setGrowthProgress(float progress) {
    growthProgress = std::max(0.0f, std::min(100.0f, progress));
}

void Plant::addGrowthProgress(float progress) {
    float oldProgress = growthProgress;
    growthProgress += progress;
    growthProgress = std::max(0.0f, std::min(100.0f, growthProgress));
    
    // Log significant growth milestones
    if (oldProgress < 25.0f && growthProgress >= 25.0f) {
        std::cout << "📈 Plant at (" << posX << ", " << posY << ") reached 25% growth" << std::endl;
    } else if (oldProgress < 50.0f && growthProgress >= 50.0f) {
        std::cout << "📈 Plant at (" << posX << ", " << posY << ") reached 50% growth" << std::endl;
    } else if (oldProgress < 75.0f && growthProgress >= 75.0f) {
        std::cout << "📈 Plant at (" << posX << ", " << posY << ") reached 75% growth" << std::endl;
    } else if (oldProgress < 100.0f && growthProgress >= 100.0f) {
        std::cout << "🎉 Plant at (" << posX << ", " << posY << ") reached 100% growth!" << std::endl;
    }
}

void Plant::setWaterLevel(int level) {
    int oldLevel = waterLevel;
    waterLevel = std::max(0, std::min(100, level));
    
    // Log if water level changed significantly
    if (oldLevel >= 30 && waterLevel < 30) {
        std::cout << "⚠️  Plant at (" << posX << ", " << posY << ") water level critical!" << std::endl;
    }
}

void Plant::setLastWateredTime(std::time_t time) {
    lastWateredTime = time;
}

void Plant::addDecayTimer(float minutes) {
    decayTimer += minutes;
}

// ============================================
// WATER MANAGEMENT
// ============================================

void Plant::updateWaterLevel(float deltaTime) {
    const PlantTypeInfo& info = getPlantTypeInfo();
    
    // Calculate time since last watered (in hours)
    std::time_t now = std::time(nullptr);
    double hoursSinceWatered = std::difftime(now, lastWateredTime) / 3600.0;
    
    // Calculate water depletion rate
    // Water depletes over the waterIntervalHours period
    float depletionRate = 100.0f / static_cast<float>(info.waterIntervalHours);
    float depletion = depletionRate * (deltaTime / 60.0f);  // deltaTime is in minutes
    
    waterLevel -= static_cast<int>(depletion);
    waterLevel = std::max(0, waterLevel);
}

bool Plant::needsWater() const {
    const PlantTypeInfo& info = getPlantTypeInfo();
    
    // Check if water level is critically low
    if (waterLevel < 30) {
        return true;
    }
    
    // Check if watering interval has passed
    std::time_t now = std::time(nullptr);
    double hoursSinceWatered = std::difftime(now, lastWateredTime) / 3600.0;
    
    return hoursSinceWatered >= info.waterIntervalHours;
}

// ============================================
// SERIALIZATION: For save/load via Memento
// ============================================

PlantData Plant::serialize() const {
    PlantData data;
    data.id = id;
    data.type = PlantTypeDatabase::getInstance()->getPlantName(plantType);
    data.posX = posX;
    data.posY = posY;
    data.state = getStateName();
    data.growthProgress = growthProgress;
    data.waterLevel = waterLevel;
    data.lastWateredTime = lastWateredTime;
    
    return data;
}

Plant* Plant::deserialize(const PlantData& data) {
    // Get plant type from name (Flyweight Pattern)
    PlantType type = PlantTypeDatabase::getInstance()->getPlantTypeByName(data.type);
    
    // Create plant
    Plant* plant = new Plant(data.id, type, data.posX, data.posY);
    
    // Restore growth state
    plant->growthProgress = data.growthProgress;
    plant->waterLevel = data.waterLevel;
    plant->lastWateredTime = data.lastWateredTime;
    
    // Set appropriate state based on saved state name (State Pattern)
    if (data.state == "Seed") {
        plant->setState(new SeedState());
    } else if (data.state == "Growing") {
        plant->setState(new GrowingState());
    } else if (data.state == "Ripe") {
        plant->setState(new RipeState());
    } else if (data.state == "Decaying") {
        plant->setState(new DecayingState());
    } else if (data.state == "Dead") {
        plant->setState(new DeadState());
    }
    
    std::cout << "📦 Deserialized plant: " << data.type 
              << " (" << data.state << ") at (" << data.posX << ", " << data.posY << ")" << std::endl;
    
    return plant;
}