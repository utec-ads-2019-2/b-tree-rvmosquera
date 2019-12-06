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

    //Tester::testInsertion();

    const int order = 5;

    Mocker mocker;
    BTree<int> bTree(order);

    bTree.insert(6);
    bTree.insert(16);
    bTree.insert(1);
    bTree.insert(5);
    bTree.insert(10);
    bTree.insert(3);

    bTree.print();
    bTree.remove(1);
    bTree.remove(5);
    bTree.remove(16);
    bTree.remove(6);
    bTree.remove(6);
    bTree.remove(3);
    bTree.remove(10);
    bTree.print();
    bTree.insert(17);
    bTree.print();
    //bTree.remove(10);


    return EXIT_SUCCESS;
}