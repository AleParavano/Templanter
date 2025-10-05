#include "WorkerRole.h"
#include "Staff/Worker.h"
#include "Greenhouse/Plant.h"
#include <iostream>
#include <random>

// ============================================
// WatererRole Implementation
// ============================================
WatererRole::WatererRole(Worker* w) 
    : WorkerRole(w), patrolTimer(0.0f) {
}

void WatererRole::performDuty(float deltaTime) {
    patrolTimer += deltaTime;
    
    // Patrol every 5 minutes
    if (patrolTimer >= 5.0f) {
        patrolTimer = 0.0f;
        // In actual game, this would check plants in assigned zone
        // For now, it's just a placeholder
    }
}

std::string WatererRole::getDescription() const {
    return "Waters plants automatically when needed. Efficiency: " + 
           std::to_string(static_cast<int>(worker->getSkillEfficiency("watering") * 100)) + "%";
}

void WatererRole::waterPlant(Plant* plant) {
    if (plant == nullptr) return;
    
    float efficiency = worker->getSkillEfficiency("watering");
    float baseTime = 10.0f;  // 10 seconds base
    float actualTime = baseTime / efficiency;
    
    worker->setIsBusy(true);
    worker->setBusyTimer(actualTime);
    
    plant->water();
    
    std::cout << worker->getName() << " watered " 
              << plant->getPlantTypeInfo().name 
              << " (took " << actualTime << "s)" << std::endl;
    
    // Gain experience
    worker->gainExperience(5);
}

// ============================================
// HarvesterRole Implementation
// ============================================
HarvesterRole::HarvesterRole(Worker* w) 
    : WorkerRole(w), patrolTimer(0.0f) {
}

void HarvesterRole::performDuty(float deltaTime) {
    patrolTimer += deltaTime;
    
    // Check for ripe plants every 3 minutes
    if (patrolTimer >= 3.0f) {
        patrolTimer = 0.0f;
        // In actual game, check for ripe plants
    }
}

std::string HarvesterRole::getDescription() const {
    return "Harvests ripe plants automatically. Efficiency: " + 
           std::to_string(static_cast<int>(worker->getSkillEfficiency("harvesting") * 100)) + "%";
}

void HarvesterRole::harvestPlant(Plant* plant) {
    if (plant == nullptr || !plant->canHarvest()) return;
    
    float efficiency = worker->getSkillEfficiency("harvesting");
    float baseTime = 8.0f;  // 8 seconds base
    float actualTime = baseTime / efficiency;
    
    worker->setIsBusy(true);
    worker->setBusyTimer(actualTime);
    
    int value = 0;
    if (plant->harvest(value)) {
        // Apply skill bonus
        int skillLevel = worker->getHarvestingSkill();
        float bonus = 0.05f * (skillLevel - 1);  // 0%, 5%, 10%, 15%, 25%
        int bonusValue = static_cast<int>(value * bonus);
        int totalValue = value + bonusValue;
        
        std::cout << worker->getName() << " harvested " 
                  << plant->getPlantTypeInfo().name 
                  << " for $" << totalValue;
        
        if (bonusValue > 0) {
            std::cout << " (+" << bonusValue << " skill bonus)";
        }
        
        std::cout << std::endl;
        
        // Gain experience
        worker->gainExperience(15);
    }
}

// ============================================
// CashierRole Implementation
// ============================================
CashierRole::CashierRole(Worker* w) 
    : WorkerRole(w), customersServed(0) {
}

void CashierRole::performDuty(float deltaTime) {
    // Cashiers wait for customers
    // Actual serving happens via serveCustomer() calls
}

std::string CashierRole::getDescription() const {
    int skillLevel = worker->getCashierSkill();
    float serviceTime = 10.0f / worker->getSkillEfficiency("cashier");
    
    return "Serves customers at counter. Service time: " + 
           std::to_string(static_cast<int>(serviceTime)) + "s per customer";
}

void CashierRole::serveCustomer() {
    float efficiency = worker->getSkillEfficiency("cashier");
    float baseTime = 10.0f;  // 10 seconds base
    float actualTime = baseTime / efficiency;
    
    worker->setIsBusy(true);
    worker->setBusyTimer(actualTime);
    
    customersServed++;
    
    // Chance for tip based on skill
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> tipChance(0.0, 1.0);
    
    int skillLevel = worker->getCashierSkill();
    float tipProbability = 0.1f * skillLevel;  // 10%, 20%, 30%, 40%, 50%
    
    bool gotTip = tipChance(gen) < tipProbability;
    
    std::cout << worker->getName() << " served customer #" << customersServed;
    
    if (gotTip) {
        std::uniform_int_distribution<> tipAmount(5, 15);
        int tip = tipAmount(gen);
        std::cout << " (got $" << tip << " tip!)";
    }
    
    std::cout << std::endl;
    
    // Gain experience
    worker->gainExperience(8);
}

// ============================================
// SecurityRole Implementation
// ============================================
SecurityRole::SecurityRole(Worker* w) 
    : WorkerRole(w), robbersStopped(0), alertLevel(1.0f) {
}

void SecurityRole::performDuty(float deltaTime) {
    // Security guards patrol and watch
    // Actual intervention happens via attemptStopRobber()
    
    // Alert level slightly increases over time (more vigilant)
    alertLevel += deltaTime * 0.01f;
    if (alertLevel > 2.0f) alertLevel = 2.0f;
}

std::string SecurityRole::getDescription() const {
    int skillLevel = worker->getSecuritySkill();
    int successRate = 70 + (skillLevel - 1) * 10;  // 70%, 80%, 90%, 95%, 100%
    
    return "Prevents robberies. Success rate: " + std::to_string(successRate) + "%";
}

bool SecurityRole::attemptStopRobber() {
    int skillLevel = worker->getSecuritySkill();
    
    // Calculate success chance
    // Level 1: 70%, Level 2: 80%, Level 3: 90%, Level 4: 95%, Level 5: 100%
    float successRate = 0.7f + (skillLevel - 1) * 0.1f;
    if (skillLevel >= 5) successRate = 1.0f;
    
    successRate *= alertLevel;  // Alert level bonus
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    
    bool success = dist(gen) < successRate;
    
    if (success) {
        robbersStopped++;
        std::cout << "🛡️  " << worker->getName() << " stopped a robber! (Total: " 
                  << robbersStopped << ")" << std::endl;
        
        // Gain experience
        worker->gainExperience(25);
        
        // Reset alert level (relaxed after catching one)
        alertLevel = 1.0f;
    } else {
        std::cout << "⚠️  Robber got past " << worker->getName() << "!" << std::endl;
        
        // Increase alert for next time
        alertLevel += 0.5f;
    }
    
    return success;
}