#include "../Treaps/Treap.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

#include <random>

int main()
{
	// Analysis parameters. Handle with care!
	const int MIN_SIZE = 1;					// Should be a power of 2
	const int MAX_SIZE = 1024*1024*64;		// Should be a power of 2
	const int TEST_RUNS_PER_SIZE = 8;
	
	const char* OUTPUT_FILE_NAME = "results.txt";

	mt19937_64 randomizer(random_device{}());	// Random generator. rand() only returns [0..2^16). This returns [0..2^64)
	ofstream resultfile;

	cout << "============================================================" << endl;
	cout << "Testing treap depths" << endl;
	cout << "Min. size: " << MIN_SIZE << endl;
	cout << "Max. size: " << MAX_SIZE << endl;
	cout << "Runs/size: " << TEST_RUNS_PER_SIZE << endl;
	cout << "------------------------------------------------------------" << endl;
	resultfile.open(OUTPUT_FILE_NAME);

	for (int size = MIN_SIZE; size <= MAX_SIZE; size *=2)	// Test different treap sizes: 1, 2, 4, 8, ..., MAX_SIZE
	{
		cout << "Testing size = " << size << ":\t\t";
		for (int run = 0; run < TEST_RUNS_PER_SIZE; run++)	// Run a number of times to get different depths for every treap size
		{
			Treap<int> treap;

			for (int insertionCount = 0; insertionCount < size; insertionCount++)	// Insert another set of random elements into treap
			{
				treap.insert(static_cast<int>(randomizer()));
			}

			resultfile << treap.count() << ";" << treap.depth() << endl;			// Write results to file
			resultfile.flush();
			cout << "*";
		}
		cout << " - OK!" << endl;
	}

	cout << "Done. Results stored in " << OUTPUT_FILE_NAME << endl;
	cout << "============================================================" << endl;

	resultfile.close();
	return 0;
}
