//**************************************************************************************************************************************
//	CODE FILENAME:	LaraAssignWeek1.cpp
//
//	DESCRIPTION:	This program will read two data files.  One data file is full of
//					vehicle data which will be placed in two separate stacks - cars & vans.
//					while the second file will be read line by line.  The second file contains
//					info on either how many passengers are in need of a vehicle OR info on a
//					vehicle being returned.  The program must assign vehicles to the groups
//					in the most optimal fashion. When the stacks are empty, any additional
//					passengers will be added to a queue data structure.  If a return vehicle
//					is read, then it will pick up any passengers in the queue (to its max capacity).
//					Otherwise, if no passengers are in teh queue, the returned vehicle is 
//					placed on its proper stack.  Program ends when there is no more data to 
//					be read from the second file. 
//
//	CLASS/TERM:	CS372 - Advance programming and Algorithm / 15SU
//
//	DESIGNER:	Jose Lara
//
//		Functions:	Main Function.
//					ReadFiles - checks for files to read returns true with success
//					CreateHead - creates head of list for car stack and van stack
//					CreateStacks - initiates stacks by separating cars and vans into different linked lists
//					PushStack - places additional node of autoRec struct onto stack
//					PopStack - removes a node from a stack
//					IsEmptyStack - checks for nodes on stack
//					PushReport - outputs report when pushing nodes to stack
//					PopReport - outputs a report when popping nodes from a stack
//					DisplayLotData - outputs a report stating how many of each vehicle is on the stack
//					AssignVehicle - assigns a vehicle to the next group of employees from the groupList text file			   
//					CreateQueue - creates a head for the queue
//					IsEmptyQueue - checks for employees waiting in the queue
//					EnQueue - add a group of employees to the queue
//					DeQueue - remove a group of employees from the queue
//					QueueCount - return the number of people in the queue after all files read
//					AssignReturn - returned vehicles are either assigned to a stack or take a group from the queue
// 					LotStatusReport - Creates a new file where the return vehicles to the lot are store. 
//**************************************************************************************************************************************
#include <iostream>     // Header is to access the cin and cout objects.
#include <iomanip>      // use for setw() func
#include <string>		// Header for String usage
#include <fstream>		// ifstream and oftream in ReadData function and LotStatusReport function
#include <cstdlib>

using namespace std;	// this is to avoid the std prefix when writting the code.

// constant variables
const int   CAR_MAX = 4,    // maximum number of people in car
            VAN_MAX = 8;    // maximum number of people in van


// struct for vehicles node
struct autoRec{
	char type;		// auto type = 'A' || van type = 'v'
	string plate;	// license plate
	int capacity;	// number of passengers
	autoRec* next;	// next node
};
// struct for vehicle list heads
struct dataList{
	int count;		// number of nodes in list
	autoRec* head;  // head pointer for stacks
};

//struct for passenger queue
struct groupRec{
	int members;		//number of people in group
	groupRec* next;		// next node
};

// struct for queue head
struct queueType{
	groupRec* front;	// points to the front of queue
	groupRec* back;		// points to the back of queue
};
//******************************************************************************
// Functions prototype
//******************************************************************************
bool ReadFile(ifstream& vehicleList, ifstream& groupList, string str1, string str2);
dataList* CreateHead();
void CreateStacks(dataList* carStack, dataList* vanStack, ifstream& vehicleList);
void PushStack(autoRec* newNode, dataList* &topOfList);
void PushReport(autoRec* newNode);
void PopStack(dataList* &topOfList);
void PopReport(autoRec* &newNode);
bool IsEmptyStack(dataList* topOfList);
void DisplayLotData(dataList* carStack, dataList* vanStack);
queueType* CreateQueue();
void EnQueue(int people, queueType* &listFront);
void DeQueue(queueType* &listFront);
bool IsEmptyQueue(queueType* listFront);
void QueueCount(queueType* listFront);
void AssignVehicle(ifstream& groupList,dataList* &carTop, dataList* &vanTop, queueType* &passengers);
void AssignReturn(ifstream& groupList,dataList* &carTop, dataList* &vanTop, queueType* &passengers);
void LotStatusReport(char type, string plate);

