#include <iostream>
#include "Structs.h"
#include "Additional.h"
#include "Rate.h"
using namespace std;

int main()
{
    Singer pl;
    Rate lt;
	
	 cout << "Avaliable commands:\n"
		<< "get-m, get-s, del-m, del-s, update-m, update-s, insert-m, insert-s, info-all, close\n";


	while (true)
	{
		string comand;
		int id;

		cin >> comand;
		if (comand == "get-m") {
			 cout << "Enter singer's Id\n";
			 cin >> id;
			if (id > 0 && getSinger(&pl, id)) {
				printSinger(&pl);
			}
			else
				 cout << "Singer wasn't found" <<  endl;
		}
		else if (comand == "get-s") {
			 cout << "Enter singer's Id\n";
			 cin >> id;
			if (getSinger(&pl, id)) {
				 cout << "Enter awards's Id\n";
				 cin >> id;
				if (getRate(&lt, pl, id)) {
					printRate(lt, pl);
				}
				else {
					 cout << "Wrong awards Id" <<  endl;
				}
			}
			else
				 cout << "Wrong singer Id" <<  endl;
		}
		else if (comand == "del-m") {
			cout << "Enter singer's Id\n";
			 cin >> id;
			if (!getSinger(&pl, id))
				 cout << "Wrong singer Id" <<  endl;
			else if (delSinger(&pl, lt))
				 cout << "The singer has been successfully deleted\n";
			else
				 cout << "Something went wrong =(" <<  endl;
		}
		else if (comand == "del-s") {
			 cout << "Enter singer's Id\n";
			 cin >> id;
			if (getSinger(&pl, id)) {
				 cout << "Enter awards's Id\n";
				 cin >> id;
				if (getRate(&lt, pl, id)) {
					delRate(lt, &pl);
					 cout << "Item has been successfully deleted\n";
				}
				else
					 cout << "Wrong awards Id" <<  endl;
			}
			else
				 cout << "Wrong singer Id" <<  endl;
		}
		else if (comand == "update-m") {
			 cout << "Enter singer's Id\n";
			 cin >> id;

			pl.id = id;
			if (!getSinger(&pl, id))
				 cout << "Wrong singer Id" <<  endl;
			else {
				readSinger(&pl);
				if (updateSinger(&pl))
					 cout << "The singer has been updated\n";
				else
					 cout << "Something went wrong =(" <<  endl;
			}
		}
		else if (comand == "update-s") {
			 cout << "Enter singer's Id\n";
			 cin >> id;
			if (getSinger(&pl, id)) {
				 cout << "Enter awards's Id\n";
				 cin >> id;
				if (getRate(&lt, pl, id)) {
					int number;
					 cout << "Enter awards's name\n";
					 cin >> number;
					lt.number = number;
					updateRate(lt, id);
					 cout << "awards has been updated\n";
				}
				else
					 cout << "Wrong awards Id" <<  endl;
			}
			else
				 cout << "Wrong singer Id" <<  endl;
		}
		else if (comand == "insert-m") {
			readSinger(&pl);
			insertSinger(pl);
		}
		else if (comand == "insert-s") {
			 cout << "Enter singer's Id\n";
			 cin >> id;
			if (getSinger(&pl, id)) {
				int id_c = id;
				lt.singer_Id = id;
				 cout << "Enter awards's Id\n";
				 cin >> id;
				if (IsUnique(pl, lt, id)) {
					lt.id = id;
					lt.singer_Id = id_c;
					int number;
					 cout << "Enter awards's number\n";
					 cin >> number;
					lt.number = number;
					insertRate(lt, pl);
					 cout << "awards has been added\n";
				}
				else
					 cout << "Singer already has that Id\n";
			}
			else
				 cout << "Wrong singer Id" <<  endl;
		}
		else if(comand == "info-all")
				getEveryone(&pl);
		else if (comand == "close") {
			 cout << "Closing...";
			exit(0);
		}
		else
			 cout<<"Wrong command\n";

		 cout <<  endl;
	}
	return 0;
}
