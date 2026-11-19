// Project2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <stdexcept>
#include <vector>

template <typename T>
class Queue {
private:
    T* data;
    int capacity;
    int frontIndex, rearIndex, count;

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < count; i++)
            newData[i] = data[(frontIndex + i) % capacity];
        delete[] data;
        data = newData;
        frontIndex = 0;
        rearIndex = count;
        capacity = newCapacity;
    }

public:
    Queue(int initialCapacity = 10)
        : capacity(initialCapacity), frontIndex(0), rearIndex(0), count(0) {
        data = new T[capacity];
    }

    ~Queue() { delete[] data; }

    void enqueue(const T& value) {
        if (count >= capacity) resize();
        data[rearIndex] = value;
        rearIndex = (rearIndex + 1) % capacity;
        count++;
    }

    T dequeue() {
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        T value = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;
        return value;
    }

    T front() const {
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        return data[frontIndex];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

struct Client {
    int arrivalTime;   // время прихода
    int serviceTime;   // время обслуживания
};

int main() {
    setlocale(LC_ALL, "RU_ru");
    // Клиенты: (приход, обслуживание)
    std::vector<Client> clients = {
        {0, 5}, {1, 3}, {2, 4}, {10, 2}, {11, 6}
    };

    Queue<Client> q;
    int currentTime = 0;
    int totalWait = 0;
    int served = 0;

    for (const auto& c : clients) q.enqueue(c);

    while (!q.isEmpty()) {
        Client c = q.dequeue();
        // Касса начинает обслуживание не раньше прихода клиента
        int startTime = (currentTime > c.arrivalTime) ? currentTime : c.arrivalTime;
        int wait = startTime - c.arrivalTime;
        totalWait += wait;
        served++;
        currentTime = startTime + c.serviceTime;

        std::cout << "Клиент (приход=" << c.arrivalTime
            << ", обслуживание=" << c.serviceTime << "): ждал "
            << wait << " мин, касса освободится в " << currentTime << "\n";
    }

    std::cout << "Среднее время ожидания: "
        << (served ? double(totalWait) / served : 0) << " мин\n";
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
