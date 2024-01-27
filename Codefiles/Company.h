#pragma once
#include "iostream"
#include "fstream"
#include "ostream"
#include "CargoLists.h"
#include "TruckLists.h"
#include "Event.h"
#include "Truck.h"
#include "PreparationEvent.h"
#include "CancelEvent.h"
#include "PromoteEvent.h"
#include "string"
#include "UI.h"
#include<windows.h>

class Company
{
private:
	Queue<Event*> EventList_pointers;			//List of event pointers
	CargoLists Cargo_List;						//List of Cargos
	TruckLists Truck_List;						//List of Trucks
	UI UI_Object;								//Object of UIs
	int curr_H;									//The current date(hour)
	int curr_D;									//The current date(days)
public:

	/********************* Constructor *********************/
	Company()
	{
		curr_H=0;	curr_D=0;	LoadFile();
	}


	/********************* The Main Function *********************/
	void Simulator()
	{
		int mode=UI_Object.userInput_Mode();
		UI_Object.userInput_Proceed(); //Wait for a click
		Event* tmp = EventList_pointers.dequeue(); //tmp = the first event
		while (!EventList_pointers.isEmpty() || !Cargo_List.IsEmpty_WaitingList(N) || !Cargo_List.IsEmpty_WaitingList(S) || !Cargo_List.IsEmpty_WaitingList(V) || !Truck_List.IsEmpty_LoadingTrucks() || !Truck_List.IsEmpty_MovingTrucks())
		{

			//--------------------
			//Execute the events that should be executed at that hour
			while (tmp&&tmp->GetEventTime_H() == curr_H && tmp->GetEventTime_D() == curr_D)
			{
				tmp->Execute(Cargo_List);
				tmp = EventList_pointers.dequeue();
			}
			//--------------------
			Truck_List.Check_Cargo_IsCancelled(Truck_List);
			//Assigning The Cargos:
			if(curr_D==2 && curr_H==18)
				curr_D=curr_D;
			Truck_List.Check_LoadingToMoving(Truck_List,curr_H,curr_D);

			if(curr_H<5 | curr_H>23)
			{
				Truck_List.Check_PauseLoading_OffHours();
			}
			bool Continue= true;
			while(Continue)
			{
				Continue = Truck_List.Assign_TruckLists(Cargo_List,Truck_List,V,curr_H,curr_D); // Assigning all Available VIP cargos 
			}
			Continue= true;
			while(Continue)
			{
				Continue = Truck_List.Assign_TruckLists(Cargo_List,Truck_List,S,curr_H,curr_D); // Assigning all Available Special cargos 
			}
			Continue= true;
			while(Continue)
			{
				Continue = Truck_List.Assign_TruckLists(Cargo_List,Truck_List,N,curr_H,curr_D); // Assigning all Available Normal cargos 
			}
			Cargo_List.Check_AutoPormote_CargoLists(curr_H,curr_D);
			Truck_List.CheckEmptyToLoading(Truck_List,curr_H,curr_D,Cargo_List);
			//--------------------
			// Check Every Truck.

			Truck_List.CheckCargo_MovingToDelivered(curr_H,curr_D,Cargo_List); // ALL THE TIME
			Truck_List.Check_MovingToEmptyORCheckup(Truck_List,curr_H,curr_D); // ALL THE TIME
			Truck_List.Check_CheckupToAvailable(Truck_List,curr_H,curr_D); // ALL THE TIME
			
				//Pick one cargo from each cargo type and move it to moving cargo list(s).
			
				if (mode!=3)
					UI_Object.Print_currH(curr_H, curr_D, Cargo_List,Truck_List);

			if (mode==1)
				UI_Object.userInput_Proceed(); //Wait for a click
			else if (mode==2)
			{
				Sleep(1000);
				UI_Object.endline();
			}

			curr_H++;
			if (curr_H == 24)
			{
				curr_H = 0;
				curr_D++;//new day
			}
		}
		UI_Object.SimlationEndsMessage();
	}


