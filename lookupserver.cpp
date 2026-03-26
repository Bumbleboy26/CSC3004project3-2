
/* lookupserver.cpp
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
#include "fifo.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main ()
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

   while(running)
   {
      request.openread();
      string input = request.recv();
      request.fifoclose();

      cout << "Request received: " << input << endl;

      Ref ref(input);
      verse = webBible.lookup(ref, result);

      string response;
      
      if(result == SUCCESS)
      {
         response = "SUCCESS|" + verse.getVerse();
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
         response = "ERROR| an error has occurred";
      }

      reply.openwrite();
      reply.send(response);
      reply.fifoclose();

      cout << "Reply sent: " << response << endl;
   }
}
