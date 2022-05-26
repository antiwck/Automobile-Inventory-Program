#ifndef INVENTORY_UPDATE_H_
#define INVENTORY_UPDATE_H_

struct part_update {
	char part_id[buffer];
	char part[16];
	char supplier_id[4];
	char warehouse[4];
	int quantity;
};

void inventory_update() {
	int warehouse_search(struct part_update*);
	void quantity_verification_update(struct part_update, char*);

	struct part_update part;
	char opt;			// Increment or decrement selection
	char temp[10];		// Temporarily store quantity as string then convert to integer
	bool id_found;


	do {
		system("cls");
		printf("Inventory update sub-menu\n");
		printf("--------------------------------------------\n");
		printf("[-1 to exit to main menu]\n");
		id_found = false;

		while (!id_found) {
			printf("\nEnter part ID   : ");
			gets_s(part.part_id);

			if (strstr(part.part_id, "-1")) {
				break;
			}

			for (int i = 0; i < buffer; i++) {
				part.part_id[i] = toupper(part.part_id[i]);
			}
			id_found = warehouse_search(&part);
		}

		if (strstr(part.part_id, "-1")) {
			break;
		}

		do {
			printf("\nPart quantity update:-");
			printf("\n1. Increase");
			printf("\n2. Decrease");
			printf("\nSelection: ");
			scanf_s("%c", &opt);
			while (getchar() != '\n');
			if (opt != '1' && opt != '2') {
				printf("Invalid option. Please retry.\n");
			}
		} while (opt != '1' && opt != '2');

		do {
			printf("\nPlease enter quantity : ");
			gets_s(temp);
			part.quantity = abs(atoi(temp));
			if (part.quantity == 0) {
				printf("Invalid quantity. Please retry.\n");
			}
			else {
				quantity_verification_update(part, &opt);
			}
		} while (part.quantity == 0);
		_getch();
	} while (1);
}

int warehouse_search(struct part_update* part) {
	char warehouse[4];
	FILE* f_ptr;
	char line[buffer];			// To temporarily store each line of file when reading
	char part_id_in_line[7];	// To store the part id in file of each line

	for (int i = 0; i < 2; i++) {
		warehouse[i] = part->part_id[i];
	}
	warehouse[2] = '\0';

	if (!strcmp(warehouse, "CT")) {
		strcpy(part->warehouse, "WCT");
		f_ptr = fopen("WCT.txt", "r");
	}
	else if (!strcmp(warehouse, "CV")) {
		strcpy(part->warehouse, "WCV");
		f_ptr = fopen("WCV.txt", "r");
	}
	else if (!strcmp(warehouse, "AC")) {
		strcpy(part->warehouse, "WAC");
		f_ptr = fopen("WAC.txt", "r");
	}
	else {
		f_ptr = NULL;
	}

	if(f_ptr != NULL){
		while (!feof(f_ptr)) {
			fgets(line, buffer, f_ptr);
			for (int i = 0; i < 6; i++) {
				part_id_in_line[i] = line[i];
			}
			part_id_in_line[6] = '\0';
			if (!strcmp(part_id_in_line, part->part_id)) {
				fclose(f_ptr);
				return 1;
			}
		}
	}

	printf("Part id entered is not valid. Please try again.\n");
	return 0;
}

void quantity_verification_update(struct part_update part, char* opt) {
	char line[buffer];
	int available_quantity;				// To store the old quantity from file
	int line_ctr = 0, new_line_ctr = 0;	// To store the number line which part id's quantity is to be changed and for temporary file line ctr
	int new_quantity;					// To store combined value of old quantity and new quantity
	char new_line[buffer];				// To store changed line
	char warehouse_file[8];
	FILE* f_ptr;

	if (!strcmp(part.warehouse, "WCT")) {	// Negate function because 0 if condition satisfied
		strcpy(warehouse_file, "WCT.txt");
	}
	else if (!strcmp(part.warehouse, "WCV")) {
		strcpy(warehouse_file, "WCV.txt");
	}
	else {
		strcpy(warehouse_file, "WAC.txt");
	}

	f_ptr = fopen(warehouse_file, "r");

	fseek(f_ptr, 0, SEEK_SET);
	while (!feof(f_ptr)) {
		line_ctr++;
		fgets(line, buffer, f_ptr);
		if (strstr(line, part.part_id)) {
			sscanf(line, "%[^|]|%[^|]|%[^|]|%d", part.part_id, part.part, part.supplier_id, &available_quantity);
			break;
		}
	}

	if (*opt == '2' && available_quantity < part.quantity) {
		printf("\nCurrent stock does not have amount required!");
		printf("\nCurrent quantity in stock is %d", available_quantity);
		printf("\nPlease retry");
		fclose(f_ptr);
	}
	else {
		if (*opt == '1') {
			// Add old with new quantity
			new_quantity = available_quantity + part.quantity;
			printf("\nScuccesfully added %d units to part %s\nThe current part quantity is %d", part.quantity, part.part_id, new_quantity);
		}
		else {
			// Deduct old with new quantity
			new_quantity = available_quantity - part.quantity;
			printf("\nScuccesfully deducted %d units from part %s\nThe current part quantity is %d", part.quantity, part.part_id, new_quantity);
		}

		if (new_quantity <= 10) {
			printf("\n\nWARNING! Part %s is below 10 units. Please restock soon.", part.part_id);
		}

		// Formatting line to replace quantity in file
		sprintf(new_line, "%s|%s|%s|%d\n", part.part_id, part.part, part.supplier_id, new_quantity);

		// Change date into new file from old file
		FILE* new_fptr = fopen("Temporary.txt", "w");	// Open a new temporarify file
		fseek(f_ptr, 0, SEEK_SET);
		while ((fgets(line, 100, f_ptr)) != NULL) {
			new_line_ctr++;
			if (new_line_ctr == line_ctr) {             // If found the line to replace
				fputs(new_line, new_fptr);              // Input new string into file
			}
			else {
				fputs(line, new_fptr);                   // If not then input old string into file
			}
		}

		fclose(new_fptr);
		fclose(f_ptr);

		remove(warehouse_file);						// Removing old file
		rename("Temporary.txt", warehouse_file);	// Renaming temporary file to replace old file
	}
}

#endif