#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <cstdlib>

// Шаблонный логгер для записи действий
template<typename T>
class Logger {
    std::ofstream logFile;
public:
    Logger(const std::string& filename) : logFile(filename) {}
    ~Logger() { if (logFile.is_open()) logFile.close(); }
    void log(const T& message) {
        std::cout << message << std::endl;
        if (logFile.is_open())
            logFile << message << std::endl;
        logFile.flush();
    }
};

// Класс Inventory — управление инвентарём
class Inventory {
    std::vector<std::unique_ptr<std::string>> items;
public:
    void addItem(const std::string& item) {
        items.push_back(std::make_unique<std::string>(item));
        std::cout << "Equipped " << item << "." << std::endl;
    }

    void displayInventory() const {
        std::cout << "\n🎒 Inventory:" << std::endl;
        for (const auto& item : items) {
            std::cout << "- " << *item << std::endl;
        }
    }

    bool usePotion() {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (**it == "Health Potion") {
                items.erase(it);
                return true;
            }
        }
        return false;
    }

    bool hasPotion() const {
        for (const auto& item : items) {
            if (*item == "Health Potion") return true;
        }
        return false;
    }

    bool hasSword() const {
        for (const auto& item : items) {
            if (*item == "Sword") return true;
        }
        return false;
    }

    bool hasAxe() const {
        for (const auto& item : items) {
            if (*item == "Battle Axe") return true;
        }
        return false;
    }

    int getAttackBonus() const {
        if (hasAxe()) return 10;
        if (hasSword()) return 5;
        return 0;
    }

    const std::vector<std::unique_ptr<std::string>>& getItems() const {
        return items;
    }

    bool isEmpty() const {
        return items.empty();
    }

    void clear() {
        items.clear();
    }
};

// Базовый класс Character
class Character {
protected:
    std::string name;
    int health;
    int attack;
    int defense;
    int level;
    int experience;
    int maxHealth;

public:
    static Logger<std::string> logger; // Публичный статический логгер

    Character(const std::string& n = "Hero", int h = 100, int a = 20, int d = 10)
        : name(n), health(h), attack(a), defense(d), level(1), experience(0), maxHealth(h) {
    }

    Character(const Character& other)
        : name(other.name), health(other.health), attack(other.attack),
        defense(other.defense), level(other.level), experience(other.experience),
        maxHealth(other.maxHealth) {
    }

    Character& operator=(const Character& other) {
        if (this != &other) {
            name = other.name;
            health = other.health;
            attack = other.attack;
            defense = other.defense;
            level = other.level;
            experience = other.experience;
            maxHealth = other.maxHealth;
        }
        return *this;
    }

    virtual void attackEnemy(Character& enemy) {
        int weaponBonus = inventory ? inventory->getAttackBonus() : 0;
        int totalAttack = attack + weaponBonus;
        int damage = totalAttack - enemy.getDefense();

        if (damage > 0) {
            enemy.takeDamage(damage);
            Character::logger.log(name + " attacks " + enemy.getName() + " for " + std::to_string(damage) + " damage!");
        }
        else {
            Character::logger.log(name + " attacks " + enemy.getName() + ", but it has no effect.");
        }
    }

