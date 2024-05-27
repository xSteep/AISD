/* "Имеется файл в котором слова состоящие из двух и более 
букв написаны заглавными буквами, переписать файл так, чтобы
термины были набраны строчными буквами в кавычках"
Visual Studio 2022
*/
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <sstream>

using namespace std;

bool IsWordUpper(const wstring& word)
{
	for (auto ch : word)
	{
		if (!((65 <= ch && ch <= 90) 
			|| ch == 168 
			|| (192 <= ch && ch <= 223))) 
		{
			return false;
		}
	}

	return true;
}

wchar_t ToLowerChar(wchar_t ch)
{
	int shift = 32; 
	if (ch == 168)
	{
		shift = 16; 
	}
	return (ch + shift);
}

wstring ToLowerWord(wstring word)
{
	wstring result;
	bool period;
	period = false;
	if (word[word.size() - 1] == '.')
	{
		word = word.substr(0, word.size() - 1);
		period = true;
	}

	if (word.length() > 1 && IsWordUpper(word))
	{
		result += '\"';
		for (auto ch : word)
		{
			result += ToLowerChar(ch);
		}
		result += '\"';
	}
	else
	{
		result += word;
	}

	if (period)
	{
		result += '.';
	}
	result += ' ';

	return result;
}

wstring ToLowerLine(const wstring& line)
{
	wstring word, result;
	wstringstream ss(line);
	while (ss >> word)
	{
		result += ToLowerWord(word);
	}
	return result;
}

void QuoteWords(wistream& input, wostream& output)
{
	wstring line;
	while (getline(input, line))
	{
		output << ToLowerLine(line) << endl;
	}
}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	if (argc != 3)
	{
		std::cout << "Invalid arguments count\n";
		std::cout << "Usage <inputf file name> <output file name>\n";
		return 1;
	}

	string inputFileName = argv[1];
	string outputFileName = argv[2];

	wifstream input;
	input.open(inputFileName);
	if (!input.is_open())
	{
		std::cout << "Failed to open '" << inputFileName << "' for reading\n";
		return 1;
	}

	wofstream output;
	output.open(outputFileName);
	if (!output.is_open())
	{
		std::cout << "Failed to open '" << outputFileName << "' for writing\n";
	}

	QuoteWords(input, output);

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