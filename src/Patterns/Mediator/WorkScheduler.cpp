#include "WorkScheduler.h"
#include "Staff/Worker.h"
#include "Greenhouse/Plant.h"
#include "Patterns/Command/Command.h"
#include <iostream>
#include <algorithm>

WorkScheduler::WorkScheduler() {
    std::cout << "📋 WorkScheduler initialized" << std::endl;
}

WorkScheduler::~WorkScheduler() {
    // Clean up global task queue
    while (!globalTaskQueue.empty()) {
        Command* cmd = globalTaskQueue.front();
        globalTaskQueue.pop();
        delete cmd;
    }
    
    std::cout << "📋 WorkScheduler destroyed" << std::endl;
}

// ============================================
// OBSERVER PATTERN: Concrete Observer Implementation
// ============================================

void WorkScheduler::onPlantNeedsWater(Plant* plant) {
    if (plant == nullptr) {
        std::cerr << "WorkScheduler::onPlantNeedsWater - null plant" << std::endl;
        return;
    }
    
    std::cout << "🚨 WorkScheduler observed: Plant needs water at (" 
              << plant->getPosX() << ", " << plant->getPosY() << ")" << std::endl;
    
    // Create water command and assign to appropriate worker
    Command* waterCmd = new WaterPlantCommand(nullptr, plant);
    assignTask(waterCmd, "Waterer");
}

void WorkScheduler::onPlantRipe(Plant* plant) {
    if (plant == nullptr) {
        std::cerr << "WorkScheduler::onPlantRipe - null plant" << std::endl;
        return;
    }
    
    std::cout << "✅ WorkScheduler observed: Plant is ripe at (" 
              << plant->getPosX() << ", " << plant->getPosY() << ")" << std::endl;
    
    // Create harvest command and assign to appropriate worker
    Command* harvestCmd = new HarvestPlantCommand(nullptr, plant);
    assignTask(harvestCmd, "Harvester");
}

void WorkScheduler::onPlantDecaying(Plant* plant) {
    if (plant == nullptr) {
        std::cerr << "WorkScheduler::onPlantDecaying - null plant" << std::endl;
        return;
    }
    
    std::cout << "⚠️  WorkScheduler observed: Plant is decaying at (" 
              << plant->getPosX() << ", " << plant->getPosY() << ")" << std::endl;
    
    // Urgent harvest command - decaying plants should be harvested immediately
    Command* harvestCmd = new HarvestPlantCommand(nullptr, plant);
    assignTask(harvestCmd, "Harvester");
}

void WorkScheduler::onPlantDead(Plant* plant) {
    if (plant == nullptr) {
        std::cerr << "WorkScheduler::onPlantDead - null plant" << std::endl;
        return;
    }
    
    std::cout << "💀 WorkScheduler observed: Plant died at (" 
              << plant->getPosX() << ", " << plant->getPosY() << ")" << std::endl;
    
    // Dead plants need to be removed (could create a RemovePlantCommand)
    // For now, just log the event
}

// ============================================
// MEDIATOR PATTERN: Worker Management
// ============================================

void WorkScheduler::registerWorker(Worker* worker) {
    if (worker == nullptr) {
        std::cerr << "WorkScheduler::registerWorker - null worker" << std::endl;
        return;
    }
    
    workers.push_back(worker);
    std::cout << "📋 WorkScheduler: Registered worker '" << worker->getName() 
              << "' (Total: " << workers.size() << ")" << std::endl;
}

void WorkScheduler::unregisterWorker(Worker* worker) {
    if (worker == nullptr) {
        return;
    }
    
    auto it = std::find(workers.begin(), workers.end(), worker);
    if (it != workers.end()) {
        std::string name = (*it)->getName();
        workers.erase(it);
        std::cout << "📋 WorkScheduler: Unregistered worker '" << name 
                  << "' (Remaining: " << workers.size() << ")" << std::endl;
    }
}

Worker* WorkScheduler::findBestWorkerForTask(const std::string& taskType) {
    // First, try to find a worker with the specific role who is not busy
    for (auto* worker : workers) {
        if (!worker->getIsBusy() && worker->hasRole(taskType)) {
            return worker;
        }
    }
    
    // No suitable worker found
    return nullptr;
}

Worker* WorkScheduler::findAvailableWorker() {
    // Find any worker who is not busy
    for (auto* worker : workers) {
        if (!worker->getIsBusy()) {
            return worker;
        }
    }
    
    // No available worker
    return nullptr;
}

