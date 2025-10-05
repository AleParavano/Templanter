#include "GameMemento.h"
#include <ctime>
#include <iomanip>
#include <sstream>

// GameMemento constructor
GameMemento::GameMemento(int d, int h, int m, double curr, double rat,
                         const std::vector<PlantData>& p, int ghW, int ghH,
                         const std::vector<WorkerData>& w,
                         const InventoryData& inv,
                         const StatisticsData& stats)
    : day(d), hour(h), minute(m), currency(curr), rating(rat),
      plants(p), greenhouseWidth(ghW), greenhouseHeight(ghH),
      workers(w), inventory(inv), statistics(stats),
      version("1.0") {
    
    // Generate timestamp
    std::time_t now = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
    timestamp = ss.str();
    
    // Generate save name
    std::stringstream nameSS;
    nameSS << "Day " << day << " - $" << static_cast<int>(currency);
    saveName = nameSS.str();
}

// Serialize to JSON
std::string GameMemento::toJSON() const {
    json j;
    
    // Metadata
    j["metadata"] = {
        {"saveName", saveName},
        {"timestamp", timestamp},
        {"version", version}
    };
    
    // Core data
    j["time"] = {
        {"day", day},
        {"hour", hour},
        {"minute", minute}
    };
    
    j["economy"] = {
        {"currency", currency},
        {"rating", rating}
    };
    
    // Greenhouse
    json plantsJson = json::array();
    for (const auto& plant : plants) {
        plantsJson.push_back(plant.toJson());
    }
    j["greenhouse"] = {
        {"plants", plantsJson},
        {"width", greenhouseWidth},
        {"height", greenhouseHeight}
    };
    
    // Workers
    json workersJson = json::array();
    for (const auto& worker : workers) {
        workersJson.push_back(worker.toJson());
    }
    j["workers"] = workersJson;
    
    // Inventory
    j["inventory"] = inventory.toJson();
    
    // Statistics
    j["statistics"] = statistics.toJson();
    
    return j.dump(4);  // Pretty print with 4-space indent
}

// Deserialize from JSON
GameMemento GameMemento::fromJSON(const std::string& jsonStr) {
    json j = json::parse(jsonStr);
    
    // Extract core data
    int d = j["time"]["day"];
    int h = j["time"]["hour"];
    int m = j["time"]["minute"];
    double curr = j["economy"]["currency"];
    double rat = j["economy"]["rating"];
    
    // Extract plants
    std::vector<PlantData> plants;
    for (const auto& pJson : j["greenhouse"]["plants"]) {
        plants.push_back(PlantData::fromJson(pJson));
    }
    int ghW = j["greenhouse"]["width"];
    int ghH = j["greenhouse"]["height"];
    
    // Extract workers
    std::vector<WorkerData> workers;
    for (const auto& wJson : j["workers"]) {
        workers.push_back(WorkerData::fromJson(wJson));
    }
    
    // Extract inventory
    InventoryData inv = InventoryData::fromJson(j["inventory"]);
    
    // Extract statistics
    StatisticsData stats = StatisticsData::fromJson(j["statistics"]);
    
    // Create memento
    GameMemento memento(d, h, m, curr, rat, plants, ghW, ghH, workers, inv, stats);
    
    // Restore metadata
    memento.saveName = j["metadata"]["saveName"];
    memento.timestamp = j["metadata"]["timestamp"];
    memento.version = j["metadata"]["version"];
    
    return memento;
}