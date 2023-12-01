#include "specific.h"

int get_row_to_insert(int index);
int get_start_index(int row_number);
int get_end_index(int row_number);
int get_row_capacity(int row_number);
void initialise_row(bsa* b, int row_number);
void insert_in_row(bsa* b, int insertion_row, int indx, int d);
bool insert_to_string(char* str, int* str_ind, BSA_row* row_arr, int row);
void insert_num_to_str(char* str, int* str_ind, int num);
void clear_string(char* str);
void free_row(bsa* b, int row);
void update_last_index(bsa* b);
// Testing Functions
void test_get_row_to_insert(void);
void test_get_row_capacity(void);
void test_get_start_index(void);
void test_get_end_index(void);
void test_initialise_row(void);
void test_free_row(void);
void test_insert_num_to_str(void);


bsa* bsa_init(void){
    bsa* b = (bsa*)calloc(BSA_ROWS, sizeof(bsa));
    b->last_filled_index = -1;
    b->row_array = (BSA_row*)calloc(BSA_ROWS, sizeof(BSA_row));

    return b;
}

bool bsa_set(bsa* b, int indx, int d){
    if(b == NULL || indx < 0){
        return false;
    }
    int insertion_row = get_row_to_insert(indx);
    if(b->row_array[insertion_row].capacity == 0){
        initialise_row(b, insertion_row);
    }
    int offset = get_start_index(insertion_row);
    if(b->row_array[insertion_row].set_flag[indx-offset] == false){
        b->row_array[insertion_row].size++;
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
    int offset = get_start_index(row);

    if(b->row_array[row].size == 0){
        return NULL;
    }
    if(b->row_array[row].set_flag[indx-offset] == false){
        return NULL;
    }

    int* result = &(b->row_array[row].arr[indx-offset]);
    return result;
}

// Delete element at index indx - forces a shrink
// if that was the only cell in the row occupied.
bool bsa_delete(bsa* b, int indx){
    int row_num = get_row_to_insert(indx);
    int offset = get_start_index(row_num);
    if(b->row_array[row_num].set_flag[indx-offset] == false){
        // printf("inside for row = %i, index = %i\n", row_num, indx-offset);
        return false;
    }
    b->row_array[row_num].set_flag[indx-offset] = false;
    b->row_array[row_num].size--;
    int size = b->row_array[row_num].size;
    update_last_index(b);
    if(size == 0){
        free_row(b, row_num);
    }

    return true;
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
    clear_string(str);
    int str_ind = 0;
    int last_row = get_row_to_insert(b->last_filled_index);

    for(int row=0;row<=last_row;row++){
        str[str_ind++] = '{';

        insert_to_string(str, &str_ind, &(b->row_array[row]), row);

        //if(did_insert == true){
        //    str[str_ind++] = ' ';
        //}
        str[str_ind++] = '}';
    }
    return true;
}

// Clears up all space used
bool bsa_free(bsa* b){
    if(b == NULL){
        return false;
    }
    for(int i=0;i<BSA_ROWS;i++){
        free_row(b, i);
    }
    free(b->row_array);
    free(b);
    b = NULL;
    return true;
}

// Allow a user-defined function to be applied to each (valid) value 
// in the array. The user defined 'func' is passed a pointer to an int,
// and maintains an accumulator of the result where required.
void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc){
    if(b == NULL || acc == NULL){
        return;
    }
    int last_index = b->last_filled_index;
    for(int i=0;i<=last_index;i++){
        int* val = bsa_get(b, i);
        if(val != NULL){
            func(val, acc);
        }
    }
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
    int capacity = get_row_capacity(row_number);
    b->row_array[row_number].arr = (int*)calloc(capacity, sizeof(int));
    b->row_array[row_number].capacity = capacity;
    b->row_array[row_number].size = 0;
    b->row_array[row_number].set_flag = (bool*)calloc(capacity, sizeof(bool));
}

int get_start_index(int row_number){
    if(row_number < 0){
        return -1;
    }
    return ((1 << row_number)-1);
}

int get_end_index(int row_number){
    if(row_number < 0){
        return -1;
    }
    int end = get_start_index(row_number)*2;
    return end;
}

int get_row_capacity(int row_number){
    if(row_number < 0) return 0;
    int size = (1 << row_number);
    return size;
}

void insert_in_row(bsa* b, int insertion_row, int indx, int d){
    int offset = get_start_index(insertion_row);
    indx = indx - offset;
    b->row_array[insertion_row].arr[indx] = d;
    // printf("value true for row = %i, index = %i, offset = %i\n", insertion_row, indx, offset);
    b->row_array[insertion_row].set_flag[indx] = true;
}

bool insert_to_string(char* str, int* str_ind, BSA_row* row_arr, int row){
    bool flag = false;
    if(row_arr->size == 0){
        return flag;
    }

    for(int col=0;col<row_arr->capacity;col++){
        if(row_arr->set_flag[col] == true){
            if(flag != false){
                str[*str_ind] = ' ';
                *str_ind = *str_ind + 1;
            }
            flag = true;
            str[*str_ind] = '[';
            *str_ind = *str_ind + 1;

            int actual_index = get_start_index(row) + col;
            // printf("row = %i, col = %i, actual_col = %i\n", row, col, actual_index);
            insert_num_to_str(str, str_ind, actual_index);

            str[*str_ind] = ']';
            *str_ind = *str_ind + 1;

            str[*str_ind] = '=';
            *str_ind = *str_ind + 1;

            insert_num_to_str(str, str_ind, row_arr->arr[col]);
            //if(col != row_arr->size-1){
            // printf("col = %i and row size = %i, val = %i\n", col, row_arr->size, row_arr->arr[col]);
        //      str[*str_ind] = ' ';
          //      *str_ind = *str_ind + 1;
            //}
        }
    }
    return flag;
}

