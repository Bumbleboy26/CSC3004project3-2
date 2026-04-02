
/* lookupserver.cpp
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * lookupserver for Project 3 Part 3
 *
 *
 * STUDENT NAME: Coen Hall
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include "fifo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main()
{
   // Create Bible object to process the raw text file
   Bible webBible("/home/class/csc3004/Bibles/web-complete");

   Fifo request("request");
   Fifo reply("reply");


   Verse verse;
   LookupResult result;
	
   cout << "Using Bible from: ";
   webBible.display();

   bool running = true;

   // Keep the server running continuously
   while(running)
   {
      //Read the message from the request pipe
      request.openread();
      string input = request.recv();
      request.fifoclose();

      cout << "Request received: " << input << endl;

      int book, chapter, verseNum, numVerses;


      //Find the colons that separate book chapter, and verse
      size_t p1 = input.find(':');
      size_t p2 = input.find(':', p1 + 1);
      size_t p3 = input.find(':', p2 + 1);

      book = stoi(input.substr(0, p1));
      chapter = stoi(input.substr(p1 + 1, p2 - p1 - 1));
      verseNum = stoi(input.substr(p2 + 1, p3 - p2 - 1));
      numVerses = stoi(input.substr(p3 + 1));

      string response = "";

      if(book >= 1 && book <= 66 && chapter >= 1 && verseNum >= 1 && numVerses >= 1)
      {
         Ref ref(book, chapter, verseNum);
         verse = webBible.lookup(ref, result);
      
         if(result == SUCCESS)
         {
            response = "SUCCESS|";
            response += to_string(verse.getRef().getBook()) + ":";
            response += to_string(verse.getRef().getChapter()) + ":";
            response += to_string(verse.getRef().getVerse()) + " ";
            response += verse.getVerse();

            for(int i = 1; i < numVerses; i++)
            {
               Verse nextVerse = webBible.nextVerse(result);
               if(result != SUCCESS)
               {
                  break;
               }

               response += "|";
               response += to_string(nextVerse.getRef().getVerse());
               response += nextVerse.getVerse();
            }
         }
         else if(result == NO_BOOK)
         {
            response = "ERROR| no book";
         }
         else if(result == NO_CHAPTER)
         {
            response = "ERROR| no chapter";
         }
         else if(result == NO_VERSE)
         {
            response = "ERROR| no verse";
         }
         else
         {
            response = "ERROR| an error has occurred";
         }
      }
      else
      {
         response = "ERROR| invalid input";
      }

      //Send the message through the reply pipe 
      reply.openwrite();
      reply.send(response);
      reply.fifoclose();

      cout << "Reply sent: " << response << endl;
   }
}
