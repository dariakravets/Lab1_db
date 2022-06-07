#pragma once
#pragma warning(disable : 4996)

#include "Singer.h"

bool IsUnique(Singer gr, Rate& st, int id) {

	if (gr.address_of_first_rate == -1)
		return true;
	FILE* db = fopen("Rate.fl", "r+b");
	
	fseek(db, gr.address_of_first_rate, SEEK_SET);

	for (int i = 0; i < gr.amountOfFirstPlaces; ++i) {
		fread(&st, sizeof(Rate), 1, db);
		fclose(db);

		if (st.id == id)
			return false;
		db = fopen("Rate.fl", "r+b");
		fseek(db, st.nextL_address, SEEK_SET);
	}
	fclose(db);
	return true;
}

void getEveryone(Singer* gr) {
	FILE* index = fopen("Singer.ind", "rb");
	if (index == NULL) {
		 cout << "Database can't be found\n";
		return;
	}
	int st_count = 0;
	int gr_count = 0;

	fseek(index, 0, SEEK_END);
	int indAmount = ftell(index) / sizeof(Indexing);


	for (int i = 1; i <= indAmount; ++i) {
		if (getSinger(gr, i)) {
			gr_count++;
			st_count += gr->amountOfFirstPlaces;

			 cout << "Singer " << gr->name << " has " << gr->amountOfFirstPlaces << " awards.\n";
		}
	}
	fclose(index);

	 cout << "Singers: " << gr_count <<  endl;
	 cout << "Items: " << st_count;
	 cout <<  endl;
}


