#ifndef OBSERVER_H
#define OBSERVER_H

// Forward declaration
class Subject;

// ============================================
// OBSERVER (Abstract Interface)
// ============================================
// Observer gets notified when Subject changes
class Observer {
public:
    virtual ~Observer() = default;
    
    // Called when the subject this observer is watching changes
    virtual void update(Subject* subject) = 0;
};

#endif // OBSERVER_H