#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

//warna text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
//background text
#define BLKB "\e[40m"
#define REDB "\e[41m"
#define GRNB "\e[42m"
#define YELB "\e[43m"
#define BLUB "\e[44m"
#define MAGB "\e[45m"
#define CYNB "\e[46m"
#define WHTB "\e[47m"
//underlined text
#define UBLK "\e[4;30m"
#define URED "\e[4;31m"
#define UGRN "\e[4;32m"
#define UYEL "\e[4;33m"
#define UBLU "\e[4;34m"
#define UMAG "\e[4;35m"
#define UCYN "\e[4;36m"
#define UWHT "\e[4;37m"
//reset color
#define CRESET "\e[0m"


typedef struct Data{
	//ini adalah struct Data
	char location[101];
	char city[101];
	long long int price;
	int room;
	int bathroom;
	int carpark;
	char type[101];
	char furnish[101];
} Data;

//Memory allocation array dan kawan2nya
Data *dataArr = NULL;
int currArrSize = 0;
int currDataSize = 0;
void allocateMemory (int initialCap);
void resizeArray();
void deallocateMemory();

//non-essential func:
void exitMsg();
void pressEnter();
void printLogo();

//essential func:
void mainMenu();
void readFile();
//(1) display()
void display();
void displayTableHeader();
void displayTableFooter();
//(2) selectRow()
void selectRow();
bool findStr(char target[], char columnName[]);
bool findLong (long long int target);
bool findInt(int target, char columnName[]);
void findLocation(char target[]);
void findCity(char target[]);
void findPrice(long long int target);
void findRooms(int target);
void findBathroom(int target);
void findCarpark(int target);
void findType(char target[]);
void findFurnish(char target[]);
//3 sortBy()
void sortBy();
void sortByColumn(char columnName[], int left, int right);
void mergeSort(int left, int right, char field[], char choice[]);
void mergeAsc(int left, int mid, int right, char field[]);
void mergeDesc(int left, int mid, int right, char field[]);
void print10();
//4 Export()
void Export();

int main(){
	system("cls");
	int initialCap = 3941;
	allocateMemory(initialCap);
	readFile();
	mainMenu();
	deallocateMemory();
	return 0;
}

//INI KE BAWAH: essential func ===============================================
void mainMenu(){
	//ini adalah main menunya.
	int selection;
	do{
		system("cls");
		printLogo();
		usleep(50000);
		printf(UGRN"What do you want to do?\n"CRESET);
		usleep(50000);
		printf(YEL"1. Display Data\n");
		usleep(50000);
		printf(YEL"2. Search Data\n");
		usleep(50000);
		printf(YEL"3. Sort Data\n");
		usleep(50000);
		printf(YEL"4. Export Data\n");
		usleep(50000);
		printf(YEL"5. Exit\n");
		usleep(50000);
		printf(CRESET"Your Choice: ");
		scanf("%d", &selection); getchar();
		puts("");
		
		switch(selection){
			case 1:
				display();
				pressEnter();
				break;
			case 2:
				selectRow();
				pressEnter();
				break;
			case 3:
				sortBy();
				pressEnter();
				break;
			case 4:
				Export();
				pressEnter();
				break;
			case 5:
				exitMsg();
				break;
		}
	} while(selection != 5);
}

//4 export() ===============================================
void Export(){
	//function ini digunakan untuk export data
	char temp[101];
	printf(CYN"File name: ");
	scanf("%[^\n]", temp); getchar();
	char fileName[101];
	sprintf(fileName, "%s.csv", temp);
	printf(UGRN"Data successfully written to file %s\n", fileName);
	printf(CRESET"");
	FILE *f = fopen(fileName, "w");
	for (int i = 0; i < currDataSize; i++){
		fprintf(f, "%s,%s,%lld,%d,%d,%d,%9s,%s\n", 
			dataArr[i].location,
			dataArr[i].city,
			dataArr[i].price,
			dataArr[i].room,
			dataArr[i].bathroom,
			dataArr[i].carpark,
			dataArr[i].type,
			dataArr[i].furnish);
	}
}

