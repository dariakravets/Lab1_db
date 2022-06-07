#pragma once
#pragma warning(disable : 4996)
#include <string>

struct Singer {
	int id;
	 string name;
	long address_of_first_rate = -1;
	int amountOfFirstPlaces = 0;
};


struct Rate {
	int id;
	int number;
	int singer_Id;
	bool isFree; 
	long my_address;
	long nextL_address;
};

struct Indexing {
	int id;
	int address;
	bool exists; 
};