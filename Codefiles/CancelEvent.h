#pragma once
#include "event.h"
#include"string"
class CancelEvent :public Event
{
public:
	CancelEvent(string time,int id): Event(time,id) {}

	virtual void Execute(CargoLists &C)
	{
		Cargo* ptr = C.GetWaitingCargo(ID,C,N);
		if(ptr!=nullptr)
		if (!ptr->Get_IsLoaded())							//if it's not loaded: execute
		{
		C.CancelCargo(ptr);
		ptr->SetIsCancelled(true);
		}
	}

	~CancelEvent()
	{}

};

