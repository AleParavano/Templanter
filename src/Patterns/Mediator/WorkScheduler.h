#ifndef WORKSCHEDULER_H
#define WORKSCHEDULER_H

#include <vector>
#include <queue>
#include <string>
#include "Patterns/Observer/Observer.h"  // ← ADD THIS

// Forward declarations
class Worker;
class Plant;
class Command;

// Mediator Pattern: Coordinates workers and tasks
// Also implements Observer Pattern to watch plants
class WorkScheduler : public PlantObserver {  // ← ADD THIS INHERITANCE
private:
    std::vector<Worker*> workers;
    std::queue<Command*> globalTaskQueue;
    
    // Task assignment strategies
    Worker* findBestWorkerForTask(const std::string& taskType);
    Worker* findAvailableWorker();
    
public:
    WorkScheduler();
    ~WorkScheduler();
    
    // Observer Pattern implementation
    void onPlantNeedsWater(Plant* plant) override;
    void onPlantRipe(Plant* plant) override;
    void onPlantDecaying(Plant* plant) override;
    
    // Worker management
    void registerWorker(Worker* worker);
    void unregisterWorker(Worker* worker);
    std::vector<Worker*> getWorkers() const { return workers; }
    
    // Task distribution
    void assignTask(Command* task, const std::string& taskType);
    void distributeTaskToWorkers(Command* task);
    void processGlobalTasks();
    
    // Notifications (deprecated - now uses Observer pattern)
    void notifyPlantNeedsWater(Plant* plant);
    void notifyPlantRipe(Plant* plant);
    
    // Worker queries
    int getAvailableWorkerCount() const;
    Worker* getWorkerById(int id);
    std::vector<Worker*> getWorkersByRole(const std::string& role);
    
    // Daily operations
    void payAllWorkers(double& balance);
    void updateAllWorkers(float deltaTime);
};

#endif // WORKSCHEDULER_H