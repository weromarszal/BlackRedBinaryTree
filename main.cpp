#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <cmath>
#include <time.h>
#include <cstdlib>

using namespace std;

class Person {
public:
    string name;
    int age;

    Person() {}

    Person(int age, string name) {
        this->name = name;
        this->age = age;
    }
};

template<class T>
class Node {
public:
    T value;
    Node<T> *parent = NULL;
    Node<T> *leftKid = NULL;
    Node<T> *rightKid = NULL;
    bool isRed = true;
    int index; //?

    Node(T value) {
        this->value = value;
    }
};

template<class T>
class BlackRedTree {
    int elementCounter = 0;

public:
    Node<T> *root = NULL;

    ~BlackRedTree() {
        clear();
    }

    T *find(T data, function<bool(T, T)> compareElement, function<int(T, T)> compare) {
        Node<T> *tmp = root;
        while (!(compareElement(tmp->value, data)) && (tmp != NULL)) {
            if (compare(tmp->value, data) == -1) {
                tmp = tmp->leftKid;
            } else {
                tmp = tmp->rightKid;
            }
        }
        if (tmp == NULL) {
            return NULL;
        }
        return &tmp->value;
    }

    void preOrder(function<void(Node<T> *)> f) {
        preOrderRecursion(root, f);
    }

    void inOrder(function<void(Node<T> *)> f) {
        inOrderRecursion(root, f);
    }

    void postOrder(function<void(Node<T> *)> f) {
        postOrderRecursion(root, f);
    }

    void clear() {
        postOrder(clearElement);
    }

    int height() {
        return heightRecursion(root);
    }


//dodawanie bez rotacji i bez kolorow
    void addElement(T data, function<int(T, T)> compare) {
        Node<T> *node = new Node<T>(data);
        if (elementCounter == 0) {
            root = node;
            elementCounter++;
            coloring(node);
            return;
        }
        Node<T> *tmp = root;
        while (true) {
            int x = compare(data, tmp->value);
            if (x == 0 || x == -1) {
                if (tmp->leftKid == NULL) {
                    tmp->leftKid = node;
                    node->parent = tmp;
                    elementCounter++;
                    coloring(node);
                    return;
                } else {
                    tmp = tmp->leftKid;
                }
            } else if (tmp->rightKid == NULL) {
                tmp->rightKid = node;
                node->parent = tmp;
                elementCounter++;
                coloring(node);
                return;
            } else {
                tmp = tmp->rightKid;
            }
        }
    }


//    string toString() {
//
//    }
//
private:

    void coloringCase1(Node<T> *node) {
        Node<T> *father = node->parent;
        Node<T> *grandfather = father->parent;
        Node<T> *leftUncle = grandfather->leftKid;
        Node<T> *rightUncle = grandfather->rightKid;
        while (true) {
            if (node == root) {
                node->isRed = false;
                return;
            }
            if (father->isRed = false) {
                return;
            } else if (father == grandfather->leftKid) {
                if (rightUncle->isRed == true) {
                    father->isRed = false;
                    rightUncle->isRed = false;
                    grandfather->isRed = true;
                }
            } else if (father == grandfather->rightKid) {
                if (leftUncle->isRed == true) {
                    father->isRed = false;
                    leftUncle->isRed = false;
                    grandfather->isRed = true;
                }
            }
            node = grandfather;
        }
    }

    void coloringCase2(Node<T> *node) {
        Node<T> *father = node->parent;
        Node<T> *grandfather = father->parent;
        Node<T> *leftUncle = grandfather->leftKid;
        Node<T> *rightUncle = grandfather->rightKid;

        if (father == grandfather->leftKid) {
            if (rightUncle->isRed == false && father->rightKid == node) {
                rotationLeft(father);
                node = father;
            }
        } else if (father == grandfather->rightKid) {
            if (leftUncle->isRed == false && father->leftKid == node) {
                rotationRight(father);
                node = father;
            }
        }
    }

    void coloringCase3(Node<T> *node) {
        Node<T> *father = node->parent;
        Node<T> *grandfather = father->parent;
        Node<T> *leftUncle = grandfather->leftKid;
        Node<T> *rightUncle = grandfather->rightKid;

        if (father == grandfather->leftKid) {
            if (leftUncle->isRed == false && father->leftKid == node) {
                rotationRight(grandfather);
            }
            if (father == grandfather->rightKid) {
                if (rightUncle->isRed == false && father->rightKid == node) {
                    rotationLeft(grandfather);
                }
            }
            if (grandfather->isRed == true) {
                grandfather->isRed = false;
            } else {
                grandfather->isRed = true;
            }
            if (father->isRed == true) {
                father->isRed = false;
            } else {
                father->isRed = true;
            }
        }
    }

