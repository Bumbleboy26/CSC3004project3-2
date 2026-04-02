/* bibleindex.cpp
* STUDENT NAME: Coen Hall
* DATE: April 2, 2026
*/


#include <iostream>
#include <stdio.h>
#include <string.h>
#include "fifo.h"

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"

using namespace std;
using namespace cgicc;

static const string names[66] =
{
   "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy", "Joshua",
   "Judges", "Ruth", "1 Samuel", "2 Samuel", "1 Kings", "2 Kings",
   "1 Chronicles", "2 Chronicles", "Ezra", "Nehemiah", "Esther",
   "Job", "Psalms", "Proverbs", "Ecclesiastes", "Song of Solomon",
   "Isaiah", "Jeremiah", "Lamentations", "Ezekiel", "Daniel", "Hosea",
   "Joel", "Amos", "Obadiah", "Jonah", "Micah", "Nahum",
   "Habakkuk", "Zephaniah", "Haggai", "Zechariah", "Malachi",
   "Matthew", "Mark", "Luke", "John", "Acts", "Romans",
   "1 Corinthians", "2 Corinthians", "Galatians", "Ephesians",
   "Philippians", "Colossians", "1 Thessalonians", "2 Thessalonians",
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

   Cgicc cgi;

   form_iterator st = cgi.getElement("search_type");
   form_iterator book = cgi.getElement("book");
   form_iterator chapter = cgi.getElement("chapter");
   form_iterator verse = cgi.getElement("verse");
   form_iterator nv = cgi.getElement("num_verse");

   bool validInput = false;
   if(chapter != cgi.getElements().end())
   {
      int chapterNum = chapter->getIntegerValue();
      if(chapterNum > 150)
      {
         cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
      }
      else if(chapterNum <= 0)
      {
         cout << "<p>The chapter number must be a positive number.</p>" << endl;
      }
      else
      {
         validInput = true;
      }
   }

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
         cout << "<p>The verse must be a positive number.</p>" << endl;
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
   }

   string response = "";

   if(validInput)
   {
      int bookNum = book->getIntegerValue();
      int chapterNum = chapter->getIntegerValue();
      int verseNum = verse->getIntegerValue();

      string requestMessage = to_string(bookNum) + ":" +
                              to_string(chapterNum) + ":" +
                              to_string(verseNum) + ":" +
                              to_string(numVerses);

      Fifo request("request");
      Fifo reply("reply");

      request.openwrite();
      request.send(requestMessage);
      request.fifoclose();

      reply.openread();
      response = reply.recv();
      reply.fifoclose();
   }

   if(validInput)
   {
      cout << "<p>" << response << "</p>" << endl;
   }
   else
   {
      cout << "<p>Invalid input.</p>" << endl;
   }

   return 0;

}
