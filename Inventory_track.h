#ifndef INVENTORY_TRACK_H_
#define INVENTORY_TRACK_H_

struct part_data {
    char id[7];
    char part[16];
    char supplier[7];
	int quantity;
};

void inventory_track() {
	int sort_all_parts(char*, struct part_data*);
	void show_below_10(char*, struct part_data*);
    void display(int, struct part_data*);

	struct part_data part[buffer];      // Assuming max parts in a warehouse is 100
    char warehouse_file[8];
	char warehouse_selection[buffer];
    int opt;
    int ctr;

	do {
		system("cls");
		printf("Inventory track sub-menu\n");
		printf("--------------------------------------------\n");
		printf("[-1 to exit to main menu]\n");

		do{
			printf("\nWarehouse selection:-");
			printf("\n1. WCT [Warehouse CITY]");
			printf("\n2. WCV [Warehouse CIVIC]");
			printf("\n3. WAC [Warehouse ACCORD]");
			printf("\nSelection: ");
			gets_s(warehouse_selection);

			
            if (!strcmp(warehouse_selection, "1")) {
                strcpy(warehouse_file, "WCT.txt");
                break;
            } else if (!strcmp(warehouse_selection, "2")) {
                strcpy(warehouse_file, "WCV.txt");
                break;
            } else if (!strcmp(warehouse_selection, "3")) {
                strcpy(warehouse_file, "WAC.txt");
                break;
            } else if (!strcmp(warehouse_selection, "-1")){
                break;
            } else {
                printf("\nInvalid warehouse slection. Please retry.\n");
            }

		} while (1);

		if (strstr(warehouse_selection, "-1")) {
			break;
		}

        do{
            printf("\nPart display:-");
            printf("\n1. Show all");
            printf("\n2. Show parts with less than 10 units");
            printf("\nSelection: ");
            scanf_s("%d", &opt);
            while (getchar() != '\n');
            switch (opt) {
                case 1:
                    ctr = sort_all_parts(warehouse_file, part);
                    display(ctr, part);
                    break;
                case 2:
                    show_below_10(warehouse_file, part);
                    break;
                default:
                    printf("\nInvalid option. Please retry");
                    _getch();
                    break;
            }
        } while (opt != 1 && opt != 2);
	} while (1);
}

int sort_all_parts(char *warehouse, struct part_data* part) {
	int i = 0, j, ctr = 0;                          // When using ctr, ctr - 1, because last line is empty
    char line[buffer][buffer], temp_line[buffer];

    FILE* f_ptr = fopen(warehouse, "r");
    if (f_ptr != NULL)
    {
        while (!feof(f_ptr)) {
            fgets(line[i], buffer, f_ptr);
            i++;
            ctr++;
        }

        // Bubble sort algorithm
        for (i = 0; i < ctr - 1; i++) {
            for (j = 0; j < ctr - 2 - i; j++) {             // j < ctr - 2 - i, to stop string from comparing if any s1 is shorter
                if (strcmp(line[j], line[j + 1]) > 0) {     // strcmp compares the strings lexicographically and return different between them, 0 if ==, + if s1 > s2, - if s1 < s2
                    //swap line[j] and line[j+1]
                    strcpy(temp_line, line[j]);
                    strcpy(line[j], line[j + 1]);
                    strcpy(line[j + 1], temp_line);
                }
            }
        }
        fclose(f_ptr);

        for (i = 0; i < ctr - 1; i++)
        {
            sscanf(line[i], "%[^|]|%[^|]|%[^|]|%d", part[i].id, part[i].part, part[i].supplier, &part[i].quantity);
        }

        fclose(f_ptr);
    }
    
    return ctr;
};

void show_below_10(char* warehouse, struct part_data* part) {
    bool found_below_10 = false;
    int ctr = sort_all_parts(warehouse, part);
    int count = 0;

    for (int i = 0; i < ctr - 1; i++) {
        if (part[i].quantity <= 10) {
            printf("\n------------------------------------");
            printf("\nPart #%d\nPart id     : %s\nPart        : %s\nSupplier id : %s\nQuantity    : %d", i + 1, part[i].id, part[i].part, part[i].supplier, part[i].quantity);
            count++;
            found_below_10 = true;
        }
    }
    if (!found_below_10) {
        printf("\nThere are no parts below 10 units in this warehouse.");
    }
    else {
        printf("\n\nThere are %d parts that are below 10 units.", count);
    }
    _getch();
};

void display(int ctr, struct part_data* part) {
    if (ctr == 0) {
        printf("\nNo parts available in inventory.");
    }
    else {
        for (int i = 0; i < ctr - 1; i++)
        {
            printf("\n------------------------------------");
            printf("\nPart #%d\nPart id     : %s\nPart        : %s\nSupplier id : %s\nQuantity    : %d", i + 1, part[i].id, part[i].part, part[i].supplier, part[i].quantity);
        }
    }
    _getch();
};

#endif