    virtual void takeDamage(int damage) {
        if (damage < 0) damage = 0;
        health -= damage;
        if (health <= 0) {
            health = 0;
            Character::logger.log(name + " has died!");
            std::cout << name << " has died!" << std::endl;
            return;
        }

        if (inventory && inventory->hasPotion()) {
            char choice;
            std::cout << "Use Health Potion? (y/n): ";
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                inventory->usePotion();
                heal(30);
            }
        }
    }

    virtual void heal(int amount) {
        if (health <= 0) throw std::runtime_error("Cannot heal a dead character.");
        health += amount;
        if (health > maxHealth) health = maxHealth;
        Character::logger.log(name + " heals for " + std::to_string(amount) + " HP.");
        std::cout << name << " heals for " << amount << " HP!" << std::endl;
    }

    virtual void gainExperience(int exp) {
        experience += exp;
        while (experience >= 100) {
            level++;
            experience -= 100;
            attack += 10;
            defense += 5;
            health += 50;
            maxHealth += 50;
            Character::logger.log(name + " leveled up to level " + std::to_string(level) + "!");
            std::cout << name << " leveled up to level " << level << "!" << std::endl;
        }
    }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense
            << ", Level: " << level << ", Experience: " << experience << std::endl;
    }

    bool isAlive() const { return health > 0; }
    virtual std::string getName() const { return name; }
    virtual int getHealth() const { return health; }
    virtual int getAttack() const { return attack; }
    virtual int getDefense() const { return defense; }
    virtual int getLevel() const { return level; }
    virtual int getExperience() const { return experience; }
    virtual ~Character() = default;

    void setInventory(Inventory* inv) { inventory = inv; }
    void setLevel(int l) { level = l; }
    void setExperience(int e) { experience = e; }
    void setHealth(int h) { health = h; }
    void setAttack(int a) { attack = a; }
    void setDefense(int d) { defense = d; }

private:
    Inventory* inventory = nullptr;
};

Logger<std::string> Character::logger{ "game_log.txt" };

// Класс Monster — наследуется от Character
class Monster : public Character {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Character(n, h, a, d) {
    }

    virtual void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    virtual std::string getType() const = 0;
    virtual std::unique_ptr<Monster> clone() const = 0;
};

// Конкретные типы монстров
class Goblin : public Monster {
public:
    Goblin() : Monster("Goblin", 45, 12, 6) {}
    Goblin(int h, int a, int d) : Monster("Goblin", h, a, d) {}

    void attackEnemy(Character& enemy) override {
        if (rand() % 100 < 20) {
            Character::logger.log("🛡️ Goblin dodges the attack!");
            std::cout << "🛡️ Goblin dodges the attack!" << std::endl;
            return;
        }

        int damage = getAttack() - enemy.getDefense();
        if (damage > 0) {
            enemy.takeDamage(damage);
            Character::logger.log(name + " attacks " + enemy.getName() + " for " + std::to_string(damage) + " damage!");
        }
        else {
            Character::logger.log(name + " attacks " + enemy.getName() + ", but it has no effect.");
        }
    }

    std::string getType() const override { return "Goblin"; }
    std::unique_ptr<Monster> clone() const override {
        return std::make_unique<Goblin>(health, attack, defense);
    }
};

class Dragon : public Monster {
public:
    Dragon() : Monster("Dragon", 150, 30, 15) {}
    Dragon(int h, int a, int d) : Monster("Dragon", h, a, d) {}

    void attackEnemy(Character& enemy) override {
        int damage = getAttack() - enemy.getDefense();
        if (rand() % 100 < 30) {
            damage += 10;
            Character::logger.log("🔥 Fire Breath!");
        }

        if (damage > 0) {
            enemy.takeDamage(damage);
            Character::logger.log(name + " attacks " + enemy.getName() + " for " + std::to_string(damage) + " damage!");
        }
        else {
            Character::logger.log(name + " attacks " + enemy.getName() + ", but it has no effect.");
        }
    }

    std::string getType() const override { return "Dragon"; }
    std::unique_ptr<Monster> clone() const override {
        return std::make_unique<Dragon>(health, attack, defense);
    }
};

class Skeleton : public Monster {
public:
    Skeleton() : Monster("Skeleton", 60, 15, 7) {}
    Skeleton(int h, int a, int d) : Monster("Skeleton", h, a, d) {}

    void attackEnemy(Character& enemy) override {
        int damage = getAttack() - enemy.getDefense();
        if (rand() % 100 < 20) {
            damage += 5;
            Character::logger.log("💀 Bone Breaker!");
        }

        if (damage > 0) {
            enemy.takeDamage(damage);
            Character::logger.log(name + " attacks " + enemy.getName() + " for " + std::to_string(damage) + " damage!");
        }
        else {
            Character::logger.log(name + " attacks " + enemy.getName() + ", but it has no effect.");
        }
    }