//3 sortBy() ===============================================
void sortBy() {
	//function ini untuk ngesort data kita berdasarkan column yang diinginkan user
    char columnName[100];
    printf(CYN"Choose column [Case Sensitive]: ");
    scanf("%[^\n]", columnName);
    getchar();
    printf(CRESET"");

    if (strcmp(columnName, "Location") == 0 ||
        strcmp(columnName, "City") == 0 ||
        strcmp(columnName, "Price") == 0 ||
        strcmp(columnName, "Rooms") == 0 ||
        strcmp(columnName, "Bathroom") == 0 ||
        strcmp(columnName, "Carpark") == 0 ||
        strcmp(columnName, "Type") == 0 ||
        strcmp(columnName, "Furnish") == 0) {
        //kalau nama column valid, sort data sesuai nama column yang diinput
        sortByColumn(columnName, 0, currDataSize - 1);
    } else {
    	//kalau gk valid, print gk valid
        printf(RED"No column found :(\n"CRESET);
    }
}

void sortByColumn(char columnName[], int left, int right){
	char choice[101];
	//di function ini, user diminta untuk sort data secara ascending atau descending
	//program akan terus meminta input dari user jika yang diinput bukan "asc" atau "desc"
    do {
        printf(CYN"Sort ascending or descending [\"asc\" or \"desc\"]? ");
        scanf("%[^\n]", choice);
        getchar();
        printf(CRESET"");
    } while (strcmp(choice, "asc") != 0 && strcmp(choice, "desc") != 0);

    mergeSort(left, right, columnName, choice);
    print10();
}

void mergeSort(int left, int right, char field[], char choice[]){
	//ini function untuk mergeSort
	if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(left, mid, field, choice);
        mergeSort(mid + 1, right, field, choice);
        if (strcmp(choice, "asc") == 0) {
        	//kalau "asc", mergeAsc()
            mergeAsc(left, mid, right, field);
        } else {
        	//kalau bukan "asc" ("desc"), mergeDesc()
            mergeDesc(left, mid, right, field);
        }
    }
}

void mergeDesc(int left, int mid, int right, char field[]){
	//ini function untuk merge secara descending
	int i, j, k;
	int n1 = mid - left + 1;
	int n2 = right - mid;
	
	Data leftArr[n1];
	Data rightArr[n2];
	
	for (i = 0; i < n1; i++){
		leftArr[i] = dataArr[left + i];
	}
	for (i = 0; i < n2; i++){
		rightArr[i] = dataArr[mid + 1 + i];
	}
	
	i = 0, j = 0, k = left;
	while(i < n1 && j < n2){
		int compareResult;
		if (strcmp(field, "Location") == 0) {
            compareResult = strcmp(leftArr[i].location, rightArr[j].location);
        }
		else if (strcmp(field, "City") == 0) {
            compareResult = strcmp(leftArr[i].city, rightArr[j].city);
        }
		else if (strcmp(field, "Price") == 0) {
            compareResult = (leftArr[i].price - rightArr[j].price);
        }
        else if (strcmp(field, "Rooms") == 0) {
            compareResult = (leftArr[i].room - rightArr[j].room);
        }
        else if (strcmp(field, "Bathroom") == 0) {
            compareResult = (leftArr[i].bathroom - rightArr[j].bathroom);
        }
        else if (strcmp(field, "Carpark") == 0) {
            compareResult = (leftArr[i].carpark - rightArr[j].carpark);
        }
        else if (strcmp(field, "Type") == 0) {
            compareResult = strcmp(leftArr[i].type, rightArr[j].type);
        }
        else if (strcmp(field, "Furnish") == 0) {
            compareResult = strcmp(leftArr[i].furnish, rightArr[j].furnish);
        }

        if (compareResult >= 0) {
            dataArr[k] = leftArr[i];
            i++;
        } else {
            dataArr[k] = rightArr[j];
            j++;
        }
        k++;
	}
	while (i < n1) {
        dataArr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        dataArr[k] = rightArr[j];
        j++;
        k++;
    }
}

