// Name        : Project 2 Course Planner.cpp
// Author      : Melanie Probst

#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

// define a structure to hold course information
struct Course {
    string courseNumber; // unique identifier
    string name;
    string prerequisite1;
    string prerequisite2;
    int numPrerequisites;
    Course() {
        numPrerequisites = 0;
    }
};

// Internal structure for tree node
struct Node {
    Course course ;
    Node* left;
    Node* right;

    //default constructor
    Node() {
        left = nullptr; 
        right = nullptr;
    }

    //initialized constructor
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }
};

/*
 * Define a class to implement tree
 * 
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNumber);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    delete root;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    this->inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if root equal to null ptr
    if (root == nullptr) {
        // root is equal to new node course
        root = new Node(course);
    }
    // else
    else {
        // add Node root and course
        this->addNode(root, course);
    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) {
    // set current node equal to root
    Node* currNode = root;
    // keep looping downwards until bottom reached or matching course number is found
    while (currNode != nullptr) {
        // if match found, return current bid
        if (currNode->course.courseNumber.compare(courseNumber) == 0) {
            return currNode->course;
        }
        // if bid is smaller than current node then traverse left
        if (courseNumber.compare(currNode->course.courseNumber) < 0) {
            currNode = currNode->left;
        }
        // else larger so traverse right
        else {
            currNode = currNode->right;
        }
    }

    Course course;
    return course;
}

/**
 * Add a course to some node
 *
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.courseNumber.compare(course.courseNumber) > 0) {
        // if no left node
        if (node->left == nullptr) {
        // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            //this node becomes right
            node->right = new Node(course);
        }
        //else recurse down the right node
        else {
            this->addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder left
        inOrder(node->left);
        //output course number and name
        cout << node->course.courseNumber << ", " << node->course.name << endl;
        //InOder right
        inOrder(node->right);
      }  
}

/**
 * Display the course information
 *
 */
void displayCourse(Course course) {
    cout << course.courseNumber << ", " << course.name << endl;
    if (course.numPrerequisites == 1) {
        cout << "Prerequisites: " << course.prerequisite1 << endl;
    }
    if (course.numPrerequisites == 2) {
        cout << "Prerequisites: " << course.prerequisite1 << ", " << course.prerequisite2 << endl;
    }
    return;
}

/**
 * Load a CSV file containing courses into a tree
 *
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add the courses
            Course course;
            course.courseNumber = file[i][0];
            course.name = file[i][1];
            if (file[i][2] != "") {
                course.prerequisite1 = file[i][2];
                course.numPrerequisites = 1;
            }
            if (file[i][3] != "") {
                course.prerequisite2 = file[i][3];
                course.numPrerequisites = 2;
            }

            // push course to the end
            bst->Insert(course);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/*
 * main() method
 */
int main() {
    //initialize csv path
    string csvPath = "CourseInformation.csv";

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst = new BinarySearchTree();

    Course course;

    int choice = 0;
    cout << "Welcome to the course planner." << endl;
    while (choice != 9) {
        //create menu
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do?" << endl;
        cin >> choice;
        cout << endl;

        //if the input is not a menu option
        if (choice > 3 && choice != 9) {
            cout << choice << " is not a valid option." << endl;
        }

        //option 1
        if (choice == 1) {
            //load the courses
            loadCourses(csvPath, bst);
        }

        //option 2
        if (choice == 2) {
            //sort the courses
            cout << "Here is a sample schedule:" << endl;
            bst->InOrder();
            cout << endl;
        }

        //option 3
        if (choice == 3) {
            //allow for user input
            string courseKey;
            cout << "Which course do you want to know more about?" << endl;
            cin >> courseKey;
            cout << endl;

            //find the course
            course = bst->Search(courseKey);

            if (!course.courseNumber.empty()) {
                displayCourse(course);
                cout << endl;
            }
            else {
                cout << "Course number " << courseKey << " not found." << endl;
            }
        }
    }

    cout << "Thank you for using the course planner!" << endl;

	return 0;
}
