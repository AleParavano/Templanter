#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config {
    // Window Settings
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    const std::string GAME_TITLE = "TEMPLANTER";
    const int FPS_LIMIT = 60;
    
    // Tile Settings
    const int TILE_SIZE = 64;  // 64x64 pixels per tile
    const int INITIAL_GRID_WIDTH = 3;
    const int INITIAL_GRID_HEIGHT = 3;
    
    // Time Settings
    const float REAL_SECONDS_PER_GAME_MINUTE = 1.0f;  // 1 real second = 1 game minute
    const int MINUTES_PER_HOUR = 60;
    const int HOURS_PER_DAY = 24;
    const int DAY_START_HOUR = 6;   // 6 AM
    const int NIGHT_START_HOUR = 20; // 8 PM
    
    // Economy
    const double STARTING_MONEY = 500.0;
    const double STARTING_RATING = 3.0;
    
    // Worker Settings
    const double BASE_WORKER_SALARY = 50.0;
    const double WORKER_HIRE_COST = 150.0;
    const int WORKER_SKILL_TRAIN_COST[] = {0, 100, 250, 500, 1000}; // Cost to upgrade to each level
    
    // Customer Settings
    const float CUSTOMER_REGULAR_WAIT_TIME = 120.0f;  // 2 minutes in seconds
    const float CUSTOMER_VIP_WAIT_TIME = 60.0f;       // 1 minute
    const float ROBBER_CHANCE = 0.05f;                // 5% chance
    const float VIP_CHANCE = 0.10f;                   // 10% chance
    
    // Rating Effects
    const float RATING_SUCCESS_BONUS = 0.1f;
    const float RATING_NO_STOCK_PENALTY = 0.3f;
    const float RATING_TIMEOUT_PENALTY = 0.2f;
    const float RATING_ROBBERY_PENALTY = 0.5f;
    const float RATING_SECURITY_BONUS = 0.1f;
    
    // Save System
    const std::string SAVE_DIRECTORY = "saves/";
    const std::string SAVE_EXTENSION = ".json";
    const int MAX_SAVE_SLOTS = 3;
    
    // Paths
    const std::string ASSETS_PATH = "assets/";
    const std::string SPRITES_PATH = ASSETS_PATH + "sprites/";
    const std::string FONTS_PATH = ASSETS_PATH + "fonts/";
}

#endif // CONFIG_H