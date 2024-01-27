#pragma once
#include "GeneraList.h"
#include "PriorityQueue.h"
#include "Queue.h"
#include "Cargo.h"
class CargoLists
{
private:
	Priority_Queue<Cargo*> Waiting_VIPCargos_List;			// List of Waiting VIP Cargos
	GeneralList<Cargo*> Waiting_NormalCargos_List;			// List of Waiting Normal Cargos
	Queue<Cargo*> Waiting_SpecialCargos_List;				// List of Waiting Special Cargos
	Queue<Cargo*> Delivered_List;							// List of ALL delivered Cargos
	int DeliverdCount_Normal;								// Number of Normal delivered cargos
	int DeliverdCount_VIP;									// Number of VIP delivered cargos
	int DeliverdCount_Special;								// Number of Special delivered cargos	
	int MaxW;												// Max Wait of a cargo in hours
	int AutoPromotedCargos_Count;							// Count of Auto Promoted cargos
public:

	/********************* Constructor *********************/
	CargoLists()
	{
		DeliverdCount_Normal=0;		DeliverdCount_Special=0;	DeliverdCount_VIP=0;	AutoPromotedCargos_Count=0;		MaxW=2;
	}


	/********************* Adding New Cargo (New Events) *********************/
	void AddCargo(Cargo* ptr)															//Adds Cargo To the appropriate List
	{
		switch (ptr->GetType())
		{
		case V:
			{
				Waiting_VIPCargos_List.enqueue(ptr, ptr->GetPriority());	break;
			}
		case N:
			{
				Waiting_NormalCargos_List.InsertEnd(ptr);					break;
			}
		case S:
			{
				Waiting_SpecialCargos_List.enqueue(ptr);					break;
			}
		}
	}


	/********************* Checks *********************/
	void Check_AutoPormote_CargoLists(int curr_H,int curr_D)
	{
		GeneralList<Cargo*> tmp = Waiting_NormalCargos_List;
		while (!tmp.isEmpty())
		{
			Cargo* ptr = tmp.DeleteFirst();
			if(ptr->Check_AutoPormote(curr_H,curr_D))
			{
				PromoteCargo(ptr,0);													//Promote the cargo with 0 extra cost because it exceeded the waiting days
				AutoPromotedCargos_Count++;	
			}
		}
	}


	/********************* Assigning A Cargo *********************/
	Cargo* Assign_Cargo_CargoLists(CargoType T,CargoLists C)
	{
		Cargo* ptr=nullptr;
		switch (T)
		{
		case N:																			// we get the first unassigned cargo pointer and return it
			{
				if (!C.Waiting_NormalCargos_List.isEmpty())								// if not empty we assign
				{
					Cargo* tmp;
					while(!C.Waiting_NormalCargos_List.isEmpty())
					{
						tmp = C.Waiting_NormalCargos_List.DeleteFirst();
						if (!tmp->GetAssigned())										//if not assigned break
						{
							ptr = tmp;		break;
						}
					}
				}
				break;
			}
		case V:																			//same for V and S
			{				  
				if(!C.Waiting_VIPCargos_List.isEmpty())
				{
					Cargo* tmp;
					while(!C.Waiting_VIPCargos_List.isEmpty())
					{
						tmp = C.Waiting_VIPCargos_List.dequeue();
						if (!tmp->GetAssigned()) 
						{
							ptr = tmp;		break;
						}
					}
				}
				break;
			}
		case S:
			{							
				if(!C.Waiting_SpecialCargos_List.isEmpty())
				{
					Cargo* tmp;
					while(!C.Waiting_SpecialCargos_List.isEmpty())
					{
						tmp = C.Waiting_SpecialCargos_List.dequeue();
						if (!tmp->GetAssigned())
						{
							ptr = tmp;		break;
						}
					}
				}
				break;
			}
		}
		return ptr;
	}


	/********************* Moving A Cargo *********************/
	void MoveCargo_MovingToDeliverd(Cargo* ptr)
	{
		Delivered_List.enqueue(ptr);													//There's no moving cargo list because it's inside the truck
		switch(ptr->GetType())
		{
		case N:
			{
			DeliverdCount_Normal++;		break;
			}		
		case V:
			{
			DeliverdCount_VIP++;		break;
			}		
		case S:
			{
			DeliverdCount_Special++;	break;
			}
		}
	}

	void RemoveCargoFromWaiting(CargoType T) 
	{
		switch(T)
		{
		case N:
		{
			Waiting_NormalCargos_List.DeleteFirst();	break;
		}
		case V:
			{
			Waiting_VIPCargos_List.dequeue();			break;
			}
		case S:
			{			
			Waiting_SpecialCargos_List.dequeue();		break;
			}
		}
	}