void insert_num_to_str(char* str, int* str_ind, int num){
    char converted[CHAR_ARR_SIZE] = {0};
    sprintf(converted, "%i", num);
    int num_chars = strlen(converted);
    strcat(str, converted);
    *str_ind = *str_ind + num_chars;
    // printf("%i converted to ", num);
    // puts(converted);
    // printf("result string is ");
    // puts(str);
}

void clear_string(char* str){
    int i = 0;
    while(str[i] != '\0'){
        str[i] = '\0';
        i++;
    }
}

void free_row(bsa* b, int row){
    if(b == NULL || b->row_array == NULL) return;
    if(b->row_array[row].arr != NULL){
        free(b->row_array[row].arr);
        b->row_array[row].arr = NULL;
    }
    if(b->row_array[row].set_flag != NULL){
        free(b->row_array[row].set_flag);
        b->row_array[row].set_flag = NULL;
    }
    b->row_array[row].size = 0;
    b->row_array[row].capacity = 0;
}

void update_last_index(bsa* b){
    int index = b->last_filled_index;
    while(index >= 0){
        if(bsa_get(b, index) != NULL){
            b->last_filled_index = index;
            return;
        }
        index--;
    }
    b->last_filled_index = -1;
}

// You'll this to test the other functions you write
void test(void){
    test_get_row_capacity();
    test_get_end_index();
    test_get_start_index();
    test_get_row_to_insert();
    test_initialise_row();
    test_free_row();
    test_insert_num_to_str();
}
                      
void test_get_row_to_insert(void){
    assert(get_row_to_insert(-70) == -1);
    assert(get_row_to_insert(-1) == -1);
    assert(get_row_to_insert(0) == 0);
    assert(get_row_to_insert(1) == 1);
    assert(get_row_to_insert(100) == 6);
    assert(get_row_to_insert((1 << 29)-1) == 29);
    assert(get_row_to_insert(15) == 4);
}
void test_get_row_capacity(void){
    assert(get_row_capacity(-1) == 0);
    assert(get_row_capacity(-599) == 0);
    assert(get_row_capacity(0) == 1);
    assert(get_row_capacity(1) == 2);
    assert(get_row_capacity(12) == (1 << 12));
    assert(get_row_capacity(29) == (1 << 29));
}
void test_get_start_index(void){
    assert(get_start_index(0) == 0);
    assert(get_start_index(1) == 1);
    assert(get_start_index(29) == (1 << 29) -1);
    assert(get_start_index(4) == 15);
    assert(get_start_index(18) == (1 << 18)-1);
    assert(get_start_index(-2) == -1);
    assert(get_start_index(-70) == -1);
}
void test_get_end_index(void){
    assert(get_end_index(0) == 0);
    assert(get_end_index(1) == 2);
    assert(get_end_index(29) == 2*((1 << 29) -1));
    assert(get_end_index(4) == 30);
    assert(get_end_index(18) == 2*((1 << 18)-1));
    assert(get_end_index(-2) == -1);
    assert(get_end_index(-70) == -1);
}

void test_initialise_row(void){
    bsa* b = bsa_init();
    initialise_row(b, 2);
    assert(b->row_array[2].size == 0);
    assert(b->row_array[2].capacity == 4);
    assert(b->row_array[2].arr != NULL);
    assert(b->row_array[2].set_flag != NULL);
    assert(b->row_array[2].set_flag[0] == false);
    assert(b->row_array[2].set_flag[1] == false);
    assert(b->row_array[2].set_flag[3] == false);

    initialise_row(b, 6);
    assert(b->row_array[6].size == 0);
    assert(b->row_array[6].capacity == 64);
    assert(b->row_array[6].arr != NULL);
    assert(b->row_array[6].set_flag != NULL);
    assert(b->row_array[6].set_flag[0] == false);
    assert(b->row_array[6].set_flag[5] == false);
    assert(b->row_array[6].set_flag[63] == false);

    bsa_free(b);
}

void test_free_row(void){
    bsa* b = bsa_init();
    initialise_row(b, 2);
    initialise_row(b, 4);
    free_row(b, 3);
    free_row(b, 2);
    assert(b->row_array[2].arr == NULL);
    assert(b->row_array[2].set_flag == NULL);
    assert(b->row_array[2].size == 0);
    assert(b->row_array[2].capacity == 0);
    free_row(b, 4);
    assert(b->row_array[4].arr == NULL);
    assert(b->row_array[4].set_flag == NULL);
    assert(b->row_array[4].size == 0);
    assert(b->row_array[4].capacity == 0);
    bsa_free(b);
}

void test_insert_num_to_str(void){
    char str[20];
    insert_num_to_str(str, 0, 124);
    assert(strcmp(str, "124") == 0);
    clear_string(str);
    insert_num_to_str(str, 0, 7456);
    assert(strcmp(str, "7456") == 0);
    clear_string(str);
    insert_num_to_str(str, 0, 12837);
    assert(strcmp(str, "12837") == 0);
    clear_string(str);
    insert_num_to_str(str, 0, -9459);
    assert(strcmp(str, "-9459") == 0);
}

