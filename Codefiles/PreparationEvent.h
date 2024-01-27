#pragma once
#include "event.h"
#include"string"
#include "CargoLists.h"
class PreparationEvent :
	public Event
{
private:
	const int LoadUnloadTime;			// Load & Unload time of a cargo
	CargoType Type;						// Type of a cargo
	const int Delivery_Distance;		// Delivery distance of a cargo
	const int cost;						// Cost of a cargo
	int AutoPromotionLimit;				// The autopromotion limit 
public:
	PreparationEvent(string time, int id, int L, char t, int d, int c, int Autopor) : Event(time, id), LoadUnloadTime(L), Delivery_Distance(d), cost(c)
	{
		if (t == 'N')
			Type = N;
		if (t == 'S')
			Type = S;
		if (t == 'V')
			Type = V;
		SetAutoPormotion(Autopor);
	}

	void SetAutoPormotion(int Autopor)
	{
		if (Type == N)
			AutoPromotionLimit = Autopor;
		else
			AutoPromotionLimit = -1;
	}

	virtual void Execute(CargoLists &C)
	{
		Cargo* Pnewcargo = new Cargo(EventTime_D, EventTime_H, LoadUnloadTime, Type, Delivery_Distance, cost, ID,AutoPromotionLimit);
		C.AddCargo(Pnewcargo);							///Adding the cargo
	}

	~PreparationEvent();

};

