#include "Command.h"
#include <iostream>

// Forward declare what we need to avoid circular dependencies
class Worker;
class Plant;

// We'll implement the commands here without including Worker.h yet
// The includes will be at the bottom or we use forward declarations

// ============================================
// WaterPlantCommand Implementation
// ============================================
WaterPlantCommand::WaterPlantCommand(Worker* worker, Plant* plant)
    : Command(worker), targetPlant(plant) {
}

void WaterPlantCommand::execute() {
    if (targetPlant == nullptr || executor == nullptr) {
        std::cout << "Cannot execute WaterPlantCommand: null pointer" << std::endl;
        return;
    }
    
    // Note: Actual implementation will be in Worker class methods
    // For now, just log
    std::cout << "WaterPlantCommand executed" << std::endl;
}

std::string WaterPlantCommand::getDescription() const {
    return "Water Plant";
}

// ============================================
// HarvestPlantCommand Implementation
// ============================================
HarvestPlantCommand::HarvestPlantCommand(Worker* worker, Plant* plant)
    : Command(worker), targetPlant(plant) {
}

void HarvestPlantCommand::execute() {
    if (targetPlant == nullptr || executor == nullptr) {
        std::cout << "Cannot execute HarvestPlantCommand: null pointer" << std::endl;
        return;
    }
    
    std::cout << "HarvestPlantCommand executed" << std::endl;
}

std::string HarvestPlantCommand::getDescription() const {
    return "Harvest Plant";
}

// ============================================
// ServeCustomerCommand Implementation
// ============================================
ServeCustomerCommand::ServeCustomerCommand(Worker* worker, int custId)
    : Command(worker), customerId(custId) {
}

void ServeCustomerCommand::execute() {
    if (executor == nullptr) {
        std::cout << "Cannot execute ServeCustomerCommand: null executor" << std::endl;
        return;
    }
    
    std::cout << "ServeCustomerCommand executed for customer #" << customerId << std::endl;
}

std::string ServeCustomerCommand::getDescription() const {
    return "Serve Customer #" + std::to_string(customerId);
}

// ============================================
// PatrolCommand Implementation
// ============================================
PatrolCommand::PatrolCommand(Worker* worker, int zone)
    : Command(worker), zoneId(zone) {
}

void PatrolCommand::execute() {
    if (executor == nullptr) {
        std::cout << "Cannot execute PatrolCommand: null executor" << std::endl;
        return;
    }
    
    std::cout << "PatrolCommand executed for zone " << zoneId << std::endl;
}

std::string PatrolCommand::getDescription() const {
    return "Patrol Zone " + std::to_string(zoneId);
}

// ============================================
// RestCommand Implementation
// ============================================
RestCommand::RestCommand(Worker* worker, float durationMinutes)
    : Command(worker), duration(durationMinutes) {
}

void RestCommand::execute() {
    if (executor == nullptr) {
        std::cout << "Cannot execute RestCommand: null executor" << std::endl;
        return;
    }
    
    std::cout << "RestCommand executed - resting for " << duration << " minutes" << std::endl;
}

std::string RestCommand::getDescription() const {
    return "Rest for " + std::to_string(static_cast<int>(duration)) + " minutes";
}