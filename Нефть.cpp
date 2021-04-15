#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>   
#include <sstream>
#include <set>
#include <iterator>

using namespace std;

class Point_of_Graf
{
public:
	Point_of_Graf(int a, long long int b, int c, bool d, int e);
	int to_point;
	long long int flow;
	int cost;
	bool is_revers;
	int reversed_number_graf;
};

Point_of_Graf::Point_of_Graf(int a, long long int b, int c, bool d, int e)
{
	to_point = a;
	flow = b;
	cost = c;
	is_revers = d;
	reversed_number_graf = e;
}

class Point_of_Flow
{
public:
	Point_of_Flow(int a, int b, int c, long long int d, int e);
	long long int cost;
	int from_point;
	int graf_cost;
	int min_flow;
	int number_in_vector;
};

Point_of_Flow::Point_of_Flow(int a, int b, int c, long long int d, int e)
{
	from_point = a;
	graf_cost = b;
	min_flow = c;
	cost = d;
	number_in_vector = e;
}

int main()
{
	int n, m;
	ifstream input;
	input.open("input.txt");
	string line;
	getline(input, line);
	istringstream istring(line);
	vector<string> string_1
	{
		istream_iterator<string>(istring), {}
	};
	n = stoi(string_1[0]);
	m = stoi(string_1[1]);
	vector <int> a;
	getline(input, line);
	istringstream istring_2(line);
	vector<string> string_2
	{
		istream_iterator<string>(istring_2), {}
	};
	for (int i = 0; i < string_2.size(); i++)
		a.push_back(stoi(string_2[i]));
	vector <int> b;
	getline(input, line);
	istringstream istring_3(line);
	vector<string> string_3
	{
		istream_iterator<string>(istring_3), {}
	};
	for (int i = 0; i < string_3.size(); i++)
		b.push_back(stoi(string_3[i]));
	vector <vector<Point_of_Graf>> Graf(n + m + 2);
	long long int FlowIn = 0;
	for (int i = 0; i < n; i++)
	{
		getline(input, line);
		istringstream istring(line);
		vector<string> next_string
		{
			istream_iterator<string>(istring), {}
		};
		for (int j = 0; j < m; j++)
		{
			int C = stoi(next_string[j]);
			Graf[i].push_back(Point_of_Graf(j + n, 9000000000000000, C, false, Graf[j + n].size()));
			Graf[j + n].push_back(Point_of_Graf(i, 0, -C, true, Graf[i].size() - 1));
		}
	}
	input.close();
	for (int i = 0; i < n; i++)
	{
		Graf[n + m].push_back(Point_of_Graf(i, a[i], 0, false, Graf[i].size()));
	}
	for (int i = 0; i < m; i++)
	{
		Graf[i + n].push_back(Point_of_Graf(n + m + 1, b[i], 0, false, Graf[n + m + 1].size()));
	}
	int s = n + m, t = n + m + 1;
	while (true)
	{
		vector <Point_of_Flow> Ford(n + m + 2, Point_of_Flow(-1, 0, 2000000000, 9000000000000000, -1));
		Ford[s].cost = 0;
		Ford[s].from_point = 0;
		while (true)
		{
			bool is_change = false;
			for (int i = n + m + 1; i >= 0; i--)
			{
				for (int j = 0; j < Graf[i].size(); j++)
				{
					if (Ford[i].cost != 9000000000000000 && Graf[i][j].flow != 0 && Ford[i].cost + Graf[i][j].cost < Ford[Graf[i][j].to_point].cost)
					{
						Ford[Graf[i][j].to_point].cost = Ford[i].cost + Graf[i][j].cost;
						Ford[Graf[i][j].to_point].from_point = i;
						Ford[Graf[i][j].to_point].number_in_vector = j;
						Ford[Graf[i][j].to_point].graf_cost = Graf[i][j].cost;
						is_change = true;
					}
				}
			}
			if (!is_change)
				break;
		}
		if (Ford[t].from_point == -1)
		{
			ofstream ouput;
			ouput.open("output.txt");
			ouput << FlowIn;
			ouput.close();
			return 0;
		}
		else
		{
			long long int tempCost = 0;
			long long int min_flow = 2000000000;
			int now_point = t;
			int next_point = t;
			while (now_point != s)
			{
				next_point = Ford[now_point].from_point;
				tempCost += Ford[now_point].graf_cost;
				if (min_flow > Graf[next_point][Ford[now_point].number_in_vector].flow)
					min_flow = Graf[next_point][Ford[now_point].number_in_vector].flow;
				now_point = next_point;
			}
			now_point = t;
			next_point = t;
			while (now_point != s)
			{
				next_point = Ford[now_point].from_point;
				if (next_point < n && now_point < n + m)
				{
					Graf[now_point][Graf[next_point][Ford[now_point].number_in_vector].reversed_number_graf].flow += min_flow;
				}
				else
				{
					Graf[next_point][Ford[now_point].number_in_vector].flow -= min_flow;
				}
				now_point = next_point;
			}
			FlowIn += tempCost * min_flow;
		}
	}
}