#pragma once
#include "Cargo.h"
#include "Queue.h"
#include "CargoLists.h"

enum TruckType 
{   
	NTruck = 0, 
	STruck = 1, 
	VTruck = 2,
};

class Truck
{
private:
	int NumberofJourneys_TillCheckup;				// The constant number of journeys the truck completes before performing a checkup
	int NumberOfJourneys_Total;						// The total number of journeys done ever to this truck
	int ActiveTime;									// Time a truck is loading or in delivering cargos. This doesn’t include time for a truck to return after delivery.
	int NumberOfCargosDelivered_Total;				// Total Number of Cargos Delivered
	int ID;											// Id of the truck
	int Speed;										// In kilometers/hour
	int Checkup_Time;								// The duration (in hours) of checkups/maintenance that a truck needs to perform after finishing J delivery journeys.
	int TruckCapacity;								// TC is the number of cargos a truck can carry to be fully loaded.
	Priority_Queue<Cargo*> Truck_Cargo_list;		// List of loaded Cargos( After NumberOfCargosAssigned Cargos Reaches TC)
	bool IsLoading;									// Checks if Truck is Loaded or not
	bool IsInMaintenance;							// Checks if The Truck IsInMaintenance or not
	TruckType Type;									// Type of the truck
	CargoType CurrentCargoType;					    // The current Cargo Type that the truck can carry (Each Truck can carry only one type per journey)
	int StartMovingTime_D;							// The Time at which the truck start moving (days)
	int StartMovingTime_H;							// The Time at which the truck start moving (hours)
	int DeliveryTime_D;								// The Time at which the truck finish delivery (days)
	int DeliveryTime_H;								// The Time at which the truck finish delivery (hours)
	int FinishCheckup_D;							// The Time at which the truck finish checkup (days)
	int FinishCheckup_H;							// The Time at which the truck finish checkup (hours)
	bool IsCalculatedActiveTime;					// Checks if the active time can be calculated or not
	bool IsNightShift;
public:

	/********************* Constructor *********************/
	Truck(int Sp, int Checkup_T, int TC, int Journeys, int  id, TruckType T)
	{
		Speed=Sp;					Checkup_Time=Checkup_T;				TruckCapacity=TC;		NumberofJourneys_TillCheckup=Journeys;		IsLoading=false;			StartMovingTime_D=-1;
		StartMovingTime_H=-1;		FinishCheckup_D=-1;					FinishCheckup_H=-1;		DeliveryTime_D=-1;							DeliveryTime_H=-1;		ActiveTime=0;						CurrentCargoType=NoType;
		NumberOfJourneys_Total=0;	NumberOfCargosDelivered_Total = 0;	ID = id;				IsInMaintenance=false;						Type =T;				IsCalculatedActiveTime = false;		 IsNightShift=false;
	}



	/********************* Assigning & Loading *********************/
	bool AssignCargo(Cargo* ptr,int curr_H,int curr_D)
	{
		if (ptr==nullptr)
			return false;
		if(Truck_Cargo_list.GetCount()==0)
		{
			CurrentCargoType=ptr->GetType();														// first cargo assigned type (must be the same for the rest)
		}
		if(ptr->GetType()==CurrentCargoType)														// if the joining cargo is the same type as previous ones, then we can assign it
		{
			if (Truck_Cargo_list.GetCount() < TruckCapacity) 
			{
				Truck_Cargo_list.enqueue(ptr,(INT_MIN-ptr->Get_Delivery_Distance()));				// priority according to the distance ( biggest distance will be the least priority as it goes out the last one )
				ptr->SetAssigned(true,curr_H,curr_D);
				return true;
			}		
			else return false;
		}
		else return false;
	}

	bool LoadTruck(CargoLists &Cargo_List,int currDateHours) 
	{
		if ( (Truck_Cargo_list.GetCount() == TruckCapacity )  || Check_MaxWaitRole(currDateHours, Cargo_List)) 	//	- Loading Rule: If some cargos are ready and waiting to be assigned to a truck,	don’t start loading cargos as long as the number of ready cargos is less than	capacity of the fist available truck (TC). This rule may be ignored in favor of Maximum Wait Rule 
		{
			Priority_Queue<Cargo*> tmp_Truck_Cargo_list = Truck_Cargo_list;
			Cargo* ptr;
			while (!tmp_Truck_Cargo_list.isEmpty())
			{
				ptr = tmp_Truck_Cargo_list.dequeue();
				ptr->Set_IsLoaded(true);
				ptr->Set_TruckID(ID);																			// Removing The Cargo From The Waiting List of Cargos
				Cargo_List.RemoveCargoFromWaiting(ptr->GetType());
			}
			IsLoading=true;
		}
		return IsLoading;
	}



