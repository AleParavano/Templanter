#include "VIPCustomer.h"
#include "Patterns/Visitor/CustomerVisitor.h"
#include "Greenhouse/PlantTypes.h"
#include "Core/Config.h"
#include <random>

VIPCustomer::VIPCustomer(int id, const std::string& name)
    : Customer(id, name, CustomerType::VIP, Config::CUSTOMER_VIP_WAIT_TIME) {
    
    // VIPs prefer rare plants (Advanced tier)
    PlantTypeDatabase* db = PlantTypeDatabase::getInstance();
    auto advancedPlants = db->getPlantsByTier(PlantTier::ADVANCED);
    
    if (!advancedPlants.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, advancedPlants.size() - 1);
        requestedPlant = advancedPlants[dist(gen)];
    } else {
        // Fallback to random
        requestedPlant = db->getRandomPlantByDemand();
    }
}

void VIPCustomer::accept(CustomerVisitor* visitor) {
    visitor->visitVIPCustomer(this);
}

Plant* VIPCustomer::selectPlant() {
    // In actual game, would check inventory
    return nullptr;
}