    std::string getType() const override { return "Skeleton"; }
    std::unique_ptr<Monster> clone() const override {
        return std::make_unique<Skeleton>(health, attack, defense);
    }

    bool dropsAxe() const {
        Character::logger.log("Skeleton dropped Battle Axe!");
        return rand() % 100 < 70;
    }
};

// Сохранение и загрузка
class GameSaverLoader {
public:
    static void saveGame(const Character& player, const Inventory& inventory,
        const std::vector<std::unique_ptr<Monster>>& monsters,
        const std::string& filename) {
        std::ofstream file(filename);
        if (!file) throw std::runtime_error("Could not open file for saving.");

        // Сохраняем героя
        file << player.getName() << "\n"
            << player.getHealth() << "\n"
            << player.getAttack() << "\n"
            << player.getDefense() << "\n"
            << player.getLevel() << "\n"
            << player.getExperience() << "\n";

        // Сохраняем инвентарь
        const auto& items = inventory.getItems();
        file << items.size() << "\n";
        for (const auto& item : items) {
            file << *item << "\n";
        }

        // Сохраняем монстров
        file << monsters.size() << "\n";
        for (const auto& monster : monsters) {
            file << monster->getType() << "\n"
                << monster->getHealth() << "\n"
                << monster->getAttack() << "\n"
                << monster->getDefense() << "\n";
        }

        std::cout << "💾 Game saved.\n";
    }

    static Character loadGame(Inventory& inventory,
        std::vector<std::unique_ptr<Monster>>& monsters,
        const std::string& filename) {
        std::ifstream file(filename);
        if (!file) throw std::runtime_error("Could not open file for loading.");

        // Загружаем героя
        std::string name;
        int hp, atk, def, level, exp;

        std::getline(file, name);
        file >> hp >> atk >> def >> level >> exp;

        // Очищаем инвентарь перед загрузкой
        inventory.clear();

        // Загружаем инвентарь
        size_t itemCount;
        file >> std::ws >> itemCount;
        for (size_t i = 0; i < itemCount; ++i) {
            std::string item;
            std::getline(file >> std::ws, item);
            inventory.addItem(item);
        }

        // Загружаем монстров
        size_t monsterCount;
        file >> std::ws >> monsterCount;
        for (size_t i = 0; i < monsterCount; ++i) {
            std::string type;
            int m_hp, m_atk, m_def;

            std::getline(file >> std::ws, type);
            file >> m_hp >> m_atk >> m_def;

            std::unique_ptr<Monster> newMonster;
            if (type == "Goblin") newMonster = std::make_unique<Goblin>(m_hp, m_atk, m_def);
            else if (type == "Dragon") newMonster = std::make_unique<Dragon>(m_hp, m_atk, m_def);
            else if (type == "Skeleton") newMonster = std::make_unique<Skeleton>(m_hp, m_atk, m_def);
            else continue;

            if (newMonster->isAlive()) {
                monsters.push_back(std::move(newMonster));
            }
        }

        Character loaded(name, hp, atk, def);
        loaded.setLevel(level);
        loaded.setExperience(exp);
        loaded.setInventory(&inventory);

        return loaded;
    }
};

// Класс Game — управление игрой
class Game {
public:
    Inventory inventory;
    Character hero;
    std::vector<std::unique_ptr<Monster>> monsters;

    Game() : hero("Hero", 100, 20, 10) {
        srand(static_cast<unsigned>(time(nullptr)));
        hero.setInventory(&inventory);
        monsters.push_back(std::make_unique<Goblin>());
        monsters.push_back(std::make_unique<Dragon>());
        monsters.push_back(std::make_unique<Skeleton>());
        inventory.addItem("Sword");
    }

