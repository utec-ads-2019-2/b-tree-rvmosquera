#include "tester.h"

void Tester::testInsertion() {
    const int num_elements = 130;
    const int order = 5;

    Mocker mocker;
    auto bTree = new BTree<int>(order);
    vector<int> elements;

    for(int i = 0; i < num_elements; ++i) {
        auto randomInt = mocker.generateRandomInt(7);
        if(bTree->insert(randomInt))
            elements.push_back(randomInt);
    }

    sort(elements.begin(), elements.end());
    auto vectorSize = elements.size();

    int ocurrences=0;

    for(auto e: elements) {
        if(bTree->find(e))
            ++ocurrences;
    }

    ASSERT(ocurrences == vectorSize, "There is a problem with insert method");
    cout << "Insertion test OK" <<endl;

    delete bTree;
}

void Tester::testDeletion() {
    const int num_elements = 8;
    const int order = 5;

    Mocker mocker;
    BTree<int> bTree(order);
    vector<int> elements;

    for(int i = 0; i < num_elements; ++i) {
        auto randomInt = mocker.generateRandomInt(7);
        if(bTree.insert(randomInt))
            elements.push_back(randomInt);
    }

    //sort(elements.begin(), elements.end());
    auto vectorSize = elements.size();

    int ocurrencesDeleted=0;

    for(auto e: elements) {
        bTree.remove(e);
        if(!bTree.find(e))
            ++ocurrencesDeleted;
    }

    ASSERT(ocurrencesDeleted == vectorSize, "There is a problem with delete method");
    cout << "Deletion test OK" <<endl;

}