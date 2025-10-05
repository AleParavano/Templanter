#include "PlantTypes.h"
#include <algorithm>
#include <random>
#include <stdexcept>

PlantTypeDatabase* PlantTypeDatabase::instance = nullptr;

PlantTypeDatabase::PlantTypeDatabase() {
    initializePlantData();
}

PlantTypeDatabase* PlantTypeDatabase::getInstance() {
    if (instance == nullptr) {
        instance = new PlantTypeDatabase();
    }
    return instance;
}

void PlantTypeDatabase::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

void PlantTypeDatabase::initializePlantData() {
    // TIER 1: BEGINNER PLANTS
    plantData[PlantType::BASIL] = {
        PlantType::BASIL, "Basil", PlantTier::BEGINNER,
        48,    // 2 days growth
        6,     // Water every 6 hours
        5,     // $5 seed cost
        20,    // $20 sell price
        120,   // 2 hour grace period
        0.1f,  // Slow decay
        20.0f  // 20% demand
    };
    
    plantData[PlantType::LETTUCE] = {
        PlantType::LETTUCE, "Lettuce", PlantTier::BEGINNER,
        60,    // 2.5 days
        5,     // Water every 5 hours
        7,     // $7 seed
        25,    // $25 sell
        120,   // 2 hour grace
        0.12f, // Slow decay
        18.0f  // 18% demand
    };
    
    plantData[PlantType::TOMATO] = {
        PlantType::TOMATO, "Tomato", PlantTier::BEGINNER,
        72,    // 3 days
        8,     // Water every 8 hours
        8,     // $8 seed
        30,    // $30 sell
        120,   // 2 hour grace
        0.15f, // Slow decay
        17.0f  // 17% demand
    };
    
    // TIER 2: INTERMEDIATE PLANTS
    plantData[PlantType::CARROT] = {
        PlantType::CARROT, "Carrot", PlantTier::INTERMEDIATE,
        96,    // 4 days
        7,     // Water every 7 hours
        12,    // $12 seed
        45,    // $45 sell
        60,    // 1 hour grace
        0.2f,  // Medium decay
        15.0f  // 15% demand
    };
    
    plantData[PlantType::ROSE] = {
        PlantType::ROSE, "Rose", PlantTier::INTERMEDIATE,
        120,   // 5 days
        10,    // Water every 10 hours
        18,    // $18 seed
        70,    // $70 sell
        60,    // 1 hour grace
        0.25f, // Medium decay
        12.0f  // 12% demand
    };
    
    plantData[PlantType::SUNFLOWER] = {
        PlantType::SUNFLOWER, "Sunflower", PlantTier::INTERMEDIATE,
        132,   // 5.5 days
        9,     // Water every 9 hours
        20,    // $20 seed
        75,    // $75 sell
        60,    // 1 hour grace
        0.22f, // Medium decay
        8.0f   // 8% demand
    };
    
    // TIER 3: ADVANCED PLANTS
    plantData[PlantType::TULIP] = {
        PlantType::TULIP, "Tulip", PlantTier::ADVANCED,
        144,   // 6 days
        6,     // Water every 6 hours (high maintenance!)
        30,    // $30 seed
        110,   // $110 sell
        30,    // 30 min grace
        0.4f,  // Fast decay
        5.0f   // 5% demand
    };
    
    plantData[PlantType::ORCHID] = {
        PlantType::ORCHID, "Orchid", PlantTier::ADVANCED,
        168,   // 7 days
        5,     // Water every 5 hours (very high maintenance!)
        40,    // $40 seed
        150,   // $150 sell
        15,    // 15 min grace
        0.5f,  // Very fast decay
        3.0f   // 3% demand
    };
    
    plantData[PlantType::BONSAI] = {
        PlantType::BONSAI, "Bonsai", PlantTier::ADVANCED,
        192,   // 8 days
        8,     // Water every 8 hours (but needs pruning)
        55,    // $55 seed
        200,   // $200 sell
        30,    // 30 min grace
        0.45f, // Fast decay
        1.5f   // 1.5% demand
    };
    
    plantData[PlantType::CACTUS] = {
        PlantType::CACTUS, "Rare Cactus", PlantTier::ADVANCED,
        240,   // 10 days
        20,    // Water every 20 hours (long intervals but MUST be precise!)
        75,    // $75 seed
        300,   // $300 sell
        10,    // 10 min grace (very unforgiving!)
        0.6f,  // Extremely fast decay if timing is wrong
        0.5f   // 0.5% demand (very rare)
    };
}

const PlantTypeInfo& PlantTypeDatabase::getPlantInfo(PlantType type) const {
    auto it = plantData.find(type);
    if (it != plantData.end()) {
        return it->second;
    }
    throw std::runtime_error("Plant type not found in database");
}

PlantType PlantTypeDatabase::getPlantTypeByName(const std::string& name) const {
    for (const auto& pair : plantData) {
        if (pair.second.name == name) {
            return pair.first;
        }
    }
    throw std::runtime_error("Plant name not found: " + name);
}

std::string PlantTypeDatabase::getPlantName(PlantType type) const {
    return getPlantInfo(type).name;
}

std::vector<PlantType> PlantTypeDatabase::getPlantsByTier(PlantTier tier) const {
    std::vector<PlantType> result;
    for (const auto& pair : plantData) {
        if (pair.second.tier == tier) {
            result.push_back(pair.first);
        }
    }
    return result;
}

PlantType PlantTypeDatabase::getRandomPlantByDemand() const {
    // Create weighted distribution based on demand probability
    std::vector<PlantType> types;
    std::vector<float> weights;
    
    for (const auto& pair : plantData) {
        types.push_back(pair.first);
        weights.push_back(pair.second.demandProbability);
    }
    
    // Random selection
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::discrete_distribution<> dist(weights.begin(), weights.end());
    
    int index = dist(gen);
    return types[index];
}