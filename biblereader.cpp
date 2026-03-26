
/* biblereader.cpp
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * main function for Project 1
 *
 * NOTE: You may add code to this file, but do not
 * delete any code or delete any comments.
 *
 * STUDENT NAME: Coen Hall
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>

using namespace std;

static const string names[66] =
{
   "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy", "Joshua", "Judges", "Ruth",
   "1 Samuel", "2 Samuel", "1 Kings", "2 Kings", "1 Chronicles", "2 Chronicles", "Ezra", "Nehemiah",
   "Esther", "Job", "Psalms", "Proverbs", "Ecclesiastes", "Song of Solomon", "Isaiah",
   "Jeremiah", "Lamentations", "Ezekial", "Daniel", "Hosea", "Joel", "Amos", "Obadiah",
   "Jonah", "Micah", "Nahum", "Habakkuk", "Zephaniah", "Haggai", "Zechariah", "Malachi",
   "Matthew", "Mark", "Luke", "John", "Acts", "Romans", "1 Corinthians", "2 Corinthians",
   "Galatians", "Ephesians", "Phillipians", "Collosians", "1 Thessalonians", "2 Thessalonians",
   "1 Timothy", "2 Timothy", "Titus", "Philemon", "Hebrews", "James", "1 Peter", "2 Peter",
   "1 John", "2 John", "3 John", "Jude", "Revelation"
};

int main (int argc, char **argv)
{
   // Create Bible object to process the raw text file
   Bible webBible("/home/class/csc3004/Bibles/web-complete");
	
   Verse verse;
   int bookNum, chapterNum, verseNum;
   int numVerses = 1;
   LookupResult result;
	
   cout << "Using Bible from: ";
   webBible.display();
	
   // Prompt for input: get reference to lookup
   // This is good for testing your classes quickly for the first milestone.
   // TODO: your final program should get input from command line arguments instead.

   if(argc == 3)
   {
      cout << "Error: verse number is missing." << endl;
      return 0;
   }

   if(argc == 4 || argc == 5)
   {
      bookNum = atoi(argv[1]);
      chapterNum = atoi(argv[2]);
      verseNum = atoi(argv[3]);
      if(argc == 5)
      {
         numVerses = atoi(argv[4]);
      }
   }
   else
   {

      cout << "Enter 3 integers for book, chapter, and verse to find: " << flush;
      cin >> bookNum >> chapterNum >> verseNum;

      cout << "Enter the number of verses to display (or 1): ";
      cin >> numVerses;
   }


   // Create a reference from the numbers
   Ref ref(bookNum, chapterNum, verseNum);

   // Use the Bible object to retrieve the verse by reference
   cout << "Looking up reference: ";
   ref.display();
   cout << endl;

   verse = webBible.lookup(ref, result);
   if(result != SUCCESS)
   {
      if(result == NO_BOOK)
      {
         cout << "Error: no such book." << endl;
         return 0;
      }
      else if(result == NO_CHAPTER)
      {
         cout << "Error: no such chapter " << chapterNum << " in " << names[bookNum - 1] << endl;
         return 0;
      }
      else if(result == NO_VERSE)
      {
         cout << "Error: no such verse " << verseNum << " in " << names[bookNum - 1] << " " <<  chapterNum << endl;
         return 0;
      }
      else
      {
        cout << "Error: lookup failed." << endl;
        return 0;
      }
   }

   cout << "Result status: " << result << endl;
   cout << endl;

   cout << verse.getRef().getVerse() << " "  << verse.getVerse() << endl;

   for(int i = 1; i < numVerses; i++)
   {
      Verse nextVerse = webBible.nextVerse(result);

      if(result != SUCCESS)
      {
         break;
      }

      if(nextVerse.getRef().getBook() != ref.getBook())
      {
         break;
      }

      cout << nextVerse.getRef().getVerse() << " " << nextVerse.getVerse() << endl;
   }
}