//******************************************************************************
//  Function - Main
//  Description:    make calls to create the stacks, and the queue. also assings
//                  variables to stor the data from the file. A loop is used for
//                  calling the functions and checks the file being read. it then
//                  outputs a report for how many vehicles and passengers are left
//
//  Input:  the parameters in main are use to import the files for vehicles.txt and
//          checkoutData.txt as arguments
//
//  Output: it returns 0 if program finish succefully, and 1 for file error
//
//  Functions used: ReadFiles, DisplayLotData, QueueCount, AssignReturn, CreateStacks
//                  CreateHead, AssignReturn, CreateQueue
//******************************************************************************
int main(int argc, const char* argv[]){

	bool checkFile;                 //  use for testing the text files were read good
	char ch;                        //  use to read checkouts || returns

	ifstream 	vehicleList,        //  to read data from vehicle.txt
				groupList;          //  to read data from checkoutData.txt

	dataList* carStack;             //  declares car list
	dataList* vanStack;             //  declares van list
	queueType* passengers;          //  declares queue passenger list
    
    // checks for file
	checkFile = ReadFile(vehicleList, groupList, argv[1], argv[2]);
    
    cout << endl; // make a space to output code

    // exit program if does not exist
    if(!checkFile){
        cout << "File Error - Cannot read the file." << endl;
        cin.get();

        return 1; //end program
    }
    
    carStack = CreateHead();        //create linked lists for stack of cars
    vanStack = CreateHead();        //create linked lists for stack of vans
    passengers = CreateQueue();     //create linked lists for queue of passengers
    
    // Puts the data onto the corresponding stack
    CreateStacks(carStack, vanStack, vehicleList);

    groupList >> ch; // primes read the file

    while(groupList){
        if(ch == 'C')
            AssignVehicle(groupList, carStack, vanStack, passengers);   // checkout a car  from the stack
        
        else
            AssignReturn(groupList,carStack, vanStack, passengers);      // returns vehicle to the stack
	
        groupList >> ch;
	}
    
    //display how many vehicles remain
    DisplayLotData(carStack, vanStack);
    
    //display how many passengers were not assigned a vehicle
    QueueCount(passengers);


	system("PAUSE");
	return 0;
    
} // end of main

//******************************************************************************
//  Function - ReadFiles;
//  Description: checks for successful read of text files by the program
//
//  Input:  vehicleList, groupList and two strings representing argv[1] and argv[2]
//  Output: a boolean value
//  Functions used: none
//******************************************************************************

bool ReadFile(ifstream& vehicleList, ifstream& groupList, string str1, string str2){

	bool isFileGood;

	vehicleList.open(str1.c_str());
	groupList.open(str2.c_str());

	if(vehicleList &&  groupList)
		isFileGood = true;
	else
		isFileGood = false;

	return isFileGood;
    
} // end of ReadFile

//******************************************************************************
//  Function - CreateStacks;
//  Description: creates two linked lists - list of cars and list of vans
//
//  Input: carStack, vanStack, vehicleList
//  Output:
//  Function used: PushStack, DisplayLotData
//******************************************************************************
void CreateStacks(dataList* carStack, dataList* vanStack, ifstream& vehicleList){

	char ch;	// stores 'A' for auto or 'V' for van
	string str; // stores the license plate
	int num; 	// stores the vehicle capacity

	vehicleList >> ch;

	while(vehicleList){

		autoRec *newNode = new (nothrow) autoRec;

		if(newNode == NULL)
			cout << "Error - out of memory" << endl;

		else{

			//retrieve the rest of the data
			vehicleList >> str;
			vehicleList >> num;

			//assigns data to struct values
			newNode->type = ch;
			newNode->plate = str;
			newNode->capacity = num;

			//initialize newNode pointer to null
			newNode->next = NULL;
		}

		if(ch == 'A')
			PushStack(newNode, carStack);  // add newNode to carStack
		else
			PushStack(newNode, vanStack);  // add newNode to vanStack

	vehicleList >> ch;
	}

	DisplayLotData(carStack, vanStack);
    
} // end of CreateStack

