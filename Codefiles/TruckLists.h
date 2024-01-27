#pragma once
#include "CargoLists.h"
#include "Truck.h"
#include "Queue.h"
#include <ctime>

class TruckLists
{
private:
	Priority_Queue<Truck*> Available_VIPTrucks_List;				//Available VIP Trucks List
	Priority_Queue<Truck*> Available_SpecialTrucks_List;			//Available Special Trucks List
	Priority_Queue<Truck*> Available_NormalTrucks_List;				//Available Normal Trucks List
	Priority_Queue<Truck*> Loading_Trucks_List;						//loading Trucks List
	Priority_Queue<Truck*> Moving_Trucks_List;						//Moving Trucks List	
	Queue<Truck*> InCheckup_VIPTrucks_List;							//InCheckup VIP Trucks List
	Queue<Truck*> InCheckup_SpecialTrucks_List;						//InCheckup Special Trucks List
	Queue<Truck*> InCheckup_NormalTrucks_List;						//InCheckup Normal Trucks List
	int Number_of_VIP_Trucks;										//Number Of All VIP Trucks
	int Number_of_Normal_Trucks;									//Number Of All Normal Trucks
	int Number_of_Special_Trucks;									//Number Of All Special Trucks
	bool IsLoading_Normal;											//Checks If there's a current loading Normal Truck
	bool IsLoading_VIP;	;											//Checks If there's a current loading VIP Truck
	bool IsLoading_Special;;										//Checks If there's a current loading Special Truck
public:

	/********************* Constructor *********************/
	TruckLists()
	{
		Number_of_VIP_Trucks=-1;		Number_of_Normal_Trucks=-1;		Number_of_Special_Trucks=-1;
		IsLoading_Normal=false;			IsLoading_VIP=false;			IsLoading_Special=false;
	}


	/********************* Adding The Trucks ( Start Of Simulation ) *********************/
	void AddTrucks(int Speed,int CheckupD, int TC, int J,int NumTrucks,TruckType T,int &j)			//j is the counter of id of trucks
	{
		for (int i = 0; i < NumTrucks; i++)
		{
			Truck *ptr = new Truck(Speed, CheckupD, TC, J, j++, T);
			switch(T)
			{
			case NTruck:
				{
					Available_NormalTrucks_List.enqueue(ptr,TC*Speed);	break;
				}
			case VTruck:
				{
					Available_VIPTrucks_List.enqueue(ptr,TC*Speed);		break;
				}
			case STruck:
				{
					Available_SpecialTrucks_List.enqueue(ptr,TC*Speed);	break;
				}
			}
		}
	}