void mergeAsc(int left, int mid, int right, char field[]){
	//ini function untuk merge secara ascending
	int i, j, k;
	int n1 = mid - left + 1;
	int n2 = right - mid;
	
	Data leftArr[n1];
	Data rightArr[n2];
	
	for (i = 0; i < n1; i++){
		leftArr[i] = dataArr[left + i];
	}
	for (i = 0; i < n2; i++){
		rightArr[i] = dataArr[mid + 1 + i];
	}
	
	i = 0, j = 0, k = left;
	while(i < n1 && j < n2){
		int compareResult;
		if (strcmp(field, "Location") == 0) {
            compareResult = strcmp(leftArr[i].location, rightArr[j].location);
        }
		else if (strcmp(field, "City") == 0) {
            compareResult = strcmp(leftArr[i].city, rightArr[j].city);
        }
		else if (strcmp(field, "Price") == 0) {
            compareResult = (leftArr[i].price - rightArr[j].price);
        }
        else if (strcmp(field, "Rooms") == 0) {
            compareResult = (leftArr[i].room - rightArr[j].room);
        }
        else if (strcmp(field, "Bathroom") == 0) {
            compareResult = (leftArr[i].bathroom - rightArr[j].bathroom);
        }
        else if (strcmp(field, "Carpark") == 0) {
            compareResult = (leftArr[i].carpark - rightArr[j].carpark);
        }
        else if (strcmp(field, "Type") == 0) {
            compareResult = strcmp(leftArr[i].type, rightArr[j].type);
        }
        else if (strcmp(field, "Furnish") == 0) {
            compareResult = strcmp(leftArr[i].furnish, rightArr[j].furnish);
        }

        if (compareResult <= 0) {
            dataArr[k] = leftArr[i];
            i++;
        } else {
            dataArr[k] = rightArr[j];
            j++;
        }
        k++;
	}
	
	while (i < n1) {
        dataArr[k] = leftArr[i];
        i++;
        k++;
    }
    while (j < n2) {
        dataArr[k] = rightArr[j];
        j++;
        k++;
    }
}

void print10(){
	//ini untuk print 10 data pertama setelah disort
	displayTableHeader();
	for (int i = 0; i < 10; i++){
		printf("| %-25s | %-13s | %-9lld | %-5d | %-8d | %-8d | %-9s | %-11s |\n", 
			dataArr[i].location,
			dataArr[i].city,
			dataArr[i].price,
			dataArr[i].room,
			dataArr[i].bathroom,
			dataArr[i].carpark,
			dataArr[i].type,
			dataArr[i].furnish);
	}
	displayTableFooter();
}

