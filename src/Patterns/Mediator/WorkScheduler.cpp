#include "WorkScheduler.h"
#include "Staff/Worker.h"
#include "Greenhouse/Plant.h"
#include "Patterns/Command/Command.h"
#include <iostream>
#include <algorithm>

WorkScheduler::WorkScheduler() {
}

WorkScheduler::~WorkScheduler() {
    // Clean up global task queue
    while (!globalTaskQueue.empty()) {
        Command* cmd = globalTaskQueue.front();
        globalTaskQueue.pop();
        delete cmd;
    }
}

// ============================================
// Observer Pattern Implementation
// ============================================
void WorkScheduler::onPlantNeedsWater(Plant* plant) {
    if (plant == nullptr) return;
    
    std::cout << "🚨 WorkScheduler: Plant needs water!" << std::endl;
    
    // Create water command
    Command* waterCmd = new WaterPlantCommand(nullptr, plant);
    assignTask(waterCmd, "Waterer");
}

void WorkScheduler::onPlantRipe(Plant* plant) {
    if (plant == nullptr) return;
    
    std::cout << "✅ WorkScheduler: Plant is ripe!" << std::endl;
    
    // Create harvest command
    Command* harvestCmd = new HarvestPlantCommand(nullptr, plant);
    assignTask(harvestCmd, "Harvester");
}

void WorkScheduler::onPlantDecaying(Plant* plant) {
    if (plant == nullptr) return;
    
    std::cout << "⚠️  WorkScheduler: Plant is decaying!" << std::endl;
    
    // Priority harvest before it dies
    Command* harvestCmd = new HarvestPlantCommand(nullptr, plant);
    assignTask(harvestCmd, "Harvester");
}

// ============================================
// Rest of the implementation stays the same
// ============================================

void WorkScheduler::registerWorker(Worker* worker) {
    if (worker != nullptr) {
        workers.push_back(worker);
        std::cout << "📋 WorkScheduler: Registered worker" << std::endl;
    }
}

void WorkScheduler::unregisterWorker(Worker* worker) {
    auto it = std::find(workers.begin(), workers.end(), worker);
    if (it != workers.end()) {
        workers.erase(it);
        std::cout << "📋 WorkScheduler: Unregistered worker" << std::endl;
    }
}

Worker* WorkScheduler::findBestWorkerForTask(const std::string& taskType) {
    for (auto* worker : workers) {
        if (!worker->getIsBusy() && worker->hasRole(taskType)) {
            return worker;
        }
    }
    return nullptr;
}

Worker* WorkScheduler::findAvailableWorker() {
    for (auto* worker : workers) {
        if (!worker->getIsBusy()) {
            return worker;
        }
    }
    return nullptr;
}

void WorkScheduler::assignTask(Command* task, const std::string& taskType) {
    if (task == nullptr) return;
    
    Worker* worker = findBestWorkerForTask(taskType);
    
    if (worker != nullptr) {
        worker->addTask(task);
        std::cout << "📋 Task assigned to worker" << std::endl;
    } else {
        globalTaskQueue.push(task);
        std::cout << "📋 Task queued (no available worker)" << std::endl;
    }
}

void WorkScheduler::distributeTaskToWorkers(Command* task) {
    if (task == nullptr) return;
    
    Worker* worker = findAvailableWorker();
    if (worker != nullptr) {
        worker->addTask(task);
    } else {
        globalTaskQueue.push(task);
    }
}

void WorkScheduler::processGlobalTasks() {
    while (!globalTaskQueue.empty()) {
        Worker* worker = findAvailableWorker();
        if (worker == nullptr) {
            break;
        }
        
        Command* task = globalTaskQueue.front();
        globalTaskQueue.pop();
        worker->addTask(task);
    }
}

// Deprecated methods (kept for backwards compatibility)
void WorkScheduler::notifyPlantNeedsWater(Plant* plant) {
    onPlantNeedsWater(plant);
}

void WorkScheduler::notifyPlantRipe(Plant* plant) {
    onPlantRipe(plant);
}

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

void WorkScheduler::payAllWorkers(double& balance) {
    std::cout << "\n💰 Paying all workers..." << std::endl;
    
    for (auto* worker : workers) {
        double salary = worker->getSalary();
        
        if (balance >= salary) {
            balance -= salary;
            std::cout << "  Paid worker $" << salary << std::endl;
        } else {
            std::cout << "  ⚠️  Not enough money!" << std::endl;
        }
    }
    
    std::cout << "  Remaining: $" << balance << "\n" << std::endl;
}

void WorkScheduler::updateAllWorkers(float deltaTime) {
    for (auto* worker : workers) {
        worker->update(deltaTime);
    }
    processGlobalTasks();
}