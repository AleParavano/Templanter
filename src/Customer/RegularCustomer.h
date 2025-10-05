#ifndef REGULARCUSTOMER_H
#define REGULARCUSTOMER_H

#include "Customer.h"
#include "Greenhouse/PlantTypes.h"

// Concrete Customer: Regular customer
class RegularCustomer : public Customer {
private:
    PlantType requestedPlant;
    
public:
    RegularCustomer(int id, const std::string& name);
    
    void accept(CustomerVisitor* visitor) override;
    Plant* selectPlant() override;
    double getPriceMultiplier() const override { return 1.0; }
    std::string getTypeString() const override { return "Regular"; }
    
    PlantType getRequestedPlant() const { return requestedPlant; }
};

#endif // REGULARCUSTOMER_H