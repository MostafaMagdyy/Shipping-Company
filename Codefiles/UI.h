#pragma once
#include <iostream>
#include <string>
using namespace std;
class UI
{
public:
	//____________________________________________________________________________________________________________________//
	// ===================== USER INPUT ===================== //
	int userInput_Mode()
	{
		int x;
		cout<<"===== Please Select Program Mode ===="<<endl;
		cout<<"Enter 1 : Interactive Mode"<<endl;
		cout<<"Enter 2 : Step-By-Step Mode"<<endl;
		cout<<"Enter 3 : Silent Mode"<<endl;
		cin>>x;
		while (x<1 || x>3)
		{
			cout<<"Error: Please Enter A Value From 1 To 3"<<endl;
			cin>>x;
		}

		if (x==1)
		{
			cout<<"-------------------- Interactive Mode --------------------"<<endl<<"Please Press Enter Every Hour To Proceed "<<endl<<" Simulation Starts... "<<endl<<endl;
		}
		else if(x==2)
		{
			cout<<"-------------------- Step-By-Step Mode --------------------"<<endl<<"Output of Every Hour Will be Printed Every Second "<<endl<<" Simulation Starts... "<<endl<<endl;
		}
		else if(x==3)
		{
			cout<<"-------------------- Silent Mode --------------------"<<endl<<" Simulation Starts... "<<endl<<endl;	
		}
		return x;
	}

	void userInput_Proceed()
	{
		while (cin.get()!='\n');
	}

	void userInput_string(string &s)
	{
		cin>>s;
	}

	//____________________________________________________________________________________________________________________//
	// ===================== Interface Output ===================== //

	void Print_GetFileName(string &s)
	{
		cout << "Please Enter The Input File Name:" << " ";
		userInput_string(s);
	}

	void endline()
	{
		cout<<endl;
	}

	void SimlationEndsMessage()
	{
		cout<<"Simulation ends, Output File Created"<<endl;
	}



	void Print_currH(int curr_H,int curr_D,CargoLists Cargo_List,TruckLists T)
	{
		cout<<"Current Time (Day:Hour):"<<curr_D<<":"<<curr_H<<endl;
		PrintWaitingCargosList(Cargo_List.GetWaitingCargos_Count(),Cargo_List);
		cout<<endl;
		PrintLoadingTrucksList(T.LoadingTrucksCount(),T);
		cout<<endl;
		PrintEmptyTrucksList(T.GetEmptyTrucks_Count(),T);
		cout<<endl;
		PrintMovingTrucksList(T.MovingTrucksCount(),T);
		cout<<endl;
		PrintInCheckupTrucksList(T.GetInCheckupTrucks_Count(),T);
		cout<<endl;
		PrintDeliveredCargosList(Cargo_List.GetDeliveredCargos_Count(),Cargo_List);
		cout<<endl;
	}

	// ----- Side-Functions For Interface Output ----- //
	void PrintWaitingCargosList(int WaitingCargoList_Count, CargoLists Cargo_List)
	{
		cout<<WaitingCargoList_Count<<" Waiting Cargos: [";
		CargoLists tmp_Cargo_List = Cargo_List;
		Queue<int> tmp = tmp_Cargo_List.Get_WaitingCargos_IDs_List(N);
		while(!tmp.isEmpty())
		{
			cout<<tmp.dequeue(); 
			if (tmp.GetCount()>0)
				cout<<",";
		}
		cout<<"] (";

		tmp = tmp_Cargo_List.Get_WaitingCargos_IDs_List(S);
		while(!tmp.isEmpty())
		{
			cout<<tmp.dequeue(); 
			if (tmp.GetCount()>0)
				cout<<",";
		}
		cout<<") {";

		tmp = tmp_Cargo_List.Get_WaitingCargos_IDs_List(V);
		while(!tmp.isEmpty())
		{
			cout<<tmp.dequeue(); 
			if (tmp.GetCount()>0)
				cout<<",";
		}
		cout<<"}";
	}

	
	void PrintEmptyTrucksList(int EmptyTrucksList_Count, TruckLists Truck_List)
	{
		cout<<EmptyTrucksList_Count<<" Empty Trucks: [";
		TruckLists tmp_TruckList = Truck_List;
		Queue<int> tmp = tmp_TruckList.Get_EmptyTrucks_IDs_List(NTruck);
		while(!tmp.isEmpty())
		{
			cout<<tmp.dequeue(); 
			if (tmp.GetCount()>0)
				cout<<",";
		}
		cout<<"] (";
		tmp_TruckList = Truck_List;
		tmp = tmp_TruckList.Get_EmptyTrucks_IDs_List(STruck);
		while(!tmp.isEmpty())
		{
			cout<<tmp.dequeue(); 
			if (tmp.GetCount()>0)
				cout<<",";
		}
		cout<<") {";
		tmp_TruckList = Truck_List;
		tmp = tmp_TruckList.Get_EmptyTrucks_IDs_List(VTruck);
		while(!tmp.isEmpty())
		{
			cout<<tmp.dequeue(); 
			if (tmp.GetCount()>0)
				cout<<",";
		}
		cout<<"}";
	}
	void GetOutputFileName(string&s)
	{
		cout << "Please Enter The Output File Name" << " ";
		cin >> s;
	}
	void PrintInCheckupTrucksList(int InCheckupTrucksList_Count, TruckLists Truck_List)
	{
		cout<<InCheckupTrucksList_Count<<" In-Checkup Trucks: [";
		TruckLists tmp_TruckList = Truck_List;
		Queue<int> tmp = tmp_TruckList.Get_InCheckupTrucks_IDs_List(NTruck);
		while(!tmp.isEmpty())
		{
			cout<<tmp.dequeue(); 
			if (tmp.GetCount()>0)
				cout<<",";
		}
		cout<<"] (";
		tmp_TruckList = Truck_List;
		tmp = tmp_TruckList.Get_InCheckupTrucks_IDs_List(STruck);
		while(!tmp.isEmpty())
		{
			cout<<tmp.dequeue(); 
			if (tmp.GetCount()>0)
				cout<<",";
		}
		cout<<") {";
		tmp_TruckList = Truck_List;
		tmp = tmp_TruckList.Get_InCheckupTrucks_IDs_List(VTruck);
		while(!tmp.isEmpty())
		{
			cout<<tmp.dequeue(); 
			if (tmp.GetCount()>0)
				cout<<",";
		}
		cout<<"}";
	}

