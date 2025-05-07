#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>     // для std::quoted
#include <stdexcept>

// Base User class
class User {
private:
    std::string name;
    int id;
    int accessLevel;

public:
    User(const std::string& name, int id, int accessLevel)
        : name(name), id(id), accessLevel(accessLevel) {
        if (name.empty()) throw std::invalid_argument("User name cannot be empty.");
        if (accessLevel < 0) throw std::invalid_argument("Access level cannot be negative.");
    }

    const std::string& getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    void setName(const std::string& newName) { name = newName; }
    void setId(int newId) { id = newId; }
    void setAccessLevel(int level) { accessLevel = level; }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", ID: " << id << ", Access Level: " << accessLevel;
    }

    virtual ~User() = default;
};

// Derived class Student
class Student : public User {
private:
    std::string group;

public:
    Student(const std::string& name, int id, int accessLevel, const std::string& group)
        : User(name, id, accessLevel), group(group) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Group: " << group;
    }

    const std::string& getGroup() const {
        return group;
    }
};

// Derived class Teacher
class Teacher : public User {
private:
    std::string department;

public:
    Teacher(const std::string& name, int id, int accessLevel, const std::string& department)
        : User(name, id, accessLevel), department(department) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Department: " << department;
    }

    const std::string& getDepartment() const {
        return department;
    }
};

// Derived class Administrator
class Administrator : public User {
private:
    std::string role;

public:
    Administrator(const std::string& name, int id, int accessLevel, const std::string& role)
        : User(name, id, accessLevel), role(role) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Role: " << role;
    }

    const std::string& getRole() const {
        return role;
    }
};

// Resource class
class Resource {
private:
    std::string name;
    int requiredAccessLevel;

public:
    Resource(const std::string& name, int requiredAccessLevel)
        : name(name), requiredAccessLevel(requiredAccessLevel) {
        if (name.empty()) throw std::invalid_argument("Resource name cannot be empty.");
    }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    void display() const {
        std::cout << "Resource: " << name << ", Required Access Level: " << requiredAccessLevel << "\n";
    }

    const std::string& getName() const { return name; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }
};

// Template AccessControlSystem class
template<typename T>
class AccessControlSystem {
private:
    std::vector<std::unique_ptr<User>> users;
    std::vector<std::unique_ptr<Resource>> resources;

public:
    void addUser(std::unique_ptr<User> user) {
        users.push_back(std::move(user));
    }

    void addResource(std::unique_ptr<Resource> resource) {
        resources.push_back(std::move(resource));
    }

    void checkAllAccess() const {
        for (const auto& user : users) {
            for (const auto& res : resources) {
                std::cout << user->getName() << " -> " << res->getName() << ": "
                    << (res->checkAccess(*user) ? "Access Granted" : "Access Denied") << "\n";
            }
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        for (const auto& user : users) {
            out << "USER " << user->getName() << " " << user->getId() << " " << user->getAccessLevel();
            if (dynamic_cast<const Student*>(user.get())) {
                out << " STUDENT " << dynamic_cast<const Student*>(user.get())->getGroup();
            }
            else if (dynamic_cast<const Teacher*>(user.get())) {
                out << " TEACHER " << dynamic_cast<const Teacher*>(user.get())->getDepartment();
            }
            else if (dynamic_cast<const Administrator*>(user.get())) {
                out << " ADMIN " << dynamic_cast<const Administrator*>(user.get())->getRole();
            }
            out << "\n";
        }

        for (const auto& res : resources) {
            out << "RESOURCE \"" << res->getName() << "\" " << res->getRequiredAccessLevel() << "\n";
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        std::string line;
        while (std::getline(in, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "USER") {
                std::string name; int id, level;
                iss >> name >> id >> level;
                std::string userType;
                iss >> userType;
                if (userType == "STUDENT") {
                    std::string group;
                    iss >> group;
                    users.push_back(std::make_unique<Student>(name, id, level, group));
                }
                else if (userType == "TEACHER") {
                    std::string dept;
                    iss >> dept;
                    users.push_back(std::make_unique<Teacher>(name, id, level, dept));
                }
                else if (userType == "ADMIN") {
                    std::string role;
                    iss >> role;
                    users.push_back(std::make_unique<Administrator>(name, id, level, role));
                }
                else {
                    users.push_back(std::make_unique<User>(name, id, level));
                }
            }
            else if (type == "RESOURCE") {
                std::string name;
                int reqLevel;

                iss >> std::skipws;
                iss >> std::quoted(name); // читаем строку в кавычках
                iss >> reqLevel;

                resources.push_back(std::make_unique<Resource>(name, reqLevel));
            }
        }
    }

    void displayAllUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
            std::cout << "\n";
        }
    }

    void displayAllResources() const {
        for (const auto& res : resources) {
            res->display();
        }
    }
};

int main() {
    try {
        AccessControlSystem<int> system;

        system.addUser(std::make_unique<Student>("Stas", 121, 1, "Group-121"));
        system.addUser(std::make_unique<Teacher>("Olga", 322, 3, "Physics"));
        system.addUser(std::make_unique<Administrator>("Alex", 303, 5, "IT"));

        system.addResource(std::make_unique<Resource>("Physics Lab", 3));
        system.addResource(std::make_unique<Resource>("Library", 1));

        system.saveToFile("university_data.txt");

        std::cout << "\n--- Checking Access Before Load ---\n";
        system.checkAllAccess();

        // Загрузка из файла
        AccessControlSystem<int> loadedSystem;
        loadedSystem.loadFromFile("university_data.txt");

        std::cout << "\n--- Loaded Users ---\n";
        loadedSystem.displayAllUsers();

        std::cout << "\n--- Loaded Resources ---\n";
        loadedSystem.displayAllResources();

        std::cout << "\n--- Checking Access After Load ---\n";
        loadedSystem.checkAllAccess();

    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << "\n";
    }

    return 0;
}