	/********************* Assigning The Trucks ( Called Each (In-Working) Hour ) *********************/
	bool Assign_TruckLists(CargoLists &C,TruckLists &Truck_List,CargoType Tcargo,int curr_H,int curr_D)
	{
		bool Done=false;	Truck* ptr=nullptr;		TruckType Type;		bool Continue=false; TruckLists T=Truck_List;
		switch (Tcargo)
		{
		case V:																						//VIPCargo Assignment: 1)VIPTruck 2)NormalTruck 3)SpecialTruck
			{
				while(!T.Available_VIPTrucks_List.isEmpty())								//VIPTruck Check
				{
					ptr = T.Available_VIPTrucks_List.dequeue();										//If exists we take it
					do
					{
						if(curr_H<5 || curr_H>23)
						{	
							if(ptr->CheckNightShift())
								Done = ptr->AssignCargo(C.Assign_Cargo_CargoLists(Tcargo,C),curr_H,curr_D);		//Assign the first UnAssigned Cargo To This Truck
						}
						else
							Done = ptr->AssignCargo(C.Assign_Cargo_CargoLists(Tcargo,C),curr_H,curr_D);		//Assign the first UnAssigned Cargo To This Truck
					}	
					while(Done);
					if(Done)
						Continue=true;
				}	
				if(!Done)
					while(!T.Available_NormalTrucks_List.isEmpty())
					{
						ptr = T.Available_NormalTrucks_List.dequeue();										//If exists we take it
						do
						{
							if(curr_H<5 || curr_H>23)
							{	
								if(ptr->CheckNightShift())
									Done = ptr->AssignCargo(C.Assign_Cargo_CargoLists(Tcargo,C),curr_H,curr_D);		//Assign the first UnAssigned Cargo To This Truck
							}
							else
								Done = ptr->AssignCargo(C.Assign_Cargo_CargoLists(Tcargo,C),curr_H,curr_D);		//Assign the first UnAssigned Cargo To This Truck
						}	
						while(Done);
						if(Done)
							Continue=true;
					}	
					if(!Done)
						while(!T.Available_SpecialTrucks_List.isEmpty())
						{
							ptr = T.Available_SpecialTrucks_List.dequeue();
							do
							{
								if(curr_H<5 || curr_H>23)
								{	
									if(ptr->CheckNightShift())
										Done = ptr->AssignCargo(C.Assign_Cargo_CargoLists(Tcargo,C),curr_H,curr_D);		//Assign the first UnAssigned Cargo To This Truck
								}
								else
									Done = ptr->AssignCargo(C.Assign_Cargo_CargoLists(Tcargo,C),curr_H,curr_D);		//Assign the first UnAssigned Cargo To This Truck
							}	
							while(Done);
							if(Done)
								Continue=true;
						}	
						break;
			}

		case S:																						//SpecialCargo Assignment: 1)SpecialTruck
			{
				while(!T.Available_SpecialTrucks_List.isEmpty())									//SpecialTruck Check
				{
					ptr = T.Available_SpecialTrucks_List.dequeue();									//If exists we take it
					do
					{
						if(curr_H<5 || curr_H>23)
						{	
							if(ptr->CheckNightShift())
								Done = ptr->AssignCargo(C.Assign_Cargo_CargoLists(Tcargo,C),curr_H,curr_D);		//Assign the first UnAssigned Cargo To This Truck
						}
						else
							Done = ptr->AssignCargo(C.Assign_Cargo_CargoLists(Tcargo,C),curr_H,curr_D);		//Assign the first UnAssigned Cargo To This Truck
					}	
					while(Done);
					if(Done)
						Continue=true;
				}
				break;
			}

		case N:																						//NormalCargo Assignment: 1)NormalTruck 2)VIPTruck
			{

				while(!T.Available_NormalTrucks_List.isEmpty()) //if Not Empty
				{
					ptr = T.Available_NormalTrucks_List.dequeue();
					do
					{
						if(curr_H<5 || curr_H>23)
						{	
							if(ptr->CheckNightShift())
								Done = ptr->AssignCargo(C.Assign_Cargo_CargoLists(Tcargo,C),curr_H,curr_D);		//Assign the first UnAssigned Cargo To This Truck
						}
						else
							Done = ptr->AssignCargo(C.Assign_Cargo_CargoLists(Tcargo,C),curr_H,curr_D);		//Assign the first UnAssigned Cargo To This Truck
					}	
					while(Done);
					if(Done)
						Continue=true;
				}	
				if(!Done)
					while(!T.Available_VIPTrucks_List.isEmpty()) //if Not Empty
					{
						ptr = T.Available_VIPTrucks_List.dequeue();
						do
						{
							if(curr_H<5 || curr_H>23)
							{	
								if(ptr->CheckNightShift())
									Done = ptr->AssignCargo(C.Assign_Cargo_CargoLists(Tcargo,C),curr_H,curr_D);		//Assign the first UnAssigned Cargo To This Truck
							}
							else
								Done = ptr->AssignCargo(C.Assign_Cargo_CargoLists(Tcargo,C),curr_H,curr_D);		//Assign the first UnAssigned Cargo To This Truck
						}	
						while(Done);
						if(Done)
							Continue=true;
					}
					break;
			}
		}
		return Done;
	}


