#include "Worker.h"
#include "Core/Config.h"  // ← ADD THIS LINE
#include "Patterns/Decorator/WorkerRole.h"
#include "Patterns/Command/Command.h"
#include <algorithm>
#include <iostream>
#include <random>

Worker::Worker(int workerId, const std::string& workerName)
    : id(workerId), name(workerName),
      wateringSkill(1), harvestingSkill(1), cashierSkill(1), securitySkill(1),
      overallLevel(1), experience(0), salary(Config::BASE_WORKER_SALARY),
      assignedZone(-1), isBusy(false), busyTimer(0.0f) {
    
    // Randomize starting skills (1-2 stars)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 2);
    
    wateringSkill = dist(gen);
    harvestingSkill = dist(gen);
    cashierSkill = dist(gen);
    securitySkill = dist(gen);
}

Worker::~Worker() {
    // Clean up roles
    for (auto* role : roles) {
        delete role;
    }
    
    // Clean up tasks
    for (auto* task : taskQueue) {
        delete task;
    }
}

void Worker::update(float deltaTime) {
    // Update busy timer
    if (isBusy) {
        busyTimer -= deltaTime;
        if (busyTimer <= 0.0f) {
            isBusy = false;
            busyTimer = 0.0f;
        }
    }
    
    // Execute tasks if not busy
    if (!isBusy && !taskQueue.empty()) {
        executeNextTask();
    }
}

void Worker::addRole(WorkerRole* role) {
    if (role != nullptr) {
        roles.push_back(role);
    }
}

void Worker::removeRole(WorkerRole* role) {
    auto it = std::find(roles.begin(), roles.end(), role);
    if (it != roles.end()) {
        delete *it;
        roles.erase(it);
    }
}

void Worker::clearRoles() {
    for (auto* role : roles) {
        delete role;
    }
    roles.clear();
}

bool Worker::hasRole(const std::string& roleName) const {
    for (const auto* role : roles) {
        if (role->getRoleName() == roleName) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> Worker::getRoleNames() const {
    std::vector<std::string> names;
    for (const auto* role : roles) {
        names.push_back(role->getRoleName());
    }
    return names;
}

void Worker::addTask(Command* task) {
    if (task != nullptr) {
        taskQueue.push_back(task);
    }
}

void Worker::executeNextTask() {
    if (taskQueue.empty()) {
        return;
    }
    
    Command* task = taskQueue.front();
    taskQueue.erase(taskQueue.begin());
    
    // Execute task
    task->execute();
    
    // Gain experience
    gainExperience(10);
    
    // Clean up
    delete task;
}

void Worker::clearTasks() {
    for (auto* task : taskQueue) {
        delete task;
    }
    taskQueue.clear();
}

void Worker::trainSkill(const std::string& skillName) {
    if (skillName == "watering" && wateringSkill < 5) {
        wateringSkill++;
        std::cout << name << " trained Watering skill to " << wateringSkill << " stars!" << std::endl;
    } else if (skillName == "harvesting" && harvestingSkill < 5) {
        harvestingSkill++;
        std::cout << name << " trained Harvesting skill to " << harvestingSkill << " stars!" << std::endl;
    } else if (skillName == "cashier" && cashierSkill < 5) {
        cashierSkill++;
        std::cout << name << " trained Cashier skill to " << cashierSkill << " stars!" << std::endl;
    } else if (skillName == "security" && securitySkill < 5) {
        securitySkill++;
        std::cout << name << " trained Security skill to " << securitySkill << " stars!" << std::endl;
    }
}

int Worker::getSkillLevel(const std::string& skillName) const {
    if (skillName == "watering") return wateringSkill;
    if (skillName == "harvesting") return harvestingSkill;
    if (skillName == "cashier") return cashierSkill;
    if (skillName == "security") return securitySkill;
    return 0;
}

float Worker::getSkillEfficiency(const std::string& skillName) const {
    int skill = getSkillLevel(skillName);
    
    // Convert skill level to efficiency multiplier
    // Level 1: 100% time (1.0x speed)
    // Level 2: 85% time (1.18x speed)
    // Level 3: 70% time (1.43x speed)
    // Level 4: 55% time (1.82x speed)
    // Level 5: 40% time (2.5x speed)
    
    switch (skill) {
        case 1: return 1.0f;
        case 2: return 1.18f;
        case 3: return 1.43f;
        case 4: return 1.82f;
        case 5: return 2.5f;
        default: return 1.0f;
    }
}

void Worker::gainExperience(int amount) {
    experience += amount;
    
    // Check for level up
    while (canLevelUp()) {
        levelUp();
    }
}

void Worker::levelUp() {
    overallLevel++;
    experience -= getExperienceToNextLevel();
    
    // Increase salary
    salary += 5.0;
    
    std::cout << "🎉 " << name << " leveled up to Level " << overallLevel << "!" << std::endl;
    std::cout << "   Salary increased to $" << salary << "/day" << std::endl;
}

bool Worker::canLevelUp() const {
    return experience >= getExperienceToNextLevel();
}

int Worker::getExperienceToNextLevel() const {
    return overallLevel * 100;  // Level 1→2: 100 XP, Level 2→3: 200 XP, etc.
}

WorkerData Worker::serialize() const {
    WorkerData data;
    data.id = id;
    data.name = name;
    data.wateringSkill = wateringSkill;
    data.harvestingSkill = harvestingSkill;
    data.cashierSkill = cashierSkill;
    data.securitySkill = securitySkill;
    data.overallLevel = overallLevel;
    data.experience = experience;
    data.salary = salary;
    
    // Serialize current role (just first one for simplicity)
    if (!roles.empty()) {
        data.currentRole = roles[0]->getRoleName();
    } else {
        data.currentRole = "None";
    }
    
    data.assignedZone = assignedZone;
    return data;
}

Worker* Worker::deserialize(const WorkerData& data) {
    Worker* worker = new Worker(data.id, data.name);
    
    worker->wateringSkill = data.wateringSkill;
    worker->harvestingSkill = data.harvestingSkill;
    worker->cashierSkill = data.cashierSkill;
    worker->securitySkill = data.securitySkill;
    worker->overallLevel = data.overallLevel;
    worker->experience = data.experience;
    worker->salary = data.salary;
    worker->assignedZone = data.assignedZone;
    
    // Note: Roles will be re-assigned by the game when loading
    
    return worker;
}