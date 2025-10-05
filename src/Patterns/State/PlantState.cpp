#include "PlantState.h"
#include "../../Greenhouse/Plant.h"
#include <iostream>

// ============================================
// SeedState Implementation
// ============================================
void SeedState::update(Plant* plant, float deltaTime) {
    // Seeds just wait to germinate
    plant->addGrowthProgress(deltaTime);
    
    // Transition to Growing when germination complete (10% progress)
    if (plant->getGrowthProgress() >= 10.0f) {
        plant->setState(new GrowingState());
        delete this;
    }
}

void SeedState::water(Plant* plant) {
    plant->setWaterLevel(100);
    plant->setLastWateredTime(std::time(nullptr));
}

// ============================================
// GrowingState Implementation
// ============================================
void GrowingState::update(Plant* plant, float deltaTime) {
    // Check water level
    plant->updateWaterLevel(deltaTime);
    
    if (plant->getWaterLevel() > 20) {
        // Plant is watered, grows normally
        plant->addGrowthProgress(deltaTime);
    } else if (plant->getWaterLevel() > 0) {
        // Low water, grows slowly
        plant->addGrowthProgress(deltaTime * 0.5f);
    } else {
        // No water, stops growing and starts decaying
        plant->addGrowthProgress(deltaTime * -0.2f);
    }
    
    // Transition to Ripe when fully grown
    if (plant->getGrowthProgress() >= 100.0f) {
        plant->setState(new RipeState());
        delete this;
    }
    
    // Transition to Decaying if severely neglected
    if (plant->getGrowthProgress() < 0.0f) {
        plant->setGrowthProgress(0.0f);
        plant->setState(new DecayingState());
        delete this;
    }
}

void GrowingState::water(Plant* plant) {
    plant->setWaterLevel(100);
    plant->setLastWateredTime(std::time(nullptr));
    plant->notifyObservers("watered");
}

// ============================================
// RipeState Implementation
// ============================================
void RipeState::update(Plant* plant, float deltaTime) {
    // Check water level
    plant->updateWaterLevel(deltaTime);
    
    // Ripe plants stay ripe but start decaying if not harvested
    plant->addDecayTimer(deltaTime);
    
    // After 24 hours ripe, start decaying
    if (plant->getDecayTimer() > 24.0f * 60.0f) {  // 24 hours in minutes
        plant->setState(new DecayingState());
        delete this;
    }
}

void RipeState::water(Plant* plant) {
    plant->setWaterLevel(100);
    plant->setLastWateredTime(std::time(nullptr));
}

// ============================================
// DecayingState Implementation
// ============================================
void DecayingState::update(Plant* plant, float deltaTime) {
    // Decaying plants lose value over time
    plant->addDecayTimer(deltaTime);
    plant->updateWaterLevel(deltaTime);
    
    // After 48 hours decaying, plant dies
    if (plant->getDecayTimer() > 72.0f * 60.0f) {  // 72 hours total
        plant->setState(new DeadState());
        delete this;
    }
}

void DecayingState::water(Plant* plant) {
    // Watering can slow decay but not reverse it
    plant->setWaterLevel(100);
    plant->setLastWateredTime(std::time(nullptr));
}

// ============================================
// DeadState Implementation
// ============================================
void DeadState::update(Plant* plant, float deltaTime) {
    // Dead plants don't do anything
    // They need to be removed manually
}

void DeadState::water(Plant* plant) {
    // Watering a dead plant does nothing
    std::cout << "Cannot water a dead plant!" << std::endl;
}