	/********************* Checks *********************/
	void Check_Cargo_MovingToDelivered(int curr_H,int curr_D,CargoLists &Cargo_List)
	{
		Priority_Queue<Cargo*> tmp = Truck_Cargo_list;
		Cargo* ptr = tmp.dequeue();
		if(ptr==nullptr)
			return;
		while( ptr->GetDeliveryTime_D() == curr_D && ptr->GetDeliveryTime_H() == curr_H)
		{
			Cargo* temp = Truck_Cargo_list.dequeue();
			Cargo_List.MoveCargo_MovingToDeliverd(temp);
			ptr = tmp.dequeue();
			NumberOfCargosDelivered_Total++;
			if (ptr==nullptr)
				break;
		}
	}

	bool Check_Assigned_NotLoaded()
	{
		if(Truck_Cargo_list.GetCount()==TruckCapacity && !IsLoading)											// if it has number of assigned cargos = truck capacity and not loaded
			return true;
		else return false;
	}

	bool CheckNightShift()
	{
		if(CurrentCargoType==V)
			IsNightShift=true;
		return IsNightShift;
	}

	void CheckCargo_IsCancelled_Truck()
	{
		Priority_Queue<Cargo*> tmp;
		Cargo* ptr;
		if(CurrentCargoType==N)
		{
		while( !Truck_Cargo_list.isEmpty())
		{
			ptr = Truck_Cargo_list.dequeue();
			if(!ptr->GetIsCancelled() && ptr->GetType()==N) // if it's not cancelled
				tmp.enqueue(ptr,(INT_MIN-ptr->Get_Delivery_Distance()));
		}
		Truck_Cargo_list=tmp;
		}
	}

	bool Check_TruckCheckup()
	{
		return(NumberOfJourneys_Total%NumberofJourneys_TillCheckup==0);
	}

	bool Check_MaxWaitRole(int CurrDateHours, CargoLists C)
	{
		Priority_Queue<Cargo*> tmp_Truck_Cargo_list = Truck_Cargo_list;
		Cargo* ptr;
		while(!tmp_Truck_Cargo_list.isEmpty())
		{
			ptr = tmp_Truck_Cargo_list.dequeue();
			if( (CurrDateHours-ptr->AssignedDateHours()) >= C.GetMaxWait()) 									//If the period between assigning the cargo and the current time equals max wait time
				return true;
		}
		return false;
	}


	/********************* Statistics *********************/
	int TotalLoadUnloadTime() //in hours
	{
		Priority_Queue<Cargo*> tmp = Truck_Cargo_list;
		Cargo* ptr;
		int Total=0;
		while(!tmp.isEmpty())
		{
			ptr = tmp.dequeue();
			Total+=ptr->GetLoadUnloadTime();
		}
		return Total;
	}

	int DeliveryInterval()
	{
		int Max=INT_MIN;
		Priority_Queue<Cargo*> tmp = Truck_Cargo_list;
		Cargo* ptr;
		while(!tmp.isEmpty())
		{
			ptr = tmp.dequeue();
			if(ptr->Get_Delivery_Distance() > Max)
				Max=ptr->Get_Delivery_Distance();
		}
		int loadunload = TotalLoadUnloadTime();
		return 2*(Max/Speed)+loadunload;
	}

	double Truck_Utilization(int SimTime)
	{
		if (NumberOfCargosDelivered_Total == 0)																	// To avoid dividing by zero
		{
			return 0;
		}
		else
		{
			return (double(NumberOfCargosDelivered_Total) / (TruckCapacity*NumberOfJourneys_Total) *(double(ActiveTime) / SimTime));
		}
	}

	void SetActive_Time()
	{
		Priority_Queue<Cargo*> tmp = Truck_Cargo_list;
		Cargo* ptr;
		int Max = INT_MIN;
		while(!tmp.isEmpty())
		{
			ptr = tmp.dequeue();
			if(ptr->Get_Delivery_Distance() > Max)
				Max=ptr->Get_Delivery_Distance();
		}
		int loadunload = TotalLoadUnloadTime();
		int PrevActiveTime = (Max/Speed)+2*loadunload;
		ActiveTime+=PrevActiveTime;
	}


