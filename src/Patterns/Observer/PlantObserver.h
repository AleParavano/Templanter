#ifndef PLANTOBSERVER_H
#define PLANTOBSERVER_H

#include "Observer.h"
#include "Subject.h"
#include "PlantSubject.h"
#include <iostream>

// Forward declaration
class Plant;

// ============================================
// CONCRETE OBSERVER BASE: PlantObserver
// ============================================
// Base class for all observers that want to observe plants
// Provides type-safe event handling
class PlantObserver : public Observer {
public:
    virtual ~PlantObserver() = default;
    
    // Implement Observer interface - dispatches to specific event handlers
    void update(Subject* subject) override {
        // Safely cast subject to PlantSubject
        PlantSubject* plantSubject = dynamic_cast<PlantSubject*>(subject);
        if (plantSubject == nullptr) {
            std::cerr << "Error: PlantObserver received update from non-PlantSubject" << std::endl;
            return;
        }
        
        // Get event type and plant
        std::string event = plantSubject->getEvent();
        Plant* plant = plantSubject->getPlant();
        
        // Dispatch to appropriate handler based on event
        if (event == "NEEDS_WATER") {
            onPlantNeedsWater(plant);
        } else if (event == "RIPE") {
            onPlantRipe(plant);
        } else if (event == "DECAYING") {
            onPlantDecaying(plant);
        } else if (event == "DEAD") {
            onPlantDead(plant);
        }
    }
    
protected:
    // Abstract event handlers - concrete observers must implement these
    virtual void onPlantNeedsWater(Plant* plant) = 0;
    virtual void onPlantRipe(Plant* plant) = 0;
    virtual void onPlantDecaying(Plant* plant) = 0;
    virtual void onPlantDead(Plant* plant) = 0;
};

#endif // PLANTOBSERVER_H