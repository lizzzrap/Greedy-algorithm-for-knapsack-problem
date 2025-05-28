#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int value;  // ценность предмета
    int weight; // вес предмета
    double ratio; // удельная ценность (value/weight)
};

// Функция для сравнения предметов по удельной ценности (для сортировки по убыванию)
bool compareItems(Item a, Item b) {
    return a.ratio > b.ratio;
}

// Жадный алгоритм для задачи о рюкзаке
double getKnapsackMaxValue(int capacity, vector<Item>& items) {
    // Вычисляем удельную ценность для каждого предмета
    for (auto& item : items) {
        item.ratio = (double)item.value / item.weight;
    }

    // Сортируем предметы по убыванию удельной ценности
    sort(items.begin(), items.end(), compareItems);

    double totalValue = 0.0;
    int currentWeight = 0;

    for (const auto& item : items) {
        if (currentWeight + item.weight <= capacity) {
            // Берем предмет целиком
            currentWeight += item.weight;
            totalValue += item.value;
        }
        else {
            // Берем часть предмета (для дробной задачи)
            int remainingWeight = capacity - currentWeight;
            totalValue += item.ratio * remainingWeight;
            break;
        }
    }

    return totalValue;
}

int main() {
    int n, capacity;
    vector<Item> items;

    std::ifstream in("./data/ks_4_0"); // окрываем файл для чтения
    if (in.is_open()) {
        in >> n >> capacity;
        items.resize(n);
        for (int i = 0; i < n; ++i) {
            in >> items[i].value >> items[i].weight;
        }
        in.close();
    }
    else {
        std::cout << "File not found." << std::endl;
        return -1;
    }


    double maxValue = getKnapsackMaxValue(capacity, items);

    cout << "Maximum value of a full knapsack: " << maxValue << endl;

    return 0;
}