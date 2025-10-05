#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include "Core/GameData.h"
#include "Core/TimeManager.h"
#include "Core/SaveManager.h"
#include "Patterns/Memento/GameMemento.h"
#include "Staff/Worker.h"
#include "Patterns/Decorator/WorkerRole.h"
#include "Patterns/Command/Command.h"
#include "Patterns/Mediator/WorkScheduler.h"
#include "Greenhouse/Plant.h"
#include "Greenhouse/PlantTypes.h"

void printHeader(const std::string& title) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << "========================================" << std::endl;
}

void printTestResult(const std::string& test, bool passed) {
    std::cout << "[" << (passed ? "✓ PASS" : "✗ FAIL") << "] " << test << std::endl;
}

// ============================================
// CORE SYSTEM TESTS
// ============================================
bool testTimeManager() {
    printHeader("Testing TimeManager (Singleton)");
    
    TimeManager* tm = TimeManager::getInstance();
    TimeManager* tm2 = TimeManager::getInstance();
    
    bool singletonTest = (tm == tm2);
    bool initialDay = (tm->getCurrentDay() == 1);
    bool initialHour = (tm->getCurrentHour() == 6);
    
    printTestResult("Singleton pattern", singletonTest);
    printTestResult("Initial day is 1", initialDay);
    printTestResult("Initial hour is 6 AM", initialHour);
    
    std::cout << "\n✓ TimeManager tests completed!" << std::endl;
    return singletonTest && initialDay && initialHour;
}

bool testSaveLoadSystem() {
    printHeader("Testing SaveManager & Memento Pattern");
    
    SaveManager* sm = SaveManager::getInstance();
    
    // Create test data
    std::vector<PlantData> testPlants;
    PlantData plant1;
    plant1.id = 0;
    plant1.type = "Tomato";
    plant1.posX = 0;
    plant1.posY = 0;
    plant1.state = "Growing";
    plant1.growthProgress = 50;
    plant1.waterLevel = 80;
    plant1.lastWateredTime = 123456;
    testPlants.push_back(plant1);
    
    std::vector<WorkerData> testWorkers;
    InventoryData testInventory;
    testInventory.nutrients = 20;
    testInventory.storageCapacity = 40;
    
    StatisticsData testStats;
    testStats.totalPlantsGrown = 25;
    testStats.totalMoneyEarned = 1500.50;
    
    GameMemento memento = GameMemento::createTestMemento(
        10, 14, 30, 1234.56, 4.2,
        testPlants, 3, 3, testWorkers, testInventory, testStats
    );
    
    bool saveSuccess = sm->saveGame(memento, "test_save");
    bool existsTest = sm->saveExists("test_save");
    
    printTestResult("Save game to file", saveSuccess);
    printTestResult("Save file exists", existsTest);
    
    sm->deleteSave("test_save");
    
    std::cout << "\n✓ SaveManager tests completed!" << std::endl;
    return saveSuccess && existsTest;
}

// ============================================
// PLANT SYSTEM TESTS
// ============================================
bool testPlantSystem() {
    printHeader("Testing Plant System");
    
    PlantTypeDatabase* db = PlantTypeDatabase::getInstance();
    
    const PlantTypeInfo& basilInfo = db->getPlantInfo(PlantType::BASIL);
    bool hasBasil = (basilInfo.name == "Basil");
    
    Plant* tomato = new Plant(1, PlantType::TOMATO, 0, 0);
    bool plantCreated = (tomato != nullptr);
    
    printTestResult("Plant database loaded", hasBasil);
    printTestResult("Plant created", plantCreated);
    
    delete tomato;
    
    std::cout << "\n✓ Plant system tests completed!" << std::endl;
    return hasBasil && plantCreated;
}