	/********************* Checks ( Called Each Hour ) *********************/
	void CheckEmptyToLoading(TruckLists &TruckList,int curr_H,int curr_D,CargoLists &C)
	{
		Truck* ptr;
		TruckLists T=TruckList;
		while(!T.Available_NormalTrucks_List.isEmpty())														//traverses each list
		{
			ptr = T.Available_NormalTrucks_List.dequeue();
			if(curr_H<5 || curr_H>23)																		//if in off-working hours:
			{	
				if(ptr->CheckNightShift())																	//check if the truck is in night shift
					CheckEmptyToLoading_Truckelement(ptr,TruckList,curr_H,curr_D,C);
			}
			else
				CheckEmptyToLoading_Truckelement(ptr,TruckList,curr_H,curr_D,C);

		}		
		while(!T.Available_VIPTrucks_List.isEmpty())														//same as above
		{
			ptr = T.Available_VIPTrucks_List.dequeue();
			if(curr_H<5 || curr_H>23)
			{	
				if(ptr->CheckNightShift())
					CheckEmptyToLoading_Truckelement(ptr,TruckList,curr_H,curr_D,C);
			}
			else
				CheckEmptyToLoading_Truckelement(ptr,TruckList,curr_H,curr_D,C);
		}		
		while(!T.Available_SpecialTrucks_List.isEmpty())														//same as above
		{
			ptr = T.Available_SpecialTrucks_List.dequeue();
			if(curr_H<5 || curr_H>23)
			{	
				if(ptr->CheckNightShift())
					CheckEmptyToLoading_Truckelement(ptr,TruckList,curr_H,curr_D,C);
			}
			else
				CheckEmptyToLoading_Truckelement(ptr,TruckList,curr_H,curr_D,C);
		}
	}

	void CheckEmptyToLoading_Truckelement(Truck* ptr,TruckLists &T,int curr_H,int curr_D,CargoLists &C)
	{
		if(ptr!=nullptr)
			if(ptr->Check_Assigned_NotLoaded() || ptr->Check_MaxWaitRole(curr_H+curr_D*24, C))					//If All trucks are assigned and not loaded or one cargo has exceeded its max wait rule
			{
				bool IsLoading;
				switch(ptr->GetTruckType())
				{
				case NTruck:
					{
						IsLoading = IsLoading_Normal;
						break;
					}
				case VTruck:
					{
						IsLoading = IsLoading_VIP;
						break;
					}
				case STruck:
					{
						IsLoading = IsLoading_Special;
						break;
					}
				}
				if(!IsLoading && ptr->GetTruckCargoListCount()>0)
					if(ptr->LoadTruck(C,(curr_H+curr_D*24)))												// if there's no trucks loading in the same time and the truck is loaded successfully
					{
						Truck* p;
						switch(ptr->GetTruckType())
						{
						case NTruck:
							{
								p= T.Available_NormalTrucks_List.dequeue();
								IsLoading_Normal=true;
								break;
							}
						case VTruck:
							{
								p= T.Available_VIPTrucks_List.dequeue();
								IsLoading_VIP=true;
								break;
							}
						case STruck:
							{
								p= T.Available_SpecialTrucks_List.dequeue();
								IsLoading_Special=true;
								break;
							}
						}

						int priority = INT_MAX-(p->TotalLoadUnloadTime()+(curr_D*24)+curr_H);			//priority according to the total load unload time of the cargos in the truck (lowest has the highest priority)
						T.Loading_Trucks_List.enqueue(p,priority);
						int Start_Time_D=curr_D;
						int Start_Time_H=curr_H;

						Start_Time_D+=(Start_Time_H+p->TotalLoadUnloadTime())/24; 
						Start_Time_H=(Start_Time_H+p->TotalLoadUnloadTime())%24;
						p->Set_StartMovingTime(Start_Time_D,Start_Time_H);								//Setting the time at which it will start moving
					}
			}
	}

