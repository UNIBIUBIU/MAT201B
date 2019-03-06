#include <iostream>
#include <string>
using namespace std;
int main() {
  int characterCount = 0;
  int wordCount = 0;
  int lineCount = 0;

  while (true) {
    printf("Type a sentence (then hit return): ");
    string line;
    getline(cin, line);

    if (!cin.good()) {
      printf("Done\n");
      return 0; // a success state
    }

    int length = 0;
    while (true) {
      if (line[length] == '\0')
        break;
      length = 1 + length;
    }
    printf("the length is %d characters\n", length);

    characterCount += length; // characterCount = characterCount + length

    int words = 0;
    char last = ' ';
    int c = -1;
    for (int i = 0; i < length; i++) {
      if (last == ' ' && line[i] != ' ') {
        words++;
        if (i != 0) {
          for (int j = i - 2; j > c; j--) {
            cout << line[j];
          }
          cout << " ";
          c = i - 1;
        }
      }

      last = line[i];
    }
    for (int j = length; j > c; j--) {
      cout << line[j];
    }

    printf("\nthe number of words is %d\n", words);
    wordCount += words;
    printf("\t%d\t%d\t%d\n", lineCount, wordCount, characterCount);
  }
}
