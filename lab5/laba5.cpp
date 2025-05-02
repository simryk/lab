#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T>
class Queue {
private:
    std::vector<T> items; 

public:
  
    void push(const T& item) {
        items.push_back(item);
    }


    void pop() {
        if (items.empty()) {
            throw std::out_of_range("Queue is empty, cannot pop.");
        }
        items.erase(items.begin());
    }


    T front() const {
        if (items.empty()) {
            throw std::out_of_range("Queue is empty, cannot access front.");
        }
        return items.front(); 
    }

    bool isEmpty() const {
        return items.empty();
    }

    size_t size() const {
        return items.size();
    }
};

int main() {
 
    Queue<std::string> stringQueue;
    stringQueue.push("One");
    stringQueue.push("Two");
    stringQueue.push("Three");

    std::cout << "String Queue:\n";
    while (!stringQueue.isEmpty()) {
        std::cout << stringQueue.front() << std::endl;
        stringQueue.pop();
    }

    // Пример использования очереди для целых чисел
    Queue<int> intQueue;
    intQueue.push(100);
    intQueue.push(200);
    intQueue.push(300);

    std::cout << "\nInteger Queue:\n";
    while (!intQueue.isEmpty()) {
        std::cout << intQueue.front() << std::endl;
        intQueue.pop();
    }

    return 0;
}