	/********************* Statistics *********************/
	int Avg_WaitTime()
	{
		Queue<Cargo*> tmp = Delivered_List;		Cargo* ptr;		int sum=0;
		while(!tmp.isEmpty())
		{
			ptr = tmp.dequeue();
			sum += ptr->WaitingTime();													// Getting the sum of Wait time
			
		}
		return sum/(DeliverdCount_Normal+DeliverdCount_VIP+DeliverdCount_Special);		// Dividing it by the number of cargos
	}

	int Auto_Promoted_Percentage()
	{
		return (AutoPromotedCargos_Count*100)/(DeliverdCount_Normal+AutoPromotedCargos_Count);
	}


	/********************* Functions Needed in Event Classes *********************/
	Cargo* GetWaitingCargo(int ID, CargoLists tmp, CargoType Type)
	{
		Cargo* ptr;
		switch (Type)
		{
		case N:
			{
				while (tmp.Waiting_NormalCargos_List.GetCount() != 0)					//Traverses and gets the cargo with the needed id (same for below)
				{
					ptr = tmp.Waiting_NormalCargos_List.DeleteFirst();
					if (ptr->GetID() == ID)
						return ptr;
				}	break;
			}
		case V:
			{
				while (tmp.Waiting_VIPCargos_List.GetCount() != 0)
				{
					ptr = tmp.Waiting_VIPCargos_List.dequeue();
					if (ptr->GetID() == ID)
						return ptr;
				}	break;
			}
		case S:
			{
				while (tmp.Waiting_SpecialCargos_List.GetCount() != 0)
				{
					ptr = tmp.Waiting_SpecialCargos_List.dequeue();
					if (ptr->GetID() == ID)
						return ptr;
				}	break;
			}
		}
		return nullptr;																	//means no cargo found with this id
	}

	void CancelCargo(Cargo* ptr)
	{
		Waiting_NormalCargos_List.DeleteNode(ptr);
	}

	void PromoteCargo(Cargo* ptr,int extra)
	{
		if (!ptr)
			return;
		Waiting_NormalCargos_List.DeleteNode(ptr);
		ptr->SetType();
		ptr->Set_Cost(extra);															//Adding the extra cost to it
		ptr->SetAssigned(false,-1,-1);
		Waiting_VIPCargos_List.enqueue(ptr, ptr->GetPriority());						//Moving the cargo from Waiting Normal List to Waiting VIP List
	}

	void SetMaxW(int M)
	{
		MaxW=M;
	}

	int GetMaxWait()
	{
		return MaxW;
	}


	/********************* Functions Needed in UI Class & Output File *********************/
	bool IsEmpty_WaitingList(CargoType T)
	{
		switch (T)
		{
		case N:
			return Waiting_NormalCargos_List.isEmpty();
		case V:
			return Waiting_VIPCargos_List.isEmpty();
		case S:
			return Waiting_SpecialCargos_List.isEmpty();
		}
	}

	int GetWaitingCargos_Count()
	{
		return (Waiting_NormalCargos_List.GetCount() + Waiting_SpecialCargos_List.GetCount() + Waiting_VIPCargos_List.GetCount());
	}

	int GetDeliveredCargos_Count()
	{
		return (Delivered_List.GetCount());
	}


	Queue<int> Get_WaitingCargos_IDs_List(CargoType T)
	{
		Queue<int> p;	Cargo* ptr;
		switch (T)
		{
		case N:
			{
				while (!Waiting_NormalCargos_List.isEmpty())							//All cout-s should be in the ui function so we return a list of the ids to UI in a Queue<int> to print it
				{
					ptr = Waiting_NormalCargos_List.DeleteFirst();
					p.enqueue(ptr->GetID());
				}
				return p;

			}
		case V:
			{
				while (!Waiting_VIPCargos_List.isEmpty())
				{
					ptr = Waiting_VIPCargos_List.dequeue();
					p.enqueue(ptr->GetID());
				}
				return p;

			}
		case S:
			{
				while (Waiting_SpecialCargos_List.GetCount() != 0)
				{
					ptr = Waiting_SpecialCargos_List.dequeue();
					p.enqueue(ptr->GetID());
				}
				return p;

			}
		}
	}

	Queue<int> Get_DeliveredCargos_IDs_List(CargoType T, CargoLists tmp)	
	{
		Queue<int> p;	Cargo* ptr;
		while (!tmp.Delivered_List.isEmpty())											// Same as above
		{
			ptr = tmp.Delivered_List.dequeue();
			if (ptr->GetType() == T)
				p.enqueue(ptr->GetID());
		}
		return p;
	}


	Queue<Cargo*>&Get_Delivered_List()
	{
		return Delivered_List;
	}

	int GetCount_Delivered_Normal()
	{
		return  DeliverdCount_Normal;
	}

	int GetCount_Delivered_Special()
	{
		return DeliverdCount_Special;
	}

	int GetCount_Delivered_Vip()
	{
	return 	DeliverdCount_VIP;
	}
};

