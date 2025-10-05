#ifndef GROWTHCYCLE_H
#define GROWTHCYCLE_H

class Plant;

// Template Method Pattern: Abstract growth algorithm
class GrowthCycle {
public:
    virtual ~GrowthCycle() = default;
    
    // Template Method: Defines the skeleton of the growth algorithm
    void executeGrowthCycle(Plant* plant, float deltaTime) {
        checkEnvironment(plant);
        calculateGrowthRate(plant, deltaTime);
        applyGrowth(plant, deltaTime);
        checkStateTransition(plant);
    }
    
protected:
    // Abstract methods to be implemented by subclasses
    virtual void checkEnvironment(Plant* plant) = 0;
    virtual void calculateGrowthRate(Plant* plant, float deltaTime) = 0;
    virtual void applyGrowth(Plant* plant, float deltaTime) = 0;
    virtual void checkStateTransition(Plant* plant) = 0;
};

// Concrete implementation: Normal growth cycle
class NormalGrowthCycle : public GrowthCycle {
protected:
    void checkEnvironment(Plant* plant) override;
    void calculateGrowthRate(Plant* plant, float deltaTime) override;
    void applyGrowth(Plant* plant, float deltaTime) override;
    void checkStateTransition(Plant* plant) override;
};

// Concrete implementation: Boosted growth (with nutrients)
class BoostedGrowthCycle : public GrowthCycle {
private:
    float boostMultiplier;
    
public:
    BoostedGrowthCycle(float multiplier = 1.5f) : boostMultiplier(multiplier) {}
    
protected:
    void checkEnvironment(Plant* plant) override;
    void calculateGrowthRate(Plant* plant, float deltaTime) override;
    void applyGrowth(Plant* plant, float deltaTime) override;
    void checkStateTransition(Plant* plant) override;
};

#endif // GROWTHCYCLE_H