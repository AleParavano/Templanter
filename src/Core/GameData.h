#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <string>
#include <vector>
#include <ctime>

// ============================================
// DATA STRUCTURES (Pure C++ - No I/O)
// ============================================

struct PlantData {
    int id;
    std::string type;
    int posX, posY;
    std::string state;
    float growthProgress;
    int waterLevel;
    std::time_t lastWateredTime;
    
    PlantData() : id(0), posX(0), posY(0), growthProgress(0.0f), 
                  waterLevel(100), lastWateredTime(0) {}
};

struct WorkerData {
    int id;
    std::string name;
    int wateringSkill;
    int harvestingSkill;
    int cashierSkill;
    int securitySkill;
    int overallLevel;
    int experience;
    double salary;
    std::string currentRole;
    int assignedZone;
    
    WorkerData() : id(0), wateringSkill(1), harvestingSkill(1), 
                   cashierSkill(1), securitySkill(1), overallLevel(1),
                   experience(0), salary(50.0), assignedZone(-1) {}
};

struct InventoryItemData {
    std::string itemType;
    std::string itemName;
    int quantity;
    
    InventoryItemData() : quantity(0) {}
    InventoryItemData(const std::string& type, const std::string& name, int qty)
        : itemType(type), itemName(name), quantity(qty) {}
};

struct InventoryData {
    std::vector<InventoryItemData> items;
    int nutrients;
    int storageCapacity;
    
    InventoryData() : nutrients(0), storageCapacity(100) {}
};

struct StatisticsData {
    int totalPlantsGrown;
    double totalMoneyEarned;
    int totalCustomersServed;
    int totalRobberiesStopped;
    std::vector<std::string> achievementsUnlocked;
    
    StatisticsData() : totalPlantsGrown(0), totalMoneyEarned(0.0),
                       totalCustomersServed(0), totalRobberiesStopped(0) {}
};

struct TimeData {
    int day;
    int hour;
    int minute;
    
    TimeData() : day(1), hour(6), minute(0) {}
    TimeData(int d, int h, int m) : day(d), hour(h), minute(m) {}
};

struct EconomyData {
    double currency;
    double rating;
    
    EconomyData() : currency(500.0), rating(3.0) {}
    EconomyData(double c, double r) : currency(c), rating(r) {}
};

struct GreenhouseData {
    int width;
    int height;
    std::vector<PlantData> plants;
    
    GreenhouseData() : width(3), height(3) {}
    GreenhouseData(int w, int h) : width(w), height(h) {}
};

// ============================================
// COMPLETE GAME STATE
// ============================================

class GameData {
public:
    // Metadata
    std::string saveName;
    std::string timestamp;
    
    // Core game state
    TimeData time;
    EconomyData economy;
    GreenhouseData greenhouse;
    std::vector<WorkerData> workers;
    InventoryData inventory;
    StatisticsData statistics;
    
    GameData();
    
    void generateSaveName();
    void generateTimestamp();
    void printSummary() const;
};

#endif // GAMEDATA_H