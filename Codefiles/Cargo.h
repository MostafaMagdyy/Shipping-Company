#pragma once
enum CargoType 
{   
	N = 0, 
	S = 1, 
	V = 2,
	NoType=3,
};

class Cargo
{
private:
	int PT_day;						// The Time (day) when the cargo is ready to be assigned to a truck.
	int PT_hour;					// The Time (hour) when the cargo is ready to be assigned to a truck. 
	int LoadUnloadTime;				// Cargo Type
	CargoType Type;					// The Time (day:hour) when the cargo is ready to be assigned to a truck.
	int Delivery_Distance;			// The distance (in kilometers) from the company to the delivery location of the cargo.
	int cost;					    // The cost of delivering the cargo.
	int ID;							// Cargo ID
	int TruckID;					// Id of Truck holding the cargo
	int AutoPormotionLimit;			// Days Till AutoPromotion
	int IsLoaded;					// Checks if cargo is loaded or not
	bool Assigned;					// Cargo Assigned or not
	int MovingTime_D;				// Start Moving Time
	int MovingTime_H;				// Start Moving Time (hours)
	int DeliveryTime_H;				// Delivery Time ( hours )
	int DeliveryTime_D;				// Delivery Time ( days )
	bool IsAutoPromoted;			// Check if it's auto promoted or not
	int AssignedDate_D;				// The time at which the cargo is assigned to a truck ( days )
	int AssignedDate_H;				// The time at which the cargo is assigned to a truck ( hours )
	bool IsCancelled;				// Checks if it has been cancelled or not
public:

	/********************* Constructor *********************/
	Cargo(int pt_d, int pt_h, int LTime, CargoType T, int Delivery_D, int C, int id, int Autopor)
	{
		PT_day=pt_d;		PT_hour=pt_h;					LoadUnloadTime=LTime;	Type=T;					Delivery_Distance=Delivery_D;	cost=C;				ID=id;		
		TruckID=-1;			AutoPormotionLimit = Autopor;	IsLoaded=false;			Assigned=false;			IsAutoPromoted=false;			AssignedDate_D=-1;	AssignedDate_H=-1;		
		MovingTime_D=-1;	MovingTime_H=-1;				DeliveryTime_H=-1;		DeliveryTime_D=-1;		IsCancelled=false;
	}


	/********************* Statistics *********************/
	double GetPriority()
	{
		return ((cost*PT_day)*(1.0/Delivery_Distance));								// Dependent on cost & PT time & Delivery distance
	}

	int WaitingTime()
	{
		int MovingTimeHours = MovingTime_D*24 + MovingTime_H;
		int PT_TimeHours = PT_day*24 + PT_hour;
		return (MovingTimeHours-PT_TimeHours);										// Time From preparation till moving
	}

	bool Check_AutoPormote(int curr_H,int curr_D)
	{
		int currHours= curr_D*24 + curr_H;
		int AutoPHours = AutoPormotionLimit*24;
		int PTHours = PT_day*24 + PT_hour;
		if( (currHours-PTHours >= AutoPHours) && !Assigned)							// If current time - preparation time equals number of hours of autop& not assigned
		{
			IsAutoPromoted=true;
			return IsAutoPromoted;

		}
		else return false;
	}


	/********************* Setters And Getters *********************/
	void Set_IsLoaded(bool IL)
	{
		IsLoaded=IL;
	}

	bool Get_IsLoaded()
	{
		return IsLoaded;
	}

	void Set_Cost(int extra)
	{
		if (extra>=0)
		{
			cost+=extra;
		}
	}

	void Set_TruckID(int ID)
	{
		TruckID=ID;
	}

	int Get_TruckID()
	{
		return TruckID;
	}

	int GetID()
	{
		return ID;
	}

	int GetLoadUnloadTime()
	{
		return LoadUnloadTime;
	}

	int GetCost()
	{
		return cost;
	}

	CargoType GetType()
	{
		return Type;
	}

	void SetType()
	{
		Type = V;
	}

	void SetAssigned(bool B,int curr_H,int curr_D)
	{	
		AssignedDate_D= curr_D;
		AssignedDate_H= curr_H;
		Assigned = B;
	}

	int AssignedDateHours()
	{
		return (AssignedDate_D*24 + AssignedDate_H);
	}

	bool GetAssigned()
	{
		return Assigned;
	}

	int Get_Delivery_Distance()
	{
		return Delivery_Distance;
	}

	void SetDeliveryTime(int D,int H)
	{
		DeliveryTime_D=D;
		DeliveryTime_H=H;
	}

	void SetMovingTime(int D,int H)
	{

		MovingTime_D = D;
		MovingTime_H = H;
	}

	int GetMovingTime_Day()
	{
		return MovingTime_D;
	}

	int GetMovingTime_Hour()
	{
		return MovingTime_H;
	}

	int GetDeliveryTime_D()
	{
		return DeliveryTime_D;
	}

	int GetDeliveryTime_H()
	{
		return DeliveryTime_H;
	}

	int GetPT_Day()
	{
		return PT_day;
	}

	int GetPT_Hour()
	{
		return PT_hour;

	}

	void SetIsCancelled(bool IsC)
	{
		IsCancelled=IsC;
	}

	bool GetIsCancelled()
	{
		return IsCancelled;
	}

	~Cargo();
};