//******************************************************************************
//  Function - CreateHead;
//  Description: creates head pointer with a counter
//
//  Input:
//  Output: a head pointer
//  Function used:  none
//******************************************************************************
dataList* CreateHead(){

	// allocate new memory and checks if available
	dataList* newStackList = new(nothrow) dataList;

	if(newStackList == NULL)
		cout << "Error - out of memory Space!" << endl;

	else{
		newStackList->count = 0;
		newStackList->head = NULL;     // assigns a null value to head of stack
	}

	return newStackList;

} // end of CreateStack

//******************************************************************************
//  Function - PushStack;
//  Description: adds a node to the top of stack
//
//  Input: the newNode and (carStack or vanStack)
//  Output:
//  Function used: IsEmptyStack, PushReport
//******************************************************************************
void PushStack(autoRec* newNode, dataList* &topOfList){

	bool empty = IsEmptyStack(topOfList);

	PushReport(newNode);

	if(empty)
		topOfList->head = newNode;

	else{
		newNode->next = topOfList->head;
		topOfList->head = newNode;
	}

	topOfList->count++;

} // end of PushStack

//******************************************************************************
//  Function - PushReport;
//  Description:   it outputs the vehicle being added to stacks and outputs
//                 a message telling the user which stack the vehicle is being added to
//  Input: carStack or vanStack
//  Output:
//  Function used: none
//******************************************************************************
void PushReport(autoRec* newNode){

	char ch = newNode->type;

	if(ch == 'A')
		cout << "Placing " << newNode->plate << " into automobile area " << endl;
	else
		cout << "Placing " << newNode->plate << " into van area. " << endl;

} // end of PushReport

//******************************************************************************
//  Function - PopReport;
//  Description: it outputs the vehicle to remove from stack and outputs a message
//             telling the user the vehicle type/plate number/capacity
//  Input: carStack or vanStack
//  Output: the data of the vehicle being taken from the stack
//  Function used: none
//******************************************************************************
void PopReport(dataList* &topOfList){

	char ch = topOfList->head->type;

	if(ch == 'A')
		cout << setw(15) << "Automobile " << topOfList->head->plate << " holds "<<topOfList->head->capacity << " passengers." << endl;
	else
		cout << setw(15) << "Van " << topOfList->head->plate << " holds "<< topOfList->head->capacity << " passengers." << endl;
    
} // end of PopReport

//******************************************************************************
//  Function - PopStack;
//  Description: removes a node from stack
//
//  Input: carStack or vanStack
//  Output:
//  Function used: PopReport
//******************************************************************************
void PopStack(dataList* &topOfList){

	dataList* temp = new (nothrow) dataList;

    int num = topOfList->count;

    PopReport(topOfList);

	if(num == 1){
        
        temp->head = topOfList->head;
        topOfList->head = NULL;
        delete temp->head;
	}
	else{
		temp->head = topOfList->head;
		topOfList->head = topOfList->head->next;

		delete temp->head;
	}

	topOfList->count--; //decread
    
} // end of PopStack

//******************************************************************************
//  Function - IsEmptyStack;
//  Description: checks to see if stacks are empty
//
//  Input: carStack or vanStack
//  Output: true/false
//  Function calls: none.
//******************************************************************************
bool IsEmptyStack(dataList* topOfList){
    bool flag;
    
    if (topOfList == NULL)
        flag = true;
    else
        flag = false;
    
    return flag;
    
} // end of isEmptyStack

