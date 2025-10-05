#ifndef PLANTTYPES_H
#define PLANTTYPES_H

#include <string>
#include <map>
#include <vector> 


// Enum for plant types
enum class PlantType {
    BASIL,
    LETTUCE,
    TOMATO,
    CARROT,
    ROSE,
    SUNFLOWER,
    TULIP,
    ORCHID,
    BONSAI,
    CACTUS
};

// Plant tier/difficulty
enum class PlantTier {
    BEGINNER,      // Easy & cheap
    INTERMEDIATE,  // Medium difficulty
    ADVANCED       // Hard & expensive
};

// Flyweight Pattern: Shared immutable plant data
struct PlantTypeInfo {
    PlantType type;
    std::string name;
    PlantTier tier;
    
    // Growth
    int growthTimeHours;      // Hours to grow from seed to ripe
    int waterIntervalHours;   // Hours between watering
    
    // Economy
    int seedCost;
    int sellPrice;
    
    // Maintenance
    int waterGracePeriodMinutes;  // Grace period for watering
    float decayRate;              // How fast it decays if neglected (0.0 - 1.0)
    
    // Customer demand (probability %)
    float demandProbability;
};

// Flyweight Factory: Manages shared plant type data
class PlantTypeDatabase {
private:
    static PlantTypeDatabase* instance;
    std::map<PlantType, PlantTypeInfo> plantData;
    
    PlantTypeDatabase();
    void initializePlantData();

public:
    static PlantTypeDatabase* getInstance();
    static void destroyInstance();
    
    const PlantTypeInfo& getPlantInfo(PlantType type) const;
    PlantType getPlantTypeByName(const std::string& name) const;
    std::string getPlantName(PlantType type) const;
    
    // Get all plants by tier
    std::vector<PlantType> getPlantsByTier(PlantTier tier) const;
    
    // Random plant selection (weighted by demand)
    PlantType getRandomPlantByDemand() const;
};

#endif // PLANTTYPES_H