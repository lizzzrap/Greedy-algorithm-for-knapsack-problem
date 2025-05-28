#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>


using namespace std;


const string testPath = "./data/";
const string testFiles[] = {
    "ks_4_0",
    "ks_19_0",
    "ks_30_0",
    "ks_40_0",
    "ks_45_0",
    "ks_50_0",
    "ks_50_1",
    "ks_60_0",
    "ks_100_0",
    "ks_100_1",
    "ks_100_2",
    "ks_200_0",
    "ks_200_1",
    "ks_300_0",
    "ks_400_0",
    "ks_500_0",
    "ks_1000_0",
    "ks_10000_0",
};


struct Item {
    int value;  // ценность предмета
    int weight; // вес предмета
    double ratio; // удельная ценность (value/weight)
};


typedef bool CompareItemsFunc(Item, Item);


bool compareItemsByValue(Item a, Item b) {
    return a.value > b.value;
}

bool compareItemsByRatio(Item a, Item b) {
    return a.ratio > b.ratio;
}

double getKnapsackMaxValue(int capacity, vector<Item>& items, CompareItemsFunc* cmpFun) {

    sort(items.begin(), items.end(), cmpFun);

    double totalValue = 0.0;
    int currentWeight = 0;

    for (const auto& item : items) {
        if (currentWeight + item.weight <= capacity) {
            currentWeight += item.weight;
            totalValue += item.value;
        }
        else {
            int remainingWeight = capacity - currentWeight;
            totalValue += item.ratio * remainingWeight;
            break;
        }
    }

    return totalValue;
}


int main() {

    cout << setw(10) << "TestFile" << setw(30) << "Maximum value (opt by value)" << "\t" << "Maximum value (opt by ratio)" << endl;

    for (string testFile : testFiles)
    {
        cout << setw(10) << testFile;


        int n, capacity;
        vector<Item> items;

        ifstream in(testPath + testFile);
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

        for (auto& item : items) {
            item.ratio = (double)item.value / item.weight;
        }

        using chrono::high_resolution_clock;
        using chrono::duration_cast;
        using chrono::nanoseconds;

        auto t1 = high_resolution_clock::now();
        double maxValue = getKnapsackMaxValue(capacity, items, compareItemsByValue);
        auto t2 = high_resolution_clock::now();
        auto ns = duration_cast<nanoseconds>(t2 - t1);
        cout << setw(15) << maxValue << setw(15) << ns.count() << "ns";

        t1 = high_resolution_clock::now();
        maxValue = getKnapsackMaxValue(capacity, items, compareItemsByRatio);
        t2 = high_resolution_clock::now();
        ns = duration_cast<nanoseconds>(t2 - t1);
        cout << setw(15) << maxValue << setw(15) << ns.count() << "ns" << endl;
    }

    return 0;
}
