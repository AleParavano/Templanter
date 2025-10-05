#include "Customer.h"
#include <iostream>

Customer::Customer(int customerId, const std::string& customerName, CustomerType custType, float maxWait)
    : id(customerId), name(customerName), type(custType), 
      waitTimer(maxWait), maxWaitTime(maxWait),
      served(false), leftAngry(false) {
}

void Customer::update(float deltaTime) {
    if (!served) {
        waitTimer -= deltaTime;
        
        if (waitTimer <= 0.0f && !leftAngry) {
            leftAngry = true;
            std::cout << "⏰ Customer " << name << " timed out and left angry!" << std::endl;
        }
    }
}