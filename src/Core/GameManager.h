#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include "TimeManager.h"
#include "SaveManager.h"
#include "Config.h"
#include "Patterns/Memento/Caretaker.h"
#include "Patterns/Memento/GameMemento.h"
#include "Greenhouse/Plant.h"

// Forward declarations
class Plant;
class Worker;
class Customer;
class WorkScheduler;
class StorageContainer;
class Inventory;

enum class GameState {
    MAIN_MENU,
    PLAYING,
    PAUSED,
    SHOP_MENU,
    WORKER_MENU,
    SAVE_MENU
};

// Main game controller (Originator in Memento Pattern)
class GameManager {
private:
    // SFML
    sf::RenderWindow* window;
    sf::Font font;
    
    // Game state
    GameState currentState;
    
    // Core managers
    TimeManager* timeManager;
    SaveManager* saveManager;
    WorkScheduler* workScheduler;
    Caretaker* caretaker;  // Memento Pattern: Caretaker
    
    // Game data
    double currency;
    double customerRating;
    int greenhouseWidth;
    int greenhouseHeight;
    std::vector<Plant*> plants;
    std::vector<Worker*> workers;
    std::queue<Customer*> customerQueue;
    
    // Storage & Inventory
    StorageContainer* mainStorage;
    Inventory* inventory;
    
    // UI state
    Plant* selectedPlant;
    Worker* selectedWorker;
    int selectedTileX;
    int selectedTileY;
    
    // Timers
    float customerSpawnTimer;
    float dayTimer;
    
    // Statistics
    int totalPlantsGrown;
    int totalCustomersServed;
    double totalMoneyEarned;
    
    // Helper methods
    void initializeWindow();
    void initializeGame();
    void handleInput();
    void update(float deltaTime);
    void render();
    
    // Game logic
    void spawnCustomer();
    void processCustomers(float deltaTime);
    void updatePlants(float deltaTime);
    void updateWorkers(float deltaTime);
    void checkDayEnd();
    
    // UI methods
    void drawGreenhouse();
    void drawUI();
    void drawPlantInfo(Plant* plant);
    void drawWorkerInfo(Worker* worker);
    void drawMainMenu();
    void drawPauseMenu();
    
public:
    GameManager();
    ~GameManager();
    
    void run();
    
    // Game actions
    void startNewGame();
    void loadGame(const std::string& slotName);
    void saveGame(const std::string& slotName);
    void pauseGame();
    void resumeGame();
    void quitGame();
    
    // Memento Pattern: Originator methods
    void createSnapshot();
    void restoreSnapshot(int index);
    void undoToLastSnapshot();
    void listSnapshots() const;
    
    // Player actions
    bool plantSeed(PlantType type, int x, int y);
    bool waterPlant(int x, int y);
    bool harvestPlant(int x, int y);
    bool hireWorker(const std::string& name);
    bool buyItem(const std::string& itemName);
    
    // Getters
    double getCurrency() const { return currency; }
    double getRating() const { return customerRating; }
    bool isRunning() const { return window && window->isOpen(); }
};

#endif // GAMEMANAGER_H