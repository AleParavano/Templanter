#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <vector>

// Forward declarations
class Plant;

// Observer Pattern: Observer interface
class PlantObserver {
public:
    virtual ~PlantObserver() = default;
    virtual void onPlantNeedsWater(Plant* plant) = 0;
    virtual void onPlantRipe(Plant* plant) = 0;
    virtual void onPlantDecaying(Plant* plant) = 0;
};

// Observer Pattern: Subject (Observable)
class PlantSubject {
private:
    std::vector<PlantObserver*> observers;

public:
    virtual ~PlantSubject() = default;
    
    void addObserver(PlantObserver* observer);
    void removeObserver(PlantObserver* observer);
    void notifyNeedsWater(Plant* plant);
    void notifyRipe(Plant* plant);
    void notifyDecaying(Plant* plant);
};

#endif // OBSERVER_H