	void Check_LoadingToMoving(TruckLists &Truck_List,int curr_H,int curr_D)
	{
		TruckLists tmp=Truck_List;
		Truck* ptr;
		bool Continue=false;
		while(!tmp.Loading_Trucks_List.isEmpty())																//Traverses loading trucks
		{
			ptr= tmp.Loading_Trucks_List.dequeue();																//Takes the first one to check it
			if(curr_H<5 || curr_H>23)																			//if in night shift check if the truck is night shifted
			{	
				if(ptr->CheckNightShift())
					Continue=true;
			}
			else
				Continue=true;
			if (curr_H>=ptr->Get_StartMovingTimeH() && curr_D>=ptr->Get_StartMovingTimeD() && Continue)			//if the start moving time > current time 
			{
				Truck* t;
				t= Truck_List.Loading_Trucks_List.dequeue();
				switch(t->GetTruckType())
				{
				case NTruck:
					{
						Truck_List.IsLoading_Normal=false;
						break;
					}
				case VTruck:
					{
						Truck_List.IsLoading_VIP=false;
						break;
					}
				case STruck:
					{
						Truck_List.IsLoading_Special=false;
						break;
					}
				}
				int priority = INT_MAX-(t->DeliveryInterval()+(curr_D*24)+curr_H);										//priority according to the DI
				Truck_List.Moving_Trucks_List.enqueue(t,priority);
				int DeliveryTime_D=curr_D;
				int DeliveryTime_H=curr_H;
				DeliveryTime_D+= (DeliveryTime_H+t->DeliveryInterval())/24; 
				DeliveryTime_H= (DeliveryTime_H+t->DeliveryInterval())%24;
				t->Set_DeliveryTime(DeliveryTime_D,DeliveryTime_H);
				t->SetDeliveryTime_EachCargo(curr_H,curr_D);															//Setting the time at which it will be delivered
			}
		}
	}

	void CheckCargo_MovingToDelivered(int curr_H,int curr_D,CargoLists &Cargo_List)
	{
		TruckLists T=*this;
		Truck* ptr;

		while (!T.Moving_Trucks_List.isEmpty())																			//Traverses loading trucks
		{
			ptr = T.Moving_Trucks_List.dequeue();																		//Takes the first one to check it
			if (!ptr->Get_IsCalculatedAvgTime())																		//Setting the active time
			{
				ptr->SetActive_Time();
				ptr->Set_IsCalculatedActiveTime(true);
			}

			{
				srand( (unsigned)time( NULL ) );																		//Random numbers generated between 0 and 1
				if((float) rand()/RAND_MAX<0.05)																		// very low possability of failure (5%)
				{
					ptr= Moving_Trucks_List.dequeue();
					ptr->Delivery_Failure(Cargo_List);															// Moving the cargos to the waiting lists
					ptr->Reset_TruckData();																		// Reseting the data
					switch(ptr->GetTruckType())
					{

					case VTruck:
						{
							InCheckup_VIPTrucks_List.enqueue(ptr);		break;
						}
					case NTruck:
						{
							InCheckup_NormalTrucks_List.enqueue(ptr);	break;

						}
					case STruck:
						{
							InCheckup_SpecialTrucks_List.enqueue(ptr);	break;

						}
					}							
					int CheckupTime_D=curr_D;
					int CheckupTime_H=curr_H;
					CheckupTime_D+=(CheckupTime_H+ptr->GetCheckupTime())/24; 
					CheckupTime_H=(CheckupTime_H+ptr->GetCheckupTime())%24;
					ptr->SetFinishCheckup(CheckupTime_D,CheckupTime_H);											//Adding to the checkup list
					break;

				}
				else
					ptr->Check_Cargo_MovingToDelivered(curr_H,curr_D,Cargo_List);
			}
		}
	}

