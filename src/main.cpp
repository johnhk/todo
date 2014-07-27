#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <string>
#include <cstring>
#include <algorithm>

#include "include/task.h"
#include "include/todo.h"
#include "include/utility.h"

using namespace std;

int main (int argc, char* argv[]) {
	// converting all options to the string format
	string arg[100];
	for (int i=0;i<argc;i++) arg[i]=argv[i];

	// If no argument is provided list all the undone tasks.
	if (argc < 2) arg[argc++] = "list";

	todo_t* todo = new todo_t();

	// Calculating the default URL of the file ie. ~/.todo.txt
	string username (getenv("USER"));
	string filename = "/home/" + username + "/.todo.txt";

	// Streams for various file I/O.
	ifstream ifile;
	ofstream ofile;

	// Load the initial file data to the todo class (if any).
	ifile.open (filename);
	if (ifile.is_open()) {
		string line;
		while (getline (ifile, line)) {
			task_t* temp = new task_t (line);
			todo->push (temp);
		}
		ifile.close();
	}
	else {
		fprintf (stderr, "The todo database file was not created previously.\nSo I created the file.\n");
		// Creating the File from ofstream;
		ofile.open (filename);
		ofile.close();
		exit (EXIT_FAILURE);
	}

	// TODO: add the exception handling to the file IO code after this point.
	// Started Argument parsing.
	if (arg[1] == "cleanup") {
		todo->cleanup();

		ofile.open (filename);
		ofile<<todo->toStringAll();
		ofile.close();
	}
	else if (arg[1] == "list") {
		if (arg[2] == "all") {
			cout<<todo->toStringAll();
		}
		else {
			cout<<todo->toStringUndone();
		}
	}
	else if (arg[1] == "all") {
		cout<<todo->toStringAll();
	}
	else if (arg[1] == "add") {
		string content = "";
		// creating the content to be added from all the later paramerters.
		for (int i=2;i<argc;i++) content += " " + arg[i];
		content.erase (0,1);
		if (content.length() == 0) {
			fprintf (stderr, "Please give some content to your TODO.\nThis implementation still lacks AI, so it can't understand your thoughts.\n");
			exit (EXIT_FAILURE);
		}
		task_t* task = new task_t(content, false);
		todo->push (task);

		ofile.open (filename);
		ofile<<todo->toStringAll();
		ofile.close();
	}
	else if (isNumber (arg[1])) {
		if (arg[2].length() == 0 || argc < 3) {
			fprintf (stderr, "What do you want me todo with this task?\n");
			exit (EXIT_FAILURE);
		}
		// Converting string to int;
		int x = toNumber (arg[1]);

		if (x<1 || x>todo->size()) {
			fprintf (stderr, "Enter a valid todo number.\n");
			exit (EXIT_FAILURE);
		}

		if (arg[2] == "done") {
			todo->setDone(x);

			ofile.open (filename);
			ofile<<todo->toStringAll();
			ofile.close();
		}
		else if (arg[2] == "undone") {
			todo->setUndone(x);

			ofile.open (filename);
			ofile<<todo->toStringAll();
			ofile.close();
		}
		else if (arg[2] == "remove" || arg[2] == "delete") {
			todo->erase (x);

			ofile.open (filename);
			ofile<<todo->toStringAll();
			ofile.close();
		}
		else if (arg[2] == "modify" || arg[2] == "change") {
			string content = "";
			// combining all the further options into a string.
			for (int i=3;i<argc;i++) content += " " + arg[i];
			content.erase (0,1);

			todo->modifyTask (x, content);

			ofile.open(filename);
			ofile<<todo->toStringAll();
			ofile.close();
		}
	}

	exit (EXIT_SUCCESS);
}
