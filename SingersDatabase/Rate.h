#pragma once
#pragma warning(disable : 4996)

#include "Structs.h"
#include "Singer.h"
#include <vector>

Rate lt3_12p;

bool updateSinger(Singer* pl);

void printRate(Rate Rate, Singer pl){
	 cout << "Singer's name is " << pl.name
		<< " Awards number is " << Rate.number <<  endl;
}

void getToLastAddress(FILE* db, Rate& Rate, Singer pl) {
	long add = Rate.my_address;
	db = fopen("Rate.fl", "r+b");

	fseek(db, pl.address_of_first_rate, SEEK_SET);

	for (int i = 0; i < pl.amountOfFirstPlaces; ++i) {
		fread(&Rate, sizeof(Rate), 1, db);
		fseek(db, Rate.nextL_address, SEEK_SET);
	}

	Rate.nextL_address = add;
	fwrite(&Rate, sizeof(Rate), 1, db);
}


void changeAddresses(FILE* db, Rate rate, Rate prev_l, Singer* pl) {
	if (rate.my_address == pl->address_of_first_rate) {
		if (rate.my_address == rate.nextL_address)
			pl->address_of_first_rate = -1;
		else
			pl->address_of_first_rate = rate.nextL_address;
	}
	else {
		if (rate.my_address == rate.nextL_address)
			prev_l.my_address = prev_l.nextL_address;
		else
			prev_l.nextL_address = rate.nextL_address;

		fseek(db, prev_l.my_address, SEEK_SET);
		fwrite(&prev_l, sizeof(rate), 1, db);
	}
}



void deleteAddress(long lt_address) {
	FILE* trash = fopen("lt_trash.txt", "rb");
	int count = 0;
	
	fread(&count, sizeof(int), 1, trash);

	 vector<long> queue(count);
	for (int i = 0; i < count; ++i)
		fread(&queue[i], sizeof(long), 1, trash);

	fclose(trash);

	trash = fopen("lt_trash.txt", "wb");
	++count;
	fwrite(&count, sizeof(int), 1, trash);

	for (auto el : queue)
		fwrite(&el, sizeof(long), 1, trash);
	
	fwrite(&lt_address, sizeof(long), 1, trash);

	fclose(trash);
}

void changeTrash(FILE* trash, Rate* lt, int counter) {
	 vector<long> del(counter);
	for (int i = 0; i < counter; ++i)
		fread(&del[i], sizeof(long), 1, trash);
	
	lt->my_address = del[0];
	lt->nextL_address = del[0];

	fclose(trash);
	trash = fopen("lt_trash.txt", "wb");
	--counter;

	fwrite(&counter, sizeof(long), 1, trash);

	for (int i = 1; i <= counter; ++i)
		fwrite(&del[i], sizeof(long), 1, trash);

	fclose(trash);
}

bool insertRate(Rate& lt, Singer& pl) {
	lt.isFree = true;

	FILE* db = fopen("Rate.fl", "a+b");
	FILE* trash = fopen("lt_trash.txt", "a+b");

	int count = 0;
	fread(&count, sizeof(int), 1, trash);
	if (count > 0) {
		changeTrash(trash, &lt, count);
		fclose(db);
		db = fopen("Rate.fl", "r+b");
		fseek(db, lt.my_address, SEEK_SET);
	}
	else {
		fseek(db, 0, SEEK_END);

		int dbSize = ftell(db);

		lt.my_address = dbSize;
		lt.nextL_address = dbSize;
	}

	fwrite(&lt, sizeof(Rate), 1, db);

	if (!pl.amountOfFirstPlaces) {
		pl.address_of_first_rate = lt.my_address;
	}
	else {
		fclose(db);
		getToLastAddress(db, lt, pl);
	}

	fclose(db);
	++pl.amountOfFirstPlaces;
	updateSinger(&pl);
	return true;
}

bool getRate(Rate* lt, Singer pl, int id) {

	if (pl.amountOfFirstPlaces == 0) {
		return false;
	}


	FILE* db = fopen("Rate.fl", "rb");

	fseek(db, pl.address_of_first_rate, SEEK_SET);
	fread(lt, sizeof(struct Rate), 1, db);

	for (int i = 0; i < pl.amountOfFirstPlaces; ++i) {
		if (lt->id == id) {
			fclose(db);
			return true;
		}
		fseek(db, lt->nextL_address, SEEK_SET);
		fread(lt, sizeof(struct Rate), 1, db);
	}
	fclose(db);
	return false;
}

bool updateRate(Rate lt, int id) {
	FILE* db = fopen("Rate.fl", "r+b");
	
	fseek(db, lt.my_address, SEEK_SET);
	fwrite(&lt, sizeof(Rate), 1, db);
	fclose(db);

	return true;
}

void delRate(Rate& lt, Singer* pl) {
	FILE* db = fopen("Rate.fl", "r+b");

	fseek(db, pl->address_of_first_rate, SEEK_SET);
	do {
		fread(&lt3_12p, sizeof(Rate), 1, db);
		fseek(db, lt3_12p.nextL_address, SEEK_SET);
	} while (lt3_12p.nextL_address != lt.my_address && lt.my_address != pl->address_of_first_rate);

	changeAddresses(db, lt, lt3_12p, pl);
	lt.isFree = 0;
	deleteAddress(lt.my_address);


	fseek(db, lt.my_address, SEEK_SET);
	fwrite(&lt, sizeof(Rate), 1, db);
	fclose(db);

	--pl->amountOfFirstPlaces;
	updateSinger(pl);


}