	void Check_MovingToEmptyORCheckup(TruckLists &Truck_List,int curr_H,int curr_D)
	{
		TruckLists tmp=Truck_List;		Truck* ptr;			Truck* temp;

		ptr= tmp.Moving_Trucks_List.dequeue();
		if(ptr==nullptr)
			return;

		while (curr_H == ptr->GetDeliveryTime_H() && curr_D == ptr->GetDeliveryTime_D())							//if delivery time < current time
		{
			ptr->NumberOfJourneysTotal_Increment();
			if(ptr->Check_TruckCheckup())
			{
				temp= Truck_List.Moving_Trucks_List.dequeue();														//adding the moving to delivered
				temp->Reset_TruckData();
				switch(ptr->GetTruckType())
					{

					case VTruck:
						{
							InCheckup_VIPTrucks_List.enqueue(ptr);		break;
						}
					case NTruck:
						{
							InCheckup_NormalTrucks_List.enqueue(ptr);	break;

						}
					case STruck:
						{
							InCheckup_SpecialTrucks_List.enqueue(ptr);	break;

						}
					}
						int CheckupTime_D=curr_D;																		//setting checkup time
						int CheckupTime_H=curr_H;
						if(temp->CheckNeedsMaintainance() )// If the truck needs maintainance we Double the checkup time
						{
							CheckupTime_D+= (CheckupTime_H + temp->GetCheckupTime()*2) / 24; 
							CheckupTime_H= (CheckupTime_H + temp->GetCheckupTime()*2) % 24;
						}
						else
						{
							CheckupTime_D+=(CheckupTime_H+temp->GetCheckupTime())/24; 
							CheckupTime_H=(CheckupTime_H+temp->GetCheckupTime())%24;
						}
						temp->SetFinishCheckup(CheckupTime_D,CheckupTime_H);
			}
			else
			{

				temp= Truck_List.Moving_Trucks_List.dequeue();															//Moving the truck to empty list again as it doesn't need checkup
				temp->Reset_TruckData();
				switch(temp->GetTruckType())
				{					
				case NTruck:
					{
						Truck_List.Available_NormalTrucks_List.enqueue(temp,temp->GetSpeed()*temp->GetTruckCapacity());
						break;
					}	
				case VTruck:
					{
						Truck_List.Available_VIPTrucks_List.enqueue(temp,temp->GetSpeed()*temp->GetTruckCapacity());
						break;

					}
				case STruck:
					{
						Truck_List.Available_SpecialTrucks_List.enqueue(temp,temp->GetSpeed()*temp->GetTruckCapacity());
						break;
					}
				}
			}

			ptr= tmp.Moving_Trucks_List.dequeue();
			if(ptr==nullptr)
				return;
		}
	}

	void Check_Cargo_IsCancelled(TruckLists tmp)				//Checks if the cargo is cancelled by an event
	{
		Truck* ptr;
		while(!tmp.Available_NormalTrucks_List.isEmpty())
		{
			ptr=tmp.Available_NormalTrucks_List.dequeue();
			ptr->CheckCargo_IsCancelled_Truck();
		}
		while(!tmp.Available_VIPTrucks_List.isEmpty())
		{
			ptr=tmp.Available_VIPTrucks_List.dequeue();
			ptr->CheckCargo_IsCancelled_Truck();
		}
	}

