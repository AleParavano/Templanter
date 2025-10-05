#ifndef CUSTOMERFACTORY_H
#define CUSTOMERFACTORY_H

#include "Customer/Customer.h"
#include <string>
#include <vector>

// Factory Pattern: Creates different types of customers
class CustomerFactory {
private:
    static int nextCustomerId;
    static std::vector<std::string> nameList;
    
    static void initializeNames();
    static std::string getRandomName();
    
public:
    // Factory method: Creates customer based on probabilities
    static Customer* createRandomCustomer();
    
    // Factory methods: Create specific customer types
    static Customer* createRegularCustomer();
    static Customer* createVIPCustomer();
    static Customer* createRobber();
    
    // Utility
    static void resetIdCounter() { nextCustomerId = 1; }
};

#endif // CUSTOMERFACTORY_H