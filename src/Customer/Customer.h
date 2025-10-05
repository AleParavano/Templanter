#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

// Forward declarations
class CustomerVisitor;
class Plant;

// Customer types enum
enum class CustomerType {
    REGULAR,
    VIP,
    ROBBER
};

// Base Customer class (will be created by Factory)
class Customer {
protected:
    int id;
    std::string name;
    CustomerType type;
    float waitTimer;
    float maxWaitTime;
    bool served;
    bool leftAngry;
    
public:
    Customer(int customerId, const std::string& customerName, CustomerType custType, float maxWait);
    virtual ~Customer() = default;
    
    // Visitor Pattern: Accept visitor
    virtual void accept(CustomerVisitor* visitor) = 0;
    
    // Strategy Pattern: Purchase behavior
    virtual Plant* selectPlant() = 0;  // Returns nullptr if selection fails
    virtual double getPriceMultiplier() const = 0;
    
    // Update
    void update(float deltaTime);
    
    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    CustomerType getType() const { return type; }
    float getWaitTimer() const { return waitTimer; }
    float getMaxWaitTime() const { return maxWaitTime; }
    bool isServed() const { return served; }
    bool hasLeftAngry() const { return leftAngry; }
    bool hasTimedOut() const { return waitTimer <= 0.0f; }
    
    // Setters
    void setServed(bool s) { served = s; }
    void markLeftAngry() { leftAngry = true; }
    
    // Display
    virtual std::string getTypeString() const = 0;
};

#endif // CUSTOMER_H