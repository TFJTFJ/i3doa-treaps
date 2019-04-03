#include "../Treaps/Treap.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

#include <random>

int main()
{
	const int MIN_SIZE = 1;
	const int MAX_SIZE = 1024*1024*4;

	const int RUNS_PER_SIZE = 32;
	mt19937_64 gen(random_device{}());

	vector<pair<int, int> > depths;
	ofstream resultfile;
	resultfile.open("results.txt");

	for (int size = MIN_SIZE; size <= MAX_SIZE; size *=2)
	{
		for (int run = 0; run < RUNS_PER_SIZE; run++)
		{
			Treap<int> t;

			for (int insertionCount = 0; insertionCount < size; insertionCount++)	// Insert another set of elements into treap
			{
				t.insert(static_cast<int>(gen()));
			}

			pair<int, int> point;
			point.first = t.count();
			point.second = t.depth();
			depths.push_back(point);
		}
	}

	for(vector<pair<int, int> >::iterator ix = depths.begin(); ix != depths.end(); ++ix)
	{
		resultfile << ix->first << ";" << ix->second << endl;
	}
	cout << "Done" << endl;
	resultfile.close();
	return 0;
}
