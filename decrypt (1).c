#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#define MAX 10240
#define MAX_WORD_COUNT 60000                //we have less than 60000 words
#define MAX_WORD_LENGTH 80                //each word is less than 80 letters

//Using these two global variables can be justified :)
char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];        //2-d array to hold all the words
int word_count = 0;                    //number of words, initilized to 0

//Note the words in the dictionary file are sorted
//This fact could be useful
void read_file_to_array(char *filename)
{
    FILE *fp;

    //open the file for reading
    fp = fopen(filename, "r");
    if(fp==NULL)
    {
        printf("Cannot open file %s.\n", filename);
        exit(-1);
    }
    //make sure when each word is saved in the array words,
    //it does not ends with a '\n'
    //see how this is done using fscanf 
    while(!feof(fp))
        fscanf(fp, "%s", words[word_count++]);
    fclose(fp);
}

//TODO
//Test wether a string word is in the dictionary
//Return 1 if word is in the dictionary
//Return 0 otherwise
//Be efficient in implementing this function
//Efficiency is needed to pass test cases in limited time
int in_dict(char *word)
{
    int left = 0;
    int right = word_count - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(word, words[mid]);
        
        if (cmp == 0) {
            return 1; // Word found in dictionary
        } else if (cmp < 0) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return 0; // Word not found in dictionary

}

//TODO
//Use key and shift to decrypt the encrypted message
//len is the length of the encrypted message
//Note the encrypted message is stored as an array of integers (not chars)
//The result is in decrypted

void decryption(unsigned char key, unsigned char shift, const int *encrypted, int len, char *decrypted) //len is teh length of encrpty message 
{
    for(int i = 0; i < len; i++)
    {
        decrypted[i] = (char)((encrypted[i] ^ key) >> shift); //decrypt the message and stores in decrypted array 
    }

    decrypted[len] = '\0'; //decrypted message ended 
}

//TODO
//calculate a score for a message msg
//the score is used to determine whether msg is the original message
int message_score(const char *msg)
{

    int score = 0; 

    char copy[MAX];  // A copy of the decrypted message for tokenization
    strcpy(copy, msg);

    // Use strtok to tokenize the message into words
    char *token = strtok(copy, " "); //split a string into smaller tokens by blankspace 
    while (token != NULL) {
        // Check if each word (token) is in the dictionary
        if (in_dict(token)) {
            score++;  // Increment the score if the word is found in the dictionary
        }
        token = strtok(NULL, " "); //second call is used to iterate through the tokens in the string one by one
    }

    return score;

}

//search using all the (key, shift) combinations
//to find the original message
//result is saved in message
void search(const int *encrypted, int len, char *message)
{
	char decrypted[MAX];

    int max_score = 0;
    strcpy(message, "");
    for(unsigned char k = 0; k < 255; k++)
    {
        for(unsigned char shift = 0; shift <= 24; shift++)
        {
            decryption(k, shift, encrypted, len, decrypted);
			int score = message_score(decrypted);
			if(score > max_score)
			{	
				max_score = score;
				strcpy(message, decrypted);
			}
        }
    }
}

//TODO
//read the encrypted message from the file to encrypted
//return number of bytes read
int read_encrypted(char *filename, int *encrypted)
{
    int fd = open(filename, O_RDONLY); //open specific file and read 

    //check if file successfully opens
    if (fd < 0) 
    {
        printf("Cannot open file %s\n", filename);
        return -1;
    }

    //if file successfully opens, reads the encrypted message from the file into the encrypted array, also tracks number of bytes!!! in "len" 
    int n_bytes = read(fd, encrypted, MAX * sizeof(int)); //n_bytes may be less than MAX * sizeof(int) if the file contains fewer bytes
    
    close(fd); //prevent data leak

    //error check the value of len 
    if (n_bytes < 0)
    {
        printf("Error reading from file %s\n", filename);
        return -1;
    }

    //after error check, return correct length of the encrypted message
    return n_bytes/ sizeof(int); 

}

//Do not change the main() function
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("%s encrypted-message\n", argv[0]);
		return 0;
	}

	read_file_to_array("dict.txt");

	int encrypted[MAX];
	int len = read_encrypted(argv[1], encrypted);

	char message[MAX];
	strcpy(message, "");
	search(encrypted, len, message);

	printf("%s\n", message);

	return 0;
}