// ============================================
// WORKER SYSTEM TESTS
// ============================================
bool testWorkerSystem() {
    printHeader("Testing Worker System");
    
    // Test 1: Worker creation
    Worker* worker = new Worker(1, "Alice");
    bool workerCreated = (worker != nullptr);
    printTestResult("Worker created", workerCreated);
    
    // Test 2: Decorator Pattern (Roles)
    WorkerRole* watererRole = new WatererRole(worker);
    worker->addRole(watererRole);
    bool hasWaterer = worker->hasRole("Waterer");
    
    WorkerRole* harvesterRole = new HarvesterRole(worker);
    worker->addRole(harvesterRole);
    bool hasHarvester = worker->hasRole("Harvester");
    
    printTestResult("Decorator Pattern - Waterer role", hasWaterer);
    printTestResult("Decorator Pattern - Harvester role", hasHarvester);
    
    // Test 3: Command Pattern
    Plant* plant = new Plant(1, PlantType::TOMATO, 0, 0);
    Command* waterCmd = new WaterPlantCommand(worker, plant);
    worker->addTask(waterCmd);
    bool hasTask = (worker->getTaskCount() > 0);
    printTestResult("Command Pattern - Task added", hasTask);
    
    worker->executeNextTask();
    bool taskExecuted = (worker->getTaskCount() == 0);
    printTestResult("Command Pattern - Task executed", taskExecuted);
    
    // Test 4: Skill training
    int oldSkill = worker->getWateringSkill();
    worker->trainSkill("watering");
    int newSkill = worker->getWateringSkill();
    bool skillImproved = (newSkill > oldSkill);
    printTestResult("Skill training works", skillImproved);
    
    // Test 5: Mediator Pattern
    WorkScheduler scheduler;
    scheduler.registerWorker(worker);
    bool workerRegistered = (scheduler.getWorkers().size() == 1);
    printTestResult("Mediator Pattern - Worker registered", workerRegistered);
    
    // Test 6: Experience & Leveling
    int oldLevel = worker->getOverallLevel();
    worker->gainExperience(150);
    int newLevel = worker->getOverallLevel();
    bool leveledUp = (newLevel > oldLevel);
    printTestResult("Experience & leveling system", leveledUp);
    
    delete worker;
    delete plant;
    
    std::cout << "\n✓ Worker system tests completed!" << std::endl;
    return workerCreated && hasWaterer && hasHarvester && hasTask && 
           taskExecuted && skillImproved && workerRegistered && leveledUp;
}

// ============================================
// SFML TEST
// ============================================
bool testSFML() {
    printHeader("Testing SFML");
    
    try {
        sf::RenderWindow window(sf::VideoMode(400, 300), "TEMPLANTER Test");
        printTestResult("SFML window creation", true);
        window.close();
        
        std::cout << "\n✓ SFML tests completed!" << std::endl;
        return true;
    } catch (const std::exception& e) {
        printTestResult("SFML window creation", false);
        return false;
    }
}

// ============================================
// MAIN
// ============================================
int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║                                        ║" << std::endl;
    std::cout << "║     TEMPLANTER - Full Test Suite      ║" << std::endl;
    std::cout << "║                                        ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    
    int totalTests = 0;
    int passedTests = 0;
    
    std::cout << "\n=== CORE SYSTEMS ===" << std::endl;
    if (testTimeManager()) passedTests++;
    totalTests++;
    
    if (testSaveLoadSystem()) passedTests++;
    totalTests++;
    
    std::cout << "\n=== PLANT SYSTEM ===" << std::endl;
    if (testPlantSystem()) passedTests++;
    totalTests++;
    
    std::cout << "\n=== WORKER SYSTEM ===" << std::endl;
    if (testWorkerSystem()) passedTests++;
    totalTests++;
    
    std::cout << "\n=== GRAPHICS SYSTEM ===" << std::endl;
    if (testSFML()) passedTests++;
    totalTests++;
    
    // Final results
    printHeader("FINAL RESULTS");
    std::cout << "\nTests Passed: " << passedTests << "/" << totalTests << std::endl;
    
    double percentage = (static_cast<double>(passedTests) / totalTests) * 100.0;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(1) 
              << percentage << "%" << std::endl;
    
    if (passedTests == totalTests) {
        std::cout << "\n🎉 ALL TESTS PASSED! 🎉" << std::endl;
        std::cout << "\n✅ Patterns Implemented (10/14):" << std::endl;
        std::cout << "  1. Singleton (TimeManager, SaveManager)" << std::endl;
        std::cout << "  2. Memento (Save/Load)" << std::endl;
        std::cout << "  3. State (Plant States)" << std::endl;
        std::cout << "  4. Template Method (Growth Cycles)" << std::endl;
        std::cout << "  5. Flyweight (Plant Type Database)" << std::endl;
        std::cout << "  6. Observer (Plant Notifications)" << std::endl;
        std::cout << "  7. Decorator (Worker Roles)" << std::endl;
        std::cout << "  8. Command (Task Queue)" << std::endl;
        std::cout << "  9. Mediator (Work Scheduler)" << std::endl;
        std::cout << "  10. Strategy (Growth Strategies)" << std::endl;
        std::cout << "\n📋 Remaining Patterns (4/14):" << std::endl;
        std::cout << "  11. Factory (Customer/Product Creation)" << std::endl;
        std::cout << "  12. Visitor (Customer Behavior)" << std::endl;
        std::cout << "  13. Composite (Storage Hierarchy)" << std::endl;
        std::cout << "  14. Iterator (Inventory Iteration)" << std::endl;
        std::cout << "\nReady to continue building! 🚀" << std::endl;
    } else {
        std::cout << "\n⚠️  Some tests failed. Please review above." << std::endl;
    }
    
    std::cout << "\n========================================\n" << std::endl;
    
    // Cleanup singletons
    TimeManager::destroyInstance();
    SaveManager::destroyInstance();
    PlantTypeDatabase::destroyInstance();
    
    return (passedTests == totalTests) ? 0 : 1;
}