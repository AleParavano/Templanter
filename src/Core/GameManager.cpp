#include "GameManager.h"
#include "Greenhouse/Plant.h"
#include "Greenhouse/PlantTypes.h"
#include "Staff/Worker.h"
#include "Customer/Customer.h"
#include "Patterns/Mediator/WorkScheduler.h"
#include "Patterns/Composite/StorageComponent.h"
#include "Patterns/Iterator/InventoryIterator.h"
#include "Patterns/Factory/CustomerFactory.h"
#include <iostream>
#include "Patterns/Decorator/WorkerRole.h"  // ← ADD THIS


GameManager::GameManager()
    : window(nullptr), currentState(GameState::MAIN_MENU),
      currency(Config::STARTING_MONEY), customerRating(Config::STARTING_RATING),
      greenhouseWidth(Config::INITIAL_GRID_WIDTH), greenhouseHeight(Config::INITIAL_GRID_HEIGHT),
      selectedPlant(nullptr), selectedWorker(nullptr),
      selectedTileX(-1), selectedTileY(-1),
      customerSpawnTimer(0.0f), dayTimer(0.0f),
      totalPlantsGrown(0), totalCustomersServed(0), totalMoneyEarned(0.0) {
    
    // Initialize singletons
    timeManager = TimeManager::getInstance();
    saveManager = SaveManager::getInstance();
    
    // Initialize managers
    workScheduler = new WorkScheduler();
    
    // Initialize storage
    mainStorage = new StorageContainer("Main Warehouse", 1000);
    inventory = new Inventory();
    
    initializeWindow();
}

GameManager::~GameManager() {
    // Clean up plants
    for (auto* plant : plants) {
        delete plant;
    }
    
    // Clean up workers
    for (auto* worker : workers) {
        delete worker;
    }
    
    // Clean up customers
    while (!customerQueue.empty()) {
        delete customerQueue.front();
        customerQueue.pop();
    }
    
    delete workScheduler;
    delete mainStorage;
    delete inventory;
    delete window;
    
    // Destroy singletons
    TimeManager::destroyInstance();
    SaveManager::destroyInstance();
    PlantTypeDatabase::destroyInstance();
}

void GameManager::initializeWindow() {
    window = new sf::RenderWindow(
        sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT),
        Config::GAME_TITLE
    );
    window->setFramerateLimit(Config::FPS_LIMIT);
    
    // Try to load font (will fail silently for now since we don't have assets)
    if (!font.loadFromFile(Config::FONTS_PATH + "arial.ttf")) {
        std::cout << "Warning: Could not load font" << std::endl;
    }
}

void GameManager::initializeGame() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "     🌱 TEMPLANTER - New Game 🌱" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // Initialize greenhouse grid
    plants.resize(greenhouseWidth * greenhouseHeight, nullptr);
    
    // Give player some starting resources
    currency = Config::STARTING_MONEY;
    customerRating = Config::STARTING_RATING;
    
    // Add starting items to inventory
    inventory->addItem("Tomato Seeds", "seed", 10, 8);
    inventory->addItem("Basil Seeds", "seed", 10, 5);
    inventory->addItem("Nutrient Pack", "nutrient", 3, 50);
    
    // Hire one starting worker
    Worker* starter = new Worker(1, "Bob");
    starter->addRole(new WatererRole(starter));
    workers.push_back(starter);
    workScheduler->registerWorker(starter);
    
    std::cout << "✓ Starting balance: $" << currency << std::endl;
    std::cout << "✓ Starting rating: " << customerRating << " stars" << std::endl;
    std::cout << "✓ Greenhouse size: " << greenhouseWidth << "x" << greenhouseHeight << std::endl;
    std::cout << "✓ Starting worker: " << starter->getName() << std::endl;
    std::cout << "\nGame initialized! Press SPACE to start." << std::endl;
}

void GameManager::run() {
    sf::Clock clock;
    
    while (window->isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        handleInput();
        
        if (currentState == GameState::PLAYING) {
            update(deltaTime);
        }
        
        render();
    }
}

