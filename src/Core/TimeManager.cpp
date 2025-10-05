#include "TimeManager.h"
#include <sstream>
#include <iomanip>

// Initialize static instance
TimeManager* TimeManager::instance = nullptr;

TimeManager::TimeManager() 
    : currentDay(1), currentHour(Config::DAY_START_HOUR), currentMinute(0),
      secondsAccumulator(0.0f), isPaused(false), deltaTime(0.0f) {
    clock.restart();
}

TimeManager* TimeManager::getInstance() {
    if (instance == nullptr) {
        instance = new TimeManager();
    }
    return instance;
}

void TimeManager::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

void TimeManager::update() {
    if (isPaused) {
        deltaTime = 0.0f;
        clock.restart();  // Keep clock in sync
        return;
    }
    
    // Calculate delta time
    deltaTime = clock.restart().asSeconds();
    
    // Accumulate time
    secondsAccumulator += deltaTime;
    
    // Check if a game minute has passed
    while (secondsAccumulator >= Config::REAL_SECONDS_PER_GAME_MINUTE) {
        secondsAccumulator -= Config::REAL_SECONDS_PER_GAME_MINUTE;
        
        // Advance time
        currentMinute++;
        
        if (currentMinute >= Config::MINUTES_PER_HOUR) {
            currentMinute = 0;
            currentHour++;
            
            if (currentHour >= Config::HOURS_PER_DAY) {
                currentHour = 0;
                currentDay++;
            }
        }
    }
}

void TimeManager::pause() {
    isPaused = true;
}

void TimeManager::resume() {
    isPaused = false;
    clock.restart();  // Reset clock to avoid time jump
}

void TimeManager::togglePause() {
    if (isPaused) {
        resume();
    } else {
        pause();
    }
}

bool TimeManager::isDay() const {
    return currentHour >= Config::DAY_START_HOUR && 
           currentHour < Config::NIGHT_START_HOUR;
}

bool TimeManager::isNight() const {
    return !isDay();
}

std::string TimeManager::getTimeString() const {
    std::stringstream ss;
    
    // Convert to 12-hour format
    int displayHour = currentHour % 12;
    if (displayHour == 0) displayHour = 12;
    
    std::string period = (currentHour < 12) ? "AM" : "PM";
    
    ss << std::setfill('0') << std::setw(2) << displayHour << ":"
       << std::setfill('0') << std::setw(2) << currentMinute << " " << period;
    
    return ss.str();
}

std::string TimeManager::getTimeOfDayString() const {
    if (currentHour >= 6 && currentHour < 12) return "Morning";
    if (currentHour >= 12 && currentHour < 17) return "Afternoon";
    if (currentHour >= 17 && currentHour < 20) return "Evening";
    return "Night";
}

void TimeManager::setTime(int hour, int minute) {
    currentHour = hour;
    currentMinute = minute;
}

void TimeManager::reset() {
    currentDay = 1;
    currentHour = Config::DAY_START_HOUR;
    currentMinute = 0;
    secondsAccumulator = 0.0f;
    isPaused = false;
    clock.restart();
}