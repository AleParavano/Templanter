#include "CustomerVisitor.h"
#include "Customer/RegularCustomer.h"
#include "Customer/VIPCustomer.h"
#include "Customer/Robber.h"
#include "Greenhouse/PlantTypes.h"
#include "Core/Config.h"
#include <iostream>

// ============================================
// ServeCustomerVisitor Implementation
// ============================================
ServeCustomerVisitor::ServeCustomerVisitor(double& cash, double& customerRating, bool& inventory)
    : cashRegister(cash), rating(customerRating), hasInventory(inventory) {
}

void ServeCustomerVisitor::visitRegularCustomer(RegularCustomer* customer) {
    PlantTypeDatabase* db = PlantTypeDatabase::getInstance();
    const PlantTypeInfo& plantInfo = db->getPlantInfo(customer->getRequestedPlant());
    
    std::cout << "  Serving " << customer->getName() 
              << " - Wants: " << plantInfo.name << std::endl;
    
    if (hasInventory) {
        double price = plantInfo.sellPrice;
        cashRegister += price;
        rating += Config::RATING_SUCCESS_BONUS;
        customer->setServed(true);
        
        std::cout << "  ✅ Sold for $" << price << std::endl;
    } else {
        rating -= Config::RATING_NO_STOCK_PENALTY;
        customer->markLeftAngry();
        
        std::cout << "  ❌ Out of stock! Customer left unhappy." << std::endl;
    }
}

void ServeCustomerVisitor::visitVIPCustomer(VIPCustomer* customer) {
    PlantTypeDatabase* db = PlantTypeDatabase::getInstance();
    const PlantTypeInfo& plantInfo = db->getPlantInfo(customer->getRequestedPlant());
    
    std::cout << "  Serving VIP " << customer->getName() 
              << " - Wants: " << plantInfo.name << std::endl;
    
    if (hasInventory) {
        double price = plantInfo.sellPrice * customer->getPriceMultiplier();
        cashRegister += price;
        rating += Config::RATING_SUCCESS_BONUS * 2.0f;  // VIPs boost rating more
        customer->setServed(true);
        
        std::cout << "  ✅ Sold for $" << price << " (VIP bonus!)" << std::endl;
    } else {
        rating -= Config::RATING_NO_STOCK_PENALTY * 1.5f;  // VIPs are more disappointed
        customer->markLeftAngry();
        
        std::cout << "  ❌ Out of stock! VIP left very unhappy." << std::endl;
    }
}

void ServeCustomerVisitor::visitRobber(Robber* robber) {
    std::cout << "  🚨 ROBBER DETECTED: " << robber->getName() << std::endl;
    
    // Security check (simplified - would check for security workers)
    bool hasSecurityCheck = false;  // In full game, check for security workers
    
    if (hasSecurityCheck) {
        robber->setCaught(true);
        rating += Config::RATING_SECURITY_BONUS;
        std::cout << "  🛡️  Robber caught by security!" << std::endl;
    } else {
        cashRegister -= robber->getAttemptedTheftAmount();
        rating -= Config::RATING_ROBBERY_PENALTY;
        std::cout << "  💸 Robber stole $" << robber->getAttemptedTheftAmount() << "!" << std::endl;
    }
}

// ============================================
// DisplayCustomerVisitor Implementation
// ============================================
void DisplayCustomerVisitor::visitRegularCustomer(RegularCustomer* customer) {
    PlantTypeDatabase* db = PlantTypeDatabase::getInstance();
    const PlantTypeInfo& plantInfo = db->getPlantInfo(customer->getRequestedPlant());
    
    std::cout << "👤 " << customer->getName() 
              << " (Regular) - Wants: " << plantInfo.name
              << " | Wait: " << customer->getWaitTimer() << "s" << std::endl;
}

void DisplayCustomerVisitor::visitVIPCustomer(VIPCustomer* customer) {
    PlantTypeDatabase* db = PlantTypeDatabase::getInstance();
    const PlantTypeInfo& plantInfo = db->getPlantInfo(customer->getRequestedPlant());
    
    std::cout << "⭐ " << customer->getName() 
              << " (VIP) - Wants: " << plantInfo.name
              << " | Wait: " << customer->getWaitTimer() << "s" << std::endl;
}

void DisplayCustomerVisitor::visitRobber(Robber* robber) {
    std::cout << "🚨 " << robber->getName() 
              << " (ROBBER) - Attempting to steal $" 
              << robber->getAttemptedTheftAmount() << std::endl;
}

// ============================================
// WaitTimeVisitor Implementation
// ============================================
void WaitTimeVisitor::visitRegularCustomer(RegularCustomer* customer) {
    totalCustomers++;
    totalWaitTime += (customer->getMaxWaitTime() - customer->getWaitTimer());
}

void WaitTimeVisitor::visitVIPCustomer(VIPCustomer* customer) {
    totalCustomers++;
    totalWaitTime += (customer->getMaxWaitTime() - customer->getWaitTimer());
}

void WaitTimeVisitor::visitRobber(Robber* robber) {
    // Don't count robbers in wait time stats
}

float WaitTimeVisitor::getAverageWaitTime() const {
    if (totalCustomers == 0) return 0.0f;
    return totalWaitTime / totalCustomers;
}