#ifndef CUSTOMERVISITOR_H
#define CUSTOMERVISITOR_H

// Forward declarations
class RegularCustomer;
class VIPCustomer;
class Robber;

// Visitor Pattern: Abstract visitor for customer operations
class CustomerVisitor {
public:
    virtual ~CustomerVisitor() = default;
    
    virtual void visitRegularCustomer(RegularCustomer* customer) = 0;
    virtual void visitVIPCustomer(VIPCustomer* customer) = 0;
    virtual void visitRobber(Robber* robber) = 0;
};

// Concrete Visitor: Serve customers
class ServeCustomerVisitor : public CustomerVisitor {
private:
    double& cashRegister;
    double& rating;
    bool& hasInventory;
    
public:
    ServeCustomerVisitor(double& cash, double& customerRating, bool& inventory);
    
    void visitRegularCustomer(RegularCustomer* customer) override;
    void visitVIPCustomer(VIPCustomer* customer) override;
    void visitRobber(Robber* robber) override;
};

// Concrete Visitor: Display customer info
class DisplayCustomerVisitor : public CustomerVisitor {
public:
    void visitRegularCustomer(RegularCustomer* customer) override;
    void visitVIPCustomer(VIPCustomer* customer) override;
    void visitRobber(Robber* robber) override;
};

// Concrete Visitor: Calculate wait time statistics
class WaitTimeVisitor : public CustomerVisitor {
private:
    int totalCustomers;
    float totalWaitTime;
    
public:
    WaitTimeVisitor() : totalCustomers(0), totalWaitTime(0.0f) {}
    
    void visitRegularCustomer(RegularCustomer* customer) override;
    void visitVIPCustomer(VIPCustomer* customer) override;
    void visitRobber(Robber* robber) override;
    
    float getAverageWaitTime() const;
    int getTotalCustomers() const { return totalCustomers; }
};

#endif // CUSTOMERVISITOR_H