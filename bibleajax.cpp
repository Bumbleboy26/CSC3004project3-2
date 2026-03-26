/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * updated by Deborah Wilson, February 2026
 * Mount Vernon Nazarene University
 *
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 * refers to the actual string entered in the form's "verse" field.
 *
 * STUDENT NAME: Coen Hall
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Bible.h"
#include "Ref.h"
#include "Verse.h"

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"

using namespace std;
using namespace cgicc;

static const string names[66]
{
   "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy", "Joshua",
   "Judges", "Ruth", "1 Samuel", "2 Samuel", "1 Kings", "2 Kings",
   "1 Chronicles", "2 Chronicles", "Ezra", "Nehemiah", "Esther",
   "Job", "Psalms", "Proverbs", "Ecclesiastes", "Song of Solomon",
   "Isaiah", "Jeremiah", "Lamentations", "Ezekiel", "Daniel",
   "Joel", "Amos", "Obadiah", "Jonah", "Micah", "Nahum",
   "Habakkuk", "Zephaniah", "Haggai", "Zechariah", "Malachi",
   "Matthew", "Mark", "Luke", "John", "Acts", "Romans",
   "1 Corinthians", "2 Corinthians", "Galatians", "Ephesians",
   "Phillipians", "Colossians", "1 Thessalonians", "2 Thessalonians",
   "1 Timothy", "2 Timothy", "Titus", "Philemon", "Hebrews", "James",
   "1 Peter", "2 Peter", "1 John", "2 John", "3 John", "Jude",
   "Revelation"
};

int main()
{
   /* A CGI program must send a response header with content type
    * back to the web client before any other output.
    * For an AJAX request, our response is not a complete HTML document,
    * so the response type is just plain text to insert into the web page.
    */
   cout << "Content-Type: text/plain\n\n";

   Cgicc cgi;  // create object used to access CGI request data

   // GET THE INPUT DATA
   // browser sends us a string of field name/value pairs from HTML form
   // retrieve the value for each appropriate field name
   form_iterator st = cgi.getElement("search_type");
   form_iterator book = cgi.getElement("book");
   form_iterator chapter = cgi.getElement("chapter");
   form_iterator verse = cgi.getElement("verse");
   form_iterator nv = cgi.getElement("num_verse");

   // Convert and check input data
   bool validInput = false;
   if (chapter != cgi.getElements().end())
   {
      int chapterNum = chapter->getIntegerValue();
      if (chapterNum > 150)
      {
         cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
      }
      else if (chapterNum <= 0)
      {
         cout << "<p>The chapter must be a positive number.</p>" << endl;
      }
      else
      {
         validInput = true;
      }
   }

   // TODO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you!

   if(book == cgi.getElements().end())
   {
      cout << "<p>The book number is missing.</p>" << endl;
      validInput = false;
   }
   else
   {
      int bookNum = book->getIntegerValue();
      if(bookNum < 1 || bookNum > 66)
      {
         cout << "<p>The book number (" << bookNum << ") is invalid.</p>" << endl;
         validInput = false;
      }
   }

   if(verse == cgi.getElements().end())
   {
      cout << "<p>The verse number is missing.</p>" << endl;
      validInput = false;
   }
   else
   {
      int verseNum = verse->getIntegerValue();
      if(verseNum <= 0)
      {
         cout << "<p> The verse must be a positive number.</p>" << endl;
         validInput = false;
      }
   }

   int numVerses = 1;
   if(nv != cgi.getElements().end())
   {
      int temp = nv->getIntegerValue();
      if(temp > 0)
      {
         numVerses = temp;
      }
      else
      {
         numVerses = 1;
      }
   }

   /* TODO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
    *        TO LOOK UP THE REQUESTED VERSES
    */

   Bible webBible("/home/class/csc3004/Bibles/web-complete");
   LookupResult status = OTHER;

   int bookNum = book->getIntegerValue();
   int chapterNum = chapter->getIntegerValue();
   int verseNum = verse->getIntegerValue();

   Ref ref(bookNum, chapterNum, verseNum);
   Verse foundVerse = webBible.lookup(ref, status);

   if(status != SUCCESS)
   {
      if(status == NO_BOOK)
      {
         cout << "<p>Lookup failed, no such book " << bookNum << " exists</p>" << endl;
         validInput = false;
      }
      else if(status == NO_CHAPTER)
      {
         cout << "<p>Lookup failed, no such chapter " << chapterNum << " exists</p>" << endl;
         validInput = false;
      }
      else if(status == NO_VERSE)
      {
         cout << "<p>Lookup failed, no such verse " << verseNum << " exists</p>" << endl;
         validInput = false;
      }
      else
      {
         cout << "<p>Lookup failed</p>" << endl;
         validInput = false;
      }
   }

   /* SEND BACK THE RESULTS
    * Finally we send the result back to the client on the standard output stream
    * in HTML text format.
    * This string will be inserted as is inside a container on the web page,
    * so we must include HTML formatting commands to make things look presentable!
    */
   if (validInput)
   {
      cout << "<p><b>" << names[foundVerse.getRef().getBook() - 1] << " " << chapterNum << ":" << verseNum << "</b><br>"
           << foundVerse.getVerse() << "</p>" << endl;

      for(int i = 0; i < numVerses - 1; i++)
      {
         Verse v = webBible.nextVerse(status);
         if(status != SUCCESS)
         {
            break;
         }

         cout << "<p><b>" << names[foundVerse.getRef().getBook() - 1] << " " << v.getRef().getChapter() << ":"
              << v.getRef().getVerse() << "</b><br>" << v.getVerse() << "</p>" << endl;
      }
   }
   else
   {
      cout << "<p>Invalid Input: <em>report the more specific problem.</em></p>" << endl;
   }

   return 0;
}
