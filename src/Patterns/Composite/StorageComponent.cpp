#include "StorageComponent.h"
#include <iostream>
#include <algorithm>

// ============================================
// StorageSlot Implementation (Leaf)
// ============================================
StorageSlot::StorageSlot(const std::string& slotName, int slotCapacity)
    : StorageComponent(slotName, slotCapacity) {
}

bool StorageSlot::store(int amount) {
    if (currentLoad + amount <= capacity) {
        currentLoad += amount;
        return true;
    }
    return false;
}

bool StorageSlot::retrieve(int amount) {
    if (currentLoad >= amount) {
        currentLoad -= amount;
        return true;
    }
    return false;
}

int StorageSlot::getAvailableSpace() const {
    return capacity - currentLoad;
}

void StorageSlot::display(int depth) const {
    std::string indent(depth * 2, ' ');
    std::cout << indent << "📦 " << name 
              << " [" << currentLoad << "/" << capacity << "]" << std::endl;
}

// ============================================
// StorageContainer Implementation (Composite)
// ============================================
StorageContainer::StorageContainer(const std::string& containerName, int containerCapacity)
    : StorageComponent(containerName, containerCapacity) {
}

StorageContainer::~StorageContainer() {
    for (auto* child : children) {
        delete child;
    }
}

void StorageContainer::add(StorageComponent* component) {
    if (component != nullptr) {
        children.push_back(component);
    }
}

void StorageContainer::remove(StorageComponent* component) {
    auto it = std::find(children.begin(), children.end(), component);
    if (it != children.end()) {
        delete *it;
        children.erase(it);
    }
}

StorageComponent* StorageContainer::getChild(int index) {
    if (index >= 0 && index < static_cast<int>(children.size())) {
        return children[index];
    }
    return nullptr;
}

bool StorageContainer::store(int amount) {
    // Try to distribute storage across children
    int remaining = amount;
    
    for (auto* child : children) {
        if (remaining <= 0) break;
        
        int canStore = std::min(remaining, child->getAvailableSpace());
        if (child->store(canStore)) {
            remaining -= canStore;
            currentLoad += canStore;
        }
    }
    
    return remaining == 0;
}

bool StorageContainer::retrieve(int amount) {
    // Try to retrieve from children
    int remaining = amount;
    
    for (auto* child : children) {
        if (remaining <= 0) break;
        
        int canRetrieve = std::min(remaining, child->getCurrentLoad());
        if (child->retrieve(canRetrieve)) {
            remaining -= canRetrieve;
            currentLoad -= canRetrieve;
        }
    }
    
    return remaining == 0;
}

int StorageContainer::getAvailableSpace() const {
    int totalAvailable = 0;
    for (const auto* child : children) {
        totalAvailable += child->getAvailableSpace();
    }
    return std::min(totalAvailable, capacity - currentLoad);
}

void StorageContainer::display(int depth) const {
    std::string indent(depth * 2, ' ');
    std::cout << indent << "📁 " << name 
              << " [" << currentLoad << "/" << capacity << "]" << std::endl;
    
    for (const auto* child : children) {
        child->display(depth + 1);
    }
}