//2. selectRow() ===============================================
void selectRow(){
	//function ini untuk melakukan searching
	char columnName[100];
	printf(CYN"Choose column [Case Sensitive]: ");
	scanf("%[^\n]", columnName); getchar();
	
	//melakukan selection untuk memastikan nama column yang dimasukkan itu valid
	if (strcmp(columnName, "Location") == 0){
		printf("What data do you want to find? ");
		char target[100];
		scanf("%[^\n]", target); getchar();
		
		//ngecek ada data yang dicari atau nggk pake function di bawah ini:
		bool found = findStr(target, columnName);
		if (found){
			printf(CRESET"");
			//kalo ada, show data
			displayTableHeader();
			findLocation(target);
			displayTableFooter();
		}
		else{
			//kalo gk ada, print data not found
			printf(RED"Data not found :(\n");
		}
	}
	//LAKUKAN HAL YANG SAMA UTK SEMUA TIPE DATA
	else if(strcmp(columnName, "City") == 0){
		printf("What data do you want to find? ");
		char target[100];
		scanf("%[^\n]", target); getchar();
		bool found = findStr(target, columnName);
		if (found){
			printf(CRESET"");
			displayTableHeader();
			findCity(target);
			displayTableFooter();
		}
		else{
			printf(RED"Data not found :(\n");
		}
	}
	else if(strcmp(columnName, "Price") == 0){
		printf("What data do you want to find? ");
		long long int target;
		scanf("%lld", &target); getchar();
		bool found = findLong(target);
		if (found){
			printf(CRESET"");
			displayTableHeader();
			findPrice(target);
			displayTableFooter();
		}
		else{
			printf(RED"Data not found :(\n");
		}
	}
	
	else if(strcmp(columnName, "Rooms") == 0){
		printf("What data do you want to find? ");
		int target;
		scanf("%d", &target); getchar();
		bool found = findInt(target, columnName);
		if (found){
			printf(CRESET"");
			displayTableHeader();
			findRooms(target);
			displayTableFooter();
		}
		else{
			printf(RED"Data not found :(\n");
		}
	}
	
	else if(strcmp(columnName, "Bathroom") == 0){
		printf("What data do you want to find? ");
		int target;
		scanf("%d", &target); getchar();
		bool found = findInt(target, columnName);
		if (found){
			printf(CRESET"");
			displayTableHeader();
			findBathroom(target);
			displayTableFooter();
		}
		else{
			printf(RED"Data not found :(\n");
		}
	}
	else if(strcmp(columnName, "Carpark") == 0){
		printf("What data do you want to find? ");
		int target;
		scanf("%d", &target); getchar();
		bool found = findInt(target, columnName);
		if (found){
			printf(CRESET"");
			displayTableHeader();
			findCarpark(target);
			displayTableFooter();
		}
		else{
			printf(RED"Data not found :(\n");
		}
	}
	else if(strcmp(columnName, "Type") == 0){
		printf("What data do you want to find? ");
		char target[100];
		scanf("%[^\n]", target); getchar();
		bool found = findStr(target, columnName);
		if (found){
			printf(CRESET"");
			displayTableHeader();
			findType(target);
			displayTableFooter();
		}
		else{
			printf(RED"Data not found :(\n");
		}
	}
	else if(strcmp(columnName, "Furnish") == 0){
		printf("What data do you want to find? ");
		char target[100];
		scanf("%[^\n]", target); getchar();
		bool found = findStr(target, columnName);
		if (found){
			printf(CRESET"");
			displayTableHeader();
			findFurnish(target);
			displayTableFooter();
		}
		else{
			printf(RED"Data not found :(\n");
		}
	}
	else{
		printf(RED"No column found :(\n");
	}
	printf(CRESET"");
}

//ini untuk print data yang diinginkan
void findLocation(char target[]){
	for (int i = 0; i < currDataSize; i++){
		if(strcmp(dataArr[i].location, target) == 0){
			printf("| %-25s | %-13s | %-9lld | %-5d | %-8d | %-8d | %-9s | %-11s |\n", 
				dataArr[i].location,
				dataArr[i].city,
				dataArr[i].price,
				dataArr[i].room,
				dataArr[i].bathroom,
				dataArr[i].carpark,
				dataArr[i].type,
				dataArr[i].furnish);
		}
	}
}

//ini untuk print data yang diinginkan
void findCity(char target[]){
	for (int i = 0; i < currDataSize; i++){
		if(strcmp(dataArr[i].city, target) == 0){
			printf("| %-25s | %-13s | %-9lld | %-5d | %-8d | %-8d | %-9s | %-11s |\n", 
				dataArr[i].location,
				dataArr[i].city,
				dataArr[i].price,
				dataArr[i].room,
				dataArr[i].bathroom,
				dataArr[i].carpark,
				dataArr[i].type,
				dataArr[i].furnish);
		}
	}
}

//ini untuk print data yang diinginkan
void findPrice(long long int target){
	for (int i = 0; i < currDataSize; i++){
		if(dataArr[i].price == target){
			printf("| %-25s | %-13s | %-9lld | %-5d | %-8d | %-8d | %-9s | %-11s |\n", 
				dataArr[i].location,
				dataArr[i].city,
				dataArr[i].price,
				dataArr[i].room,
				dataArr[i].bathroom,
				dataArr[i].carpark,
				dataArr[i].type,
				dataArr[i].furnish);
		}
	}
}

