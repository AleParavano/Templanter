#ifndef WORKERROLE_H
#define WORKERROLE_H

#include <string>

// Forward declaration
class Worker;
class Plant;

// Decorator Pattern: Abstract role decorator
class WorkerRole {
protected:
    Worker* worker;
    
public:
    WorkerRole(Worker* w) : worker(w) {}
    virtual ~WorkerRole() = default;
    
    virtual std::string getRoleName() const = 0;
    virtual void performDuty(float deltaTime) = 0;
    virtual std::string getDescription() const = 0;
};

// Concrete Decorator: Waterer Role
class WatererRole : public WorkerRole {
private:
    float patrolTimer;
    
public:
    WatererRole(Worker* w);
    
    std::string getRoleName() const override { return "Waterer"; }
    void performDuty(float deltaTime) override;
    std::string getDescription() const override;
    
    void waterPlant(Plant* plant);
};

// Concrete Decorator: Harvester Role
class HarvesterRole : public WorkerRole {
private:
    float patrolTimer;
    
public:
    HarvesterRole(Worker* w);
    
    std::string getRoleName() const override { return "Harvester"; }
    void performDuty(float deltaTime) override;
    std::string getDescription() const override;
    
    void harvestPlant(Plant* plant);
};

// Concrete Decorator: Cashier Role
class CashierRole : public WorkerRole {
private:
    int customersServed;
    
public:
    CashierRole(Worker* w);
    
    std::string getRoleName() const override { return "Cashier"; }
    void performDuty(float deltaTime) override;
    std::string getDescription() const override;
    
    void serveCustomer();
    int getCustomersServed() const { return customersServed; }
};

// Concrete Decorator: Security Role
class SecurityRole : public WorkerRole {
private:
    int robbersStopped;
    float alertLevel;
    
public:
    SecurityRole(Worker* w);
    
    std::string getRoleName() const override { return "Security"; }
    void performDuty(float deltaTime) override;
    std::string getDescription() const override;
    
    bool attemptStopRobber();
    int getRobbersStopped() const { return robbersStopped; }
    float getAlertLevel() const { return alertLevel; }
};

#endif // WORKERROLE_H