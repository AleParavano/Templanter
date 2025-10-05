#include "Observer.h"
#include <algorithm>

void PlantSubject::addObserver(PlantObserver* observer) {
    if (observer != nullptr) {
        observers.push_back(observer);
    }
}

void PlantSubject::removeObserver(PlantObserver* observer) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
    );
}

void PlantSubject::notifyNeedsWater(Plant* plant) {
    for (auto* observer : observers) {
        observer->onPlantNeedsWater(plant);
    }
}

void PlantSubject::notifyRipe(Plant* plant) {
    for (auto* observer : observers) {
        observer->onPlantRipe(plant);
    }
}

void PlantSubject::notifyDecaying(Plant* plant) {
    for (auto* observer : observers) {
        observer->onPlantDecaying(plant);
    }
}