//ini untuk print data yang diinginkan
void findRooms(int target){
	for (int i = 0; i < currDataSize; i++){
		if(dataArr[i].room == target){
			printf("| %-25s | %-13s | %-9lld | %-5d | %-8d | %-8d | %-9s | %-11s |\n", 
				dataArr[i].location,
				dataArr[i].city,
				dataArr[i].price,
				dataArr[i].room,
				dataArr[i].bathroom,
				dataArr[i].carpark,
				dataArr[i].type,
				dataArr[i].furnish);
		}
	}
}

//ini untuk print data yang diinginkan
void findBathroom(int target){
	for (int i = 0; i < currDataSize; i++){
		if(dataArr[i].bathroom == target){
			printf("| %-25s | %-13s | %-9lld | %-5d | %-8d | %-8d | %-9s | %-11s |\n", 
				dataArr[i].location,
				dataArr[i].city,
				dataArr[i].price,
				dataArr[i].room,
				dataArr[i].bathroom,
				dataArr[i].carpark,
				dataArr[i].type,
				dataArr[i].furnish);
		}
	}
}

//ini untuk print data yang diinginkan
void findCarpark(int target){
	for (int i = 0; i < currDataSize; i++){
		if(dataArr[i].carpark == target){
			printf("| %-25s | %-13s | %-9lld | %-5d | %-8d | %-8d | %-9s | %-11s |\n", 
				dataArr[i].location,
				dataArr[i].city,
				dataArr[i].price,
				dataArr[i].room,
				dataArr[i].bathroom,
				dataArr[i].carpark,
				dataArr[i].type,
				dataArr[i].furnish);
		}
	}
}

//ini untuk print data yang diinginkan
void findType(char target[]){
	for (int i = 0; i < currDataSize; i++){
		if(strcmp(dataArr[i].type, target) == 0){
			printf("| %-25s | %-13s | %-9lld | %-5d | %-8d | %-8d | %-9s | %-11s |\n", 
				dataArr[i].location,
				dataArr[i].city,
				dataArr[i].price,
				dataArr[i].room,
				dataArr[i].bathroom,
				dataArr[i].carpark,
				dataArr[i].type,
				dataArr[i].furnish);
		}
	}
}

//ini untuk print data yang diinginkan
void findFurnish(char target[]){
	for (int i = 0; i < currDataSize; i++){
		if(strcmp(dataArr[i].furnish, target) == 0){
			printf("| %-25s | %-13s | %-9lld | %-5d | %-8d | %-8d | %-9s | %-11s |\n", 
				dataArr[i].location,
				dataArr[i].city,
				dataArr[i].price,
				dataArr[i].room,
				dataArr[i].bathroom,
				dataArr[i].carpark,
				dataArr[i].type,
				dataArr[i].furnish);
		}
	}
}

//ngecek ada data yang dimau atau nggk
bool findInt(int target, char columnName[]){
	bool found = false;
	if(strcmp(columnName, "Rooms") == 0){
		for (int i = 0; i < currDataSize; i++){
			if (dataArr[i].room == target){
				found = true;
				break;
			}
		}
	}
	else if (strcmp(columnName, "Bathroom") == 0){
		for (int i = 0; i < currDataSize; i++){
			if (dataArr[i].bathroom == target){
				found = true;
				break;
			}
		}
	}
	else if (strcmp(columnName, "Carpark") == 0){
		for (int i = 0; i < currDataSize; i++){
			if (dataArr[i].carpark == target){
				found = true;
				break;
			}
		}
	}
	return found;
}

//ngecek ada data yang dimau atau nggk
bool findLong(long long int target){
	for (int i = 0; i < currDataSize; i++){
		if(dataArr[i].price == target){
			return true;
		}
	}
	return false;
}

//ngecek ada data yang dimau atau nggk
bool findStr(char target[], char columnName[]){
	bool found = false;
	if(strcmp(columnName, "Location") == 0) {
		for (int i = 0; i < currDataSize; i++){
			if(strcmp(target, dataArr[i].location) == 0){
				found = true;
				break;
			}
		}
	}
	else if(strcmp(columnName, "City") == 0) {
		for (int i = 0; i < currDataSize; i++){
			if(strcmp(target, dataArr[i].city) == 0){
				found = true;
				break;
			}
		}
	}
	else if(strcmp(columnName, "Type") == 0) {
		for (int i = 0; i < currDataSize; i++){
			if(strcmp(target, dataArr[i].type) == 0){
				found = true;
				break;
			}
		}
	}
	else if(strcmp(columnName, "Furnish") == 0) {
		for (int i = 0; i < currDataSize; i++){
			if(strcmp(target, dataArr[i].furnish) == 0){
				found = true;
				break;
			}
		}
	}
	return found;
}

