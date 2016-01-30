// FileIOTest_with_outputs.cpp : Defines the entry point for the console application.
//

//! This file tests the following static classes.

#include "../../../../libraries/FileIO/UserInput.h"
#include "../../../../libraries/FileIO/PrintOut.h"
#include "../../../../libraries/FileIO/FileErrors.h"
#include "../../../../libraries/FileIO/FileIn.h"	
#include "../../../../libraries/FileIO/FileOut.h"

#include "stdafx.h"


void fileout_test(){
	// also tests input
	matrix2d d2(10,matrix1d(3,4));
	FileOut::print_vector(d2,"blah.csv");
	matrix2d c2 = FileIn::read2<double>("blah.csv");
	PrintOut::print_vector(c2);

	matrix1d d1(10,4);
	FileOut::print_vector(d1,"blah.csv");
	matrix2d c1 = FileIn::read2<double>("blah.csv");
	PrintOut::print_vector(c1);
	PrintOut::print_vector(d1);

	matrix2d dpairs(10,matrix1d(2,3));
	FileOut::print_vector(dpairs,"blah.csv");
	std::vector<std::pair<double,double> > processed = FileIn::read_pairs<std::pair<double,double> >("blah.csv");

	// only lightly tested
	FileIn::read_variable_file("blah.csv");
	FileIn::detect_separator("blah.csv");

	FileOut::print_pair_container(processed,"blah.csv");
}

void fileerrors_test(){
	FileErrors::failed_file("blah");
	FileErrors::variable_not_found("blah");
	//FileErrors::not_pair("blah");	// fatal: commented out to test subsequent fatal error
	FileErrors::unrecognized_extension("blah");	// fatal
}

void userinput_test(){
	bool yes = UserInput::getYesNo("question!");

	if (yes) printf("yes!");
	else printf("no!");
	system("pause");
}

void printout_test(){
	matrix2d blah(10,matrix1d(4,3));
	PrintOut::print_vector(blah);
	PrintOut::print_vector(blah[0]);
	double foo[2][3] = {{1,2,3},{3,4,9}};
	PrintOut::print_array<double,2,3>(foo);
	
	double bar[3] = {1,2,3};
	PrintOut::print_array<double,3>(bar);	// issue here: second parameter is length, shouldn't be
}

int _tmain(int argc, _TCHAR* argv[])
{
	fileout_test();
	//fileerrors_test();
	//userinput_test();
	system("pause");
	return 0;
}