	void Check_CheckupToAvailable(TruckLists &Truck_List, int curr_H, int curr_D)				//Checks if the checkup time is up		
	{
		TruckLists tmp = Truck_List;
		Truck* ptr;
		//check for normal
		ptr = tmp.InCheckup_NormalTrucks_List.dequeue();
		if (ptr != nullptr)
			while (curr_H == ptr->GetFinishCheckup_H() && curr_D == ptr->GetFinishCheckup_D())		//if current time > finish checkup for normal ones
			{
				Truck* t;
				t = Truck_List.InCheckup_NormalTrucks_List.dequeue();
				Available_NormalTrucks_List.enqueue(t, t->GetSpeed()*t->GetTruckCapacity());
				ptr = tmp.InCheckup_NormalTrucks_List.dequeue();
				if (ptr == nullptr)
					break;
			}
			//check for VIP
			ptr = tmp.InCheckup_VIPTrucks_List.dequeue();
			if (ptr != nullptr)
				while (curr_H == ptr->GetFinishCheckup_H() && curr_D == ptr->GetFinishCheckup_D())		//if current time > finish checkup for normal ones
				{
					Truck* t;
					t = Truck_List.InCheckup_VIPTrucks_List.dequeue();
					Available_VIPTrucks_List.enqueue(t, t->GetSpeed()*t->GetTruckCapacity());
					ptr = tmp.InCheckup_VIPTrucks_List.dequeue();
					if (ptr == nullptr)
						break;
				}
				//check for Special

				ptr = tmp.InCheckup_SpecialTrucks_List.dequeue();
				if (ptr != nullptr)
					while (curr_H == ptr->GetFinishCheckup_H() && curr_D == ptr->GetFinishCheckup_D())		//if current time > finish checkup for normal ones
					{
						Truck* t;
						t = Truck_List.InCheckup_SpecialTrucks_List.dequeue();
						Available_SpecialTrucks_List.enqueue(t, t->GetSpeed()*t->GetTruckCapacity());
						ptr = tmp.InCheckup_SpecialTrucks_List.dequeue();
						if (ptr == nullptr)
							break;
					}
	}

	void Check_PauseLoading_OffHours()													//Pauses the loading in the off hours
	{
		Priority_Queue<Truck*> tmp_Loading_Trucks_List = Loading_Trucks_List;
		Truck* tmp;
		while (!tmp_Loading_Trucks_List.isEmpty())
		{
			tmp = tmp_Loading_Trucks_List.dequeue();
			if(!tmp->CheckNightShift())
				tmp->PauseLoading();
		}
	}


	/********************* Statistics *********************/
	int Total_Active_Time()
	{
		Truck*ptr;
		Priority_Queue<Truck*> tmp_Available_VIPTrucks_List = Available_VIPTrucks_List;			Priority_Queue<Truck*> tmp_Available_SpecialTrucks_List = Available_SpecialTrucks_List;
		Priority_Queue<Truck*> tmp_Available_NormalTrucks_List = Available_NormalTrucks_List;	Queue<Truck*> tmp_InCheckup_VIPTrucks_List = InCheckup_VIPTrucks_List;
		Queue<Truck*> tmp_InCheckup_SpecialTrucks_List = InCheckup_SpecialTrucks_List;			Queue<Truck*> tmp_InCheckup_NormalTrucks_List = InCheckup_NormalTrucks_List;
		int Active_Time_H = 0;
		while (!tmp_Available_VIPTrucks_List.isEmpty())									//Calculating Total Active time
		{
			ptr = tmp_Available_VIPTrucks_List.dequeue();
			Active_Time_H += ptr->GetActiveTime();
		}
		while (!tmp_Available_SpecialTrucks_List.isEmpty())
		{
			ptr = tmp_Available_SpecialTrucks_List.dequeue();
			Active_Time_H += ptr->GetActiveTime();
		}
		while (!tmp_Available_NormalTrucks_List.isEmpty())
		{
			ptr = tmp_Available_NormalTrucks_List.dequeue();
			Active_Time_H += ptr->GetActiveTime();
		}
		while (!tmp_InCheckup_VIPTrucks_List.isEmpty())
		{
			ptr = tmp_InCheckup_VIPTrucks_List.dequeue();
			Active_Time_H += ptr->GetActiveTime();
		}
		while (!tmp_InCheckup_SpecialTrucks_List.isEmpty())
		{
			ptr = tmp_InCheckup_SpecialTrucks_List.dequeue();
			Active_Time_H += ptr->GetActiveTime();
		}
		while (!tmp_InCheckup_NormalTrucks_List.isEmpty())
		{
			ptr = tmp_InCheckup_NormalTrucks_List.dequeue();
			Active_Time_H += ptr->GetActiveTime();
		}
		return Active_Time_H;
	}

