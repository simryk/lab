#include <iostream>
#include <string>

class Character {
private:
    std::string name;  // ��������� ����: ��� ���������
    int health;        // ��������� ����: ������� ��������
    int attack;        // ��������� ����: ������� �����
    int defense;       // ��������� ����: ������� ������
    const int maxHealth = health; // ������������ �������� ��������

public:
    // ����������� ��� ������������� ������
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // ����� ��� ��������� ������ ��������
    int getHealth() const {
        return health;
    }

    // ����� ��� ������ ���������� � ���������
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // ����� ��� ����� ������� ���������
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

    // ����� ��� ��������� ���������
    void heal(int amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth; // �� ��������� ������������ �������� ��������
        }
        std::cout << name << " heals for " << amount << " HP!" << std::endl;
    }

    // ����� ��� ��������� �����
    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0; 
        }
        std::cout << name << " takes " << amount << " damage!" << std::endl;
    }
};

int main() {
    // ������� ������� ����������
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // ������� ���������� � ����������
    hero.displayInfo();
    monster.displayInfo();

    // ����� ������� �������
    hero.attackEnemy(monster);
    monster.displayInfo();

    // ����� �������� ����
    hero.heal(20);
    hero.displayInfo();

    // ������ �������� ����
    monster.takeDamage(10);
    monster.displayInfo();

    // ��������� �������
    monster.heal(30);
    monster.displayInfo();

    return 0;
}