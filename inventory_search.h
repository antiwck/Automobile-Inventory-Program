#ifndef INVENTORY_SEARCH_H_
#define INVENTORY_SERACH_H_

struct part_all {
	char part_id[buffer];
	char part[16];
	int quantity;
	char supplier_id[buffer];
	char supplier_name[buffer];
	char supplier_contact[12];
	char supplier_handled_parts[buffer];
	char supplier_general_part_id[buffer];
	char warehouse[4];
	char warehouse_file[7];
};


void inventory_search() {
	void inventory_search();
	int warehouse_search(struct part_all*);
	void show_part(struct part_all*);

	struct part_all part;
	bool id_found;

	do {
		system("cls");
		printf("Inventory search sub-menu\n");
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

		if (id_found) {
			show_part(&part);
			_getch();
		}
	} while (1);
}

int warehouse_search(struct part_all* part) {
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
		strcpy(part->warehouse_file, "WCT.txt");
		f_ptr = fopen("WCT.txt", "r");
		printf("Opening WCT file");
	}
	else if (!strcmp(warehouse, "CV")) {
		strcpy(part->warehouse, "WCV");
		strcpy(part->warehouse_file, "WCV.txt");
		f_ptr = fopen("WCV.txt", "r");
		printf("Opening WCV file");
	}
	else if (!strcmp(warehouse, "AC")) {
		strcpy(part->warehouse, "WAC");
		strcpy(part->warehouse_file, "WAC.txt");
		f_ptr = fopen("WAC.txt", "r");
		printf("Opening WAC file");
	}
	else {
		f_ptr = NULL;
	}

	if (f_ptr != NULL) {
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

void show_part(struct part_all* part) {
	char line[2000];
	FILE* part_fptr = fopen(part->warehouse_file, "r");

	while (!feof(part_fptr)) {
		fgets(line, buffer, part_fptr);
		if (strstr(line, part->part_id)) {
			sscanf(line, "%[^|]|%[^|]|%[^|]|%d", part->part_id, part->part, part->supplier_id, &part->quantity);
			break;
		}
	}
	fclose(part_fptr);
	
	FILE* supplier_fptr = fopen("supplier.txt", "r");
	
	while (!feof(supplier_fptr)) {
		fgets(line, 2000, supplier_fptr);
		if (strstr(line, part->supplier_id)) {
			sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]", part->supplier_id, part->supplier_name, part->supplier_contact, part->supplier_handled_parts, part->warehouse, part->supplier_general_part_id);
			break;
		}
	}
	fclose(supplier_fptr);

	printf("\nPart details:-");
	printf("\n\nPart info:-");
	printf("\nPart id                  : %s", part->part_id);
	printf("\nPart type                : %s", part->part);
	printf("\nQuantity                 : %d", part->quantity);
	printf("\nWarehouse                : %s", part->warehouse);
	printf("\n\nSupplier info:-");
	printf("\nSupplier id              : %s", part->supplier_id);
	printf("\nSupplier name            : %s", part->supplier_name);
	printf("\nSupplier contact         : %s", part->supplier_contact);
	printf("\nSupplier parts           : %s", part->supplier_handled_parts);
	printf("\nSupplier general part id : %s", part->supplier_general_part_id);
};

#endif