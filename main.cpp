#include <iostream>
#include <tester/tester.h>

using namespace std;

int main(int argc, char *argv[]) {
    /*Mocker mocker;
    const int num_elements = 70;
    const int order = 5;
    BTree<int> bTree(order);
    vector<int> elements;

    for(int i = 0; i < num_elements; ++i) {
        auto randomInt = mocker.generateRandomInt(7);
        if(bTree.insert(randomInt))
            elements.push_back(randomInt);
    }

    sort(elements.begin(), elements.end());
    cout<<"Btree "<<endl;
    bTree.print();
    cout<<"Vector "<<endl;
    for(auto e: elements)
        cout << " " << e;*/

    Tester::testInsertion();

    return EXIT_SUCCESS;
}