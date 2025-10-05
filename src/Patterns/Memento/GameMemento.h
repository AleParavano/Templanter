#ifndef GAMEMEMENTO_H
#define GAMEMEMENTO_H

#include <string>
#include <vector>
#include "../../Core/GameData.h"

// Memento Pattern: Immutable snapshot of game state
class GameMemento {
private:
    friend class Game;
    
    // Core game data
    int day;
    int hour;
    int minute;
    double currency;
    double rating;
    
    // Greenhouse data
    std::vector<PlantData> plants;
    int greenhouseWidth;
    int greenhouseHeight;
    
    // Staff data
    std::vector<WorkerData> workers;
    
    // Inventory
    InventoryData inventory;
    
    // Statistics
    StatisticsData statistics;
    
    // Metadata
    std::string saveName;
    std::string timestamp;
    std::string version;
    
    // Private constructor - only Game can create
    GameMemento(int d, int h, int m, double curr, double rat,
                const std::vector<PlantData>& p, int ghW, int ghH,
                const std::vector<WorkerData>& w,
                const InventoryData& inv,
                const StatisticsData& stats);

public:
    // Serialization
    std::string toJSON() const;
    static GameMemento fromJSON(const std::string& jsonStr);
    
    // Factory method for testing purposes ONLY
    static GameMemento createTestMemento(
        int d, int h, int m, double curr, double rat,
        const std::vector<PlantData>& p, int ghW, int ghH,
        const std::vector<WorkerData>& w,
        const InventoryData& inv,
        const StatisticsData& stats
    ) {
        return GameMemento(d, h, m, curr, rat, p, ghW, ghH, w, inv, stats);
    }
    
    // Getters for metadata (read-only access)
    std::string getSaveName() const { return saveName; }
    std::string getTimestamp() const { return timestamp; }
    int getDay() const { return day; }
    double getCurrency() const { return currency; }
};

#endif // GAMEMEMENTO_H