void WorkScheduler::assignTask(Command* task, const std::string& taskType) {
    if (task == nullptr) {
        std::cerr << "WorkScheduler::assignTask - null task" << std::endl;
        return;
    }
    
    // Try to find best worker for this task type
    Worker* worker = findBestWorkerForTask(taskType);
    
    if (worker != nullptr) {
        worker->addTask(task);
        std::cout << "📋 Task assigned to " << worker->getName() 
                  << " (Role: " << taskType << ")" << std::endl;
    } else {
        // No suitable worker available, queue the task
        globalTaskQueue.push(task);
        std::cout << "📋 Task queued (no available " << taskType 
                  << ") - Queue size: " << globalTaskQueue.size() << std::endl;
    }
}

void WorkScheduler::distributeTaskToWorkers(Command* task) {
    if (task == nullptr) {
        std::cerr << "WorkScheduler::distributeTaskToWorkers - null task" << std::endl;
        return;
    }
    
    // Find any available worker
    Worker* worker = findAvailableWorker();
    
    if (worker != nullptr) {
        worker->addTask(task);
        std::cout << "📋 Task distributed to " << worker->getName() << std::endl;
    } else {
        // Queue if no worker available
        globalTaskQueue.push(task);
        std::cout << "📋 Task queued - Queue size: " << globalTaskQueue.size() << std::endl;
    }
}

void WorkScheduler::processGlobalTasks() {
    if (globalTaskQueue.empty()) {
        return;
    }
    
    int tasksProcessed = 0;
    
    while (!globalTaskQueue.empty()) {
        // Find available worker
        Worker* worker = findAvailableWorker();
        
        if (worker == nullptr) {
            // No workers available, stop processing
            break;
        }
        
        // Assign queued task to worker
        Command* task = globalTaskQueue.front();
        globalTaskQueue.pop();
        worker->addTask(task);
        tasksProcessed++;
    }
    
    if (tasksProcessed > 0) {
        std::cout << "📋 Processed " << tasksProcessed << " queued task(s). "
                  << "Remaining: " << globalTaskQueue.size() << std::endl;
    }
}

// ============================================
// WORKER QUERIES
// ============================================

int WorkScheduler::getAvailableWorkerCount() const {
    int count = 0;
    for (const auto* worker : workers) {
        if (!worker->getIsBusy()) {
            count++;
        }
    }
    return count;
}

Worker* WorkScheduler::getWorkerById(int id) {
    for (auto* worker : workers) {
        if (worker->getId() == id) {
            return worker;
        }
    }
    return nullptr;
}

std::vector<Worker*> WorkScheduler::getWorkersByRole(const std::string& role) {
    std::vector<Worker*> result;
    
    for (auto* worker : workers) {
        if (worker->hasRole(role)) {
            result.push_back(worker);
        }
    }
    
    return result;
}

// ============================================
// DAILY OPERATIONS
// ============================================

void WorkScheduler::payAllWorkers(double& balance) {
    if (workers.empty()) {
        std::cout << "\n💰 No workers to pay" << std::endl;
        return;
    }
    
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║          DAILY PAYROLL                 ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    std::cout << "Current Balance: $" << balance << std::endl;
    
    double totalPaid = 0.0;
    int workersPaid = 0;
    
    for (auto* worker : workers) {
        double salary = worker->getSalary();
        
        if (balance >= salary) {
            balance -= salary;
            totalPaid += salary;
            workersPaid++;
            std::cout << "  ✓ Paid " << worker->getName() 
                      << " (Level " << worker->getOverallLevel() << ") - $" 
                      << salary << std::endl;
        } else {
            std::cout << "  ✗ INSUFFICIENT FUNDS to pay " << worker->getName() 
                      << " (Need $" << salary << ")" << std::endl;
        }
    }
    
    std::cout << "\n─────────────────────────────────────────" << std::endl;
    std::cout << "Workers Paid: " << workersPaid << "/" << workers.size() << std::endl;
    std::cout << "Total Paid: $" << totalPaid << std::endl;
    std::cout << "Remaining Balance: $" << balance << std::endl;
    std::cout << "═════════════════════════════════════════\n" << std::endl;
}

void WorkScheduler::updateAllWorkers(float deltaTime) {
    // Update each worker
    for (auto* worker : workers) {
        worker->update(deltaTime);
    }
    
    // Process any queued tasks if workers become available
    processGlobalTasks();
}