void GameManager::handleInput() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Escape:
                    if (currentState == GameState::PLAYING) {
                        pauseGame();
                    } else if (currentState == GameState::PAUSED) {
                        resumeGame();
                    }
                    break;
                    
                case sf::Keyboard::Space:
                    if (currentState == GameState::MAIN_MENU) {
                        startNewGame();
                    }
                    break;
                    
                case sf::Keyboard::S:
                    if (currentState == GameState::PLAYING && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        saveGame("quicksave");
                    }
                    break;
                    
                case sf::Keyboard::P:
                    if (currentState == GameState::PLAYING) {
                        timeManager->togglePause();
                    }
                    break;
                    
                default:
                    break;
            }
        }
        
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // Get tile coordinates
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;
                
                int tileX = (mouseX - 50) / Config::TILE_SIZE;
                int tileY = (mouseY - 50) / Config::TILE_SIZE;
                
                if (tileX >= 0 && tileX < greenhouseWidth && 
                    tileY >= 0 && tileY < greenhouseHeight) {
                    selectedTileX = tileX;
                    selectedTileY = tileY;
                    
                    int index = tileY * greenhouseWidth + tileX;
                    selectedPlant = plants[index];
                    
                    std::cout << "Selected tile (" << tileX << ", " << tileY << ")" << std::endl;
                }
            }
        }
    }
}

void GameManager::update(float deltaTime) {
    // Update time
    timeManager->update();
    
    // Update timers
    customerSpawnTimer += deltaTime;
    dayTimer += deltaTime;
    
    // Update game systems
    updatePlants(deltaTime);
    updateWorkers(deltaTime);
    processCustomers(deltaTime);
    
    // Spawn customers periodically
    if (customerSpawnTimer >= 15.0f) {  // Every 15 seconds
        spawnCustomer();
        customerSpawnTimer = 0.0f;
    }
    
    // Check for day end
    if (timeManager->getCurrentHour() == 0 && timeManager->getCurrentMinute() == 0) {
        checkDayEnd();
    }
}

void GameManager::render() {
    window->clear(sf::Color(34, 139, 34));  // Forest green background
    
    switch (currentState) {
        case GameState::MAIN_MENU:
            drawMainMenu();
            break;
            
        case GameState::PLAYING:
        case GameState::PAUSED:
            drawGreenhouse();
            drawUI();
            if (currentState == GameState::PAUSED) {
                drawPauseMenu();
            }
            break;
            
        default:
            break;
    }
    
    window->display();
}

void GameManager::drawGreenhouse() {
    // Draw grid
    for (int y = 0; y < greenhouseHeight; y++) {
        for (int x = 0; x < greenhouseWidth; x++) {
            sf::RectangleShape tile(sf::Vector2f(Config::TILE_SIZE - 2, Config::TILE_SIZE - 2));
            tile.setPosition(50 + x * Config::TILE_SIZE, 50 + y * Config::TILE_SIZE);
            
            int index = y * greenhouseWidth + x;
            Plant* plant = plants[index];
            
            // Color based on plant state
            if (plant == nullptr) {
                tile.setFillColor(sf::Color(139, 90, 43));  // Brown dirt
            } else {
                std::string state = plant->getStateName();
                if (state == "Seed") {
                    tile.setFillColor(sf::Color(160, 82, 45));  // Light brown
                } else if (state == "Growing") {
                    tile.setFillColor(sf::Color(34, 139, 34));  // Green
                } else if (state == "Ripe") {
                    tile.setFillColor(sf::Color(255, 215, 0));  // Gold
                } else if (state == "Decaying") {
                    tile.setFillColor(sf::Color(139, 69, 19));  // Dark brown
                } else {
                    tile.setFillColor(sf::Color(64, 64, 64));  // Gray (dead)
                }
            }
            
            // Highlight selected tile
            if (x == selectedTileX && y == selectedTileY) {
                tile.setOutlineColor(sf::Color::Yellow);
                tile.setOutlineThickness(2);
            }
            
            window->draw(tile);
        }
    }
}

void GameManager::drawUI() {
    // Draw top bar with game info
    sf::RectangleShape topBar(sf::Vector2f(Config::WINDOW_WIDTH, 40));
    topBar.setPosition(0, 0);
    topBar.setFillColor(sf::Color(50, 50, 50));
    window->draw(topBar);
    
    // Draw text (simplified - using shapes for now since font might not load)
    sf::Text infoText;
    if (font.getInfo().family != "") {
        infoText.setFont(font);
        infoText.setCharacterSize(16);
        infoText.setFillColor(sf::Color::White);
        
        std::string info = "Day " + std::to_string(timeManager->getCurrentDay()) + 
                          " | " + timeManager->getTimeString() +
                          " | $" + std::to_string(static_cast<int>(currency)) +
                          " | Rating: " + std::to_string(static_cast<int>(customerRating)) + "/5";
        
        infoText.setString(info);
        infoText.setPosition(10, 10);
        window->draw(infoText);
    }
    
    // Draw selected plant info if any
    if (selectedPlant != nullptr) {
        drawPlantInfo(selectedPlant);
    }
}

