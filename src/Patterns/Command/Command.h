#ifndef COMMAND_H
#define COMMAND_H

#include <string>

// Forward declarations
class Worker;
class Plant;

// Command Pattern: Abstract command interface
class Command {
protected:
    Worker* executor;
    
public:
    Command(Worker* worker) : executor(worker) {}
    virtual ~Command() = default;
    
    virtual void execute() = 0;
    virtual void undo() {}  // Optional undo functionality
    virtual std::string getDescription() const = 0;
};

// Concrete Command: Water Plant
class WaterPlantCommand : public Command {
private:
    Plant* targetPlant;
    
public:
    WaterPlantCommand(Worker* worker, Plant* plant);
    
    void execute() override;
    std::string getDescription() const override;
};

// Concrete Command: Harvest Plant
class HarvestPlantCommand : public Command {
private:
    Plant* targetPlant;
    
public:
    HarvestPlantCommand(Worker* worker, Plant* plant);
    
    void execute() override;
    std::string getDescription() const override;
};

// Concrete Command: Serve Customer
class ServeCustomerCommand : public Command {
private:
    int customerId;
    
public:
    ServeCustomerCommand(Worker* worker, int custId);
    
    void execute() override;
    std::string getDescription() const override;
};

// Concrete Command: Patrol (Security)
class PatrolCommand : public Command {
private:
    int zoneId;
    
public:
    PatrolCommand(Worker* worker, int zone);
    
    void execute() override;
    std::string getDescription() const override;
};

// Concrete Command: Rest (Worker takes a break)
class RestCommand : public Command {
private:
    float duration;
    
public:
    RestCommand(Worker* worker, float durationMinutes);
    
    void execute() override;
    std::string getDescription() const override;
};

#endif // COMMAND_H