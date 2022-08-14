#include <iostream>
#include "mpi.h"
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <time.h>
using namespace std;

int main(int argc, char **argv) {	
	srand(time(NULL));	
	int agree = 1, gatheredValue = 1;
	int rank, root = 0;
	string message;
	char *dateAndTime;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == root) {
		cout << "Name: Sameet Asadullah\nRoll Number: 18i-0479\n";
	}

	while(true) {
		message = "";

		if (rank == root) {
			int day = (rand() % 30) + 1;
			int month = (rand() % 9) + 4;
			int hour = rand() % 24;			

			// Converting integer values to combined string
			stringstream stream;
			stream << day;
			message = message + stream.str() + "/";
			stream.str("");
			stream << month;
			message = message + stream.str() + "/2021, ";
			stream.str("");
			stream << hour;
			message = message + stream.str() + ":00:00";
			stream.str("");
			
			// Converting string to char*
			dateAndTime = new char[message.size() + 1];
			message.copy(dateAndTime, message.size() + 1);
			dateAndTime[message.size()] = '\0';
			cout << "\nProposed date and time by process 0 is: " << dateAndTime << endl;				
		}

		MPI_Bcast(&dateAndTime, 8, MPI_CHAR, root, MPI_COMM_WORLD);
		
		if (rank != root) {	// Asking from all other processes
			agree = rand() % 2;
			if (agree == 1) {
				cout << "Friend " << rank << " agreed." << endl;
			} else {
				cout << "Friend " << rank << " didn't agree." << endl;
			}
		}

		// Using MPI_Allreduce() to end while loop for every process
		MPI_Allreduce(&agree, &gatheredValue, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);
		
		if (rank == root && gatheredValue == 1) {
			cout << "\nAll friends agreed on " << dateAndTime << endl;
		}

		if (gatheredValue == 1) {		
			break;
		} else {
			gatheredValue = 1;
		}
	
	}

 	MPI_Finalize();
}