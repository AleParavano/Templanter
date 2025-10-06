#ifndef STORAGECOMPONENT_H
#define STORAGECOMPONENT_H

#include <string>
#include <vector>

// Composite Pattern: Abstract component for storage hierarchy
class StorageComponent {
protected:
    std::string name;
    int capacity;
    int currentLoad;
    
public:
    StorageComponent(const std::string& componentName, int cap)
        : name(componentName), capacity(cap), currentLoad(0) {}
    
    virtual ~StorageComponent() = default;
    
    // Composite operations
    virtual void add(StorageComponent* component) {}
    virtual void remove(StorageComponent* component) {}
    virtual StorageComponent* getChild(int index) { return nullptr; }
    
    // Storage operations
    virtual bool store(int amount) = 0;
    virtual bool retrieve(int amount) = 0;
    virtual int getAvailableSpace() const = 0;
    virtual int getCurrentLoad() const { return currentLoad; }
    virtual int getCapacity() const { return capacity; }
    
    // Display
    virtual void display(int depth = 0) const = 0;
    virtual bool isComposite() const = 0;
    
    std::string getName() const { return name; }
};

// Leaf: Individual storage slot
class StorageSlot : public StorageComponent {
public:
    StorageSlot(const std::string& slotName, int slotCapacity);
    
    bool store(int amount) override;
    bool retrieve(int amount) override;
    int getAvailableSpace() const override;
    void display(int depth = 0) const override;
    bool isComposite() const override { return false; }
};

// Composite: Storage container (can hold slots or other containers)
class StorageContainer : public StorageComponent {
private:
    std::vector<StorageComponent*> children;
    
public:
    StorageContainer(const std::string& containerName, int containerCapacity);
    ~StorageContainer();
    
    void add(StorageComponent* component) override;
    void remove(StorageComponent* component) override;
    StorageComponent* getChild(int index) override;
    int getChildCount() const { return children.size(); }
    
    bool store(int amount) override;
    bool retrieve(int amount) override;
    int getAvailableSpace() const override;
    void display(int depth = 0) const override;
    bool isComposite() const override { return true; }
};

#endif // STORAGECOMPONENT_H