#include <iostream>
#include <ctime>
#include <map>
#include <algorithm>
#include <vector>
#include <Windows.h>

CRITICAL_SECTION g_criticalSection;

class Gamble
{
public:
	Gamble()
	{
		srand(time(NULL));
		m_randNum = rand() % maxNumber;
	};

	void MakePrediction(std::string userName, int prediction) {
		EnterCriticalSection(&g_criticalSection);
		m_userPrediction.insert(std::pair<std::string, int>(userName, prediction));
		LeaveCriticalSection(&g_criticalSection);
	}

	void ShowPrediction() {
		EnterCriticalSection(&g_criticalSection);
		std::cout << "\nShow all players " << std::endl;
		for (std::map<std::string, int>::iterator i = m_userPrediction.begin();
			i != m_userPrediction.end();
			++i) {
			std::cout << i->first << " - " << i->second << std::endl;
		}
		LeaveCriticalSection(&g_criticalSection);
	}

	void ComparePrediction() {
		std::vector<std::pair<std::string, int>> vec;//create vector for sort map

		std::map<std::string, int> ::iterator it;
		for (it = m_userPrediction.begin(); it != m_userPrediction.end(); it++)
		{
			vec.push_back(make_pair(it->first, it->second));
		}
		sort(vec.begin(), vec.end(), [](auto a, auto b) {return (a.second < b.second);});

		for (int i = 0; i < vec.size(); i++) { //find nearest number
			if (vec[i].second > m_randNum && i > 0) {
				int difference1 = vec[i].second - m_randNum;
				int difference2 = m_randNum - vec[i - 1].second;
				std::cout << "Random Number " << m_randNum << std::endl;
				if (difference1 < difference2) {
					std::cout << vec[i].first << " THE WINNER" << std::endl;
				}
				else {
					std::cout << vec[i - 1].first << " THE WINNER" << std::endl;
				}
				return;
			}
		}
		std::cout << "Random Number " << m_randNum << std::endl;
		std::string endElementName = vec.back().first;
		std::cout << endElementName << " THE WINNER" << std::endl;
	}

private:
	int m_randNum;
	std::map<std::string, int> m_userPrediction;
	const int maxNumber = 100;
};



int main()
{
	InitializeCriticalSection(&g_criticalSection);

	Gamble gam;

	std::string userName;
	int prediction;

	for (int i = 0; i < 4; ++i) {
		std::cout << "Enter your name" << std::endl;
		std::cin >> userName;
		std::cout << "Enter your prediction from 0 to 100" << std::endl;
		std::cin >> prediction;
		gam.MakePrediction(userName, prediction);
	}

	gam.ComparePrediction();
	gam.ShowPrediction();

	DeleteCriticalSection(&g_criticalSection);
}