	/********************* Setters And Getters *********************/
	void SetDeliveryTime_EachCargo(int curr_H,int curr_D)
	{
		Priority_Queue<Cargo*> tmp = Truck_Cargo_list;
		Cargo* ptr;
		int SumOfPreviousLoadTime=0;
		while(!tmp.isEmpty())
		{
			ptr = tmp.dequeue();
			SumOfPreviousLoadTime+=ptr->GetLoadUnloadTime();
			int DT_D=curr_D;
			int DT_H=curr_H;
			int DeliveryIntervalOfEachCargo=SumOfPreviousLoadTime+(ptr->Get_Delivery_Distance()/Speed);

			DT_D+=(DT_H+DeliveryIntervalOfEachCargo)/24;  // Day + (H/24)
			DT_H= (DT_H+DeliveryIntervalOfEachCargo)%24; 

			ptr->SetDeliveryTime(DT_D,DT_H);
		}
	}

	void Set_TotalCargosDelivered(int Added_TCD)
	{
		NumberOfCargosDelivered_Total+= Added_TCD;
	}

	void Set_IsCalculatedActiveTime(bool a)
	{
		IsCalculatedActiveTime = a;
	}
	void SetFinishCheckup(int D,int H)
	{
		FinishCheckup_D=D;
		FinishCheckup_H=H;
	}

	void Set_StartMovingTime(int D,int H)
	{
		Priority_Queue<Cargo*> tmp = Truck_Cargo_list;
		Cargo* ptr;
		while(!tmp.isEmpty())
		{			
			ptr =tmp.dequeue();
			ptr->SetMovingTime(D,H);
		}

		StartMovingTime_D=D;
		StartMovingTime_H=H;
	}

	void Set_DeliveryTime(int D,int H)
	{
		DeliveryTime_D=D;
		DeliveryTime_H=H;
	}
	int GetID()
	{
		return ID;
	}

	Queue<int> Get_Cargo_List(Truck tmp)
	{
		Queue<int> p;
		Cargo* ptr;
		while (!tmp.Truck_Cargo_list.isEmpty())
		{
			ptr = tmp.Truck_Cargo_list.dequeue();
			p.enqueue(ptr->GetID());
		}
		return p;
	}

	bool GetIsLoaded()
	{
		return IsLoading;
	}
	int GetActiveTime()
	{
		return ActiveTime;
	}

	int GetDeliveryTime_D()
	{
		return DeliveryTime_D;
	}

	int GetDeliveryTime_H()
	{
		return DeliveryTime_H;
	}


	int Get_StartMovingTimeH()
	{
		return StartMovingTime_H;

	}

	int Get_StartMovingTimeD()
	{
		return StartMovingTime_D;
	}

	bool Get_IsCalculatedAvgTime()
	{
		return IsCalculatedActiveTime;
	}

	int GetFinishCheckup_D()
	{
		return FinishCheckup_D;
	}

	int GetFinishCheckup_H()
	{
		return FinishCheckup_H;
	}

	int GetCheckupTime()
	{
		return Checkup_Time;
	}

	TruckType GetTruckType()
	{
		return Type;
	}

	int GetSpeed()
	{
		return Speed;
	}

	int GetTruckCapacity()
	{
		return TruckCapacity;
	}
	
	int GetTruckCargoListCount()
	{
		return Truck_Cargo_list.GetCount();
	}


	/********************* Utilities  *********************/
	void NumberOfJourneysTotal_Increment()
	{
		NumberOfJourneys_Total++;
	}

	void Reset_TruckData()
	{
		IsLoading=false;
		StartMovingTime_D=-1;
		StartMovingTime_H=-1;
		FinishCheckup_D=-1;
		FinishCheckup_H=-1;
		IsCalculatedActiveTime = false;
		CurrentCargoType=NoType;
		IsNightShift=false;

	}

	void PauseLoading() // pauses the loading till next day
	{
		int StartMovingTimeHours = StartMovingTime_H+StartMovingTime_D*24;
		StartMovingTimeHours+=1;
		StartMovingTime_D=StartMovingTimeHours/24;
		StartMovingTime_H=StartMovingTimeHours%24;
	}


	/********************* Bonus  *********************/
	void Delivery_Failure(CargoLists &C)
	{
		Cargo* ptr;
		while(!Truck_Cargo_list.isEmpty())
		{			
			ptr = Truck_Cargo_list.dequeue();
			C.AddCargo(ptr);
		}
	}

	bool CheckNeedsMaintainance()
	{
		if(NumberOfJourneys_Total%20==0 && NumberOfJourneys_Total!=0)
		{
			IsInMaintenance=true;
			Speed=Speed/2;
		}
		return IsInMaintenance;
	}


	/********************* Destructor  *********************/
	~Truck()
	{}

};
