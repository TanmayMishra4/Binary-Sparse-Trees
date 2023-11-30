#include "specific.h"


int get_row_to_insert(int index);
int get_start_index(int row_number);
int get_end_index(int row_number);
int get_row_size(int row_number);
void initialise_row(bsa* b, int row_number);
void insert_in_row(bsa* b, int insertion_row, int indx, int d);
bool insert_to_string(char* str, int* str_ind, BSA_row* row_arr, int row);
void insert_num_to_str(char* str, int* str_ind, int num);
int reverse_number(int num);
char int_to_char(int num);

// Create an empty BSA
bsa* bsa_init(void){
    bsa* b = (bsa*)calloc(BSA_ROWS, sizeof(bsa));
    b->last_filled_index = -1;
    b->row_array = (BSA_row*)calloc(BSA_ROWS, sizeof(BSA_row));

    return b;
}

// Set element at index indx with value d i.e. b[i] = d;
// May require an allocation if it's the first element in that row
bool bsa_set(bsa* b, int indx, int d){
    if(b == NULL || indx < 0){
        return false;
    }
    int insertion_row = get_row_to_insert(indx);
    if(b->row_array[insertion_row].size == 0){
        initialise_row(b, insertion_row);
    }
    insert_in_row(b, insertion_row, indx, d);
    int last_indx = b->last_filled_index;
    b->last_filled_index = (last_indx > indx?last_indx:indx);
    return true;
}

// Return pointer to data at element b[i]
// or NULL if element is unset, or part of a row that hasn't been allocated.
int* bsa_get(bsa* b, int indx){
    if(indx < 0){
        return NULL;
    }
    if(b == NULL){
        return NULL;
    }
    int row = get_row_to_insert(indx);
    // printf("row for indx %i = %i\n", indx, row);
    int offset = get_start_index(row);
    // printf("offset = %i\n", offset);
    // printf("row size = %i\n", b->row_array[row].size);
    // printf("flag is = %i\n", b->row_array[row].set_flag[indx-offset]);

    if(b->row_array[row].size == 0){
        return NULL;
    }
    if(b->row_array[row].set_flag[indx-offset] == false){
        return NULL;
    }
    // printf("val = %i\n", b->row_array[row].arr[indx-offset]);
    int* result = &(b->row_array[row].arr[indx-offset]);
    return result;
}

// Delete element at index indx - forces a shrink
// if that was the only cell in the row occupied.
bool bsa_delete(bsa* b, int indx){
    return false;
}

// Returns maximum index written to so far or
// -1 if no cells have been written to yet
int bsa_maxindex(bsa* b){
    if(b == NULL){
        return -1;
    }
    return b->last_filled_index;
}

// Returns stringified version of structure
// Each row has its elements printed between {}, up to the maximum index.
// Rows after the maximum index are ignored.
bool bsa_tostring(bsa* b, char* str){
    if(b == NULL || str == NULL){
        return false;
    }
    int str_ind = 0;
    int last_row = get_row_to_insert(b->last_filled_index);

    for(int row=0;row<=last_row;row++){
        str[str_ind++] = '{';

        bool did_insert = insert_to_string(str, &str_ind, &(b->row_array[row]), row);

        //if(did_insert == true){
        //    str[str_ind++] = ' ';
        //}
        str[str_ind++] = '}';
    }
    return true;
}

// Clears up all space used
bool bsa_free(bsa* b){
    return false;
}

// Allow a user-defined function to be applied to each (valid) value 
// in the array. The user defined 'func' is passed a pointer to an int,
// and maintains an accumulator of the result where required.
void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc){
    return;
}

int get_row_to_insert(int index){
    for(int i=0;i<BSA_ROWS;i++){
        int start = get_start_index(i);
        int end = get_end_index(i);
        if(index >= start && index <= end){
            return i;
        }
    }
    return -1;
}

void initialise_row(bsa* b, int row_number){
    int size = get_row_size(row_number);
    b->row_array[row_number].arr = (int*)calloc(size, sizeof(int));
    b->row_array[row_number].size = size;
    b->row_array[row_number].set_flag = (bool*)calloc(size, sizeof(bool));
}

int get_start_index(int row_number){
    return ((1 << row_number)-1);
}

int get_end_index(int row_number){
    int end = get_start_index(row_number)*2;
    return end;
}

int get_row_size(int row_number){
    int size = (1 << row_number);
    return size;
}

void insert_in_row(bsa* b, int insertion_row, int indx, int d){
    int offset = get_start_index(insertion_row);
    indx = indx - offset;
    b->row_array[insertion_row].arr[indx] = d;
    b->row_array[insertion_row].set_flag[indx] = true;
}

bool insert_to_string(char* str, int* str_ind, BSA_row* row_arr, int row){
    bool flag = false;
    if(row_arr->size == 0){
        return flag;
    }
    for(int col=0;col<row_arr->size;col++){
        if(row_arr->set_flag[col] == true){
            flag = true;
	    if(col != 0){
                str[*str_ind] = ' ';
                *str_ind = *str_ind + 1;
            }
            str[*str_ind] = '[';
            *str_ind = *str_ind + 1;

            int actual_index = get_start_index(row) + col;
	    insert_num_to_str(str, str_ind, actual_index);

            str[*str_ind] = ']';
            *str_ind = *str_ind + 1;

            str[*str_ind] = '=';
            *str_ind = *str_ind + 1;

            insert_num_to_str(str, str_ind, row_arr->arr[col]);
	    //if(col != row_arr->size-1){
	        printf("col = %i and row size = %i, val = %i\n", col, row_arr->size, row_arr->arr[col]);
	//	str[*str_ind] = ' ';
	  //      *str_ind = *str_ind + 1;
	    //}
        }
    }
    return flag;
}

void insert_num_to_str(char* str, int* str_ind, int num){
    // int divisor = 10;
    // // num = reverse_number(num);
    // while(num > 0){
    //     int digit = num%divisor;
    //     num = num/divisor;
    //     str[*str_ind] = int_to_char(digit);
    //     *str_ind = *str_ind + 1;
    // }
    char converted[CHAR_ARR_SIZE] = {0};
    sprintf(converted, "%i", num);
    int num_chars = strlen(converted);
    strcat(str, converted);
    *str_ind = *str_ind + num_chars;
}

// int reverse_number(int num){
//     int rev = 0;
//     while (num != 0) {
//         int rem = num % 10;
//         rev = rev * 10 + rem;
//         num /= 10;
//     }
// }

char int_to_char(int num){
    return num + '0';
}

// You'll this to test the other functions you write
void test(void){
    return;
}
