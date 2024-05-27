/*
*   12. Имеется   дерево,   корень    которого    соответствует
основателю  рода.  Сыновья  каждой  вершины  задают  сыновей и
дочерей  соответствующего  человека.  Указываются  имена  двух
человек  (например,  А  и  В).  Сообщить,  какая  из следующих
ситуаций имеет место:
   1) А предок В;
   2) В предок А;
   3) А и В имеют ближайшего общего предка С (10).

   Visual Studio 2022
*/
#include <iostream>
#include <vector>
#include <optional>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

const char TREE_CHAR = '.';

template <typename T>
struct Tree
{
	T value;
	int level;
	vector<Tree<T>*> sons;
};

struct Args
{
	string inputFileName;
};

optional<Args> ParseArgs(int argc, char* argv[]);
Tree<wstring>* ReadTreeFromFile(string fileName);
void PrintTree(wostream& out, Tree<wstring>* root);
bool CheckIfNodeExists(Tree<wstring>* root, wstring node);
Tree<wstring>* FindLCA(Tree<wstring>* root, wstring child1, wstring child2);

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	auto args = ParseArgs(argc, argv);

	if (!args)
	{
		cout << "Invalid input file name" << endl;
		return -1;
	}

	try
	{
		Tree<wstring>* root = ReadTreeFromFile(args->inputFileName);

		bool end = false;
		while (!end)
		{
			wstring name1, name2;
			Tree<wstring>* ancestor;
			int command;

			wcout << "1 - Распечатать дерево" << endl;
			wcout << "2 - Найти предка" << endl;
			wcout << "3 - Завершить работу" << endl;
			wcout << "Введите комманду: ";
			cin >> command;
			cout << endl;

			switch (command)
			{
			case 1:
				PrintTree(wcout, root);
				break;
			case 2:
				wcout << "Введите имена: ";
				wcin >> name1 >> name2;

				if (!CheckIfNodeExists(root, name1))
				{
					wcout << "Человек не найден: " << name1 << endl;
					break;
				}
				if (!CheckIfNodeExists(root, name2))
				{
					wcout << "Человек не найден: " << name2 << endl;
					break;
				}

				ancestor = FindLCA(root, name1, name2);
				if (!ancestor)
				{
					wcout << "человек не найден" << endl;
					break;
				}

				if (ancestor->value == name1)
				{
					wcout << name1 << " предок " << name2 << endl;
				}
				else if (ancestor->value == name2)
				{
					wcout << name2 << " предок " << name1 << endl;
				}
				else
				{
					wcout << "Общий предок: " << ancestor->value << endl;
				}

				break;
			case 3:
				end = true;
				break;
			default:
				wcout << "Неизвестная комманда: " << command << endl;
				break;
			}
			cout << endl;
		}
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}

optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc != 2)
	{
		return nullopt;
	}

	Args args;
	args.inputFileName = argv[1];

	return args;
}

int ReadLevelFromString(wstring const& str)
{
	int level = 0;

	for (auto ch : str)
	{
		if (ch != TREE_CHAR)
		{
			break;
		}
		level++;
	}

	return level;
}

wstring ReadValueFromString(wstring const& str)
{
	wstring value;

	for (auto i = str.find_first_not_of(TREE_CHAR); i < str.length(); i++)
	{
		value += str[i];
	}

	return value;
}

Tree<wstring>* ReadTreeFromFile(wifstream& file, Tree<wstring>*& root, int currentLevel = 0, Tree<wstring>** prevNode = nullptr)
{
	wstring line;
	Tree<wstring>* currentNode = nullptr;
	Tree<wstring>* previousNode = prevNode ? *prevNode : nullptr;

	while (getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}
		currentNode = new Tree<wstring>();
		currentNode->level = ReadLevelFromString(line);
		currentNode->value = ReadValueFromString(line);

		if (currentNode->level == 0)
		{
			root = currentNode;
			previousNode = root;
			continue;
		}

		if ((currentNode->level - currentLevel) == 1)
		{
			root->sons.push_back(currentNode);
		}

		if ((currentNode->level - currentLevel) > 1)
		{
			previousNode->sons.push_back(currentNode);
			Tree<wstring>* notChildNode = ReadTreeFromFile(file, previousNode, currentLevel + 1, &currentNode);

			if (notChildNode)
			{
				if ((notChildNode->level - currentLevel) < 1)
				{
					return notChildNode;
				}
				else
				{
					root->sons.push_back(notChildNode);
					previousNode = notChildNode;
					continue;
				}
			}
		}

		if ((currentNode->level - currentLevel) < 1)
		{
			return currentNode;
		}
		previousNode = currentNode;
	}

	return nullptr;
}

Tree<wstring>* ReadTreeFromFile(string fileName)
{
	Tree<wstring>* root = nullptr;

	wifstream inputFile(fileName);

	if (!inputFile.is_open())
	{
		throw exception("cannot open input file");
	}

	if (ReadTreeFromFile(inputFile, root))
	{
		throw exception("something went wrong");
	}

	return root;
}

void PrintTree(wostream& out, Tree<wstring>* root)
{
	out << wstring(root->level, '.') << root->value << endl;

	for (auto& son : root->sons)
	{
		PrintTree(out, son);
	}
}

bool CheckIfNodeExists(Tree<wstring>* root, wstring node)
{
	bool result = false;

	if (root->value == node)
	{
		return true;
	}

	for (auto& son : root->sons)
	{
		result = CheckIfNodeExists(son, node);
		if (result)
		{
			return result;
		}
	}

	return result;
}

Tree<wstring>* FindLCA(Tree<wstring>* root, wstring child1, wstring child2)
{
	if (root == nullptr)
	{
		return nullptr;
	}


	if (root->value == child1 || root->value == child2)
	{
		return root;
	}

	Tree<wstring>* result = nullptr;

	for (auto& son : root->sons)
	{
		if (result != nullptr)
		{
			Tree<wstring>* secondResult = FindLCA(son, child1, child2);
			if (secondResult != nullptr)
			{
				return root;
			}
		}
		else
		{
			result = FindLCA(son, child1, child2);
		}
	}

	return result;
}