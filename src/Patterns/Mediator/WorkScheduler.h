#ifndef WORKSCHEDULER_H
#define WORKSCHEDULER_H

#include <vector>
#include <queue>
#include <string>
#include "Patterns/Observer/PlantObserver.h"

// Forward declarations
class Worker;
class Plant;
class Command;

// ============================================
// CONCRETE OBSERVER: WorkScheduler
// ============================================
// Mediator Pattern: Coordinates workers and tasks
// Observer Pattern: Observes plants and assigns tasks when events occur
class WorkScheduler : public PlantObserver {
private:
    std::vector<Worker*> workers;
    std::queue<Command*> globalTaskQueue;
    
    // Helper methods
    Worker* findBestWorkerForTask(const std::string& taskType);
    Worker* findAvailableWorker();
    
public:
    WorkScheduler();
    ~WorkScheduler();
    
    // ============================================
    // Observer Pattern: Implement PlantObserver interface
    // ============================================
    void onPlantNeedsWater(Plant* plant) override;
    void onPlantRipe(Plant* plant) override;
    void onPlantDecaying(Plant* plant) override;
    void onPlantDead(Plant* plant) override;
    
    // ============================================
    // Mediator Pattern: Worker and task management
    // ============================================
    void registerWorker(Worker* worker);
    void unregisterWorker(Worker* worker);
    std::vector<Worker*> getWorkers() const { return workers; }
    
    // Task distribution
    void assignTask(Command* task, const std::string& taskType);
    void distributeTaskToWorkers(Command* task);
    void processGlobalTasks();
    
    // Worker queries
    int getAvailableWorkerCount() const;
    Worker* getWorkerById(int id);
    std::vector<Worker*> getWorkersByRole(const std::string& role);
    
    // Daily operations
    void payAllWorkers(double& balance);
    void updateAllWorkers(float deltaTime);
};

#endif // WORKSCHEDULER_H