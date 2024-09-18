#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
	int n, delay, limit;
	cin >> n >> delay >> limit;
	vector<int> t(n);

	for (int& deadline : t)
	{
		cin >> deadline;
	}

	vector<int> timesTriggered(n);

	long int min = 0;

	while (limit > 0)
	{
		min = t[0] + timesTriggered[0] * delay;

		for (int i = 1; i < n; ++i)
		{
			long int deadline = t[i] + timesTriggered[i] * delay;
			min = std::min(min, deadline);
		}

		for (int i = 0; i < n; ++i)
		{
			if (min == t[i] + timesTriggered[i] * delay)
			{
				++timesTriggered[i];
			}
		}

		--limit;
	}

	cout << min;
	return 0;
}