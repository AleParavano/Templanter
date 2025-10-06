#ifndef INVENTORYITERATOR_H
#define INVENTORYITERATOR_H

#include <vector>
#include <string>
#include <utility>

// Item structure
struct InventoryItem {
    std::string name;
    std::string type;  // "plant", "seed", "nutrient"
    int quantity;
    int value;
    
    InventoryItem(const std::string& n, const std::string& t, int q, int v)
        : name(n), type(t), quantity(q), value(v) {}
};

// Iterator Pattern: Abstract iterator
class Iterator {
public:
    virtual ~Iterator() = default;
    
    virtual bool hasNext() const = 0;
    virtual InventoryItem* next() = 0;
    virtual void reset() = 0;
};

// Concrete Iterator: Sequential iterator
class InventoryIterator : public Iterator {
private:
    std::vector<InventoryItem*>& items;
    size_t currentIndex;
    
public:
    InventoryIterator(std::vector<InventoryItem*>& itemList);
    
    bool hasNext() const override;
    InventoryItem* next() override;
    void reset() override;
};

// Concrete Iterator: Filtered iterator (by type)
class FilteredInventoryIterator : public Iterator {
private:
    std::vector<InventoryItem*>& items;
    std::string filterType;
    size_t currentIndex;
    
    void findNext();
    
public:
    FilteredInventoryIterator(std::vector<InventoryItem*>& itemList, const std::string& type);
    
    bool hasNext() const override;
    InventoryItem* next() override;
    void reset() override;
};

// Concrete Iterator: Sorted iterator (by value)
class SortedInventoryIterator : public Iterator {
private:
    std::vector<InventoryItem*> sortedItems;
    size_t currentIndex;
    
public:
    SortedInventoryIterator(std::vector<InventoryItem*>& itemList);
    
    bool hasNext() const override;
    InventoryItem* next() override;
    void reset() override;
};

// Aggregate: Inventory collection
class Inventory {
private:
    std::vector<InventoryItem*> items;
    
public:
    ~Inventory();
    
    void addItem(const std::string& name, const std::string& type, int quantity, int value);
    void removeItem(const std::string& name);
    InventoryItem* findItem(const std::string& name);
    
    // Create different iterators
    Iterator* createIterator();
    Iterator* createFilteredIterator(const std::string& type);
    Iterator* createSortedIterator();
    
    int getTotalItems() const { return items.size(); }
    int getTotalValue() const;
    
    std::vector<InventoryItem*>& getItems() { return items; }
};

#endif // INVENTORYITERATOR_H