//1. display() ===============================================
void display(){
	//function ini untuk mendisplay data dalam structure
	int numRows;
	printf(CYN"Number of rows: ");
	scanf("%d", &numRows); getchar();
	printf(CRESET"");
	displayTableHeader();
	//cek jika angka yang diinput user lebih besar dari total data yang ada
	if (numRows > currDataSize){
		//print semuanya yang ada
		for (int i = 0; i < currDataSize; i++) {
			printf("| %-25s | %-13s | %-9lld | %-5d | %-8d | %-8d | %-9s | %-11s |\n", 
				dataArr[i].location,
				dataArr[i].city,
				dataArr[i].price,
				dataArr[i].room,
				dataArr[i].bathroom,
				dataArr[i].carpark,
				dataArr[i].type,
				dataArr[i].furnish);
		}
	}
	else{
		//print sesuai yang diinginkan user
		for (int i = 0; i < numRows; i++) {
			printf("| %-25s | %-13s | %-9lld | %-5d | %-8d | %-8d | %-9s | %-11s |\n", 
				dataArr[i].location,
				dataArr[i].city,
				dataArr[i].price,
				dataArr[i].room,
				dataArr[i].bathroom,
				dataArr[i].carpark,
				dataArr[i].type,
				dataArr[i].furnish);
		}
	}
	displayTableFooter();
}

void displayTableHeader(){
	//ini utk print headernya aja
	printf("=================================================================================================================\n");
	printf("| %-25s | %-13s | %-9s | %-5s | %-8s | %-8s | %-9s | %-11s |\n", "Location", "City", "Price", "Rooms", "Bathroom", "Carpark", "Type", "Furnish");
	printf("=================================================================================================================\n");
}

void displayTableFooter(){
	//ini utk print footernya table
	printf("=================================================================================================================\n");
}

void readFile(){
	//ini utk read semua file yang ada di file.csv
	FILE *f = fopen("file.csv", "r");
	if(f == NULL){
		printf(RED"Error in reading file\n"CRESET);
	}
	while (!feof(f)){
		if(currDataSize >= currArrSize){
			resizeArray();
		}
		fscanf(f, "%[^,],%[^,],%lld,%d,%d,%d,%[^,],%[^\n]\n", 
	       dataArr[currDataSize].location,
	       dataArr[currDataSize].city,
	       &dataArr[currDataSize].price,
	       &dataArr[currDataSize].room,
	       &dataArr[currDataSize].bathroom,
	       &dataArr[currDataSize].carpark,
	       dataArr[currDataSize].type,
	       dataArr[currDataSize].furnish);
		currDataSize++;
	}
	fclose(f);
}

//INI KE BAWAH: memory allocation ===============================================
void allocateMemory (int initialCap){
	//untuk allocateMemory dari array of Data kita
	
	// Check if memory has already been allocated
    if (dataArr != NULL) {
        printf("Memory has already been allocated\n");
        pressEnter();
        return;
    }
    dataArr = (Data*)malloc(initialCap * sizeof(Data));
    if (dataArr == NULL) {
        printf("Memory allocation error\n");
        pressEnter();
        return;
    }
    currArrSize = initialCap;
}

void resizeArray(){
	//ini kalo size yg disediakan di awal gk cukup
	
	// Double the capacity
    currArrSize *= 2;

    // Reallocate the memory
    dataArr = (Data*)realloc(dataArr, currArrSize * sizeof(Data));
    if (dataArr == NULL) {
        printf("Memory reallocation error.\n");
        pressEnter();
        return;
    }
}

void deallocateMemory(){
	// Free the allocated memory
    free(dataArr);

    // Reset pointers and sizes
    dataArr = NULL;
    currArrSize = 0;
	currDataSize = 0;
}