//******************************************************************************
//  Function - createQueue
//  Description: creates head pointer for a queue
//
//  Input:
//  Output: a head pointer
//  Function used: none
//******************************************************************************
queueType* CreateQueue(){
    
    char ch;

	// allocate new memory and checks if available
	queueType *newQueue = new (nothrow) queueType;

    if(newQueue == NULL){
        cout << "Error - out of memory!" << endl;
        cout << "Press C to continue: ";
        cin >> ch;
    }

	else
	{
		//initial values for the queue
		newQueue->front = NULL;
		newQueue->back = NULL;
	}

	return newQueue;

} // end of createQueue

//******************************************************************************
//  Function - EnQueue
//  Description: places new nodes of groups waiting for vehicles into a linked list
//
//  Input: people(number of people in group), passengers (list head for queue)
//  Output:
//  Function used: IsEmptyQueue
//******************************************************************************

void EnQueue(int people, queueType* &listFront){
    char ch;
    bool empty = IsEmptyQueue(listFront);

    groupRec *newNode = new (nothrow) groupRec;

    if(newNode == NULL){
        cout << "Error - out of memory!" << endl;
        cout << "Press C to continue: ";
        cin >> ch;
    }

    else{
        cout << endl << people << " employees waiting for a van" << endl;

        newNode->members = people;
        newNode->next = NULL;

        if(empty){
            listFront->back->next = newNode;
            listFront->back = newNode;
        }
        else{
            listFront->back->next = newNode;
            listFront->back = newNode;
        }
    }

} // end of Enqueue

//******************************************************************************
//  Function - DeQueue
//  Description: removes nodes from the queue
//
//  Input: passengers (list head for queue)
//  Output:
//  Function used: none
//******************************************************************************
void DeQueue(queueType* &listFront){

    bool empty = IsEmptyQueue(listFront);

    groupRec *temp = new (nothrow) groupRec;

    if(empty){
        cout << "List is Empty, can't remove." << endl;
    }
    else{
        temp->next = listFront->front;
		listFront->front = listFront->front->next;
	
		delete temp->next;

    }

} // end of DeQueue

//******************************************************************************
//  Function - IsEmptyQueue
//  Description: checks for extant nodes in a queue
//
//  Input: passengers (list head for queue)
//  Output:
//  Function calls:
//******************************************************************************
bool IsEmptyQueue(queueType* listFront){

    bool flag;

    if(listFront->front == NULL)
        flag = true;
    else
        flag = false;

    return flag;


} // end of isEmptyQueue

//******************************************************************************
//  Function - QueueCount
//  Description: counts members of each node and deletes that node
//
//  Input: listFront(head of queue)
//  Output: prints the number of people still waiting for a vehicle
//  Function used:
//******************************************************************************
void QueueCount(queueType* listFront){
    
    int total = 0;
    
    while(listFront->front != NULL){
        
        groupRec* temp = new groupRec;
        temp->next = listFront->front;
        
        total += listFront->front->members;
        
        listFront->front = listFront->front->next;
        
        delete temp;
        
    }
    
    listFront->front = NULL;
    
    if(total)
        cout << total << " passengers remain in the queue." << endl;
    
} // end of QueueCount

//******************************************************************************
//  Function - AssignVehicle;
//  Description: assigns vehicles to groups until both stacks are empty - when both
//             stacks are empty, remaining employees are placed in a queue
//  Input: groupList, carStack, vanStack, passengers
//  Output:
//  Function used:  IsEmptyStack, PopStack, EnQueue
//******************************************************************************
void AssignVehicle(ifstream& groupList,dataList* &carTop,dataList* &vanTop, queueType* &passengers){
    
    int people,     // members in the current group
        transport;  // how many total vehicles available from stacks

    bool    emptyVanStack,
            emptyCarStack;

    transport = carTop->count + vanTop->count;  // total people to transport
    
    // read file for employees
    groupList >> people;

    if(transport && people)
        cout << "for " << people << " employess in group, the vehicles checked out are: " << endl;

    while(transport && people){
        emptyVanStack = IsEmptyStack(vanTop);
        emptyCarStack = IsEmptyStack(carTop);

        if((people > CAR_MAX && !emptyVanStack) || (emptyCarStack && people)){

            PopStack(vanTop);

            if(people <= VAN_MAX)
                people = 0;
            else
                people -= VAN_MAX;

            transport--;
        }
        else{
            PopStack(carTop);

            if(people <= CAR_MAX)
                people = 0;
            else
                people -= CAR_MAX;

            transport--;
        }

    }

    if(people)
        EnQueue(people, passengers);

    cout << endl;
    
} // end of AssignVehicle

