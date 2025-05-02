#include <iostream>
#include <memory>
#include <vector>

class Inventory {
private:
    std::vector<std::unique_ptr<std::string>> items; 

public:
    void addItem(const std::string& item) {
        items.push_back(std::make_unique<std::string>(item));
    }

    // Метод для отображения инвентаря
    void displayInventory() const {
        std::cout << "Inventory:\n";
        for (const auto& item : items) {
            std::cout << "- " << *item << std::endl;
        }
    }
};

int main() {
    Inventory inventory;

    // Добавляем предметы в инвентарь
    inventory.addItem("Spear");
    inventory.addItem("Health Potion");
    inventory.addItem("Axe");

    // Отображаем инвентарь
    inventory.displayInventory();

    return 0;
}