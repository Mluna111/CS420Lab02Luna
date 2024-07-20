#include <thread>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <mutex>
#include <atomic>

#include "AllocLocalHistogram.h"

using namespace std;

void fileToMemoryTransfer(char* fileName, char** data, size_t& numOfBytes) {
	streampos begin, end;
	ifstream inFile(fileName, ios::in | ios_base::binary | ios::ate);
	if (!inFile)
	{
		cerr << "Cannot open " << fileName << endl;
		inFile.close();
		exit(1);
	}
	size_t size = inFile.tellg();
	char* buffer = new  char[size];
	inFile.seekg(0, ios::beg);
	inFile.read(buffer, size);
	inFile.close();
	*data = buffer;
	numOfBytes = size;
}

const int rowSize = 256;
int** localHistogram = nullptr;
atomic<long long int> globalHistogram[rowSize*rowSize];

int main(int arcc, char* argv[]) {

	size_t fileSize = 0;
	int numthreads = thread::hardware_concurrency();
	char* data = nullptr;
	char* filename = argv[1];
	fileToMemoryTransfer(argv[1], (char**)&data, fileSize);

	//initialize global histogram to 0's
	for (int k = 0; k < rowSize; k++) {
		globalHistogram[k] = 0;
	}

	//devide up 2d array rows to allocate one row per
	//thread of equal sizes
	int rows = fileSize / numthreads;
	int extra = fileSize % numthreads;
	int start = 0;    // row dimensions for first thread:
	int end = rows;

	vector<thread> workers1;

	for (int t = 1; t <= numthreads; t++)
	{
		if (t == numthreads) { // last thread processes any extra rows:
			end += extra;
		}

		workers1.push_back(thread([&data](int start, int end, int t)
			{
				for (int k = start; k < end; k++) {
					unsigned char s = data[k];
					globalHistogram[s]++;
				}
			}, start, end, t)
		);
		start = end;
		end = start + rows;
	}//for

	for_each(workers1.begin(), workers1.end(), [](thread& t)
		{
			t.join();
		}
	);

	//Print global histogram for run with only global histogram
	cout << "Run with global histogram";
	for (int k = 0; k < rowSize; k++) {
		cout << endl << k << ": " << globalHistogram[k];
	}

	//reset global histogram to all zeros for next run
	for (int k = 0; k < rowSize; k++) {
		globalHistogram[k] = 0;
	}

	rows = fileSize / numthreads;
	extra = fileSize % numthreads;
	start = 0;    // row dimensions for first thread:
	end = rows;

	vector<thread> workers2;
	localHistogram = New2dArray<int>(numthreads, rowSize);

	for (int t = 1; t <= numthreads; t++)
	{
		if (t == numthreads) { // last thread processes any extra rows:
			end += extra;
		}

		workers2.push_back(thread([&data](int start, int end, int t)
			{
				unsigned char s;
				// initialize threads chunk of localhistogram to zero:
				for (int j = 0; j < rowSize; j++)
					localHistogram[t - 1][j] = 0;
				
				//go through thread's data segment and increment local histogram based on values
				for (int k = start; k < end; k++) {
					s = data[k];
					localHistogram[t - 1][s]++;
				}

				//accumulate threads local histogram into the global histogram
				for (int k = 0; k < rowSize; k++) {
					globalHistogram[k] += localHistogram[t - 1][k];
				}
			}, start, end, t)
		);

		start = end;
		end = start + rows;
	}//for

	// wait for threads to finish:
	for_each(workers2.begin(), workers2.end(), [](thread& t)
		{
			t.join();
		}
	);

	//Print global histogram for run with local histogram
	cout << "\nRun with local histograms";
	for (int k = 0; k < rowSize; k++) {
		cout << endl << k << ": " << globalHistogram[k];
	}

	Delete2dArray(localHistogram);
	delete[] data;

	return 0;
}
