#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

enum class Cell { unopened, opened, mined };

typedef unsigned int uint;
typedef vector <vector<Cell>> dd_arr;
typedef vector <int> test_arr;


void open(uint i, uint j, dd_arr& field, uint const& n, uint const& m);

int main()
{
	uint n, m;
	cin >> n >> m;
	
	dd_arr field(n, vector<Cell>(m));

	uint minesCount;
	cin >> minesCount;

	for (uint i = 0; i < minesCount; ++i)
	{
		uint x, y;
		cin >> x >> y;

		field[y - 1][x - 1] = Cell::mined;
	}

	uint clicksCount = 0;

	for (uint i = 0; i < n; ++i)
	{
		for (uint j = 0; j < m; ++j)
		{
			if (field[i][j] == Cell::unopened)
			{
				open(i, j, field, n, m);
				++clicksCount;
			}
		}
	}

	cout << clicksCount;

	return 0;
}

void open(uint i, uint j, dd_arr& field, uint const& n, uint const& m)
{
	if (i < 0 || i >= n) 
		return;

	if (j < 0 || j >= m)
		return;

	if (field[i][j] != Cell::unopened)
		return;

	field[i][j] = Cell::opened;

	for (uint i = 0; i < n; ++i)
	{
		for (uint j = 0; j < m; ++j)
		{
			cout << static_cast<int>(field[i][j]) << " ";
		}
		cout << endl;
	}

	cout << endl;

	open(i + 1, j, field, n, m);
	open(i - 1, j, field, n, m);
	open(i, j + 1, field, n, m);
	open(i, j - 1, field, n, m);
}