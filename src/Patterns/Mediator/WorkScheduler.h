#ifndef WORKSCHEDULER_H
#define WORKSCHEDULER_H

#include <vector>
#include <queue>
#include <string>

// Forward declarations
class Worker;
class Plant;
class Command;

// Mediator Pattern: Coordinates workers and tasks
class WorkScheduler {
private:
    std::vector<Worker*> workers;
    std::queue<Command*> globalTaskQueue;
    
    // Task assignment strategies
    Worker* findBestWorkerForTask(const std::string& taskType);
    Worker* findAvailableWorker();
    
public:
    WorkScheduler();
    ~WorkScheduler();
    
    // Worker management
    void registerWorker(Worker* worker);
    void unregisterWorker(Worker* worker);
    std::vector<Worker*> getWorkers() const { return workers; }
    
    // Task distribution
    void assignTask(Command* task, const std::string& taskType);
    void distributeTaskToWorkers(Command* task);
    void processGlobalTasks();
    
    // Notifications (called by observers)
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