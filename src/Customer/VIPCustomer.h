#ifndef VIPCUSTOMER_H
#define VIPCUSTOMER_H

#include "Customer.h"
#include "Greenhouse/PlantTypes.h"

// Concrete Customer: VIP customer (prefers rare plants, pays more)
class VIPCustomer : public Customer {
private:
    PlantType requestedPlant;
    
public:
    VIPCustomer(int id, const std::string& name);
    
    void accept(CustomerVisitor* visitor) override;
    Plant* selectPlant() override;
    double getPriceMultiplier() const override { return 1.5; }  // Pays 50% more
    std::string getTypeString() const override { return "VIP"; }
    
    PlantType getRequestedPlant() const { return requestedPlant; }
};

#endif // VIPCUSTOMER_H