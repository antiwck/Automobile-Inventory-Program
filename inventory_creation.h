#ifndef INVENTORY_CREATION_H_
#define INVENTORY_CREATION_H_
#define buffer 100				// buffer fixed to used in all other header file


struct part_details {
	char supplier_id[buffer];
	char part[16];
	char part_id[7];
	int quantity;
	char warehouse[4];
};

void inventory_creation() {
	int id_search(struct part_details);
	void assign_part_id(struct part_details*);
	void store_part_details(struct part_details);
	void store_supplier_details(struct part_details);

	struct part_details part;
	bool found = false, end_func = false;
	char opt;		// Variable for option questions
	char temp[10];	// Temporarily store quantity as string then convert to int

	do {
		system("cls");
		printf("Inventory creation sub-menu\n");
		printf("--------------------------------------------\n");
		printf("[-1 to exit to main menu]\n");
		printf("\nEnter supplier ID   : ");
		gets_s(part.supplier_id);

		if (strstr(part.supplier_id, "-1")) {
			break;
		}

		for (int i = 0; i < buffer; i++) {
			part.supplier_id[i] = toupper(part.supplier_id[i]);
		}

		found = id_search(part);

		if (found) {
			// Warehouse are predetermined by supplier id
			if (!strcmp(part.supplier_id, "WCK1") || !strcmp(part.supplier_id, "WCK2")) {
				strcpy(part.warehouse, "WCT");
			}
			else if (!strcmp(part.supplier_id, "WCK3") || !strcmp(part.supplier_id, "WCK4")) {
				strcpy(part.warehouse, "WCV");
			}
			else {
				strcpy(part.warehouse, "WAC");
			}

			// WCK1, WCK3 and WCK5 handles engine and aricond only, WCK2, WCK4 and WCK6 handles brake system and ignition system only
			if (!strcmp(part.supplier_id, "WCK1") || !strcmp(part.supplier_id, "WCK3") || !strcmp(part.supplier_id, "WCK5")) {
				do {
					printf("\nPlease select part type: ");
					printf("\n1. Engine");
					printf("\n2. Air-cond");
					printf("\nPlease select with number: ");
					scanf_s("%c", &opt);
					while (getchar() != '\n');
					switch (opt) {
					case '1':
						strcpy(part.part, "ENGINE");
						break;
					case '2':
						strcpy(part.part, "AIR-COND");
						break;
					default:
						printf("\nInvalid part type. Please retry.\n");
						_getch();
						break;
					}
				} while (opt != '1' && opt != '2');
			}
			else {
				do {
					printf("\nPlease select part type: ");
					printf("\n1. Brake system");
					printf("\n2. Ignition system");
					printf("\nPlease select with number: ");
					scanf_s("%c", &opt);
					while (getchar() != '\n');
					switch (opt) {
					case '1':
						strcpy(part.part, "BRAKE SYSTEM");
						break;
					case '2':
						strcpy(part.part, "IGNITION SYSTEM");
						break;
					default:
						printf("\nInvalid part type. Please retry.\n");
						_getch();
						break;
					}
				} while (opt != '1' && opt != '2');
			}


			do {
				printf("\nPlease enter quantity  : ");
				gets_s(temp);
				part.quantity = atoi(temp);
				if (part.quantity == 0 || part.quantity <= 0) {
					printf("Invalid quantity. Please retry.\n");
				}
			} while (part.quantity == 0 || part.quantity <= 0);


			assign_part_id(&part);

			printf("\nPlease confirm information:-");
			printf("\nSupplier id   : %s", part.supplier_id);
			printf("\nWarehouse     : %s", part.warehouse);
			printf("\nPart type     : %s", part.part);
			printf("\nPart quantity : %d", part.quantity);
			printf("\nPart id       : %s", part.part_id);


			do {
				printf("\n\n[Y to save, X to retry, D to discard and return to main menu]:\n");
				scanf("%c", &opt);
				while (getchar() != '\n');

				if (opt == 'Y' || opt == 'y') {
					store_part_details(part);		// Store part details into respective inventory file
					store_supplier_details(part);	// Store part id into supplier file
					end_func = true;
					_getch();
				}
				else if (opt == 'X' || opt == 'x') {
					break;
				}
				else if (opt == 'D' || opt == 'd') {
					end_func = true;
				}
				else {
					printf("\nInvalid input. Please try again.");
					_getch();
				}
			} while (opt != 'D' && opt != 'd' && opt != 'Y' && opt != 'y' && opt != 'X' && opt != 'x');
		}
		else {
			printf("\nInvalid supplier id.");
			_getch();
		}
	} while (!end_func);
}


int id_search(struct part_details part) {
	if (!strcmp(part.supplier_id, "WCK1") ||
		!strcmp(part.supplier_id, "WCK2") ||
		!strcmp(part.supplier_id, "WCK3") ||
		!strcmp(part.supplier_id, "WCK4") ||
		!strcmp(part.supplier_id, "WCK5") ||
		!strcmp(part.supplier_id, "WCK6")) {
		return 1;
	}
	else {
		return 0;
	}
}

