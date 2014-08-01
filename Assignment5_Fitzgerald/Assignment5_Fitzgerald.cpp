/**************************************************************************
** Programmer: Luke Fitzgerald
** Filename: Assignment5_Fitzgerald.cpp
** Purpose: Random data access program
***************************************************************************/

#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <ctype.h>
#include <cmath>

//prototypes
int getInt(char *prompt);
int getInt(char *prompt, int num);
float getFloat(char *prompt);
char getChar(char *prompt);
void getString(char *prompt, char *var);
bool validInt(char *val);
bool validFloat(char *val);
bool validState(char *str);
bool validName(char *str);
bool validDiscountCode(char code);
bool validDate(char *str);
void listFile(int *idArray, int *positionArray , FILE *recordsArray, int numRecs);
bool deleteRecord(int *idArray, int *positionArray, FILE *r, int numRecords);
void searchFile(int *idArray, int *positionArray, FILE *r, int numRecs);
bool addRecord(int *idArray, int *positionArray,  FILE *r, int numRecords, char *buff);
void updateIndex(int id, int *idArray, int *positionArray, int numRecords);

struct date {
	int month, day, yr;
};
struct indexFileStructure {
	int id, pos;
};
struct recordFileStruc {
	int custID;
	char name[28], state[3], discountCode;
	double balanceDue;
	date date;
};

int main() {
	int done = 0;
	int choice;
	int numRecords = 0;
	int index = 0;
	int ID[100];
	int position[100];
	indexFileStructure record;
	FILE *f;
	FILE *r;
	char buff[40];
	bool deleteSuccess;
	bool newRecord;
	
	//insert 999 into entire index and position array
	for (int i = 0; i < 100; i++) {
		ID[i] = 999;
		position[i] = 999;
	}

	//open records file
	fopen_s(&r, "ASSIGNV1.RND", "rb+");
	//open index file
	fopen_s(&f, "ASSIGNV1.IND", "rb");
	
	fread(&record, sizeof(record), 1, f);
	while (!feof(f)) {
		numRecords ++;
		ID[index] = record.id;
		position[index] = record.pos;
		index++;
		fread(&record, sizeof(record), 1, f);
	}
	fclose(f);

	while (done == 0) {
		printf("Please choose an option\n\n");
		printf("1. List File\n2. Search the File\n3. Add a Record\n4. Delete a Record\n5. Exit\n\n");
		choice = getInt("Choice: ");

		switch (choice) {
		case 1:
			listFile(ID, position, r, numRecords);
			break;	
		case 2:
			searchFile(ID, position, r, numRecords);
			break;
		case 3:
			newRecord = addRecord(ID, position, r, numRecords, buff);
			if (newRecord == true) {
				numRecords ++;
				printf("Record added!\n\n");
			}
			break;
		case 4:
			deleteSuccess = deleteRecord(ID, position, r, numRecords);
			if (deleteSuccess == true) {
				numRecords --;
				printf("Record successfully deleted.\n\n");
			}
			break;
		case 5:
			fclose(r);
			done = 1;
			break;
		}
	
	}

	return 0;
}

bool validInt(char *val) {
	int tOrf = 0;

	for (int i = 0; i < strlen(val); i++) {
		if (isdigit(val[i]) == 0) {
			tOrf = 1;
			break;
		}
	}
	
	if (tOrf == 1) {
		printf("Customer ID has to be numbers only\n\n");
		return false;
	}
	else {
		return true;
	}
}

bool validFloat(char *val) {
	int tOrf = 0;
	int ct = 0;
	int ind = NULL;
	
	float valNum = atof(val);
	//printf("%f", valNum);
	
	for (int i = 0; i < strlen(val); i++) {
		if (isdigit(val[i]) != 0 || val[i] == '.') {
			if (val[i] == '.') {
				ind = i;
				ct++;
				if (ct > 1) {printf("You can only have 1 decimal point\n\n");return false;}
			}
		}
		else {
			tOrf = 1;
			break;
		}
	}
	
	if (fmod(valNum * 100, 1) != 0) {
		printf("You cant enter a number with more than two decimals\n\n");
		return false;
	}
			
	if (tOrf == 1) {
		printf("Balance ue can only contain numbers and periods\n\n");
		return false;
	}
	else {
		return true;
	}
}

