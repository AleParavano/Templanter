#include "RegularCustomer.h"
#include "Patterns/Visitor/CustomerVisitor.h"
#include "Greenhouse/PlantTypes.h"
#include "Core/Config.h"

RegularCustomer::RegularCustomer(int id, const std::string& name)
    : Customer(id, name, CustomerType::REGULAR, Config::CUSTOMER_REGULAR_WAIT_TIME) {
    
    // Select a random plant based on demand
    PlantTypeDatabase* db = PlantTypeDatabase::getInstance();
    requestedPlant = db->getRandomPlantByDemand();
}

void RegularCustomer::accept(CustomerVisitor* visitor) {
    visitor->visitRegularCustomer(this);
}

Plant* RegularCustomer::selectPlant() {
    // In actual game, this would check inventory and return the plant
    // For now, returns nullptr (to be implemented with inventory system)
    return nullptr;
}