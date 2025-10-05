#include "Robber.h"
#include "Patterns/Visitor/CustomerVisitor.h"
#include <random>

Robber::Robber(int id, const std::string& name)
    : Customer(id, name, CustomerType::ROBBER, 60.0f),  // 1 minute to attempt robbery
      caught(false) {
    
    // Random theft amount between $100-$500
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(100, 500);
    attemptedTheftAmount = dist(gen);
}

void Robber::accept(CustomerVisitor* visitor) {
    visitor->visitRobber(this);
}

Plant* Robber::selectPlant() {
    // Robbers don't select plants
    return nullptr;
}