bool validIDNum(int num) {
	if (num > 99 && num < 999) {
		return true;
	}
	else {
		printf("ID number must be 3 digits\n\n");
		return false;
	}
}

bool validName(char *str) {
	if (strlen(str) < 1) {
		printf("Name cannot be blank\n\n");
		return false;
	}
	else if (strlen(str) > 27) {
		printf("Name is too long");
		return false;
	}
	else {
		return true;
	}
}

bool validState(char *str) {
	char state[51][3];
	char statesFullName[40];
	int ctrl = 0;
	int first  = 0;
	int last = 50;
	int found = 0;
	int mid, index;


	//load table__1 time only
	while (ctrl == 0) {
		FILE *f;
		fopen_s(&f, "STATES.DAT", "r");

		for (int i = 0; i < 50; i++) {
			fgets(statesFullName, 40, f);
			
			fgets(state[i], 40, f);
			state[i][strlen(state[i])-1] = '\0';
		}
		ctrl = 1;
	} 

	//search table
	if (strlen(str) != 2) {
		printf("String is not 2 digits long\n");
		return false;
	}
	else {
		for (int i = 0; i < 3; i++) {
			str[i] = toupper(str[i]);
		}

		int found = 0;

		//search table
		for (int i = 0; i < 50; i++) {
			if (strcmp(str, state[i]) == 0) {
				return true;
			}
			else {
				//printf("That state abbreviation is invalid\n\n");
				found = 1;
			}
		}
		if (found == 1) {
			printf("That state abbreviation is invalid\n\n");
			return false;
		}
	}
}

bool validDiscountCode(char code) {
	if (code == NULL) {
		//printf("Yes");
		return true;
	}
	else {
		char dcode = toupper(code);

		switch (dcode) {
		case 'A':
			//printf("Yes");
			return true;
		case 'B':
			//printf("Yes");
			return true;
		case 'C':
			//printf("Yes");
			return true;
		default:
			return false;
		}
	}
}

bool validDate(char *str) {
	int m1, d1, y1;
	char mm[2];
	char dd[2];
	char yr[4];
	
	if (strlen(str) != 10) {printf("Please enter the date in the format mm/dd/yyyy\n\n"); return false;}

	for (int i = 0 ; i < strlen(str); i++) {
		if (isdigit(str[i]) != 0 || str[i] == '/') {}
		else {
			return false;
		}
	}
	
	char slash1 = str[2];
	char slash2 = str[5];

	if (slash1 != '/' || slash2 != '/') {
		printf("Please enter the date in the format mm/dd/yyyy\n\n");
		return false;
	}

	
	for (int i = 0; i < 2; i++) {
		mm[i] = str[i];
	}
	for (int i = 0; i < 2; i++) {
		dd[i] = str[i + 3]; 
	}
	for (int i = 0; i < 4; i++) {
		yr[i] = str[i + 6];
	}
	
	m1 = atoi(mm);
	d1 = atoi(dd);
	y1 = atoi(yr);

	if (m1 < 1 || m1 > 12) {return false;}
	
	if (d1 < 1 || d1 > 31) {return false;}
	
	//check to make sure the months with 30 days are accounted for
	if (m1 == 4 || m1 == 6 || m1 == 9 || m1 == 11) {
		if (d1 > 30) {printf("This month does not have that many days"); return false;}
		else {return true;}
	}
	//dealing with february
	else if (m1 == 2) {
		if (d1 < 29) {return true;}
		else if (d1 > 29) {printf("February does not have that many days"); return false;}
		else if (y1 % 4 != 0 && d1 > 28) {printf("February (of that year) does not have that many days"); return false;}
		else if (y1 % 4 == 0) {
			if (y1 % 100 == 0 && y1 % 400 != 0 && d1 > 28) {printf("February (of that year) does not have that many days"); return false;}
			else if (y1 % 100 == 0 && y1 % 400 == 0 && d1 > 29) {printf("February does not have that many days"); return false;}
			else {return true;}
		}
	}
	else {return true;} 
}

