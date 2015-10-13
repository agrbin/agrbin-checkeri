#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main( int argc, char *argv[] )
{
   assert( argc == 4 );

   ifstream finput( argv[1] );
   ifstream fsluzb( argv[2] );
   ifstream fnatj( argv[3] );

   assert( !finput.fail() && !fsluzb.fail() && !fnatj.fail() );

   string sluzb, natj;
   fsluzb >> sluzb;
   if ( !(fnatj >> natj) ) {
      printf( "0\nProgram nije ispisao rješenje u traženom formatu!\n" );
      return 0;
   }

   if ( strcmp(sluzb.c_str(), natj.c_str()) ) {
      if ( natj.length() > 50 ) {
         natj = natj.substr(0, 50) + "(...)";
      }
      if ( sluzb.length() > 50 ) {
         sluzb = sluzb.substr(0, 50) + "(...)";
      }
      printf( "0\nVaš program je ispisao %s. Točno rješenje je %s.\n", natj.c_str(), sluzb.c_str() );
      return 0;
   }

   if ( fnatj >> natj ) {
      printf( "0\nVaš program je ispisao previše podataka!\n" );
      return 0;
   }

   printf( "1\nTočno!\n" );
   return 0;
}

