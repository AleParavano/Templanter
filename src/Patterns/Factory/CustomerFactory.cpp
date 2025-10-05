#include "CustomerFactory.h"
#include "Customer/RegularCustomer.h"
#include "Customer/VIPCustomer.h"
#include "Customer/Robber.h"
#include "Core/Config.h"
#include <random>
#include <iostream>

int CustomerFactory::nextCustomerId = 1;
std::vector<std::string> CustomerFactory::nameList;

void CustomerFactory::initializeNames() {
    if (nameList.empty()) {
        nameList = {
            "Alice", "Bob", "Charlie", "Diana", "Eve", "Frank",
            "Grace", "Henry", "Ivy", "Jack", "Kate", "Liam",
            "Mia", "Noah", "Olivia", "Paul", "Quinn", "Ruby",
            "Sam", "Tina", "Uma", "Victor", "Wendy", "Xavier",
            "Yara", "Zack", "Emma", "Lucas", "Sophia", "James"
        };
    }
}

std::string CustomerFactory::getRandomName() {
    initializeNames();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, nameList.size() - 1);
    
    return nameList[dist(gen)];
}

Customer* CustomerFactory::createRandomCustomer() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    
    double roll = dist(gen);
    
    // 5% Robber, 10% VIP, 85% Regular
    if (roll < Config::ROBBER_CHANCE) {
        return createRobber();
    } else if (roll < Config::ROBBER_CHANCE + Config::VIP_CHANCE) {
        return createVIPCustomer();
    } else {
        return createRegularCustomer();
    }
}

Customer* CustomerFactory::createRegularCustomer() {
    std::string name = getRandomName();
    Customer* customer = new RegularCustomer(nextCustomerId++, name);
    
    std::cout << "👤 Regular customer arrived: " << name << std::endl;
    return customer;
}

Customer* CustomerFactory::createVIPCustomer() {
    std::string name = getRandomName() + " (VIP)";
    Customer* customer = new VIPCustomer(nextCustomerId++, name);
    
    std::cout << "⭐ VIP customer arrived: " << name << std::endl;
    return customer;
}

Customer* CustomerFactory::createRobber() {
    std::string name = "Suspicious Person";
    Customer* customer = new Robber(nextCustomerId++, name);
    
    std::cout << "🚨 Suspicious person entered..." << std::endl;
    return customer;
}