//******************************************************************************
//  Function - AssignReturn
//  Description: in the second file there are sometimes vehicles being returned
//               and this function tells the program to assign any people in the
//               queue to that vehicle, but if there are no people... add to stack
//
//  Input: groupList, carStack, vanStack, and passenger Queue
//  Output:
//  Function used: IsEmptyQueue, PushStack, DeQueue
//******************************************************************************
void AssignReturn(ifstream& groupList,dataList* &carTop, dataList* &vanTop, queueType* &passengers) {
	
	bool empty = IsEmptyQueue(passengers);
    
    char ch;
	string str;
    
	int num,
        group,
        newTotal;
	
	//read data from txt file
	groupList >> ch;
	groupList >> str;
	groupList >> num;
	
	if(empty){
		
		autoRec  *newNode = new (nothrow) autoRec;
			
			if(newNode == NULL)
				cout << "Error - out of memory!!" << endl;
     			
            else {
                    //assign data to new node
					newNode->type = ch;
			    	newNode->plate = str;
			    	newNode->capacity = num;
			    	//initiate pointer to NULL
			    	newNode->next = NULL;
			    	
			    	if(newNode->type == 'A' )   //vehicle is a car
			    		PushStack(newNode, carTop);
			    	else 							
			    		PushStack(newNode, vanTop);	//vehicle is a van	    	
		        }   // end of inner else
	        	
	        	LotStatusReport(ch, str);
	}   // end of if
	
	else {
		
		group = passengers->front->members;
		
		cout << "For " << group << " employees in group, vehicles checked out are:" << endl;
		
		if(ch == 'A'){
			cout << setw(8) << "Automobile " << str << " holds " << num << " passengers." << endl<< endl;
			
				if(group <= CAR_MAX)
			 		group = 0;
				else
					group -=(CAR_MAX);
            
        } // end of inner if
			

		else{
			cout << setw(8) << "Van " << str << " holds " << num << " passengers." << endl<< endl;
				
				if(group <= VAN_MAX)
			 		group = 0;
				else
		        	group -= VAN_MAX;
            
		} // end of inner else


		newTotal = group;
					
		if(newTotal == 0)
			DeQueue(passengers);
		else
			group = newTotal;			
			
	} // end of else
	
	cout << endl;
			   	
} // end of AssignReturn

//******************************************************************************
//  Function - displayLotData;
//  Description: counts vehicles in stacks to display to user
//             
//  Input: carStack or vanStack
//  Output: number of cars and vans currently in lot
//  Function used:
//******************************************************************************
void DisplayLotData(dataList* carTop, dataList* vanTop){
	
	cout << endl;
	cout << "Parking Lot Status:" << endl;
	cout << setw(6) << carTop->count << " automobiles available" << endl;
	cout << setw(6) << vanTop->count << " vans available" << endl << endl;
    
} // end of DisplayLotData

//******************************************************************************
//  Function - LotStatusReport;
//  Description: Creates a new file where the return vehicles to the lot are store.
//             
//  Input: char and string types
//  Output: number of cars and vans currently in lot after return
//  Function used: none
//******************************************************************************
void LotStatusReport(char type, string plate){

	ofstream outFile;
	
	outFile.open("LotStatus.txt", ios::app);

    outFile << type;
    
    if(type == 'A')
        outFile << " " << plate << " " << CAR_MAX << endl;
    else
        outFile << " " << plate << " " << VAN_MAX << endl;
    
	outFile.close();
	
}// end of LotStatusReport 
