#ifndef PLANTSUBJECT_H
#define PLANTSUBJECT_H

#include "Subject.h"
#include <string>

// Forward declaration
class Plant;

// ============================================
// CONCRETE SUBJECT: PlantSubject
// ============================================
// Concrete subject that represents an observable plant
class PlantSubject : public Subject {
private:
    Plant* plant;              // The plant being observed
    std::string currentEvent;  // Current event that triggered notification
    
public:
    explicit PlantSubject(Plant* p) : plant(p), currentEvent("") {}
    
    // Get the plant being observed
    Plant* getPlant() const { return plant; }
    
    // Get current event type
    std::string getEvent() const { return currentEvent; }
    
    // Notify observers about specific plant events
    void notifyNeedsWater() {
        currentEvent = "NEEDS_WATER";
        notify();  // Calls Subject::notify()
    }
    
    void notifyRipe() {
        currentEvent = "RIPE";
        notify();
    }
    
    void notifyDecaying() {
        currentEvent = "DECAYING";
        notify();
    }
    
    void notifyDead() {
        currentEvent = "DEAD";
        notify();
    }
};

#endif // PLANTSUBJECT_H