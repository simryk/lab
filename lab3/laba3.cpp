#include <iostream>
#include <string>

class Weapon {
private:
    std::string name;
    int damage;
    float weight;

public:
    // Конструктор
    Weapon(const std::string& n, int d, float w)
        : name(n), damage(d), weight(w) {}

    // Перегрузка оператора +
    Weapon operator+(const Weapon& other) const {
        return Weapon(name + " & " + other.name, damage + other.damage, weight + other.weight);
    }

    // Перегрузка оператора >
    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    // Метод для отображения информации об оружии
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
        os << "Weapon: " << weapon.name << ", Damage: " << weapon.damage
            << ", Weight: " << weapon.weight << " kg";
        return os;
    }
};

int main() {
    Weapon spear("Spear", 50, 3.5);
    Weapon axe("Axe", 35, 1.5);
    Weapon knife("knife", 65, 1.0);

    // Демонстрация перегрузки оператора +
    Weapon combinedWeapon = spear + axe;
    std::cout << combinedWeapon << std::endl; // Вывод нового оружия

    // Демонстрация перегрузки оператора >
    if (knife > axe) {
        std::cout << "Knife is stronger than axe" << std::endl;
    }
    else {
        std::cout << "Axe is stronger than or equal to Knife!" << std::endl;
    }

    return 0;
}
