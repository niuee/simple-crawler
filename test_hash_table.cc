// driver code to test the HashTable class

#include <string.h>
#include <assert.h>
#include <iostream>
#include <string>
#include "./HashTableTemplate.h"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define NC "\033[0m"

using std::cout;
using std::endl;

struct Student {
    string name;
    int grade;
};

Student students[] = {
    {"Rachael", 8 },
    {"Monica", 9},
    {"Phoebe", 10},
    {"Joey", 6},
    {"Ross", 8},
    {"Chandler", 7}
};

struct Vars {
    string varName;
    string value;
};

Vars vars[] = {
    {"google.com", "209.85.234.101"},
    {"facebook.com", "157.240.18.35"},
    {"amazon.com", "205.251.242.103"},
    {"purdue.edu", "128.210.7.200"},
    {"cs.purdue.edu", "128.10.25.250"}
};

void test1() {
    HashTable<int> h;

    bool e;
    e = h.insertItem("Rachael", 8);
    assert(!e);

    e = h.insertItem("Monica", 9);
    assert(!e);

    e = h.insertItem("Monica", 10);
    assert(e);

    cout  << GREEN << "Test 1 Passed" << NC << endl;
}

void test2() {
    HashTable<int> h;

    bool e;
    e = h.insertItem("Rachael", 8);
    assert(!e);

    e = h.insertItem("Monica", 9);
    assert(!e);

    e = h.insertItem("Monica", 10);
    assert(e);

    int grade;
    e = h.find("Rachael", &grade);
    assert(e);
    assert(grade == 8);

    e = h.find("Monica", &grade);
    assert(e);
    assert(grade == 10);

    cout << GREEN << "Test 2 Passed" << NC << endl;
}

void test3() {
    HashTable<int> h;

    for (unsigned int i = 0; i < sizeof(students)/sizeof(Student); i++) {
        bool e;
        e = h.insertItem(students[i].name, students[i].grade);
        assert(!e);
    }

    for (unsigned int i = 0; i < sizeof(students)/sizeof(Student); i++) {
        bool e;
        int grade;
        e = h.find(students[i].name, &grade);
        assert(e);
        assert(grade == students[i].grade);
    }

    int grade;
    bool e = h.find("John", &grade);
    assert(!e);

    e = h.removeElement("John");
    assert(!e);

    e = h.removeElement("Rachael");
    assert(e);

    e = h.find("Rachael", &grade);
    assert(!e);

    cout << GREEN << "Test 3 Passed" << NC << endl;
}

void test4() {
    HashTable<int> h;

    bool e;
    e = h.insertItem("Rachael", 8);
    assert(!e);

    e = h.insertItem("Monica", 9);
    assert(!e);

    e = h.insertItem("Monica", 10);
    assert(e);

    e = h.insertItem("Ploebe", 8);
    assert(!e);

    e = h.insertItem("Pmnebe", 7);
    assert(!e);

    int grade;
    e = h.find("Pmnebe", &grade);
    assert(e);

    e = h.removeElement("Ploebe");
    assert(e);

    HashTableIterator<int> iterator(h);

    int sum = 0;
    string key;
    while (iterator.next(key, grade)) {
        sum+=grade;
    }

    assert(sum == 25);

    cout << GREEN "Test 4 Passed" << NC << endl;
}

void test5() {
    HashTable<int> h;

    int sum = 0;
    for (unsigned int i = 0; i < sizeof(students)/sizeof(Student); i++) {
        bool e;
        e = h.insertItem(students[i].name, students[i].grade);
        assert(!e);
        sum+=students[i].grade;
    }
    //std::cout<<"Testing"<<endl;
    HashTableIterator<int> iterator(h);
    //cout<<"Testing 2"<<endl;
    int sum2 = 0;
    string key;
    int grade;
    while (iterator.next(key, grade)) {
        cout<<sum2<<endl;
        sum2+=grade;
    }

    assert(sum2 == sum);
    cout << GREEN << "Test 5 Passed" << NC << endl;
}

void test6() {
    HashTable<string> h;

    for (unsigned int i = 0; i < sizeof(vars)/sizeof(Vars); i++) {
        bool e;
        e = h.insertItem(vars[i].varName, vars[i].value);
        assert(!e);
    }

    for (unsigned int i = 0; i < sizeof(vars)/sizeof(Vars); i++) {
        bool e;
        string value;
        e = h.find(vars[i].varName, &value);
        assert(e);
        assert(!value.compare(vars[i].value));
    }

    for (unsigned int i = 0; i < sizeof(vars)/sizeof(Vars); i++) {
        string value;
        value = h[vars[i].varName];
        assert(!value.compare(vars[i].value));
    }

    string value;
    bool e = h.find("microsoft.com", &value);
    assert(!e);

    e = h.removeElement("github.com");
    assert(!e);

    e = h.removeElement("google.com");
    assert(e);

    e = h.find("cs.purdue.edu", &value);
    assert(e);

    cout << GREEN << "Test 6 Passed" << NC << endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "usage: test_hash_table <test_num>" << endl;
        return EXIT_FAILURE;
    }

    if (!strcmp(argv[1], "test1")) {
        test1();
    } else if (!strcmp(argv[1], "test2")) {
        test2();
    } else if (!strcmp(argv[1], "test3")) {
        test3();
    } else if (!strcmp(argv[1], "test4")) {
        test4();
    } else if (!strcmp(argv[1], "test5")) {
        test5();
    } else if (!strcmp(argv[1], "test6")) {
        test6();
    } else {
        cout << "invalid test!" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
