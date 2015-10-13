#include <cassert>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
   assert( argc == 4 );

   ifstream finput( argv[1] );
   ifstream fsluzb( argv[2] );
   ifstream fnatj( argv[3] );

   assert( !finput.fail() && !fsluzb.fail() && !fnatj.fail() );

   string rj, slurj;

   for( int iter=1; fsluzb >> slurj; ++iter ) {
      if( !(fnatj >> rj) ) {
         printf("0\nProgram nije ispisao rješenje u zadanom obliku!\n");
         exit(0);
      }
      if( slurj != rj ) {
         if ( rj.length() > 50 )
            rj = rj.substr(0, 50) + "(...)";

         printf("0\n%d. broj: dobio %s, a očekivao %s!\n", iter, rj.c_str(), slurj.c_str());
         exit(0);
      }
   }
   if( fnatj >> rj ) {
      printf("0\nProgram je ispisao previše podataka na izlaz!\n");
      exit(0);
   }

   printf("1\nTočno!\n");

   return 0;
}

