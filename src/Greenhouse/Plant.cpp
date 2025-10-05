#include "Plant.h"
#include <iostream>
#include <algorithm>

Plant::Plant(int plantId, PlantType type, int x, int y)
    : id(plantId), posX(x), posY(y), plantType(type),
      currentState(nullptr), growthCycle(nullptr),
      growthProgress(0.0f), waterLevel(100), decayTimer(0.0f),
      growthModifier(1.0f), pendingGrowth(0.0f),
      subject(new PlantSubject()),
      needsWaterNotified(false), ripeNotified(false) {
    
    // Initialize with Seed state
    currentState = new SeedState();
    
    // Initialize with normal growth cycle
    growthCycle = new NormalGrowthCycle();
    
    // Set initial watering time
    lastWateredTime = std::time(nullptr);
}

Plant::~Plant() {
    delete currentState;
    delete growthCycle;
    delete subject;
}

void Plant::update(float deltaTime) {
    // Execute growth cycle (Template Method Pattern)
    growthCycle->executeGrowthCycle(this, deltaTime);
    
    // Update state (State Pattern)
    currentState->update(this, deltaTime);
    
    // Check for notifications (Observer Pattern)
    if (needsWater() && !needsWaterNotified) {
        subject->notifyNeedsWater(this);
        needsWaterNotified = true;
    }
    
    if (currentState->canHarvest() && !ripeNotified) {
        subject->notifyRipe(this);
        ripeNotified = true;
    }
    
    if (getStateName() == "Decaying") {
        subject->notifyDecaying(this);
    }
}

void Plant::water() {
    currentState->water(this);
    needsWaterNotified = false;  // Reset notification flag
}

bool Plant::harvest(int& value) {
    if (!currentState->canHarvest()) {
        return false;
    }
    
    // Calculate harvest value based on state
    const PlantTypeInfo& info = getPlantTypeInfo();
    value = info.sellPrice;
    
    if (getStateName() == "Decaying") {
        // Decayed plants are worth 50% less
        value = static_cast<int>(value * 0.5f);
    }
    
    return true;
}

void Plant::applyNutrient() {
    // Switch to boosted growth cycle (Strategy Pattern element)
    delete growthCycle;
    growthCycle = new BoostedGrowthCycle(1.5f);  // 50% faster growth
    
    std::cout << "Applied nutrient to " << getPlantTypeInfo().name 
              << " - Growth boosted!" << std::endl;
}

void Plant::setState(PlantState* newState) {
    if (currentState != nullptr) {
        delete currentState;
    }
    currentState = newState;
    
    // Reset notification flags on state change
    if (newState->getStateName() == "Ripe") {
        ripeNotified = false;
    }
}

std::string Plant::getStateName() const {
    return currentState->getStateName();
}

void Plant::addObserver(PlantObserver* observer) {
    subject->addObserver(observer);
}

void Plant::removeObserver(PlantObserver* observer) {
    subject->removeObserver(observer);
}

void Plant::notifyObservers(const std::string& event) {
    if (event == "watered") {
        needsWaterNotified = false;
    }
}

const PlantTypeInfo& Plant::getPlantTypeInfo() const {
    return PlantTypeDatabase::getInstance()->getPlantInfo(plantType);
}

void Plant::setGrowthProgress(float progress) {
    growthProgress = std::max(0.0f, std::min(100.0f, progress));
}

void Plant::addGrowthProgress(float progress) {
    growthProgress += progress;
    growthProgress = std::max(0.0f, std::min(100.0f, growthProgress));
}

void Plant::setWaterLevel(int level) {
    waterLevel = std::max(0, std::min(100, level));
}

void Plant::setLastWateredTime(std::time_t time) {
    lastWateredTime = time;
}

void Plant::addDecayTimer(float minutes) {
    decayTimer += minutes;
}

void Plant::updateWaterLevel(float deltaTime) {
    // Water depletes over time based on plant's water interval
    const PlantTypeInfo& info = getPlantTypeInfo();
    
    // Calculate time since last watered (in hours)
    std::time_t now = std::time(nullptr);
    double hoursSinceWatered = std::difftime(now, lastWateredTime) / 3600.0;
    
    // Calculate water depletion rate
    float depletionRate = 100.0f / static_cast<float>(info.waterIntervalHours);
    float depletion = depletionRate * (deltaTime / 60.0f);  // Convert minutes to hours
    
    waterLevel -= static_cast<int>(depletion);
    waterLevel = std::max(0, waterLevel);
}

bool Plant::needsWater() const {
    const PlantTypeInfo& info = getPlantTypeInfo();
    
    // Check if water level is below threshold
    if (waterLevel < 30) {
        return true;
    }
    
    // Check if watering interval has passed
    std::time_t now = std::time(nullptr);
    double hoursSinceWatered = std::difftime(now, lastWateredTime) / 3600.0;
    
    return hoursSinceWatered >= info.waterIntervalHours;
}

bool Plant::canHarvest() const {
    return currentState->canHarvest();
}

int Plant::getSpriteIndex() const {
    return currentState->getSpriteIndex();
}

// Serialization
PlantData Plant::serialize() const {
    PlantData data;
    data.id = id;
    data.type = PlantTypeDatabase::getInstance()->getPlantName(plantType);
    data.posX = posX;
    data.posY = posY;
    data.state = getStateName();
    data.growthProgress = static_cast<int>(growthProgress);
    data.waterLevel = waterLevel;
    data.lastWateredTime = static_cast<long>(lastWateredTime);
    return data;
}

Plant* Plant::deserialize(const PlantData& data) {
    // Get plant type from name
    PlantType type = PlantTypeDatabase::getInstance()->getPlantTypeByName(data.type);
    
    // Create plant
    Plant* plant = new Plant(data.id, type, data.posX, data.posY);
    
    // Restore state
    plant->growthProgress = static_cast<float>(data.growthProgress);
    plant->waterLevel = data.waterLevel;
    plant->lastWateredTime = static_cast<std::time_t>(data.lastWateredTime);
    
    // Set appropriate state based on saved state name
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
    
    return plant;
}