void listFile(int *idArray, int *positionArray , FILE *recordsArray, int numRecs) {
	system("CLS");
	int pos;
	recordFileStruc struct1;

	printf("\n%-15s%-20s%-8s%-15s%-15s%-10s", "Customer ID", "Name", "State", "Discount Code", "Balance Due", "Date Due\n\n");
		
	for (int i = 0; i <= numRecs; i ++) {
		if (positionArray[i] != 999) {
			int pos = positionArray[i];

			fseek(recordsArray, sizeof(recordFileStruc) * pos, SEEK_SET);
			fread(&struct1, sizeof(recordFileStruc), 1, recordsArray);
		
			printf("\n%-15i%-20s%-8s%-15c%-15.2f%i/%i/%i", struct1.custID, struct1.name, struct1.state, struct1.discountCode, struct1.balanceDue, struct1.date.month, struct1.date.day, struct1.date.yr);
		}
	}
	printf("\n\n");

}

void searchFile(int *idArray, int *positionArray, FILE *r, int numRecords) {
	int num = 0;
	recordFileStruc struct1;
	int first = 0, found = 0, mid, index = 0, last = numRecords;

	num = getInt("Please enter the 3 digit customer ID number you are looking for: ", 1);

	while (first <= last && found == 0) {
		mid = (first + last) / 2;

		if (idArray[mid] == num) {
			found = 1;
			index = mid;
		}
		else {
			if (idArray[mid] < num) {
				first = mid + 1;
			}
			else {
				last = mid - 1;
			}
		} 
	}

	if (found == 1) {
		int local = positionArray[index];
			
		fseek(r, sizeof(recordFileStruc) * local, SEEK_SET);
		fread(&struct1, sizeof(recordFileStruc), 1, r);
			
		printf("\n%-15s%-20s%-8s%-15s%-15s%-10s", "Customer ID", "Name", "State", "Discount Code", "Balance Due", "Date Due\n\n");
		printf("\n%-15i%-20s%-8s%-15c%-15.2f%i/%i/%i\n\n", struct1.custID, struct1.name, struct1.state, struct1.discountCode, struct1.balanceDue, struct1.date.month, struct1.date.day, struct1.date.yr);
			
	}
	else {
		printf("RECORD NOT IN DATABASE\n\n");
	}
}

bool deleteRecord(int *idArray, int *positionArray, FILE *r, int numRecords) {
	int num;
	int first = 0, found = 0, mid, index, last = numRecords;

	num = getInt("Please enter the 3 digit customer ID number you want to delete: ", 1);

	while (first <= last && found == 0) {
		mid = (first + last) / 2;

		if (idArray[mid] == num) {
			found = 1;
			index = mid;
		}
		else {
			if (idArray[mid] < num) {
				first = mid + 1;
			}
			else {
				last = mid - 1;
			}
		} 
	}
	
	if (found == 1) {
		idArray[index] = 999;
		positionArray[index] = 999;

		int temp, temp2, ind, swap, last;
		last = numRecords - 1;

		while (last > 0) {
			ind = 0;
			swap = 0;
			temp = 0;
			temp2 = 0;

			while (ind < last) {
				if (idArray[ind] > idArray[ind + 1]) {
					temp = idArray[ind];
					temp2 = positionArray[ind];

					idArray[ind] = idArray[ind + 1];
					positionArray[ind] = positionArray[ind + 1];

					idArray[ind + 1] = temp;
					positionArray[ind + 1] = temp2;
					
					swap = 1;
				}
				else {
					ind ++;
				}
			}
			if (swap == 0) {
				last = 0;
			}
			else {
				last = last - 1;
			}
		}
		return true;
	}
	else {
		printf("Record was not found\n\n");
		return false;
	}
}