	void PrintLoadingTrucksList(int LoadingTruckList_Count, TruckLists Truck_List)
	{
		cout<<LoadingTruckList_Count<<" Loading Trucks: ";
		Truck* ptr = Truck_List.DequeueTruck_Loading();
		Queue<int> tmp;
		while(ptr!= nullptr)
		{
			switch (ptr->GetTruckType())
			{
			case NTruck:
				{
					{
						{
							cout<<ptr->GetID()<<"[";
							tmp = ptr->Get_Cargo_List(*ptr);
							while(!tmp.isEmpty())
							{
								cout<<tmp.dequeue(); 
								if (tmp.GetCount()>0)
									cout<<",";
							}
							cout<<"] ";
						}
					}
					break;
				}			
			case VTruck:
				{
					{
						{
							cout<<ptr->GetID()<<"{";
							tmp = ptr->Get_Cargo_List(*ptr);
							while(!tmp.isEmpty())
							{
								cout<<tmp.dequeue(); 
								if (tmp.GetCount()>0)
									cout<<",";
							}
							cout<<"} ";
						}
					}
					break;
				}			
			case STruck:
				{
					{
						{
							cout<<ptr->GetID()<<"(";
							tmp = ptr->Get_Cargo_List(*ptr);
							while(!tmp.isEmpty())
							{
								cout<<tmp.dequeue(); 
								if (tmp.GetCount()>0)
									cout<<",";
							}
							cout<<") ";
						}
					}
				}
				break;
			}
			ptr = Truck_List.DequeueTruck_Loading();
		}
	}


	void PrintMovingTrucksList(int MovingTruckList_Count, TruckLists Truck_List)
	{
		cout<<MovingTruckList_Count<<" Moving Trucks: ";
		Truck* ptr = Truck_List.DequeueTruck_Moving();
		Queue<int> tmp;
		while(ptr!= nullptr)
		{
			switch (ptr->GetTruckType())
			{
			case NTruck:
				{
					{
						{
							cout<<ptr->GetID()<<"[";
							tmp = ptr->Get_Cargo_List(*ptr);
							while(!tmp.isEmpty())
							{
								cout<<tmp.dequeue(); 
								if (tmp.GetCount()>0)
									cout<<",";
							}
							cout<<"] ";
						}
					}
					break;
				}			
			case VTruck:
				{
					{
						{
							cout<<ptr->GetID()<<"{";
							tmp = ptr->Get_Cargo_List(*ptr);
							while(!tmp.isEmpty())
							{
								cout<<tmp.dequeue(); 
								if (tmp.GetCount()>0)
									cout<<",";
							}
							cout<<"} ";
						}
					}
					break;
				}			
			case STruck:
				{
					{
						{
							cout<<ptr->GetID()<<"(";
							tmp = ptr->Get_Cargo_List(*ptr);
							while(!tmp.isEmpty())
							{
								cout<<tmp.dequeue(); 
								if (tmp.GetCount()>0)
									cout<<",";
							}
							cout<<") ";
						}
					}
				}
				break;
			}
			ptr = Truck_List.DequeueTruck_Moving();
		}
	}



		void PrintDeliveredCargosList(int DeliveredCargoList_Count, CargoLists Cargo_List)
		{
			cout<<DeliveredCargoList_Count<<" Delivered Cargos: [";
			Queue<int> tmp = Cargo_List.Get_DeliveredCargos_IDs_List(N,Cargo_List);
			while(!tmp.isEmpty())
			{
				cout<<tmp.dequeue(); 
				if (tmp.GetCount()>0)
					cout<<",";
			}
			cout<<"] (";

			tmp = Cargo_List.Get_DeliveredCargos_IDs_List(S,Cargo_List);
			while(!tmp.isEmpty())
			{
				cout<<tmp.dequeue(); 
				if (tmp.GetCount()>0)
					cout<<",";
			}
			cout<<") {";

			tmp = Cargo_List.Get_DeliveredCargos_IDs_List(V,Cargo_List);
			while(!tmp.isEmpty())
			{
				cout<<tmp.dequeue(); 
				if (tmp.GetCount()>0)
					cout<<",";
			}
			cout<<"}";
		}

	};

