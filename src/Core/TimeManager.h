#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <SFML/System.hpp>
#include "Config.h"

// Singleton Pattern: Manages game time
class TimeManager {
private:
    static TimeManager* instance;
    
    // Time tracking
    int currentDay;
    int currentHour;
    int currentMinute;
    float secondsAccumulator;
    
    bool isPaused;
    sf::Clock clock;
    float deltaTime;
    
    // Private constructor for Singleton
    TimeManager();
    
public:
    // Singleton access
    static TimeManager* getInstance();
    static void destroyInstance();
    
    // Delete copy constructor and assignment
    TimeManager(const TimeManager&) = delete;
    TimeManager& operator=(const TimeManager&) = delete;
    
    // Time updates
    void update();
    void pause();
    void resume();
    void togglePause();
    
    // Getters
    int getCurrentDay() const { return currentDay; }
    int getCurrentHour() const { return currentHour; }
    int getCurrentMinute() const { return currentMinute; }
    float getDeltaTime() const { return deltaTime; }
    bool getIsPaused() const { return isPaused; }
    
    // Time queries
    bool isDay() const;
    bool isNight() const;
    std::string getTimeString() const;
    std::string getTimeOfDayString() const;
    
    // Setters (for loading game)
    void setDay(int day) { currentDay = day; }
    void setTime(int hour, int minute);
    
    // Reset
    void reset();
};

#endif // TIMEMANAGER_H