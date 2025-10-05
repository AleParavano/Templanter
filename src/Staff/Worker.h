#ifndef WORKER_H
#define WORKER_H

#include <string>
#include <vector>
#include "Core/GameData.h"  // ← Fixed

// Forward declarations
class WorkerRole;
class Command;

// Base Worker class
class Worker {
private:
    // Identity
    int id;
    std::string name;
    
    // Skills (1-5 stars)
    int wateringSkill;
    int harvestingSkill;
    int cashierSkill;
    int securitySkill;
    
    // Meta stats
    int overallLevel;
    int experience;
    double salary;
    
    // Current assignment
    int assignedZone;  // -1 = unassigned
    
    // Decorator Pattern: Roles stacked on worker
    std::vector<WorkerRole*> roles;
    
    // Command Pattern: Task queue
    std::vector<Command*> taskQueue;
    
    // State
    bool isBusy;
    float busyTimer;
    
public:
    Worker(int workerId, const std::string& workerName);
    ~Worker();
    
    // Update
    void update(float deltaTime);
    
    // Role management (Decorator Pattern)
    void addRole(WorkerRole* role);
    void removeRole(WorkerRole* role);
    void clearRoles();
    bool hasRole(const std::string& roleName) const;
    std::vector<std::string> getRoleNames() const;
    
    // Task management (Command Pattern)
    void addTask(Command* task);
    void executeNextTask();
    void clearTasks();
    int getTaskCount() const { return taskQueue.size(); }
    
    // Skill management
    void trainSkill(const std::string& skillName);
    int getSkillLevel(const std::string& skillName) const;
    float getSkillEfficiency(const std::string& skillName) const;
    
    // Experience and leveling
    void gainExperience(int amount);
    void levelUp();
    bool canLevelUp() const;
    int getExperienceToNextLevel() const;
    
    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    int getWateringSkill() const { return wateringSkill; }
    int getHarvestingSkill() const { return harvestingSkill; }
    int getCashierSkill() const { return cashierSkill; }
    int getSecuritySkill() const { return securitySkill; }
    int getOverallLevel() const { return overallLevel; }
    int getExperience() const { return experience; }
    double getSalary() const { return salary; }
    int getAssignedZone() const { return assignedZone; }
    bool getIsBusy() const { return isBusy; }
    
    // Setters
    void setAssignedZone(int zone) { assignedZone = zone; }
    void setIsBusy(bool busy) { isBusy = busy; }
    void setBusyTimer(float timer) { busyTimer = timer; }
    
    // Serialization
    WorkerData serialize() const;
    static Worker* deserialize(const WorkerData& data);
};

#endif // WORKER_H