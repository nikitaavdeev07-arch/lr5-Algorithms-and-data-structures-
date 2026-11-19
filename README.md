---

```markdown
# Лабораторная работа №5
## Стек, очередь, дек, приоритетная очередь

**Вариант:** 1

---

## 1. Цель работы

1. Реализовать основные линейные структуры данных: стек, очередь, дек и приоритетную очередь.
2. Освоить принципы LIFO (стек) и FIFO (очередь).
3. Научиться реализовывать эти структуры на основе динамического массива и связного списка.
4. Изучить применение стека для решения задач (проверка скобок).
5. Реализовать приоритетную очередь на основе кучи (heap).

---

## 2. Теоретическая часть

### 2.1. Стек (Stack)

Стек — структура данных, работающая по принципу **LIFO** (Last In — First Out, «последним пришёл — первым ушёл»). Аналогия: стопка тарелок.

| Операция | Описание | Сложность |
|---|---|---|
| `push(value)` | Добавляет элемент на вершину | O(1) |
| `pop()` | Удаляет и возвращает элемент с вершины | O(1) |
| `top()` | Возвращает элемент с вершины без удаления | O(1) |
| `isEmpty()` | Проверяет, пуст ли стек | O(1) |
| `size()` | Возвращает количество элементов | O(1) |

### 2.2. Очередь (Queue)

Очередь — структура данных, работающая по принципу **FIFO** (First In — First Out, «первым пришёл — первым ушёл»). Аналогия: очередь в магазине.

| Операция | Описание | Сложность |
|---|---|---|
| `enqueue(value)` | Добавляет элемент в конец | O(1) |
| `dequeue()` | Удаляет и возвращает элемент из начала | O(1) |
| `front()` | Возвращает элемент из начала без удаления | O(1) |
| `isEmpty()` | Проверяет, пуста ли очередь | O(1) |
| `size()` | Возвращает количество элементов | O(1) |

### 2.3. Дек (Deque)

Дек — структура данных, позволяющая добавлять и удалять элементы с обоих концов.

| Операция | Описание | Сложность |
|---|---|---|
| `push_front(value)` | Добавляет элемент в начало | O(1) |
| `push_back(value)` | Добавляет элемент в конец | O(1) |
| `pop_front()` | Удаляет элемент из начала | O(1) |
| `pop_back()` | Удаляет элемент из конца | O(1) |
| `front()` | Возвращает первый элемент | O(1) |
| `back()` | Возвращает последний элемент | O(1) |

### 2.4. Приоритетная очередь (Priority Queue)

Приоритетная очередь — структура данных, в которой каждый элемент имеет приоритет; элемент с наибольшим приоритетом извлекается первым. Обычно реализуется на основе двоичной кучи (heap).

| Операция | Описание | Сложность |
|---|---|---|
| `push(value)` | Добавляет элемент | O(log n) |
| `pop()` | Удаляет элемент с наибольшим приоритетом | O(log n) |
| `top()` | Возвращает элемент с наибольшим приоритетом | O(1) |

---

## 3. Постановка задачи (вариант 1)

| Структура | Задание |
|---|---|
| Стек | Проверить правильность расстановки скобок в выражении (вида `[{}]`) |
| Очередь | Смоделировать очередь в банке (время обслуживания клиентов) |
| Дек | Проверить, является ли строка палиндромом (с использованием дека) |
| Приоритетная очередь | Найти K наибольших элементов в массиве |

---

## 4. Листинг программы

### 4.1. Стек — проверка расстановки скобок

```cpp
#include <iostream>
#include <string>
#include <stdexcept>

template <typename T>
class Stack {
private:
    T* data;
    int topIndex;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i <= topIndex; i++)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Stack(int initialCapacity = 10)
        : capacity(initialCapacity), topIndex(-1) {
        data = new T[capacity];
    }

    ~Stack() { delete[] data; }

    void push(const T& value) {
        if (topIndex >= capacity - 1) resize();
        data[++topIndex] = value;
    }

    T pop() {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        return data[topIndex--];
    }

    T top() const {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        return data[topIndex];
    }

    bool isEmpty() const { return topIndex == -1; }
    int size() const { return topIndex + 1; }
};

bool checkBrackets(const std::string& expr) {
    Stack<char> stack;
    for (char ch : expr) {
        if (ch == '(' || ch == '[' || ch == '{') {
            stack.push(ch);
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (stack.isEmpty()) return false;
            char open = stack.pop();
            if ((ch == ')' && open != '(') ||
                (ch == ']' && open != '[') ||
                (ch == '}' && open != '{'))
                return false;
        }
    }
    return stack.isEmpty();
}

