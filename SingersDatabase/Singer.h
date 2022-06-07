#pragma once
#pragma warning(disable : 4996)

#include "Structs.h"
#include "Rate.h"
#include <iostream>


void printSinger(Singer* gr)
{
	 cout << "Singer: " << gr->name <<  endl;
	 cout << "Singer has " << gr->amountOfFirstPlaces << " awards\n";
}

void readSinger(Singer* gr) {
	 string name;
	 cout << "Enter Singer's name\n";
	 cin >> name;
	gr->name = name;
}

bool AreFiles(FILE* db, FILE* index) {
	if (db == NULL || index == NULL) {
		return false;
	}
	return true;
}
bool IsIndex(FILE* indexer, int id) {
	fseek(indexer, 0, SEEK_END);
	auto size = ftell(indexer);

	if (size == 0 || id * sizeof(Indexing) > size) { 
		return false;
	}
	return true;
}

void deleteIds(int id) {
	FILE* trash = fopen("pl_trash.txt", "rb");

	int count = 0;
	fread(&count, sizeof(int), 1, trash);
	
	 vector<int> queue(count);
	for (int i = 0; i < count; ++i)
		fread(&queue[i], sizeof(int), 1, trash);

	fclose(trash);

	trash = fopen("pl_trash.txt", "wb");
	++count;
	fwrite(&count, sizeof(int), 1, trash);

	for (auto el : queue)
		fwrite(&el, sizeof(int), 1, trash);
	fwrite(&id, sizeof(int), 1, trash);
	fclose(trash);
}

void getTrashed(int counter, FILE* trash, Singer* pl)
{
	 vector<int> queue(counter);		

	for (int i = 0; i < counter; i++)
	{
		fread(&queue[i], sizeof(int), 1, trash);
	}

	pl->id = queue[0];									

	fclose(trash);									
	trash = fopen("pl_trash.txt", "wb");
	
	--counter;
	fwrite(&counter, sizeof(int), 1, trash);

	for (int i = 1; i <= counter; i++)
	{
		fwrite(&queue[i], sizeof(int), 1, trash);
	}

	fclose(trash);								
}

bool insertSinger(Singer pl) {
	FILE* index = fopen("Singer.ind", "a+b");
	FILE* db = fopen("Singer.fl", "a+b");
	FILE* trash = fopen("pl_trash.txt", "a+b");

	Indexing indexer;
	int counter = 0;
	fread(&counter, sizeof(int), 1, trash);

	if (counter > 0) {
		getTrashed(counter, trash, &pl);

		fclose(index);
		fclose(db);

		index = fopen("Singer.ind", "r+b");
		db = fopen("Singer.fl", "r+b");

		fseek(index, (pl.id - 1) * sizeof(Indexing), SEEK_SET);
		fread(&indexer, sizeof(Indexing), 1, index);
		indexer.exists = true;
		
		fseek(index, (pl.id - 1) * sizeof(Indexing), SEEK_SET);
		fwrite(&indexer, sizeof(Indexing), 1, index);

		fseek(db, indexer.address, SEEK_SET);
	}
	else {
		fseek(index, 0, SEEK_END);

		if (ftell(index)) {
			int temp = -1 * sizeof(Indexing);
			fseek(index, temp, SEEK_END);
			fread(&indexer, sizeof(Indexing), 1, index);

			pl.id = indexer.id + 1;
		}
		else {
			pl.id = 1;
		}
	}
	pl.amountOfFirstPlaces = 0;
	fwrite(&pl, sizeof(Singer), 1, db);

	indexer.id = pl.id;
	indexer.address = (pl.id - 1) * sizeof(Singer);
	indexer.exists = 1;

	 cout << "Singer's id is " << pl.id <<  endl;

	fseek(index, (pl.id - 1) * sizeof(Singer), SEEK_SET);
	fwrite(&indexer, sizeof(Indexing), 1, index);
	fclose(index);
	fclose(db);

	return true;
}

bool getSinger(Singer* pl, int id) {

	FILE* index = fopen("Singer.ind", "rb");
	FILE* db = fopen("Singer.fl", "rb");

	if (!AreFiles(db, index) || !IsIndex(index, id)) {
		
		return false;
	}

	Indexing indexer;

	fseek(index, (id - 1) * sizeof(Indexing), SEEK_SET);
	fread(&indexer, sizeof(Indexing), 1, index);

	if (!indexer.exists) {
		fclose(index);
		fclose(db);
		return false;
	}

	fseek(db, indexer.address, SEEK_SET);
	fread(pl, sizeof(Singer), 1, db);

	fclose(db);
	fclose(index);
	return true;
}

bool updateSinger(Singer* pl) {
	FILE* index = fopen("Singer.ind", "r+b");
	FILE* db = fopen("Singer.fl", "r+b");
	if (!AreFiles(db, index) || !IsIndex(index, pl->id)) {
		fclose(index);
		fclose(db);
		return false;
	}

	Indexing indexer;

	fseek(index, (pl->id - 1) * sizeof(Indexing), SEEK_SET);
	fread(&indexer, sizeof(Indexing), 1, index);

	if (!indexer.exists) {
		fclose(index);
		fclose(db);
		return false;
	}

	fseek(db, indexer.address, SEEK_SET);
	fwrite(pl, sizeof(Singer), 1, db);

	fclose(index);
	fclose(db);

	return true;
}

bool delSinger(Singer* pl, Rate& Rate) {

	FILE* index = fopen("Singer.ind", "r+b");
	if (index == NULL) {
		fclose(index);
		return false;
	}

	Indexing indexer;

	fseek(index, (pl->id - 1) * sizeof(Indexing), SEEK_SET);
	fread(&indexer, sizeof(Indexing), 1, index);

	indexer.exists = 0;

	fseek(index, (pl->id - 1) * sizeof(Indexing), SEEK_SET);
	fwrite(&indexer, sizeof(Indexing), 1, index);
	fclose(index);

	deleteIds(pl->id);

	if (pl->amountOfFirstPlaces > 0) {
		FILE* sts = fopen("Rate.fl", "r+b");

		fseek(sts, pl->address_of_first_rate, SEEK_SET);

		for (int i = 0; i < pl->amountOfFirstPlaces; ++i) {
			fread(&Rate, sizeof(Rate), 1, sts);
			fclose(sts);
			delRate(Rate, pl);

			FILE* sts = fopen("Rate.fl", "r+b");
			fseek(sts, Rate.nextL_address, SEEK_SET);
		}
		fclose(sts);
	}
	fclose(index);
	return true;
}