void GameManager::drawPlantInfo(Plant* plant) {
    sf::RectangleShape infoBox(sf::Vector2f(250, 150));
    infoBox.setPosition(Config::WINDOW_WIDTH - 270, 60);
    infoBox.setFillColor(sf::Color(40, 40, 40, 200));
    infoBox.setOutlineColor(sf::Color::White);
    infoBox.setOutlineThickness(2);
    window->draw(infoBox);
    
    if (font.getInfo().family != "") {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::White);
        
        std::string info = plant->getPlantTypeInfo().name + "\n";
        info += "State: " + plant->getStateName() + "\n";
        info += "Growth: " + std::to_string(static_cast<int>(plant->getGrowthProgress())) + "%\n";
        info += "Water: " + std::to_string(plant->getWaterLevel()) + "%\n";
        
        text.setString(info);
        text.setPosition(Config::WINDOW_WIDTH - 260, 70);
        window->draw(text);
    }
}

void GameManager::drawWorkerInfo(Worker* worker) {
    // Similar to plant info
}

void GameManager::drawMainMenu() {
    sf::RectangleShape menuBox(sf::Vector2f(400, 300));
    menuBox.setPosition(Config::WINDOW_WIDTH / 2 - 200, Config::WINDOW_HEIGHT / 2 - 150);
    menuBox.setFillColor(sf::Color(40, 40, 40, 220));
    menuBox.setOutlineColor(sf::Color::White);
    menuBox.setOutlineThickness(3);
    window->draw(menuBox);
    
    if (font.getInfo().family != "") {
        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(36);
        title.setFillColor(sf::Color::Green);
        title.setString("TEMPLANTER");
        title.setPosition(Config::WINDOW_WIDTH / 2 - 120, Config::WINDOW_HEIGHT / 2 - 120);
        window->draw(title);
        
        sf::Text instruction;
        instruction.setFont(font);
        instruction.setCharacterSize(20);
        instruction.setFillColor(sf::Color::White);
        instruction.setString("Press SPACE to start\nPress ESC to quit");
        instruction.setPosition(Config::WINDOW_WIDTH / 2 - 100, Config::WINDOW_HEIGHT / 2);
        window->draw(instruction);
    }
}