    void coloring(Node<T> *node) {
        coloringCase1(node);
        coloringCase2(node);
        coloringCase3(node);
    }

    void rotationRight(Node<T> *start) {
        Node<T> *startLeftChild = start->leftKid;
        Node<T> *startRightGrandChild = startLeftChild->rightKid;
        Node<T> **startPointer = getStartPointer(start);

        start->leftKid = startRightGrandChild;
        startLeftChild->rightKid = start;
        startLeftChild->parent = start->parent;
        start->parent = startLeftChild;
        *startPointer = startLeftChild;
        if (
                startRightGrandChild) {
            startRightGrandChild->parent = start;
        }
    }

    void rotationLeft(Node<T> *start) {
        Node<T> *startRightChild = start->rightKid;
        Node<T> *startLeftGrandChild = startRightChild->leftKid;
        Node<T> **startPointer = getStartPointer(start);

        start->rightKid = startLeftGrandChild;
        startRightChild->leftKid = start;
        startRightChild->parent = start->parent;
        start->parent = startRightChild;
        *startPointer = startRightChild;
        if (startLeftGrandChild) {
            startLeftGrandChild->parent = start;
        }
    }

/**
 * zwraca wskaznik na wskaznik na "punkt startowy rotacji"
 * "**" zeby mozna bylo zmienic np. roota
 */
    Node<T> **getStartPointer(Node<T> *start) {
        if (start == root) {
            return &root;
        } else if (start->parent->leftKid == start) {
            return &start->parent->leftKid;
        } else {
            return &start->parent->rightKid;
        }
    }

    void preOrderRecursion(Node<T> *node, function<void(Node<T> *)> f) {
        f(node);
        if (node->leftKid != NULL) {
            preOrderRecursion(node->leftKid, f);
        }
        if (node->rightKid != NULL) {
            preOrderRecursion(node->rightKid, f);
        }
    }

    void inOrderRecursion(Node<T> *node, function<void(Node<T> *)> f) {
        if (node->leftKid != NULL) {
            inOrderRecursion(node->leftKid, f);
        }
        f(node);
        if (node->rightKid != NULL) {
            inOrderRecursion(node->rightKid, f);
        }
    }

    void postOrderRecursion(Node<T> *node, function<void(Node<T> *)> f) {
        if (node->leftKid != NULL) {
            postOrderRecursion(node->leftKid, f);
        }
        if (node->rightKid != NULL) {
            postOrderRecursion(node->rightKid, f);
        }
        f(node);
    }

    static void clearElement(Node<T> *node) {
        delete node;
    }

    int heightRecursion(Node<T> *node) {
        int height = 0;
        if (node == NULL) {
            return height;
        }
        int leftHeight = heightRecursion(node->leftKid);
        int rightHeight = heightRecursion(node->rightKid);

        if (leftHeight >= rightHeight) {
            leftHeight++;
            return leftHeight;
        } else {
            rightHeight++;
            return rightHeight;
        }
    }

};

bool compareElement(Person person, Person pattern) {
    if (person.age == pattern.age) {
        return true;
    } else {
        return false;
    }
}

int compare(Person newPerson, Person elem) {
    if (newPerson.age == elem.age) {
        return 0;
    } else if (newPerson.age < elem.age) {
        return -1;
    } else {
        return 1;
    }
}

bool compareElementInt(int number, int pattern) {
    return number == pattern;
}

int compareInt(int number, int pattern) {
    if (number == pattern) {
        return 0;
    } else if (number < pattern) {
        return -1;
    } else {
        return 1;
    }
}

void print(Node<int> *x) {
    cout << x->value << endl;
}


int main() {

    BlackRedTree<int> brt;
//    brt.find(3, compareElementInt, compareInt);
    brt.addElement(5, compareInt);
    brt.addElement(3, compareInt);
    brt.addElement(4, compareInt);
    brt.addElement(6, compareInt);

    brt.postOrder(print);
    cout << endl;

    cout << brt.height();
    return 0;
}