	/********************* Input & Output Files *********************/
	void LoadFile()
	{
		Event*Eventptr;
		int NumNTrucks, NumSTrucks, NumVTrucks;
		int Speed_NTrucks, Speed_STrucks, SpeedVTrucks;
		int TC_NTrucks, TC_CSTrucks, TC_CVTrucks;
		int JourneysBeforeCheckup, CheckupDuration_NTrucks, CheckupDuration_STrucks, CheckupDuration_VTrucks;
		int NumofEvents, AutoPromotionLimit, MaxW;
		ifstream inputfile;
		string s;
		UI_Object.Print_GetFileName(s);
		inputfile.open(s);
		inputfile >> NumNTrucks >> NumSTrucks >> NumVTrucks;
		inputfile >> Speed_NTrucks >> Speed_STrucks >> SpeedVTrucks;
		inputfile >> TC_NTrucks >> TC_CSTrucks >> TC_CVTrucks;
		inputfile >> JourneysBeforeCheckup >> CheckupDuration_NTrucks >> CheckupDuration_STrucks >> CheckupDuration_VTrucks;
		inputfile >> AutoPromotionLimit >> MaxW; //AutoPromotionLimit For Normal Cargos Only,MaxW For All Cargos 
		inputfile >> NumofEvents;
		Cargo_List.SetMaxW(MaxW);
		// Creating The Trucks:
		int j=1;
		Truck_List.AddTrucks(Speed_NTrucks, CheckupDuration_NTrucks, TC_NTrucks, JourneysBeforeCheckup, NumNTrucks,NTruck,j);
		Truck_List.AddTrucks(Speed_STrucks, CheckupDuration_STrucks, TC_CSTrucks, JourneysBeforeCheckup, NumSTrucks,STruck,j);
		Truck_List.AddTrucks(SpeedVTrucks, CheckupDuration_VTrucks, TC_CVTrucks, JourneysBeforeCheckup, NumVTrucks,VTruck,j);
		// Creating The Events:
		for (int i = 0; i < NumofEvents; i++)
		{
			char x, type;
			string time;
			int  id, dist, LT, Cost, extra;
			inputfile >> x;
			switch (x)
			{
			case 'R':
				{
					inputfile >> type >> time >> id >> dist >> LT >> Cost;
					Eventptr = new PreparationEvent(time, id, LT, type, dist, Cost, AutoPromotionLimit);
					EventList_pointers.enqueue(Eventptr);
					break;
				}
			case'X':
				{
					inputfile >> time >> id;
					Eventptr = new CancelEvent(time, id);
					EventList_pointers.enqueue(Eventptr);
					break;
				}
			case 'P':
				{
					inputfile >> time >> id >> extra;
					Eventptr = new PromoteEvent(time, id, extra);
					EventList_pointers.enqueue(Eventptr);
					break;
				}
			default:
				break;
			}
		}
	}


