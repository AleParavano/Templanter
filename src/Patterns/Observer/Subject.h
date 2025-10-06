#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"
#include <vector>
#include <algorithm>

// ============================================
// SUBJECT (Abstract Observable)
// ============================================
// Subject maintains a list of observers and notifies them of changes
class Subject {
private:
    std::vector<Observer*> observers;
    
public:
    virtual ~Subject() = default;
    
    // Attach an observer to this subject
    void attach(Observer* observer) {
        if (observer != nullptr) {
            observers.push_back(observer);
        }
    }
    
    // Detach an observer from this subject
    void detach(Observer* observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }
    
    // Notify all attached observers
    void notify() {
        for (Observer* observer : observers) {
            observer->update(this);
        }
    }
    
protected:
    // Subclasses can access observer list if needed
    const std::vector<Observer*>& getObservers() const {
        return observers;
    }
    
    int getObserverCount() const {
        return static_cast<int>(observers.size());
    }
};

#endif // SUBJECT_H