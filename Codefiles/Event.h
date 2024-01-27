#pragma once
#include "Cargo.h"
#include"string"
#include "CargoLists.h"
class Event
{
protected:
	 int EventTime_H;
	 int EventTime_D;
	//Data of Cargo
	const int ID;

public:
	virtual void Execute(CargoLists &C)=0 ;
	Event(string time,int id):ID(id)
	{
		int index = 0;
		while (time[index] != ':')
			index++;
		EventTime_D = stoi(time.substr(0, index));
		EventTime_H = stoi(time.substr(index + 1));
	
	}
	int GetEventTime_H()
	{
		return EventTime_H;
	}

	int GetEventTime_D()
	{
		return EventTime_D;
	}

	~Event(void)
	{}
};