int main() {
    std::string tests[] = {
        "{[()]}", "([{}])", "{[}]", "(]", "((()))", "([)]", ""
    };
    for (const auto& t : tests)
        std::cout << "\"" << t << "\" -> "
                  << (checkBrackets(t) ? "OK" : "ERROR") << "\n";
    return 0;
}
```

**Сложность:** `push` / `pop` / `top` — O(1) амортизированно; проверка строки — O(n).

### 4.2. Очередь — моделирование очереди в банке

```cpp
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
```

**Сложность:** `enqueue` / `dequeue` / `front` — O(1) амортизированно.

### 4.3. Дек — проверка строки на палиндром

```cpp
#include <iostream>
#include <string>
#include <stdexcept>

template <typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    Deque() : head(nullptr), tail(nullptr), count(0) {}

    ~Deque() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    T pop_front() {
        if (isEmpty()) throw std::underflow_error("Deque is empty");
        Node* temp = head;
        T value = temp->data;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        count--;
        return value;
    }

    T pop_back() {
        if (isEmpty()) throw std::underflow_error("Deque is empty");
        Node* temp = tail;
        T value = temp->data;
        tail = tail->prev;
        if (tail != nullptr) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        count--;
        return value;
    }

    T front() const {
        if (isEmpty()) throw std::underflow_error("Deque is empty");
        return head->data;
    }

    T back() const {
        if (isEmpty()) throw std::underflow_error("Deque is empty");
        return tail->data;
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

bool isPalindrome(const std::string& str) {
    Deque<char> dq;
    for (char ch : str) dq.push_back(ch);
    while (dq.size() > 1) {
        if (dq.pop_front() != dq.pop_back()) return false;
    }
    return true;
}

int main() {
    std::string tests[] = {"radar", "hello", "level", "abba", "a", ""};
    for (const auto& s : tests)
        std::cout << "\"" << s << "\": "
                  << (isPalindrome(s) ? "YES" : "NO") << "\n";
    return 0;
}
```

**Сложность:** все операции дека — O(1); проверка палиндрома — O(n).

### 4.4. Приоритетная очередь — K наибольших элементов

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

template <typename T>
class PriorityQueue {
private:
    T* heap;
    int count;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        T* newHeap = new T[newCapacity];
        for (int i = 0; i < count; i++) newHeap[i] = heap[i];
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] > heap[parent]) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            } else break;
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;
            if (left < count && heap[left] > heap[largest]) largest = left;
            if (right < count && heap[right] > heap[largest]) largest = right;
            if (largest == index) break;
            std::swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    PriorityQueue(int initialCapacity = 10)
        : capacity(initialCapacity), count(0) {
        heap = new T[capacity];
    }

    ~PriorityQueue() { delete[] heap; }

    void push(const T& value) {
        if (count >= capacity) resize();
        heap[count] = value;
        heapifyUp(count);
        count++;
    }

    T pop() {
        if (isEmpty()) throw std::underflow_error("PriorityQueue is empty");
        T maxValue = heap[0];
        heap[0] = heap[count - 1];
        count--;
        heapifyDown(0);
        return maxValue;
    }

    T top() const {
        if (isEmpty()) throw std::underflow_error("PriorityQueue is empty");
        return heap[0];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

std::vector<int> kLargest(const std::vector<int>& arr, int k) {
    PriorityQueue<int> pq;
    for (int x : arr) pq.push(x);
    std::vector<int> result;
    for (int i = 0; i < k && !pq.isEmpty(); i++)
        result.push_back(pq.pop());
    return result;
}

int main() {
    std::vector<int> arr = {3, 1, 5, 12, 2, 11, 7, 9, 8};
    int k = 4;
    auto res = kLargest(arr, k);
    std::cout << "Массив: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n" << k << " наибольших: ";
    for (int x : res) std::cout << x << " ";
    std::cout << "\n";
    return 0;
}
```

**Сложность:** `push` / `pop` — O(log n), `top` — O(1). Поиск K наибольших — O(n log n + K log n).

---

## 5. Результаты работы

### 5.1. Стек — проверка скобок

```
"{[()]}" -> OK
"([{}])" -> OK
"{[}]" -> ERROR
"(]" -> ERROR
"((()))" -> OK
"([)]" -> ERROR
"" -> OK
```

### 5.2. Очередь — банк

```
Клиент (приход=0, обслуживание=5): ждал 0 мин, касса освободится в 5
Клиент (приход=1, обслуживание=3): ждал 4 мин, касса освободится в 8
Клиент (приход=2, обслуживание=4): ждал 6 мин, касса освободится в 12
Клиент (приход=10, обслуживание=2): ждал 2 мин, касса освободится в 14
Клиент (приход=11, обслуживание=6): ждал 3 мин, касса освободится в 20
Среднее время ожидания: 3 мин
```

### 5.3. Дек — палиндром

```
"radar": YES
"hello": NO
"level": YES
"abba": YES
"a": YES
"": YES
```

### 5.4. Приоритетная очередь — K наибольших

```
Массив: 3 1 5 12 2 11 7 9 8
4 наибольших: 12 11 9 8
```

---

## 6. Анализ сложности

| Структура | Операция | Сложность |
|---|---|---|
| Стек | `push` / `pop` / `top` | O(1) амортизированно |
| Очередь | `enqueue` / `dequeue` / `front` | O(1) амортизированно |
| Дек | `push_front` / `push_back` / `pop_front` / `pop_back` | O(1) |
| Приоритетная очередь | `push` / `pop` | O(log n) |
| Приоритетная очередь | `top` | O(1) |

---

## 7. Ответы на контрольные вопросы

1. **Принцип работы стека (LIFO).** Последний добавленный элемент извлекается первым. Добавление и удаление выполняются только с одного конца — вершины.

2. **Принцип работы очереди (FIFO).** Первый добавленный элемент извлекается первым. Добавление — в конец, удаление — из начала.

3. **Чем дек отличается от обычной очереди.** Дек позволяет добавлять и удалять элементы с обоих концов, а очередь — только с одного конца добавлять и с другого удалять.

4. **Приоритетная очередь и её применение.** Структура, где каждый элемент имеет приоритет, и первым извлекается элемент с наибольшим (или наименьшим) приоритетом. Применяется в планировщиках задач, алгоритмах Дейкстры и Прима, обработке событий, сжатии Хаффмана.

5. **Стек на динамическом массиве.** Храним массив, индекс вершины и ёмкость; при переполнении удваиваем ёмкость и копируем элементы; `push` пишет в `data[++top]`, `pop` читает `data[top--]`.

6. **Очередь на кольцевом буфере.** Храним массив фиксированной ёмкости, индексы `front`, `rear` и счётчик `count`; `enqueue` пишет в `data[rear]`, затем `rear = (rear + 1) % capacity`; `dequeue` читает `data[front]`, затем `front = (front + 1) % capacity`.

7. **Почему приоритетную очередь реализуют на куче.** Куча обеспечивает `push` и `pop` за O(log n) и `top` за O(1), при этом она компактна (массив) и проста в реализации. Другие структуры дают худшую сложность хотя бы по одной операции.

8. **Сложность push и pop в стеке.** O(1) амортизированно (resize при переполнении — O(n), но происходит редко).

9. **Сложность enqueue и dequeue в очереди.** O(1) амортизированно (resize при переполнении кольцевого буфера — O(n), но редко).

10. **Сложность операций в приоритетной очереди.** `push` — O(log n), `pop` — O(log n), `top` — O(1). Обусловлено высотой двоичной кучи (log n) при просеивании вверх/вниз.

11. **Проверка скобок стеком.** Открывающие скобки кладём в стек; при закрывающей проверяем, что вершина — парная открывающая, и снимаем её. В конце стек должен быть пуст.

12. **Очередь на двух стеках.** `enqueue` — `push` в первый стек (O(1)); `dequeue` — если второй стек пуст, перекладываем в него все элементы из первого (O(n)), затем `pop` из второго (O(1) амортизированно).

13. **Дек на двусвязном списке.** Каждый узел хранит `prev` и `next`; `push_front`/`push_back` и `pop_front`/`pop_back` выполняются за O(1) перестановкой указателей `head`/`tail`.

14. **Дек на кольцевом буфере.** Массив фиксированной ёмкости, индексы `front` и `rear`; добавление/удаление с обоих концов — сдвиг индексов по модулю ёмкости, O(1).

15. **Стек с getMin() за O(1).** Храним второй стек минимумов: при `push` кладём `min(value, currentMin)`; при `pop` снимаем с обоих стеков. `getMin()` — вершина стека минимумов.

---

## 8. Выводы

В ходе лабораторной работы реализованы четыре линейные структуры данных: стек (на динамическом массиве), очередь (на кольцевом буфере), дек (на двусвязном списке) и приоритетная очередь (на двоичной max-куче). Решены задачи варианта 1: проверка правильности расстановки скобок, моделирование очереди в банке, проверка строки на палиндром с помощью дека, поиск K наибольших элементов массива. Все операции имеют заявленную сложность; предусмотрена обработка ошибок при извлечении из пустой структуры (`std::underflow_error`). Полученные навыки применимы при реализации планировщиков, парсеров, алгоритмов на графах и обработки потоков данных.

---

## 9. Ссылка на Git-репозиторий

```
https://github.com/nikitaavdeev07-arch/lr5-Algorithms-and-data-structures-
```



