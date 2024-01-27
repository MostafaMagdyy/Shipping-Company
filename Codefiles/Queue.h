
#ifndef LINKED_QUEUE_
#define LINKED_QUEUE_

#include "Node.h"
#include "QueueADT.h"

using namespace std;


template <typename T>
class Queue :public QueueADT<T>
{
private:

	Node<T>* backPtr;
	Node<T>* frontPtr;
	int count;

public:
	Queue()
	{
	backPtr = nullptr;
	frontPtr = nullptr;
	count=0;
	}

	bool isEmpty() const
	{
		return (count==0);

	}

	bool enqueue(const T& newEntry)
	{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty

	backPtr = newNodePtr; // New node is the last node now
	count++;
	return true;
	}

	T dequeue()
	{
		Node<T> *t;
		if (frontPtr == nullptr) //if queue is null
			return NULL;
		else
		{
			t = frontPtr;
			T tmp = t->getItem();
			frontPtr = frontPtr->getNext();
			count--;
			return tmp;
		}
	}

	bool peek(T& frntEntry)  const
	{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;
	}

	~Queue()
	{
//	T temp;
//	while (dequeue(temp));
	}


	//copy constructor
	Queue(const Queue<T> & LQ)
	{
			Node<T>* NodePtr = LQ.frontPtr;
	if (!NodePtr) //LQ is empty
	{
		frontPtr = backPtr = nullptr;
		count=0;
		return;
	}

	//insert the first node
	Node<T>* ptr = new Node<T>(NodePtr->getItem());
	frontPtr = backPtr = ptr;
	NodePtr = NodePtr->getNext();

	//insert remaining nodes
	while (NodePtr)
	{
		Node<T>* ptr = new Node<T>(NodePtr->getItem());
		backPtr->setNext(ptr);
		backPtr = ptr;
		NodePtr = NodePtr->getNext();
	}
	count=LQ.count;
	}

	int GetCount()
	{
		return count;
	}


};

#endif