#include <iostream>
using namespace std;

template <typename T>
struct PriorityNode
{
	int priority;
	T info;
	struct PriorityNode<T> *next;
};

template <typename T>
class Priority_Queue
{
private:
	PriorityNode<T> *f;
	int count;

public:

	Priority_Queue()
	{
		f = NULL;
		count=0;
	}

	bool isEmpty()
	{
		return (f==nullptr);
	}

	void enqueue(T i, int p)
	{
		PriorityNode<T> *t;
		PriorityNode<T> *q;
		t = new PriorityNode<T>;
		t->info = i;
		t->priority = p;
		if (f == NULL || p > f->priority)
		{
			t->next = f;
			f = t;
		}
		else
		{
			q = f;
			while (q->next != NULL && q->next->priority >= p)
				q = q->next;
			t->next = q->next;
			q->next = t;
		}
		count++;
	}

	T dequeue()
	{
		PriorityNode<T> *t;
		if (f == nullptr) //if queue is null
			return nullptr;
		else
		{
			t = f;
			T tmp = t->info;
			f = f->next;
			t = nullptr;
		//	free(t); //free
			count--;
			return tmp;
		}
	}

	int GetCount()
	{
		return count;
	}

	void Print() 
	{
		PriorityNode<T> *ptr;
		ptr = f;
		if (f == NULL)
			return;
		else
		{
			while (ptr != NULL)
			{
				cout << ptr->info<<" " << endl;
				ptr = ptr->next;
			}
		}
	}

};