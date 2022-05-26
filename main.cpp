#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	
#include <conio.h>		// For console in and out
#include <string.h>		// For string functions
#include <Windows.h>	// For sleep
#include "inventory_creation.h"
#include "inventory_update.h"
#include "inventory_track.h"
#include "inventory_search.h"


void main_menu(int *);


int main() {
	int opt;

	do {
		system("cls");
		main_menu(&opt);
		switch (opt) {
		case 1:
			inventory_creation();
			break;
		case 2:
			inventory_update();
			break;
		case 3:
			inventory_track();
			break;
		case 4:
			inventory_search();
			break;
		case -1:
			system("cls");
			printf("---program ended---\n\n\n");
			break;
		default:
			printf("\ninvalid input. Please retry.");
			Sleep(1500);
			system("cls");
			break;
		}
	} while (opt != -1);
	return 0;
}

void main_menu(int *opt) {
	printf("Automobile Parts Inventory Management System\n");
	printf("--------------------------------------------\n");
	printf("1. Part inventory creation\n");
	printf("2. Part inventory update\n");
	printf("3. Inventory track\n");
	printf("4. Part info\n");
	printf("[-1 to exit]\n");
	printf("--------------------------------------------\n");
	printf("Selection: ");
	scanf_s("%d", opt);
	while (getchar() != '\n');
}