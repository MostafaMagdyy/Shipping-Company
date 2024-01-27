#pragma once
#include"iostream"
#include "event.h"
#include"string"
class PromoteEvent :
	public Event
{
private: 
	int extra_money;								//Extra Money for promotion
public:
	PromoteEvent(string time, int id, int extramoney) : Event(time, id)
	{
		extra_money=extramoney;
	}

	virtual void Execute(CargoLists &C)
	{
		Cargo* ptr = C.GetWaitingCargo(ID,C,N);
		if(ptr!=nullptr)
		if (!ptr->Get_IsLoaded())					//if it's not loaded: execute
		C.PromoteCargo(ptr,extra_money);

	}

	~PromoteEvent(void)
	{};

};