void assign_part_id(struct part_details* part) {	// Structure pointer

	char line[buffer];		// To temporarily store string from file when reading each line
	char id_line[buffer];	// To store the latest line when part type exist within the line in file
	char latest_part_id[7];	// To store part id from file

	char digits[2];			// To store the numerical part of id as string
	int count = 0;			// To increment the numerical of part id
	char part_id[7];		// To store the final part id

	FILE* f_ptr;
	if (!strcmp(part->warehouse, "WCT")) {	// Negate function because 0 if condition satisfied
		f_ptr = fopen("WCT.txt", "r");
	}
	else if (!strcmp(part->warehouse, "WCV")) {
		f_ptr = fopen("WCV.txt", "r");
	}
	else if (!strcmp(part->warehouse, "WAC")) {
		f_ptr = fopen("WAC.txt", "r");
	}
	else {
		f_ptr = NULL;
	}
	
	// If warehouse file existed
	if (f_ptr != NULL) {
		fseek(f_ptr, 0, SEEK_SET);
		while (!feof(f_ptr)) {
			fgets(line, buffer, f_ptr);
			if (strstr(line, part->part)) {
				strcpy(id_line, line);
			}
		}
		fclose(f_ptr);
	}

	// if no previous part id available in file
	if (!strstr(id_line, "CT") && !strstr(id_line, "CV") && !strstr(id_line, "AC")) {
		if (strstr(part->warehouse, "WCT")) {
			if (!strcmp(part->part, "ENGINE")) {
				strcpy(part_id, "CTEG");
			}
			else if (!strcmp(part->part, "AIR-COND")) {
				strcpy(part_id, "CTAC");
			}
			else if (!strcmp(part->part, "BRAKE SYSTEM")) {
				strcpy(part_id, "CTBS");
			}
			else {
				strcpy(part_id, "CTIS");
			}
		}
		else if (strstr(part->warehouse, "WCV")) {
			if (!strcmp(part->part, "ENGINE")) {
				strcpy(part_id, "CVEG");
			}
			else if (!strcmp(part->part, "AIR-COND")) {
				strcpy(part_id, "CVAC");
			}
			else if (!strcmp(part->part, "BRAKE SYSTEM")) {
				strcpy(part_id, "CVBS");
			}
			else {
				strcpy(part_id, "CVIS");
			}
		}
		else {
			if (!strcmp(part->part, "ENGINE")) {
				strcpy(part_id, "ACEG");
			}
			else if (!strcmp(part->part, "AIR-COND")) {
				strcpy(part_id, "ACAC");
			}
			else if (!strcmp(part->part, "BRAKE SYSTEM")) {
				strcpy(part_id, "ACBS");
			}
			else {
				strcpy(part_id, "ACIS");
			}
		}
		part_id[4] = '\0';	// To end the string or the console will print junk value after the four characters
	}
	else {
		// Getting the part id and assign to latest_part_id
		strncpy(latest_part_id, id_line, 6);
		latest_part_id[6] = '\0';

		// Store the leading part id alphabetical portion
		for (int i = 0; i < 4; i++) {
			part_id[i] = latest_part_id[i];
		}
		part_id[4] = '\0';

		// Store the numerical portion of part id
		for (int i = 0; i < 2; i++) {
			digits[i] = latest_part_id[i + 4];
		}

		// Change the numerical part to integer
		count = atoi(digits);
	}

	count++;

	// Format and store the final part id
	if (count < 10) {
		sprintf(part_id, "%s%1s%d", part_id, "0", count);
	}
	else {
		sprintf(part_id, "%s%d", part_id, count);
	}

	strcpy(part->part_id, part_id);
}


void store_part_details(struct part_details part) {
	FILE* f_ptr;
	if (strstr(part.warehouse, "WCT")) {
		f_ptr = fopen("WCT.txt", "a");
	}
	else if (strstr(part.warehouse, "WCV")) {
		f_ptr = fopen("WCV.txt", "a");
	}
	else if (strstr(part.warehouse, "WAC")) {
		f_ptr = fopen("WAC.txt", "a");
	}
	else {
		f_ptr = NULL;
	}

	// If no warehouse file exist
	if (f_ptr == NULL) {
		if (!strcmp(part.warehouse, "WCT")) {
			f_ptr = fopen("WCT.txt", "w");
		}
		else if (!strcmp(part.warehouse, "WCV")) {
			f_ptr = fopen("WCV.txt", "w");
		}
		else {
			f_ptr = fopen("WAC.txt", "w");
		}
	}
	fprintf(f_ptr, "%s|%s|%s|%d\n", part.part_id, part.part, part.supplier_id, part.quantity);
	fclose(f_ptr);
	printf("\nSuccessfully updated file!\n");
}

void store_supplier_details(struct part_details part) {
	char line[2000];		// Suuplier file per line may store 99 parts per part, two parts with 6 letters each, can be 1600 letters
	FILE* f_ptr = fopen("supplier.txt", "r");
	FILE* new_fptr = fopen("Temporary.txt", "w");

	while (!feof(f_ptr)) {
		fgets(line, 2000, f_ptr);
		if (strstr(line, part.supplier_id)) {
			// For first five line in supplier file
			if (line[strlen(line) - 1] == '\n') {
				line[strlen(line) - 1] = '\0';
				if (line[strlen(line) - 1] == '|') {
					fprintf(new_fptr, "%s%s\n", line, part.part_id);
				}
				else {
					fprintf(new_fptr, "%s, %s\n", line, part.part_id);
				}
			}
			// For last line in supplier file
			else {
				if (line[strlen(line) - 1] == '|') {
					fprintf(new_fptr, "%s%s", line, part.part_id);
				}
				else {
					fprintf(new_fptr, "%s, %s", line, part.part_id);
				}
			}
		}
		else {
			fputs(line, new_fptr);
		}
	}

	fclose(f_ptr);
	fclose(new_fptr);

	remove("supplier.txt");
	rename("Temporary.txt", "supplier.txt");
}

#endif