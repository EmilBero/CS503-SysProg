#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
void reverse_string(char *buff, int str_len);
int print_words(char *buff, int str_len);

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *src = user_str;
    char *dest = buff;
    int count = 0;
    int space_flag = 0;

    // Skip whitespaces in front
    while(*src == ' ' || *src == '\t'){
        src++;
    }

    // Process the string
    while (*src != '\0'){
	if (*src == ' ' || *src == '\t'){
	    // Handle consecutive spaces
	    if (!space_flag) {
		*dest++ = ' ';
		count++;
		space_flag = 1;
	    }
	} else {
	    // Check if string size > buffer size
	    if (count >= len) {
		return -1;
	    }
	    *dest++ = *src;
	    count++;
	    space_flag = 0;
	}
	src++;
    }

    // Remove spaces at the end
    if (*(dest - 1) == ' '){
	dest--;
	count--;
    }

    // Fill the rest of buffer with .
    while (count < len){
	*dest++ = '.';
	count++;
    }

    // Rturn the length of the final string
    return count;
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]\n");
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len) {
    if (str_len > len) {
        printf("Error: String length exceeds buffer length.\n");
        return -1; // Return an error code if str_len exceeds the buffer size
    }

    int word_count = 0;
    int in_word = 0;

    for (int i = 0; i < str_len; i++) {
        if (*(buff + i) != ' ' && *(buff + i) != '.') {
            if (!in_word) {
                in_word = 1;
                word_count++;
            }
        } else {
            in_word = 0;
        }
    }

    return word_count; // Return the total word count
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
// Reverse String function
void reverse_string(char *buff, int str_len) {
    int start = 0;
    int end = str_len - 1; // Only reverse up to the actual string length
    while (start < end) {
        char temp = *(buff + start);
        *(buff + start) = *(buff + end);
        *(buff + end) = temp;
        start++;
        end--;
    }

    //TEST: Ensure dots are at the end of the buffer
    int j = 0;
    for (int i = 0; i < BUFFER_SZ; i++) {
        if (*(buff + i) != '.') {
            *(buff + j++) = *(buff + i);
        }
    }
    while (j < BUFFER_SZ) {
        *(buff + j++) = '.';
    }

}

// Print Words along with length
int print_words(char *buff, int str_len) {
    int word_count = 0, char_count = 0, word_number = 1;

    printf("Word Print\n----------\n");
    for (int i = 0; i < str_len; i++) {
        if (*(buff + i) != ' ' && *(buff + i) != '.') {
            if (char_count == 0) {
                // Print the word number when starting a new word
                printf("%d. ", word_number++);
            }
            putchar(*(buff + i));
            char_count++;
        } else if (char_count > 0) {
            // End of a word
            printf("(%d)\n", char_count);
            word_count++;
            char_count = 0;
        }
    }

    // Handle the last word if the string doesn't end with a space
    if (char_count > 0) {
        printf("(%d)\n", char_count);
        word_count++;
    }

    printf("\nNumber of words returned: %d\n", word_count);
    return word_count;
}





int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    /*
	This is safe because 'argc < 2' checks to see if argv[1] exists avoiding any undefined
	behavior.Then if argv[1] doesn't exist, we display the usage message and end the
	program with exit method.
    */
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    /*
	If the user doesn't provide at least 3 arguments which the program is expecting
	the program exits, because the user would not have provided either the string, option
	flag or the program name.
    */
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3

    // Allocate space
	buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    // Check if malloc failed
	if (buff == NULL){
		printf("Error: Memory allocation failed.");
		exit(99);
	}


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        free(buff);
	exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
		free(buff);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;



        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
	// Reverse String
	case 'r':
    	    reverse_string(buff, user_str_len);
	    break;


	// Print Words and lengths
	case 'w':
	    rc = print_words(buff, user_str_len);
	    if (rc < 0) {
		printf("Error printing words, rc = %d\n", rc);
		free(buff);
        	exit(2);
	    }
    	    break;


	default:
            usage(argv[0]);
	    free(buff);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
/*          Passing both the pointer and the length helps ensure safety by preventying any
	    Any given function form reading passed the buffer size, making the function 
	    easier to work with buffers of different sizes, and also improves clarity so 
	    we know for certain how many bytes to process.
*/
