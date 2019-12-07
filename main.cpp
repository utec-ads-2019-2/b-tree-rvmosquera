#include <iostream>
#include <tester/tester.h>

using namespace std;

int main(int argc, char *argv[]) {
    /*const int order = 5;

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
    bTree.print();*/
    //bTree.remove(10);

    Tester::testInsertion();
    Tester::testDeletion();

    return EXIT_SUCCESS;
}