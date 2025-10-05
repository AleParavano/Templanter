#ifndef ROBBER_H
#define ROBBER_H

#include "Customer.h"

// Concrete Customer: Robber (tries to steal money)
class Robber : public Customer {
private:
    double attemptedTheftAmount;
    bool caught;
    
public:
    Robber(int id, const std::string& name);
    
    void accept(CustomerVisitor* visitor) override;
    Plant* selectPlant() override;
    double getPriceMultiplier() const override { return 0.0; }  // Doesn't pay
    std::string getTypeString() const override { return "Robber"; }
    
    double getAttemptedTheftAmount() const { return attemptedTheftAmount; }
    bool isCaught() const { return caught; }
    void setCaught(bool c) { caught = c; }
};

#endif // ROBBER_H