void GameManager::drawPauseMenu() {
    sf::RectangleShape overlay(sf::Vector2f(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window->draw(overlay);
    
    if (font.getInfo().family != "") {
        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setCharacterSize(48);
        pauseText.setFillColor(sf::Color::White);
        pauseText.setString("PAUSED");
        pauseText.setPosition(Config::WINDOW_WIDTH / 2 - 80, Config::WINDOW_HEIGHT / 2 - 50);
        window->draw(pauseText);
    }
}

void GameManager::startNewGame() {
    std::cout << "\n🌱 Starting new game..." << std::endl;
    initializeGame();
    currentState = GameState::PLAYING;
}

void GameManager::loadGame(const std::string& slotName) {
    std::cout << "Loading game from " << slotName << "..." << std::endl;
    // TODO: Implement loading
    currentState = GameState::PLAYING;
}

void GameManager::saveGame(const std::string& slotName) {
    std::cout << "💾 Saving game to " << slotName << "..." << std::endl;
    // TODO: Implement saving using GameMemento
}

void GameManager::pauseGame() {
    currentState = GameState::PAUSED;
    timeManager->pause();
    std::cout << "⏸️  Game paused" << std::endl;
}

void GameManager::resumeGame() {
    currentState = GameState::PLAYING;
    timeManager->resume();
    std::cout << "▶️  Game resumed" << std::endl;
}

void GameManager::quitGame() {
    window->close();
}

bool GameManager::plantSeed(PlantType type, int x, int y) {
    if (x < 0 || x >= greenhouseWidth || y < 0 || y >= greenhouseHeight) {
        return false;
    }
    
    int index = y * greenhouseWidth + x;
    if (plants[index] != nullptr) {
        std::cout << "❌ Tile already occupied!" << std::endl;
        return false;
    }
    
    PlantTypeDatabase* db = PlantTypeDatabase::getInstance();
    const PlantTypeInfo& info = db->getPlantInfo(type);
    
    if (currency < info.seedCost) {
        std::cout << "❌ Not enough money! Need $" << info.seedCost << std::endl;
        return false;
    }
    
    currency -= info.seedCost;
    
    int plantId = plants.size();
    Plant* newPlant = new Plant(plantId, type, x, y);
    plants[index] = newPlant;
    
    // Register plant with work scheduler as observer
    newPlant->addObserver(workScheduler);
    
    std::cout << "✅ Planted " << info.name << " at (" << x << ", " << y << ")" << std::endl;
    return true;
}

bool GameManager::waterPlant(int x, int y) {
    if (x < 0 || x >= greenhouseWidth || y < 0 || y >= greenhouseHeight) {
        return false;
    }
    
    int index = y * greenhouseWidth + x;
    Plant* plant = plants[index];
    
    if (plant == nullptr) {
        std::cout << "❌ No plant at this location!" << std::endl;
        return false;
    }
    
    plant->water();
    std::cout << "💧 Watered " << plant->getPlantTypeInfo().name << std::endl;
    return true;
}

bool GameManager::harvestPlant(int x, int y) {
    if (x < 0 || x >= greenhouseWidth || y < 0 || y >= greenhouseHeight) {
        return false;
    }
    
    int index = y * greenhouseWidth + x;
    Plant* plant = plants[index];
    
    if (plant == nullptr) {
        std::cout << "❌ No plant at this location!" << std::endl;
        return false;
    }
    
    if (!plant->canHarvest()) {
        std::cout << "❌ Plant is not ready to harvest!" << std::endl;
        return false;
    }
    
    int value = 0;
    if (plant->harvest(value)) {
        currency += value;
        totalMoneyEarned += value;
        totalPlantsGrown++;
        
        // Add to inventory
        std::string plantName = plant->getPlantTypeInfo().name;
        inventory->addItem(plantName, "plant", 1, value);
        
        std::cout << "🌾 Harvested " << plantName << " for $" << value << std::endl;
        
        // Remove plant
        delete plant;
        plants[index] = nullptr;
        
        return true;
    }
    
    return false;
}

bool GameManager::hireWorker(const std::string& name) {
    if (currency < Config::WORKER_HIRE_COST) {
        std::cout << "❌ Not enough money to hire worker!" << std::endl;
        return false;
    }
    
    currency -= Config::WORKER_HIRE_COST;
    
    int workerId = workers.size() + 1;
    Worker* newWorker = new Worker(workerId, name);
    workers.push_back(newWorker);
    workScheduler->registerWorker(newWorker);
    
    std::cout << "✅ Hired " << name << " as worker #" << workerId << std::endl;
    return true;
}

bool GameManager::buyItem(const std::string& itemName) {
    // TODO: Implement shop
    return false;
}

void GameManager::spawnCustomer() {
    Customer* customer = CustomerFactory::createRandomCustomer();
    customerQueue.push(customer);
}

void GameManager::processCustomers(float deltaTime) {
    // Update all customers in queue
    std::queue<Customer*> updatedQueue;
    
    while (!customerQueue.empty()) {
        Customer* customer = customerQueue.front();
        customerQueue.pop();
        
        customer->update(deltaTime);
        
        if (customer->hasTimedOut() || customer->isServed()) {
            if (customer->hasTimedOut()) {
                customerRating -= Config::RATING_TIMEOUT_PENALTY;
            }
            delete customer;
        } else {
            updatedQueue.push(customer);
        }
    }
    
    customerQueue = updatedQueue;
}

void GameManager::updatePlants(float deltaTime) {
    for (Plant* plant : plants) {
        if (plant != nullptr) {
            plant->update(deltaTime);
        }
    }
}

void GameManager::updateWorkers(float deltaTime) {
    workScheduler->updateAllWorkers(deltaTime);
}

void GameManager::checkDayEnd() {
    std::cout << "\n🌙 End of Day " << timeManager->getCurrentDay() << std::endl;
    workScheduler->payAllWorkers(currency);
}