#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <iostream>
#include <atomic>

class Monster {
public:
    std::string name;
    int health;
    int attack;
    int defense;

    Monster(std::string n, int h, int a, int d) : name(n), health(h), attack(a), defense(d) {}

    void displayInfo() {
        std::cout << "Monster: " << name << ", Health: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Character {
public:
    std::string name;
    int health;
    int attack;
    int defense;

    Character(std::string n, int h, int a, int d) : name(n), health(h), attack(a), defense(d) {}

    void displayInfo() {
        std::cout << "Character: " << name << ", Health: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

std::vector<Monster> monsters;
std::mutex monstersMutex;
std::atomic<bool> heroAlive(true); 

void generateMonsters() {
    while (heroAlive) { 
        std::this_thread::sleep_for(std::chrono::seconds(3)); 
        std::lock_guard<std::mutex> lock(monstersMutex);
        monsters.push_back(Monster("Goblin", 50, 40, 5));
        std::cout << "New monster generated!\n";
    }
}

void battle(Character& hero, Monster& monster) {
    while (hero.health > 0 && monster.health > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1)); 

        monster.health -= std::max(0, hero.attack - monster.defense);
        std::cout << hero.name << " attacks " << monster.name << "!\n";
        std::cout << monster.name << " has " << monster.health << " health left.\n";

        if (monster.health <= 0) {
            std::cout << monster.name << " has been defeated!\n";
            break; 
        }

        hero.health -= std::max(0, monster.attack - hero.defense);
        std::cout << monster.name << " attacks " << hero.name << "!\n";
        std::cout << hero.name << " has " << hero.health << " health left.\n";
    }

    if (hero.health <= 0) {
        std::cout << hero.name << " has been defeated!\n";
        heroAlive = false; 
    }
}

int main() {
    std::thread monsterGenerator(generateMonsters);
    monsterGenerator.detach();

    Character hero("Hero", 100, 50, 10);

    while (heroAlive) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lock(monstersMutex);
        if (!monsters.empty()) {
            Monster& monster = monsters.front(); 
            battle(hero, monster); 
            monsters.erase(monsters.begin()); 
        }
        for (auto& monster : monsters) {
            monster.displayInfo();
        }
    }

    std::cout << "No more monsters will be generated.\n";
    return 0;
}