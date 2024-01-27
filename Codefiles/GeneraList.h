#ifndef _LINKEDLIST
#define _LINKEDLIST
#include <iostream>
#include "Node.h"

template <typename T>
class GeneralList
{
private:
	Node<T> *Head;
	int count;
public:


	GeneralList()
	{
		Head = nullptr;
		count =0;
	}

	~GeneralList()
	{}

	void PrintList()	const
	{		
		cout<<"\nprinting list contents:\n\n";
		Node<T> *p = Head;

		while(p)
		{
			cout << "[ " << p->getItem() << " ]";
			cout << "--->";
			p = p->getNext();
		}
		cout << "NULL\n";
	}


	void InsertBeg(const T &data)
	{
		Node<T> *R = new Node<T>(data);
		R->setNext(Head);
		Head = R;
		count++;
	}

	T DeleteFirst()
	{
		if (!Head)
			return NULL;
		T tmp;
		tmp = Head->getItem();
		Head = Head->getNext();
		count--;
		return tmp;
	}
	void InsertEnd(const T &data)
	{
		if (!Head)
			InsertBeg(data);
		else
		{
			Node<T>*ptr = Head;
			while (ptr->getNext())
				ptr = ptr->getNext();
			Node<T>*newptr = new Node<T>(data);
			ptr->setNext(newptr);
			count++;
		}
	}


	void DeleteNode(const T &key)
	{

		// Store head node
		Node<T>* temp = Head;
		Node<T>* prev = nullptr;

		// If head node itself holds
		// the key to be deleted

		if (temp != nullptr && temp->getItem() == key)
		{
			Head = Head->getNext(); // Changed head
			count--;
			return;
		}

		// Else Search for the key to be deleted,
		// keep track of the previous node as we
		// need to change 'prev->next' */
		else
		{
			while (temp != nullptr && temp->getItem() != key)
			{
				prev = temp;
				temp = temp->getNext();
			}

			// If key was not present in linked list
			if (temp == nullptr)
				return;

			// Unlink the node from linked list

			prev->setNext(temp->getNext());
			count--;
		}
	}

	int GetCount()
	{

		return count;
	}

	bool isEmpty()
	{
		return (Head == nullptr);
	}  
	//If Needed: GetPointerTo
};

#endif	