//INI KE BAWAH: non-essential func ===============================================
void printLogo(){
	printf(RED"                                                     dddddddd                                                      \n");
    usleep(50000);
    printf(GRN "LLLLLLLLLLL                                          d::::::d                                                      \n");
    usleep(50000);
	printf(YEL"L:::::::::L                                          d::::::d                                                      \n");
    usleep(50000);
	printf(BLU"L:::::::::L                                          d::::::d                                                      \n");
    usleep(50000);
	printf(MAG"LL:::::::LL                                          d:::::d                                                       \n");
    usleep(50000);
	printf(CYN"  L:::::L                 aaaaaaaaaaaaa      ddddddddd:::::d   aaaaaaaaaaaaa   zzzzzzzzzzzzzzzzz  aaaaaaaaaaaaa   \n");
    usleep(50000);
	printf(YEL"  L:::::L                 a::::::::::::a   dd::::::::::::::d   a::::::::::::a  z:::::::::::::::z  a::::::::::::a  \n");
    usleep(50000);
	printf(RED"  L:::::L                 aaaaaaaaa:::::a d::::::::::::::::d   aaaaaaaaa:::::a z::::::::::::::z   aaaaaaaaa:::::a \n");
    usleep(50000);
	printf(GRN"  L:::::L                          a::::ad:::::::ddddd:::::d            a::::a zzzzzzzz::::::z             a::::a \n");
    usleep(50000);
	printf(YEL"  L:::::L                   aaaaaaa:::::ad::::::d    d:::::d     aaaaaaa:::::a       z::::::z       aaaaaaa:::::a \n");
    usleep(50000);
	printf(BLU"  L:::::L                 aa::::::::::::ad:::::d     d:::::d   aa::::::::::::a      z::::::z      aa::::::::::::a \n");
    usleep(50000);
	printf(MAG"  L:::::L                a::::aaaa::::::ad:::::d     d:::::d  a::::aaaa::::::a     z::::::z      a::::aaaa::::::a \n");
    usleep(50000);
	printf(CYN"  L:::::L         LLLLLLa::::a    a:::::ad:::::d     d:::::d a::::a    a:::::a    z::::::z      a::::a    a:::::a \n");
    usleep(50000);
	printf(YEL"LL:::::::LLLLLLLLL:::::La::::a    a:::::ad::::::ddddd::::::dda::::a    a:::::a   z::::::zzzzzzzza::::a    a:::::a \n");
    usleep(50000);
	printf(RED"L::::::::::::::::::::::La:::::aaaa::::::a d:::::::::::::::::da:::::aaaa::::::a  z::::::::::::::za:::::aaaa::::::a \n");
    usleep(50000);
	printf(GRN"L::::::::::::::::::::::L a::::::::::aa:::a d:::::::::ddd::::d a::::::::::aa:::az:::::::::::::::z a::::::::::aa:::a\n");
    usleep(50000);
	printf(BLU"LLLLLLLLLLLLLLLLLLLLLLLL  aaaaaaaaaa  aaaa  ddddddddd   ddddd  aaaaaaaaaa  aaaazzzzzzzzzzzzzzzzz  aaaaaaaaaa  aaaa\n");
	usleep(50000);
	printf(CRESET"\n==================================================================================================================");
	usleep(50000);
	printf("\n==================================================================================================================\n\n");
}

void exitMsg(){
	const char *text = CYN"Thank you for using our program\n";
	while (*text != '\0') {
        putchar(*text);
        fflush(stdout); 
        usleep(25000);
        text++;
    }
    const char *text2 = URED"Merry Christmas and Happy New Year!\n"CRESET;
    while (*text2 != '\0') {
        putchar(*text2);
        fflush(stdout);
        usleep(25000);
        text2++;
    }
	pressEnter();
}

void pressEnter(){
	const char *text = GRN"Press Enter to Continue . . ."CRESET;
	while (*text != '\0') {
        putchar(*text);
        fflush(stdout); 
        usleep(25000);
        text++;
    }
	getchar();
}