	int Avg_TruckUtilization(int SimTime)
	{
		Priority_Queue<Truck*> tmp_Available_VIPTrucks_List = Available_VIPTrucks_List;		Priority_Queue<Truck*> tmp_Available_SpecialTrucks_List = Available_SpecialTrucks_List;		Priority_Queue<Truck*> tmp_Available_NormalTrucks_List = Available_NormalTrucks_List;
		Queue<Truck*> tmp_InCheckup_VIPTrucks_List = InCheckup_VIPTrucks_List;				Queue<Truck*> tmp_InCheckup_SpecialTrucks_List = InCheckup_SpecialTrucks_List;				Queue<Truck*> tmp_InCheckup_NormalTrucks_List = InCheckup_NormalTrucks_List;
		double Sum = 0;
		Truck* ptr;	
		while (!tmp_Available_VIPTrucks_List.isEmpty())											//Calculating Avg_Truck Utilization
		{
			ptr = tmp_Available_VIPTrucks_List.dequeue();
			Sum += ptr->Truck_Utilization(SimTime);
		}
		while (!tmp_Available_SpecialTrucks_List.isEmpty())
		{
			ptr = tmp_Available_SpecialTrucks_List.dequeue();
			Sum += ptr->Truck_Utilization(SimTime);
		}
		while (!tmp_Available_NormalTrucks_List.isEmpty())
		{
			ptr = tmp_Available_NormalTrucks_List.dequeue();
			Sum += ptr->Truck_Utilization(SimTime);
		}

		while (!tmp_InCheckup_VIPTrucks_List.isEmpty())
		{
			ptr = tmp_InCheckup_VIPTrucks_List.dequeue();
			Sum += ptr->Truck_Utilization(SimTime);
		}
		while (!tmp_InCheckup_SpecialTrucks_List.isEmpty())
		{
			ptr = tmp_InCheckup_SpecialTrucks_List.dequeue();
			Sum += ptr->Truck_Utilization(SimTime);
		}
		while (!tmp_InCheckup_NormalTrucks_List.isEmpty())
		{
			ptr = tmp_InCheckup_NormalTrucks_List.dequeue();
			Sum += ptr->Truck_Utilization(SimTime);
		}
		int count = GetInCheckupTrucks_Count() + GetEmptyTrucks_Count();
		return (Sum * 100) / count;
	}

	/********************* Utilities *********************/
	Truck* DequeueTruck_Available(TruckType T)
	{
		Truck* t;
		switch (T)
		{
		case NTruck:
			{
				t = Available_NormalTrucks_List.dequeue();
				break;
			}
		case VTruck:
			{
				t = Available_VIPTrucks_List.dequeue();
				break;
			}
		case STruck:
			{
				t = Available_SpecialTrucks_List.dequeue();
				break;
			}
		}
		if (t == NULL)
			return nullptr;
		else
			return t;
	}

	Truck* DequeueTruck_Loading()
	{
		Truck* t;
		t = Loading_Trucks_List.dequeue();

		if (t == NULL)
			return nullptr;
		else
			return t;
	}


	Truck* DequeueTruck_Moving()
	{
		Truck* t;
		t = Moving_Trucks_List.dequeue();

		if (t == NULL)
			return nullptr;
		else
			return t;
	}


