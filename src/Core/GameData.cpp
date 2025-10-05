#include "GameData.h"

// PlantData serialization
json PlantData::toJson() const {
    return json{
        {"id", id},
        {"type", type},
        {"posX", posX},
        {"posY", posY},
        {"state", state},
        {"growthProgress", growthProgress},
        {"waterLevel", waterLevel},
        {"lastWateredTime", lastWateredTime}
    };
}

PlantData PlantData::fromJson(const json& j) {
    PlantData data;
    data.id = j["id"];
    data.type = j["type"];
    data.posX = j["posX"];
    data.posY = j["posY"];
    data.state = j["state"];
    data.growthProgress = j["growthProgress"];
    data.waterLevel = j["waterLevel"];
    data.lastWateredTime = j["lastWateredTime"];
    return data;
}

// WorkerData serialization
json WorkerData::toJson() const {
    return json{
        {"id", id},
        {"name", name},
        {"wateringSkill", wateringSkill},
        {"harvestingSkill", harvestingSkill},
        {"cashierSkill", cashierSkill},
        {"securitySkill", securitySkill},
        {"overallLevel", overallLevel},
        {"experience", experience},
        {"salary", salary},
        {"currentRole", currentRole},
        {"assignedZone", assignedZone}
    };
}

WorkerData WorkerData::fromJson(const json& j) {
    WorkerData data;
    data.id = j["id"];
    data.name = j["name"];
    data.wateringSkill = j["wateringSkill"];
    data.harvestingSkill = j["harvestingSkill"];
    data.cashierSkill = j["cashierSkill"];
    data.securitySkill = j["securitySkill"];
    data.overallLevel = j["overallLevel"];
    data.experience = j["experience"];
    data.salary = j["salary"];
    data.currentRole = j["currentRole"];
    data.assignedZone = j["assignedZone"];
    return data;
}

// InventoryData serialization
json InventoryData::toJson() const {
    json plantsJson = json::array();
    for (const auto& p : harvestedPlants) {
        plantsJson.push_back({{"type", p.first}, {"quantity", p.second}});
    }
    
    json seedsJson = json::array();
    for (const auto& s : seeds) {
        seedsJson.push_back({{"type", s.first}, {"quantity", s.second}});
    }
    
    return json{
        {"harvestedPlants", plantsJson},
        {"seeds", seedsJson},
        {"nutrients", nutrients},
        {"storageCapacity", storageCapacity}
    };
}

InventoryData InventoryData::fromJson(const json& j) {
    InventoryData data;
    
    for (const auto& p : j["harvestedPlants"]) {
        data.harvestedPlants.push_back({p["type"], p["quantity"]});
    }
    
    for (const auto& s : j["seeds"]) {
        data.seeds.push_back({s["type"], s["quantity"]});
    }
    
    data.nutrients = j["nutrients"];
    data.storageCapacity = j["storageCapacity"];
    return data;
}

// StatisticsData serialization
json StatisticsData::toJson() const {
    return json{
        {"totalPlantsGrown", totalPlantsGrown},
        {"totalMoneyEarned", totalMoneyEarned},
        {"totalCustomersServed", totalCustomersServed},
        {"totalRobberiesStopped", totalRobberiesStopped},
        {"achievementsUnlocked", achievementsUnlocked}
    };
}

StatisticsData StatisticsData::fromJson(const json& j) {
    StatisticsData data;
    data.totalPlantsGrown = j["totalPlantsGrown"];
    data.totalMoneyEarned = j["totalMoneyEarned"];
    data.totalCustomersServed = j["totalCustomersServed"];
    data.totalRobberiesStopped = j["totalRobberiesStopped"];
    data.achievementsUnlocked = j["achievementsUnlocked"].get<std::vector<std::string>>();
    return data;
}