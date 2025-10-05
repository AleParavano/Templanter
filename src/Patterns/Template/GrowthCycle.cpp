#include "GrowthCycle.h"
#include "../../Greenhouse/Plant.h"

// ============================================
// NormalGrowthCycle Implementation
// ============================================
void NormalGrowthCycle::checkEnvironment(Plant* plant) {
    // Check if plant has adequate water
    if (plant->getWaterLevel() < 20) {
        // Low water affects growth
        plant->setGrowthModifier(0.5f);
    } else {
        plant->setGrowthModifier(1.0f);
    }
}

void NormalGrowthCycle::calculateGrowthRate(Plant* plant, float deltaTime) {
    // Base growth rate depends on plant type
    const PlantTypeInfo& info = plant->getPlantTypeInfo();
    
    // Calculate growth per minute (convert hours to minutes)
    float totalMinutes = info.growthTimeHours * 60.0f;
    float growthPerMinute = 100.0f / totalMinutes;
    
    // Apply time delta (deltaTime is in game minutes)
    float growth = growthPerMinute * deltaTime * plant->getGrowthModifier();
    plant->setPendingGrowth(growth);
}

void NormalGrowthCycle::applyGrowth(Plant* plant, float deltaTime) {
    plant->addGrowthProgress(plant->getPendingGrowth());
}

void NormalGrowthCycle::checkStateTransition(Plant* plant) {
    // State transitions are handled by PlantState classes
    // This is just a hook for additional checks
}

// ============================================
// BoostedGrowthCycle Implementation
// ============================================
void BoostedGrowthCycle::checkEnvironment(Plant* plant) {
    // Boosted growth is less affected by low water
    if (plant->getWaterLevel() < 20) {
        plant->setGrowthModifier(0.75f * boostMultiplier);
    } else {
        plant->setGrowthModifier(1.0f * boostMultiplier);
    }
}

void BoostedGrowthCycle::calculateGrowthRate(Plant* plant, float deltaTime) {
    const PlantTypeInfo& info = plant->getPlantTypeInfo();
    
    float totalMinutes = info.growthTimeHours * 60.0f;
    float growthPerMinute = 100.0f / totalMinutes;
    
    // Apply boost multiplier
    float growth = growthPerMinute * deltaTime * plant->getGrowthModifier();
    plant->setPendingGrowth(growth);
}

void BoostedGrowthCycle::applyGrowth(Plant* plant, float deltaTime) {
    plant->addGrowthProgress(plant->getPendingGrowth());
}

void BoostedGrowthCycle::checkStateTransition(Plant* plant) {
    // Same as normal
}   