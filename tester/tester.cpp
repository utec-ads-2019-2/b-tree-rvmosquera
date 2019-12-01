#include "tester.h"

void Tester::testInsertion() {
    const int num_elements = 130;
    const int order = 5;

    Mocker mocker;
    BTree<int> bTree(order);
    vector<int> elements;

    for(int i = 0; i < num_elements; ++i) {
        auto randomInt = mocker.generateRandomInt(7);
        if(bTree.insert(randomInt))
            elements.push_back(randomInt);
    }

    sort(elements.begin(), elements.end());
    auto vectorSize = elements.size();

//    cout<<"Btree "<<endl;
//    bTree.print();
//    cout<<"Vector "<<endl;
    int ocurrences=0;

    for(auto e: elements) {
        if(bTree.find(e))
            ++ocurrences;
    }

    ASSERT(ocurrences == vectorSize, "There is a problem with insert method");
    cout << "Insertion test OK" <<endl;
}