    void start() {
        while (hero.isAlive() && !monsters.empty()) {
            std::cout << "\n--- Hero Stats ---\n";
            hero.displayInfo();
            std::cout << "--------------------\n";

            std::cout << "\n=== Main Menu ===\n";
            std::cout << "1. Fight a monster\n";
            std::cout << "2. Inventory\n";
            std::cout << "0. Save and exit\n";

            int choice;
            std::cin >> choice;

            if (choice == 0) break;

            if (choice == 1) {
                std::cout << "\nChoose monster to fight:\n";
                for (size_t i = 0; i < monsters.size(); ++i) {
                    std::cout << i + 1 << ". ";
                    monsters[i]->displayInfo();
                }

                int targetChoice;
                std::cin >> targetChoice;

                if (targetChoice < 1 || targetChoice > monsters.size()) {
                    std::cout << "❌ Invalid choice.\n";
                    continue;
                }

                Monster& target = *monsters[targetChoice - 1];
                battle(hero, target);

                if (!target.isAlive()) {
                    Character::logger.log("✅ Defeated " + target.getType());
                    hero.gainExperience(50);

                    if (rand() % 100 < 30) {
                        inventory.addItem("Health Potion");
                        Character::logger.log("🧪 Health Potion added to inventory.");
                    }

                    if (target.getType() == "Skeleton") {
                        Skeleton& skel = dynamic_cast<Skeleton&>(target);
                        if (skel.dropsAxe()) {
                            inventory.addItem("Battle Axe");
                            Character::logger.log("🗡️ Battle Axe added to inventory.");
                        }
                    }

                    monsters.erase(monsters.begin() + targetChoice - 1);
                }
            }
            else if (choice == 2) {
                useItem(hero);
            }
            else {
                std::cout << "❌ Invalid menu choice.\n";
            }
        }

        if (hero.isAlive() && monsters.empty()) {
            std::cout << "🏆 You defeated all enemies!\n";
        }
        else if (!hero.isAlive()) {
            std::cout << "☠️ Game over...\n";
        }
    }

    void resumeGame(const Character& loadedHero,
        const std::vector<std::unique_ptr<Monster>>& loadedMonsters) {
        hero = loadedHero;
        monsters.clear();

        // Клонируем монстров через их интерфейс clone()
        for (const auto& m : loadedMonsters) {
            if (m->isAlive()) {
                monsters.push_back(m->clone());
            }
        }

        std::cout << "✅ Game loaded!\n";
        start();
    }

    void battle(Character& hero, Monster& enemy) {
        Character::logger.log("⚔️ Battle started between " + hero.getName() + " and " + enemy.getName() + "!");

        while (hero.isAlive() && enemy.isAlive()) {
            hero.attackEnemy(enemy);
            if (enemy.isAlive()) enemy.attackEnemy(hero);
        }
    }

    void useItem(Character& hero) {
        inventory.displayInventory();
        std::cout << "\nChoose an action:\n";
        std::cout << "1. Use Health Potion\n";
        std::cout << "0. Go back\n";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            if (inventory.hasPotion()) {
                inventory.usePotion();
                hero.heal(30);
                Character::logger.log(hero.getName() + " used Health Potion.");
            }
            else {
                std::cout << "❌ No potions available.\n";
            }
        }
        else if (choice == 0) {
            std::cout << "↩️ Returning to main menu...\n";
        }
        else {
            std::cout << "❌ Invalid choice.\n";
        }
    }
};

// Основная функция
int main() {
    std::cout << "1. New Game\n2. Load Game\nChoose: ";
    int choice;
    std::cin >> choice;

    Game game;

    if (choice == 2) {
        try {
            std::vector<std::unique_ptr<Monster>> loadedMonsters;
            Character loadedHero = GameSaverLoader::loadGame(game.inventory, loadedMonsters, "save.dat");
            game.resumeGame(loadedHero, loadedMonsters);
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading game: " << e.what() << std::endl;
            return 1;
        }
    }
    else {
        game.start();
        GameSaverLoader::saveGame(game.hero, game.inventory, game.monsters, "save.dat");
    }

    return 0;
}