bool addRecord(int *idArray, int *positionArray, FILE *r, int numRecords, char *buff) {
	recordFileStruc newStruct;
	int valid = 0;

	//enter new ID
	while (valid == 0) {
		int id = getInt("Enter a new, 3 digit customer ID: ", 1); 
		
		for (int i = 0; i < numRecords; i++) {
			if (id == idArray[i]) {
				printf("That ID number is already taken.\n");
				break;
			}
			else {
				newStruct.custID = id;
				//printf("DONE");
				valid = 1;
			}
		}
	}
	valid = 0;

	//enter a new name
	getString("Please enter the new customer's name: ", newStruct.name);
	while (valid == 0) {
		if (validName(newStruct.name) == true) {
			valid = 1;
		}
		else {
			getString("Invalid Entry; Please enter the new customer's name: ", newStruct.name);
		}
	}
	valid = 0;

	//enter new state
	getString("Please enter the customer's 2 digit state: ", newStruct.state);
	while (valid == 0) {
		if (validState(newStruct.state) == true) {
			valid = 1;
		}
		else {
			getString("Please enter the 2 digit state: ", newStruct.state);
		}
	}
	valid = 0;

	//enter discount code
	newStruct.discountCode = getChar("Please enter a discount code (A, B, C, or leave blank): ");
	while (valid == 0) {
		if (validDiscountCode(newStruct.discountCode) == true) {
			valid = 1;
		}
		else {
			newStruct.discountCode = getChar("Please enter a valid (A, B, or C) discount code, or leave blank: ");
		}
	}
	valid = 0;

	//enter balance due
	newStruct.balanceDue = getFloat("Please enter the balance due: ");

	//get date
	getString("Please enter the payment due date (MM/DD/YYYY): ", buff);
	
	while (valid == 0) {
		if (validDate(buff) == true) {
			valid = 1;
		}
		else {
			getString("Invalid Entry; Please enter the payment due date in the format MM/DD/YYY: ", buff);
		}
	}
	valid = 0;
	
	//enter into date struct
	char month[2], day[2], year[4];
	int mm, dd, yr;
	for (int i = 0; i < 2; i++) {
		month[i] = buff[i];
		day[i] = buff[i + 3];
	}
	for (int i = 0; i < 4; i++) {
		year[i] = buff[i + 6];
	}

	mm = atoi(month);
	dd = atoi(day);
	yr = atoi(year);

	newStruct.date.day = dd;
	newStruct.date.month = mm;
	newStruct.date.yr = yr;
	
	fseek(r, sizeof(recordFileStruc), SEEK_END);
	fwrite(&newStruct, sizeof(recordFileStruc), 1, r);

	//update index file
	updateIndex(newStruct.custID, idArray, positionArray, numRecords);
	return true;
}

void updateIndex(int id, int *idArray, int *positionArray, int numRecords) {
	int valid = 0;
	int temp, temp2, ind, swap, last;
	int ct = 0;

	while (valid == 0) {
		if (idArray[ct] == 999) {
			idArray[ct] = id;
			positionArray[ct] = numRecords + 1;
			valid = 1;
			break;
		}
		ct++;
	}

	//sort
	last = numRecords - 1;

		while (last > 0) {
			ind = 0;
			swap = 0;
			temp = 0;
			temp2 = 0;

			while (ind < last) {
				if (idArray[ind] > idArray[ind + 1]) {
					temp = idArray[ind];
					temp2 = positionArray[ind];

					idArray[ind] = idArray[ind + 1];
					positionArray[ind] = positionArray[ind + 1];

					idArray[ind + 1] = temp;
					positionArray[ind + 1] = temp2;
					
					swap = 1;
				}
				else {
					ind ++;
				}
			}
			if (swap == 0) {
				last = 0;
			}
			else {
				last = last - 1;
			}
		}
}

int getInt(char *prompt, int num) {
	char buffer[50];
	int num1;
	int ctrl = 0;
	printf("%s", prompt);
	gets_s(buffer);
	
	while(ctrl == 0) {
		if (validInt(buffer) == true) {
			num1 = atoi(buffer);
				if (validIDNum(num1)) {
					ctrl = 1;
					return num1;
				}
				else {
					printf("%s", prompt);
					gets_s(buffer);
				}
		}	
		else {
			printf("%s", prompt);
			gets_s(buffer);
		}
	}
}

int getInt(char *prompt) {
	char buffer[50];
	int num1;
	
	printf("%s", prompt);
	gets_s(buffer);
	num1 = atoi(buffer);

	return num1;
}

float getFloat(char *prompt) {
	char buffer[50];
	float num1;
	int ctrl = 0;
	printf("%s", prompt);
	gets_s(buffer);
	
	while(ctrl == 0) {
		if (validFloat(buffer) == true) {
			num1 = atof(buffer);
			ctrl = 1;
			return num1;
		}	
		else {
			printf("%s", prompt);
			gets_s(buffer);
		}
	}
}

char getChar(char *prompt) {
	char buffer[2];
	char character;
	
	printf("%s", prompt);
	gets_s(buffer);
	character = buffer[0];

	return character;
}

void getString(char *prompt, char *var) {
	printf("%s", prompt);
	gets_s(var, 28);
}