	void LoadFileBonus()
	{
		Event*Eventptr;
		int NumNTrucks, NumSTrucks, NumVTrucks;
		int Speed_NTrucks, Speed_STrucks, SpeedVTrucks;
		int TC_NTrucks, TC_CSTrucks, TC_CVTrucks;
		int JourneysBeforeCheckup, CheckupDuration_NTrucks, CheckupDuration_STrucks, CheckupDuration_VTrucks;
		int NumofEvents, AutoPromotionLimit, MaxW;
		ifstream inputfile;
		string s;
		UI_Object.Print_GetFileName(s);
		inputfile.open(s);
		inputfile >> NumNTrucks >> NumSTrucks >> NumVTrucks;

		inputfile >> JourneysBeforeCheckup >> CheckupDuration_NTrucks >> CheckupDuration_STrucks >> CheckupDuration_VTrucks;
		inputfile >> AutoPromotionLimit >> MaxW; //AutoPromotionLimit For Normal Cargos Only,MaxW For All Cargos 
		Cargo_List.SetMaxW(MaxW);
		int j=1;
		for(int i=0;i<NumNTrucks;i++)
		{
		inputfile >> Speed_NTrucks >> TC_NTrucks;
		Truck_List.AddTrucks(Speed_NTrucks, CheckupDuration_NTrucks, TC_NTrucks, JourneysBeforeCheckup, 1,NTruck,j);
		}
		for(int i=0;i<NumVTrucks;i++)
		{
			inputfile >> SpeedVTrucks >> TC_CVTrucks;
		Truck_List.AddTrucks(SpeedVTrucks, CheckupDuration_VTrucks, TC_CVTrucks, JourneysBeforeCheckup, 1,VTruck,j);
		}
		for(int i=0;i<NumSTrucks;i++)
		{
			inputfile >> Speed_STrucks >> TC_CSTrucks;
		Truck_List.AddTrucks(Speed_STrucks, CheckupDuration_STrucks, TC_CSTrucks, JourneysBeforeCheckup, 1,STruck,j);
		}
		// Creating The Trucks:
		// Creating The Events:
		inputfile >> NumofEvents;
		for (int i = 0; i < NumofEvents; i++)
		{
			char x, type;
			string time;
			int  id, dist, LT, Cost, extra;
			inputfile >> x;
			switch (x)
			{
			case 'R':
				{
					inputfile >> type >> time >> id >> dist >> LT >> Cost;
					Eventptr = new PreparationEvent(time, id, LT, type, dist, Cost, AutoPromotionLimit);
					EventList_pointers.enqueue(Eventptr);
					break;
				}
			case'X':
				{
					inputfile >> time >> id;
					Eventptr = new CancelEvent(time, id);
					EventList_pointers.enqueue(Eventptr);
					break;
				}
			case 'P':
				{
					inputfile >> time >> id >> extra;
					Eventptr = new PromoteEvent(time, id, extra);
					EventList_pointers.enqueue(Eventptr);
					break;
				}
			default:
				break;
			}
		}
	}

	void Output_File()
	{
		Queue<Cargo*>Temp = Cargo_List.Get_Delivered_List();
		ofstream OutputFile;
		string s;
		Cargo*Ptr;
		UI_Object.GetOutputFileName(s);
		OutputFile.open(s);
		//Information Of Delivered Cargos
		while (!Temp.isEmpty())
		{
			Ptr = Temp.dequeue();
			OutputFile << Ptr->GetDeliveryTime_D() << ":" << Ptr->GetDeliveryTime_H() << " ";
			OutputFile << Ptr->GetID() << " " << Ptr->GetPT_Day() << ":" << Ptr->GetPT_Hour() << " ";
			OutputFile << Ptr->WaitingTime()/24 <<":"<< Ptr->WaitingTime()%24 << " "; 
			OutputFile << Ptr->Get_TruckID() << "\n";
		}
		//Stastices
		OutputFile << "Cargos"<<" " << Cargo_List.GetDeliveredCargos_Count() << " " << "[N:" << Cargo_List.GetCount_Delivered_Normal() << ",S:" << Cargo_List.GetCount_Delivered_Special() << ",V:" << Cargo_List.GetCount_Delivered_Vip() << "]" << "\n";
		OutputFile << "Cargo Avg Wait =" << Cargo_List.Avg_WaitTime()/24 << ":" << Cargo_List.Avg_WaitTime()%24 << "\n";
		OutputFile << "Auto_Pormoted Cargos:" << Cargo_List.Auto_Promoted_Percentage() << "%" << "\n";
		OutputFile << "Trucks:" << Truck_List.GetEmptyTrucks_Count() + Truck_List.GetInCheckupTrucks_Count() << " " << "[N:" << Truck_List.Get_Delivered_NormalTrucks() << ",S:" << Truck_List.Get_Delivered_SpecialTrucks() << ",V:" << Truck_List.Get_Delivered_VIPTrucks() << "]" << "\n";
		OutputFile << "Avg Active time=" << (Truck_List.Total_Active_Time() * 100) / (curr_D * 24 + curr_H) << "%" << "\n";
		OutputFile << "Avg Utilization=" << Truck_List.Avg_TruckUtilization(curr_D * 24 + curr_H) << "%" << "\n";
		OutputFile.close();
	}

	~Company()
	{
			Output_File();
	}

};

