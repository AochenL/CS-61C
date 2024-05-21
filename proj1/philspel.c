/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philspel.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(2255, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");
  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

/*
 * This should hash a string to a bucket index.  Void *s can be safely cast
 * to a char * (null terminated string) and is already done for you here 
 * for convenience.
 */
unsigned int stringHash(void *s) {
  char *string = (char *)s;
  int c;
  unsigned long hash = 5381;
  while ((c = *string++)) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash % 2255;
}


/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  char *string1 = (char *)s1;
  char *string2 = (char *)s2;
  // -- TODO --
  return strcmp(string1, string2) == 0;
}

/*
 * This function should read in every word from the dictionary and
 * store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(1)
 * to cleanly exit the program.
 *
 * Since the format is one word at a time, with new lines in between,
 * you can safely use fscanf() to read in the strings until you want to handle
 * arbitrarily long dictionary chacaters.
 */
void readDictionary(char *dictName) {
  // -- TODO --
  FILE *file = fopen(dictName, "r");

  if (dictName == NULL) {
    fprintf(stderr, "Error: Dictionary file %s not found\n", dictName);
    exit(1);
  }

  char word[61];
  while (fscanf(file, "%60s", word) == 1) {
    char *wordCopy = malloc(60 * sizeof(char));
    if (!wordCopy){
      fprintf(stderr, "Error: Not enough space for word copy");
      exit(1);
    }
    strcpy(wordCopy, word);
    insertData(dictionary, wordCopy, wordCopy);
  }

  fclose(file);
}


//a helper function that processes individual word
void processWord(char *word, int index){
      word[index] = '\0';
      /* 
       * All words should be checked 
       * against the dictionary as they are input, then with all but the first
       * letter converted to lowercase, and finally with all letters converted
       * to lowercase.
       */
      char *original = strdup(word);
      char *fLower = strdup(word);
      char *lower = strdup(word);
      if (!original || !fLower || !lower) {
         fprintf(stderr, "Error: Not enough space for word copy");
         exit(1);
      }
      for (int i = 1; i < strlen(fLower); i++) {
         fLower[i] = tolower(fLower[i]);
      }
      for (int i = 0; i < strlen(lower); i++) {
         lower[i] = tolower(lower[i]);
      }
      
      if (findData(dictionary, original) || findData(dictionary,fLower) || findData(dictionary,lower)) {
        //word exist in dic
        printf("%s", original);
      } else {
        printf("%s [sic]", original);
      }
      free(original);
      free(fLower);
      free(lower);
      //putchar(c);

}

/*
 * initialize a string and set all pointers to 0
 */
void initializeString(char *word, int size){
  for (int i = 0; i < size; i++) {
        word[i] = '\0';
  }
}


/*
 * This should process standard input (stdin) and copy it to standard
 * output (stdout) as specified in the spec (e.g., if a standard 
 * dictionary was used and the string "this is a taest of  this-proGram" 
 * was given to stdin, the output to stdout should be 
 * "this is a teast [sic] of  this-proGram").  All words should be checked
 * against the dictionary as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not found by appending " [sic]" after the error.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final 20% of your grade, you cannot assume words have a bounded length.
 */
void processInput() {
  // -- TODO --
  char word[61];
  int c;
  int index = 0;
  initializeString(word, 61);

  //read char one by one
  while ((c = getchar())!= EOF) {
    // continue to read word if char is alphabetic
    if (isalpha(c)){
      if (index < sizeof(word) - 1){
        word[index++] = c;
      } 
    } else {
      //process individual word
      if (index > 0){
        //printf("%s\n", word);
        processWord(word, index);
        initializeString(word, 61);
        index = 0;
      }
      putchar(c);
      
    } 
  }
  if (index > 0){
    printf("%s\n", word);
    processWord(word, index);
    initializeString(word, 61);
    index = 0;
  }

}
