/*   1. Автомобильная  стоянка  вмещает  N  машин  и  имеет одну
полосу с единственным въездом-выездом.  Если владелец приходит
за  машиной,  временно  выезжают и потом возвращаются в том же
порядке  все  машины,  загораживающие  проезд.  Если   стоянка
заполнена,     то    прибывшая    машина    уезжает.    Задана
последовательность номеров машин  с  признаками  прибытия  или
убытия.  Составить  протокол работы стоянки,  сообщая обо всех
событиях (6).

Visual studio 2022
*/
#include <iostream>
#include <fstream>

using namespace std;

const char ARRIVING_SIGN = 'A';
const char LEAVING_SIGN = 'L';
const char OVERFLOW_SIGN = 'O';

template <typename T>
struct Stack
{
	Stack(T element)
		: key(element), next(nullptr)
	{}

	T key;
	Stack<T>* next;
};

namespace StackUtils
{
	template <typename T>
	bool IsEmpty(Stack<T>* stack)
	{
		return stack == nullptr;
	}

	template <typename T>
	int Size(Stack<T>* stack)
	{
		int counter = 0;

		if (stack != nullptr)
		{
			counter++;
			counter += Size(stack->next);
		}

		return counter;
	}

	template <typename T>
	void Push(Stack<T>** root, T data)
	{
		Stack<T>* stackNode = new Stack<T>(data);
		stackNode->next = *root;
		*root = stackNode;
	}

	template <typename T>
	T Pop(Stack<T>** root)
	{
		if (IsEmpty(*root))
			throw exception("failed to pop element from empty stack");
		Stack<T>* temp = *root;
		*root = (*root)->next;
		T popped = temp->key;

		delete temp;
		return popped;
	}

	template <typename T>
	T Peek(Stack<T>* root)
	{
		if (IsEmpty(root))
			throw exception("failed to peek element from empty stack");
		return root->key;
	}
}

bool IsCarInParkingLot(Stack<string>*& parkingLot, string carNumber)
{
	if (StackUtils::IsEmpty(parkingLot))
	{
		return false;
	}

	if (StackUtils::Peek(parkingLot) == carNumber)
	{
		return true;
	}

	return IsCarInParkingLot(parkingLot->next, carNumber);
}

void CarLeaves(ostream& output, Stack<string>*& parkingLot, string carNumber)
{
	string tempCarNumber = StackUtils::Pop(&parkingLot);
	output << tempCarNumber << ' ' << LEAVING_SIGN << endl; 

	if (tempCarNumber == carNumber)
	{
		return;
	}
	CarLeaves(output, parkingLot, carNumber);

	StackUtils::Push(&parkingLot, tempCarNumber);
	output << tempCarNumber << ' ' << ARRIVING_SIGN << endl;
}

// прибытие машины
bool CarArrives(ostream& output, Stack<string>*& parkingLot, string carNumber, int parkingLotSize)
{
	if (StackUtils::Size(parkingLot) >= parkingLotSize)
	{
		output << carNumber << ' ' << OVERFLOW_SIGN << endl;
		return false;
	}

	StackUtils::Push(&parkingLot, carNumber); 
	output << carNumber << ' ' << ARRIVING_SIGN << endl; 
	return true;
}

// печать стека
template <typename T>
void PrintStack(Stack<T>* stack)
{
	Stack<T>* nextPtr = stack;
	while (!StackUtils::IsEmpty(nextPtr))
	{
		cout << StackUtils::Peek(nextPtr) << " ";
		nextPtr = nextPtr->next;
	}
	cout << endl;
}

int main()
{
	ifstream input("input.txt");
	if (!input.is_open())
	{
		std::cout << "Failed to open input.txt for reading\n";
		return 1;
	}

	ofstream output("output.txt");
	if (!output.is_open())
	{
		std::cout << "Failed to open output.txt for writing\n";
	}

	try
	{
		Stack<string>* parkingLot = nullptr; 

		int parkingLotSize;
		input >> parkingLotSize;

		string carNumber;
		char sign; 

		while (input >> carNumber >> sign) 
		{
			switch (sign)
			{
			case ARRIVING_SIGN:
				CarArrives(output, parkingLot, carNumber, parkingLotSize);
				break;
			case LEAVING_SIGN:
				if (IsCarInParkingLot(parkingLot, carNumber))
				{
					CarLeaves(output, parkingLot, carNumber);
				}
				else
				{
					cout << "Can't find car with number " << carNumber << endl << endl;
				}
				break;
			default:
				throw exception("Unknown sign");
			}
		}

		bool end = false;

		while (!end)
		{
			int command;

			cout << "Enter command:" << endl;
			cout << "1 - Car arrives" << endl;
			cout << "2 - Car leaves" << endl;
			cout << "3 - Print cars in the parking lot" << endl;
			cout << "4 - Finish work" << endl;
			cin >> command;
			cout << endl;

			switch (command)
			{
			case 1:
				cout << "Enter car number: ";
				cin >> carNumber;
				if (CarArrives(output, parkingLot, carNumber, parkingLotSize))
				{
					cout << "Successfuly";
				}
				else
				{
					cout << "Parking lot is overflowed";
				}
				cout << endl << endl;
				break;
			case 2:
				cout << "Enter car number: ";
				cin >> carNumber;
				if (IsCarInParkingLot(parkingLot, carNumber))
				{
					CarLeaves(output, parkingLot, carNumber);
					cout << "Successfuly";
				}
				else
				{
					cout << "Can't find car with number " << carNumber;
				}
				cout << endl << endl;
				break;
			case 3:
				cout << "Car limit: " << parkingLotSize << endl;
				PrintStack(parkingLot);
				cout << endl;
				break;
			case 4:
				end = true;
				break;
			default:
				cout << "Unknown command" << endl;
				break;
			}
		}
	}
	catch (exception const& e)
	{
		cout << e.what() << endl; 
	}

	if (input.bad())
	{
		std::cout << "Failed to read data form input file\n";
		return 1;
	}

	if (!output.flush())
	{
		std::cout << "Failed to write data to output file\n";
		return 1;
	}

	return 0;
}