#include <iostream>
#include <string>

class Character {
private:
    std::string name;  // Приватное поле: имя персонажа
    int health;        // Приватное поле: уровень здоровья
    int attack;        // Приватное поле: уровень атаки
    int defense;       // Приватное поле: уровень защиты
    const int maxHealth = health; // Максимальное значение здоровья

public:
    // Конструктор для инициализации данных
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // Метод для получения уровня здоровья
    int getHealth() const {
        return health;
    }

    // Метод для вывода информации о персонаже
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Метод для атаки другого персонажа
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }

    // Метод для исцеления персонажа
    void heal(int amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth; // Не превышать максимальное значение здоровья
        }
        std::cout << name << " heals for " << amount << " HP!" << std::endl;
    }

    // Метод для получения урона
    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0; 
        }
        std::cout << name << " takes " << amount << " damage!" << std::endl;
    }
};

int main() {
    // Создаем объекты персонажей
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // Выводим информацию о персонажах
    hero.displayInfo();
    monster.displayInfo();

    // Герой атакует монстра
    hero.attackEnemy(monster);
    monster.displayInfo();

    // Герой исцеляет себя
    hero.heal(20);
    hero.displayInfo();

    // Монстр получает урон
    monster.takeDamage(10);
    monster.displayInfo();

    // Исцеление монстра
    monster.heal(30);
    monster.displayInfo();

    return 0;
}
