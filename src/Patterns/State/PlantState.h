#ifndef PLANTSTATE_H
#define PLANTSTATE_H

#include <string>

// Forward declaration
class Plant;

// State Pattern: Abstract base class for plant states
class PlantState {
public:
    virtual ~PlantState() = default;
    
    // State behavior
    virtual void update(Plant* plant, float deltaTime) = 0;
    virtual void water(Plant* plant) = 0;
    virtual bool canHarvest() const = 0;
    virtual std::string getStateName() const = 0;
    
    // Visual representation (for rendering)
    virtual int getSpriteIndex() const = 0;
};

// Concrete State: Seed
class SeedState : public PlantState {
public:
    void update(Plant* plant, float deltaTime) override;
    void water(Plant* plant) override;
    bool canHarvest() const override { return false; }
    std::string getStateName() const override { return "Seed"; }
    int getSpriteIndex() const override { return 0; }
};

// Concrete State: Growing
class GrowingState : public PlantState {
public:
    void update(Plant* plant, float deltaTime) override;
    void water(Plant* plant) override;
    bool canHarvest() const override { return false; }
    std::string getStateName() const override { return "Growing"; }
    int getSpriteIndex() const override { return 1; }
};

// Concrete State: Ripe (ready to harvest)
class RipeState : public PlantState {
public:
    void update(Plant* plant, float deltaTime) override;
    void water(Plant* plant) override;
    bool canHarvest() const override { return true; }
    std::string getStateName() const override { return "Ripe"; }
    int getSpriteIndex() const override { return 2; }
};

// Concrete State: Decaying
class DecayingState : public PlantState {
public:
    void update(Plant* plant, float deltaTime) override;
    void water(Plant* plant) override;
    bool canHarvest() const override { return true; }  // Can still harvest but reduced value
    std::string getStateName() const override { return "Decaying"; }
    int getSpriteIndex() const override { return 3; }
};

// Concrete State: Dead (withered, needs removal)
class DeadState : public PlantState {
public:
    void update(Plant* plant, float deltaTime) override;
    void water(Plant* plant) override;
    bool canHarvest() const override { return false; }
    std::string getStateName() const override { return "Dead"; }
    int getSpriteIndex() const override { return 4; }
};

#endif // PLANTSTATE_H