#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>     // std::next, std::prev

using std::string;


// Template Used To Help Create a Node with prev and next set to NULL
template<typename T>
struct Node {
	T data;
	Node<T>* next;
	Node<T>* prev;

	Node() = delete; 
	Node(const T & element) :
		data(element), next(nullptr), prev(nullptr) {}
};

// An object that holds my various data used in a website.
class WebInfo {
 public:
	string uniform_rl;
	size_t page_size;

	WebInfo(string url, size_t pgsize) :
		uniform_rl(url), page_size(pgsize) {}
	WebInfo() {}
};

// The class where most of the magic happens.
class BrowserHistory {
public:

	// Default Constructor Used to Help Create an empty
	// Linked List.
    BrowserHistory();
	~BrowserHistory();

	// Adds a website to Linked List but also removes
	// excess wepages not being used any more.
    void visitSite(string url, size_t filesize);
    void backButton();
    void forwardButton();

	// Function That Reads actions and data for websites.
    void readFile(string fileName);

	// A Function That Retrieves The URL that the user is
	// currently using.
    string currentUrl();

	// A Function That Retrieves The Page Size that the 
	// user is currently using.
    size_t currentPageSize();
    size_t getHistoryLength();

	// Function Used to Help Remove Nodes.
	void remove(Node<WebInfo>* reNode);

private:
	// Pointers to Head and Tail.
	Node<WebInfo>* head;

	Node<WebInfo>* tail;

	size_t length_;

	// The accumilator helps keep track of how many potential
	// Websites may need to be deleted if visiting a new site
	// with older websites in the background.
	size_t accumilator_ = 0;

	// The currNode Node helps keep track of which website the
	// user may be on.
	Node<WebInfo>* currNode;
};

// Creates Empty Doubly Linked List.
BrowserHistory::BrowserHistory() {
	head = new Node<WebInfo>(WebInfo());
	tail = new Node<WebInfo>(WebInfo());

	head->next = tail;
	tail->prev = head;
}

BrowserHistory::~BrowserHistory() {
	delete head;
	delete tail;
 }

void BrowserHistory::visitSite(string url, size_t filesize) {

	// New object is created to help store the data passed
	// from the user in the main.cpp.
	WebInfo New;
	New.uniform_rl = url;
	New.page_size = filesize;

	// The if and while loop are used to help assure that
	// all conditions needed to help delete websites that
	// arent being used anymore are true.
	if (accumilator_ > 0)
	{
		// Pop_Back Type Function for Removing Forward Sites
		while (accumilator_ > 0 && accumilator_ != 0) {
			remove(tail->prev);
			accumilator_--;
		}
	}

	// Appends New Site to the Doubly Linked List.
	Node<WebInfo> * newNode = new Node<WebInfo>(New);
	newNode->prev = tail->prev;
	newNode->next = tail;
	tail->prev->next = newNode;
	tail->prev = newNode;

	// Tracker
	currNode = newNode;
}

void BrowserHistory::backButton() {
	if (currNode->prev == head) {
		std::cout << "No Previous Page\n";
	}
	else {
		// Tracker
		currNode = currNode->prev;

		// Keeps Track of Sites Traversed Back for Possible
		// Pop_Back
		accumilator_++;
	}

}

void BrowserHistory::forwardButton() {

	// Helps Make Sure There is Page That Was Visited
	if (currNode->next == tail) {
		std::cout << "No Such Page\n";
	}    
	else {
		// Tracker
		currNode = currNode->next;

		// Keeps Track of Sites Traversed Back for Possible
		// Pop_Back
		accumilator_--;
	}
}

void BrowserHistory::readFile(string filename) {
	string var0,
		var1;
	int var2;
	std::ifstream meesafile(filename);

	// A very sophisticated while loop that reads the response
	//from the file provided to help produce an action from the
	// first words on the text document that will countinue to
	// be read until there is no more "action words". If the 
	// data read in matches visit passes in 2nd and 3rd data 
	// into visit function.
	while (meesafile >> var0) {

		if (var0 == "visit") {
			meesafile >> var1 >> var2;
			visitSite(var1, var2);
		}
		else if(var0 == "back"){
			backButton();
		}
		else if (var0 == "forward") {
			forwardButton();
		}
	}



}

string BrowserHistory::currentUrl() {
	// If No Page is Found returns ""
	if (head->next == tail) {
		return "";
	}

	// Returns URL
	return currNode->data.uniform_rl;
}
size_t BrowserHistory::currentPageSize() {
	// Returns Page Size
	return currNode->data.page_size;
}

size_t BrowserHistory::getHistoryLength() {

	length_ = 0;

	Node<WebInfo>* currNode2 = head->next;

	// while loop that traverses the linked list at
	// T=the current moment and counts as it goes
	// to return the length of the linked list.
	while (currNode2 != tail)
	{
		currNode2 = currNode2->next;
		length_++;
	}
		

	return length_;
}

// Function Used To Remove Nodes
void BrowserHistory::remove(Node<WebInfo>* reNode) {
	// Checks if Linked List is Empty
	if (head->next == tail) {
		std::cout << "ERROR\n";
		return;
	}

	if (reNode == head || reNode == tail) {
		// Dummy nodes should not be removed
		std::cout << "ERROR\n";
		return;
	}

	Node<WebInfo>* nextNode = reNode->next;
	Node<WebInfo>* prevNode = reNode->prev;

	nextNode->prev = prevNode;
	prevNode->next = nextNode;
}