/**************************************************
 * Name: BinarySearchTree.cpp 
 * Author: William Brandow
 * Date: 2022-02-27
 * Version: 1.0
 *
 **************************************************/
#include <iostream>
#include <fstream>

using namespace std;

struct Course {
	string courseId;
	string title;
	string prereq1;
	string prereq2;
	
    // default constructor
	Course() {
	}
};

struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}

	Node(Course course) : Node() {
		this->course = course;
	}
};

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void InOrder(Node* node);
    Node* removeNode(Node* node, string courseId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(Course course);
    void InOrder();
    void Remove(string courseId);
    Course Search(string courseId);
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
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    this->InOrder(root);

}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if root equarl to null ptr
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
 * Remove a course
 */
void BinarySearchTree::Remove(string courseId) {
    // remove node root courseID
    this->removeNode(root, courseId);
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
    // set current node equal to root
    Node* current = root;

    // keep looping downwards until bottom reached or matching courseId found
    while (current != nullptr) {
        // if match found, return current bid
        if (current->course.courseId.compare(courseId) == 0) {
            return current->course;
        }

        // if courseId is smaller than current node then traverse left
        if (courseId.compare(current->course.courseId) < 0) {
            current = current->left;
        }
        else {
            // else larger so traverse right
            current = current->right;
        }
    }

    Course course;
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param Course cours to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.courseId.compare(course.courseId) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        else {
            // else recurse down the left node
            this->addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            // recurse down the left node
            this->addNode(node->right, course);
        }
    }
}

void BinarySearchTree::InOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        InOrder(node->left);
        cout << node->course.courseId << ": "
            << node->course.title << " | "
            << node->course.prereq1 << " | "
            << node->course.prereq2 << endl;
        InOrder(node->right);
    }
}

Node* BinarySearchTree::removeNode(Node* node, string courseId) {
    // if this node is null the return
    if (node == nullptr) {
        return node;
    }

    // recurse down left subtree
    if (courseId.compare(node->course.courseId) < 0) {
        node->left = removeNode(node->left, courseId);
    }
    else if (courseId.compare(node->course.courseId) > 0) {
        //recurse down right subtree
        node->right = removeNode(node->right, courseId);
    }
    else {
        // no children, leaf node
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        // one left child
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        //one right child
        else if (node->left == nullptr && node->right != nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        // two children
        else {
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->course = temp->course;
            node->right = removeNode(node->right, temp->course.courseId);
        }
    }
    return node;
}

/**
 * Display the course information to the console 
 */
void displayCourse(Course course) {
    cout << course.courseId << ": " << course.title << endl;
    cout << "Prerequisites: " << course.prereq1 << ", " << course.prereq2 << endl;
    return;
}

void loadCourses(string csvPath, BinarySearchTree* bst) {
    ifstream courseFile;

    courseFile.open(csvPath);

    if (!courseFile.is_open()) {
        cout << "Could not open file" << endl;
        return;
    }

    vector<string> courseData;

    // read course data into vector of stings
    while (!courseFile.eof()) {
        string data;

        getline(courseFile, data);

        courseData.push_back(data);
    }

    // close file
    courseFile.close();

    // for each course break data into tokens and store in course
    for (int i = 0; i < courseData.size(); i++) {
        vector<string> tokens;
        Course course;
        string data = courseData.at(i);
        string delimiter = ",";

        size_t pos = 0;
        string token;
        while ((pos = data.find(delimiter)) != string::npos) {
            token = data.substr(0, pos);
            tokens.push_back(token);
            data.erase(0, pos + delimiter.length());
        }
        token = data;
        tokens.push_back(token);

        course.courseId = tokens.at(0);
        course.title = tokens.at(1);

        if (tokens.size() > 2) {
            course.prereq1 = tokens.at(2);
        }

        if (tokens.size() > 3) {
            course.prereq2 = tokens.at(3);
        }

        bst->Insert(course);
    }
}


/**
 * main() method
 */
int main() {

    // process command line arguments
    string csvPath, courseKey, userInput;

    // Define a binary search tree to hold all Courses
    BinarySearchTree* bst = new BinarySearchTree();

    Course course;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bst = new BinarySearchTree();
            
            // get csv path
            cout << "Enter input file path (i.e. exampleFile.csv) : ";
            cin >> csvPath;
            
            // Complete the method call to load the bids
            loadCourses(csvPath, bst);

            //cout << bst->Size() << " courses read" << endl;


        case 2:
            bst->InOrder();
            break;

        case 3:

            cout << "Enter Course ID : ";
            cin >> courseKey;

            course = bst->Search(courseKey);

            if (!course.courseId.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course Id " << courseKey << " not found." << endl;
            }

            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