	/********************* Functions Used in UI *********************/
	Queue<int> Get_InCheckupTrucks_IDs_List(TruckType T)
	{
		Queue<int> p;
		Truck* ptr;
		switch (T)
		{
		case NTruck:
			{
				while (!InCheckup_NormalTrucks_List.isEmpty())
				{
					ptr = InCheckup_NormalTrucks_List.dequeue();
					p.enqueue(ptr->GetID());
				}
				return p;

			}
		case VTruck:
			{
				while (!InCheckup_VIPTrucks_List.isEmpty())
				{
					ptr = InCheckup_VIPTrucks_List.dequeue();
					p.enqueue(ptr->GetID());
				}
				return p;

			}
		case STruck:
			{
				while (InCheckup_SpecialTrucks_List.GetCount() != 0)
				{
					ptr = InCheckup_SpecialTrucks_List.dequeue();
					p.enqueue(ptr->GetID());
				}
				return p;														//Returning the list of ids of checkup list

			}
		}
	}

	Queue<int> Get_EmptyTrucks_IDs_List(TruckType T)
	{
		Queue<int> p;
		Truck* ptr;
		switch (T)
		{
		case NTruck:
			{
				while (!Available_NormalTrucks_List.isEmpty())
				{
					ptr = Available_NormalTrucks_List.dequeue();
					p.enqueue(ptr->GetID());
				}
				return p;

			}
		case VTruck:
			{
				while (!Available_VIPTrucks_List.isEmpty())
				{
					ptr = Available_VIPTrucks_List.dequeue();
					p.enqueue(ptr->GetID());
				}
				return p;

			}
		case STruck:
			{
				while (Available_SpecialTrucks_List.GetCount() != 0)
				{
					ptr = Available_SpecialTrucks_List.dequeue();
					p.enqueue(ptr->GetID());
				}
				return p;

			}
		}
	}

	Queue<int> Get_LoadingTrucks_IDs_List(TruckType T)
	{
		Queue<int> p;
		Truck* ptr;
		switch (T)
		{
		case NTruck:
			{
				while (!Loading_Trucks_List.isEmpty())
				{
					ptr = Loading_Trucks_List.dequeue();
					p.enqueue(ptr->GetID());
				}
				return p;

			}
		case VTruck:
			{
				while (!Loading_Trucks_List.isEmpty())
				{
					ptr = Loading_Trucks_List.dequeue();
					p.enqueue(ptr->GetID());
				}
				return p;

			}
		case STruck:
			{
				while (Loading_Trucks_List.GetCount() != 0)
				{
					ptr = Available_SpecialTrucks_List.dequeue();
					if (!ptr->GetIsLoaded())
						p.enqueue(ptr->GetID());
				}
				return p;

			}
		}
	}

	bool IsEmpty_LoadingTrucks()
	{
		return Loading_Trucks_List.isEmpty();
	}
	int LoadingTrucksCount()
	{
		return Loading_Trucks_List.GetCount();
	}

	int MovingTrucksCount()
	{
		return Moving_Trucks_List.GetCount();
	}

	bool IsEmpty_MovingTrucks()
	{
		return Moving_Trucks_List.isEmpty();
	}
	int GetEmptyTrucks_Count()
	{
		return Available_NormalTrucks_List.GetCount() + Available_SpecialTrucks_List.GetCount() + Available_VIPTrucks_List.GetCount();
	}

	int GetInCheckupTrucks_Count()
	{
		return InCheckup_NormalTrucks_List.GetCount() + InCheckup_SpecialTrucks_List.GetCount() + InCheckup_VIPTrucks_List.GetCount();
	}
	int Get_Delivered_NormalTrucks()
	{
		return InCheckup_NormalTrucks_List.GetCount() + Available_NormalTrucks_List.GetCount();
	}
	int Get_Delivered_SpecialTrucks()
	{
		return InCheckup_SpecialTrucks_List.GetCount() + Available_SpecialTrucks_List.GetCount();
	}
	int Get_Delivered_VIPTrucks()
	{
		return InCheckup_VIPTrucks_List.GetCount() + Available_VIPTrucks_List.GetCount();
	}

};
