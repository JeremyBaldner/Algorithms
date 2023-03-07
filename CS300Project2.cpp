/* Created by Jeremy Baldner
*  February 19th, 2023
*  This program reads files for college courses and provides users with
*  course information.
*/

#include <iostream>
#include <fstream> // read file
#include <stdlib.h> // clear screen
#include <string>
#include <vector>

using namespace std;

// define a structure to hold course information
struct Course {
	string courseId; // unique identifier
	string title;
	string prerequisiteOne;
	string prerequisiteTwo;
	Course() {
	}
};

// Internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a bid
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

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
	Course Search(string bidId);
};

// Default constructor
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
	delete root;
}

// Traverse the tree in order
void BinarySearchTree::InOrder() {
	inOrder(root);
}

//Insert a course
void BinarySearchTree::Insert(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		this->addNode(root, course);
	}
}

// Search for a bid
Course BinarySearchTree::Search(string courseId) {
	Node* cur = root;
	cout << "looking for course..." << endl << endl;
	while (cur != nullptr) {
		if (cur->course.courseId.compare(courseId) == 0) {
			return cur->course;
		}
		else if (cur->course.courseId.compare(courseId) > 0) {
			cur = cur->left;
		}
		else {
			cur = cur->right;
		}
	}
	Course course;
	return course;
}

// Add a bid to some node (recursive)
void BinarySearchTree::addNode(Node* node, Course course) {
	if (node->course.courseId.compare(course.courseId) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(course);
			return;
		}
		else {
			this->addNode(node->left, course);
		}
	}
	else {
		if (node->right == nullptr) {
			node->right = new Node(course);
			return;
		}
		else {
			this->addNode(node->right, course);
		}
	}
}

// Output courses in order
void BinarySearchTree::inOrder(Node* node) {
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.courseId << ": " << node->course.title << endl;
		inOrder(node->right);
	}
}

// Read file, check format, and create objects
void parseFile(BinarySearchTree* bst) {
	char filename[50];
	ifstream courseFile;
	cout << "Input file name: " << endl;
	cin.getline(filename, 50);
	cin.getline(filename, 50);
	courseFile.open(filename);

	// check if file can open
	while (!courseFile.is_open()) {
		cout << filename << " did not open" << endl << "Please input correct file name: " << endl;
		cin.getline(filename, 50);
		courseFile.open(filename);
	}

	// variables for while loop
	string data;
	vector<string> list;
	vector<string> tempList;
	string delimiter = ",";
	size_t pos = 0;
	string token;
	int i = 0;

	// while file is good, check format and create objects
	while (courseFile.good()) {
		getline(courseFile, data);
		list.push_back(data);

		// seperating lines by commas
		while ((pos = list.at(i).find(delimiter)) != std::string::npos) {
			token = list.at(i).substr(0, pos);
			tempList.push_back(token);
			list.at(i).erase(0, pos + delimiter.length());
		}
		tempList.push_back(list.at(i));

		// checking for correct file format
		if (tempList.size() < 2) {
			cout << "Currupt file: there is not a minimum of two items per line seperated by a comma." << endl;
			return;
		}

		else if (tempList.size() == 2) {
			Course course;
			course.courseId = tempList.at(0);
			course.title = tempList.at(1);
			bst->Insert(course);
		}

		else if (tempList.size() == 3) {
			Course course;
			course.courseId = tempList.at(0);
			course.title = tempList.at(1);
			course.prerequisiteOne = tempList.at(2);
			bst->Insert(course);
		}

		else if (tempList.size() == 4) {
			Course course;
			course.courseId = tempList.at(0);
			course.title = tempList.at(1);
			course.prerequisiteOne = tempList.at(2);
			course.prerequisiteTwo = tempList.at(3);
			bst->Insert(course);
		}

		tempList.clear();
		++i;
	}
	cout << "File uploaded successfully." << endl << endl;
}

int main() {

	// Define a binary search tree to hold all courses
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Course course;

	// variables for menu
	int choice = 0;
	string key;

	cout << "Welcome to the course planner." << endl << endl;

	// User menu
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "1. Load Data Structure" << endl;
		cout << "2. Print Course List" << endl;
		cout << "3. Print Course" << endl;
		cout << "9. Exit" << endl << endl;
		cout << "What would you like to do?  ";
		cin >> choice;

		switch (choice) {
		case 1:
			system("CLS");
			parseFile(bst);
			break;

		case 2:
			system("CLS");
			cout << "Alphanumeric list of all the courses in the Computer Science department:" << endl << endl;
			bst->InOrder();
			cout << endl;
			break;

		case 3:
			cout << "Input course ID: " << endl;
			cin >> key;
			system("CLS");
			course = bst->Search(key);

			if (!course.courseId.empty()) {
				cout << course.courseId << ": " << course.title << endl;
				if (!course.prerequisiteOne.empty()) {
					cout << "Prerequisite(s): " << course.prerequisiteOne;
					if (!course.prerequisiteTwo.empty()) {
						cout << ", " << course.prerequisiteTwo << endl;
					}
					cout << endl << endl;
				}
			}
			else {
				cout << "Course Id " << key << " not found." << endl;
			}
			break;
		}

		// handling exceptions
		if (choice > 3) {
			if (choice == 9) {
				break;
			}
			cout << choice << " is not an option, try again." << endl << endl;
		}
	}
	cout << "Thank you for using the course planner!" << endl;
return 0;
}