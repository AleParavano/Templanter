#include "InventoryIterator.h"
#include <algorithm>
#include <iostream>

// ============================================
// InventoryIterator Implementation
// ============================================
InventoryIterator::InventoryIterator(std::vector<InventoryItem*>& itemList)
    : items(itemList), currentIndex(0) {
}

bool InventoryIterator::hasNext() const {
    return currentIndex < items.size();
}

InventoryItem* InventoryIterator::next() {
    if (hasNext()) {
        return items[currentIndex++];
    }
    return nullptr;
}

void InventoryIterator::reset() {
    currentIndex = 0;
}

// ============================================
// FilteredInventoryIterator Implementation
// ============================================
FilteredInventoryIterator::FilteredInventoryIterator(std::vector<InventoryItem*>& itemList, const std::string& type)
    : items(itemList), filterType(type), currentIndex(0) {
    findNext();
}

void FilteredInventoryIterator::findNext() {
    while (currentIndex < items.size() && items[currentIndex]->type != filterType) {
        currentIndex++;
    }
}

bool FilteredInventoryIterator::hasNext() const {
    return currentIndex < items.size();
}

InventoryItem* FilteredInventoryIterator::next() {
    if (hasNext()) {
        InventoryItem* item = items[currentIndex++];
        findNext();
        return item;
    }
    return nullptr;
}

void FilteredInventoryIterator::reset() {
    currentIndex = 0;
    findNext();
}

// ============================================
// SortedInventoryIterator Implementation
// ============================================
SortedInventoryIterator::SortedInventoryIterator(std::vector<InventoryItem*>& itemList)
    : currentIndex(0) {
    // Copy items
    sortedItems = itemList;
    
    // Sort by value (descending)
    std::sort(sortedItems.begin(), sortedItems.end(),
        [](const InventoryItem* a, const InventoryItem* b) {
            return a->value > b->value;
        });
}

bool SortedInventoryIterator::hasNext() const {
    return currentIndex < sortedItems.size();
}

InventoryItem* SortedInventoryIterator::next() {
    if (hasNext()) {
        return sortedItems[currentIndex++];
    }
    return nullptr;
}

void SortedInventoryIterator::reset() {
    currentIndex = 0;
}

// ============================================
// Inventory Implementation
// ============================================
Inventory::~Inventory() {
    for (auto* item : items) {
        delete item;
    }
}

void Inventory::addItem(const std::string& name, const std::string& type, int quantity, int value) {
    // Check if item already exists
    InventoryItem* existing = findItem(name);
    
    if (existing) {
        existing->quantity += quantity;
    } else {
        items.push_back(new InventoryItem(name, type, quantity, value));
    }
}

void Inventory::removeItem(const std::string& name) {
    auto it = std::find_if(items.begin(), items.end(),
        [&name](const InventoryItem* item) {
            return item->name == name;
        });
    
    if (it != items.end()) {
        delete *it;
        items.erase(it);
    }
}

InventoryItem* Inventory::findItem(const std::string& name) {
    auto it = std::find_if(items.begin(), items.end(),
        [&name](const InventoryItem* item) {
            return item->name == name;
        });
    
    return (it != items.end()) ? *it : nullptr;
}

Iterator* Inventory::createIterator() {
    return new InventoryIterator(items);
}

Iterator* Inventory::createFilteredIterator(const std::string& type) {
    return new FilteredInventoryIterator(items, type);
}

Iterator* Inventory::createSortedIterator() {
    return new SortedInventoryIterator(items);
}

int Inventory::getTotalValue() const {
    int total = 0;
    for (const auto* item : items) {
        total += item->quantity * item->value;
    }
    return total;
}