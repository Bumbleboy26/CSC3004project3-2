/* Bible class function definitions
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
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

// Default constructor
Bible::Bible()
{ 
   infile = "/home/class/csc3004/Bibles/web-complete";
   isOpen = false;
   buildIndex();
}

// Constructor – pass bible filename
Bible::Bible(const string s)
{
   infile = s;
   isOpen = false;
   buildIndex();
}

void Bible::buildIndex()
{
   lastOffset = -1;
   refIndex.clear();
   instream.open(infile.c_str());
   if(!instream.is_open())
   {
      return;
   }

   instream.clear();
   instream.seekg(0);

   string line;
   while(!instream.eof())
   {
      int pos = (int)instream.tellg();

      getline(instream, line);
      if(instream.fail())
      {
         break;
      }

      Verse v(line);
      Ref r = v.getRef();
      refIndex[r] = pos;
      lastOffset = pos;
   }

   instream.close();

}

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status)
{ 
   // TODO: scan the file to retrieve the line that holds ref ...
     
   // update the status variable
   status = OTHER; // placeholder until retrieval is attempted
	
   // create and return the verse object
   Verse aVerse;   // default verse, to be replaced by a Verse object
	                // that is constructed from a line in the file.

   map<Ref, int>::iterator it = refIndex.find(ref);
   if(it != refIndex.end())
   {
      if(!instream.is_open())
      {
         instream.open(infile.c_str());
         if(!instream.is_open())
         {
            status = OTHER;
            return aVerse;
         }
      }
   }

   instream.clear();
   instream.seekg(it->second);

   string line;
   if(getline(instream, line))
   {
      status = SUCCESS;
      return Verse(line);
   }
   else
   {
      status = OTHER;
      return aVerse;
   }

   if(!instream.is_open())
   {
      instream.open(infile.c_str());
      if(!instream.is_open())
      {
         status = OTHER;
         return(aVerse);
      }
   }

   bool foundBook = false;
   bool foundChapter = false;

   instream.clear();
   instream.seekg(0);

   while(getline(instream, line))
   {
      Verse v(line);
      Ref r = v.getRef();
      
      if(r.getBook() == ref.getBook())
      {
         foundBook = true;
         
         if(r.getChapter() == ref.getChapter())
         {
            foundChapter = true;
            if(r == ref)
            {
               status = SUCCESS;
               aVerse = v;
               return(aVerse);
            }
         }
      }
   }

   if(!foundBook)
   {
      status = NO_BOOK;
   }
   else if(!foundChapter)
   {
      status = NO_CHAPTER;
   }
   else
   {
      status = NO_VERSE;
   }

   return(aVerse);
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult& status)
{
   Verse verse;

   if(!instream.is_open())
   {
      instream.open(infile.c_str());
      isOpen = true;
      if(!instream.is_open())
      {
         status = OTHER;
         return verse;
      }
   }

   string line;
   
   if(getline(instream, line))
   {
      status = SUCCESS;
      verse = Verse(line);
   }
   else
   {
      status = OTHER;
   }

   return verse;
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status)
{
   if(status == SUCCESS)
   {
      return "Success.";
   }
   else if(status == NO_BOOK)
   {
      return "Book not found.";
   }
   else if(status == NO_CHAPTER)
   {
      return "Chapter not found.";
   }
   else if(status == NO_VERSE)
   {
      return "Verse not found.";
   }
   else if(status == OTHER)
   {
      return "An error has occurred.";
   }
   else
   {
      return "An unknown error has occurred.";
   }
}

void Bible::display()
{
   cout << "Bible file: " << infile << endl;
   cout << "Index size: " << refIndex.size() << endl;
   cout << "Byte offset for Genesis 1:1: " << refIndex[Ref(1, 1, 1)] << endl;
   cout << "Byte offset for Genesis 1:2: " << refIndex[Ref(1, 1, 2)] << endl;
   cout << "Byte offset for Genesis 1:3: " << refIndex[Ref(1, 1, 3)] << endl; 
   cout << "Last offset: " << lastOffset << endl;
}
	
// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
Ref Bible::next(const Ref ref, LookupResult& status)
{
   map<Ref, int>::iterator it = refIndex.find(ref);

   if(it == refIndex.end())
   {
      status = NO_VERSE;
      return ref;
   }

   ++it;

   if(it == refIndex.end())
   {
      status = OTHER;
      return ref;
   }

   status = SUCCESS;
   return it->first;
}

// OPTIONAL: Return the reference before the given ref
Ref Bible::prev(const Ref ref, LookupResult& status)
{
   return ref;
}
