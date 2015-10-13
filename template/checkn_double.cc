#include <cassert>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>

using namespace std;

const double EPS = 1e-3;

inline bool check(double a, double b) {
    return abs(a-b) < EPS || abs((a-b)/b) < EPS;
}

int main(int argc, char *argv[])
{
   assert( argc == 4 );

   ifstream finput( argv[1] );
   ifstream fsluzb( argv[2] );
   ifstream fnatj( argv[3] );

   assert( !finput.fail() && !fsluzb.fail() && !fnatj.fail() );

   double rj, slurj;

   for( int iter=1; fsluzb >> slurj; ++iter ) {
      if( !(fnatj >> rj) ) {
         printf("0\nProgram nije ispisao rješenje u zadanom obliku!\n");
         exit(0);
      }
      if( !check(rj, slurj) ) {
         printf("0\n%d. broj: dobio %.6lf, a očekivao %.6lf!\n", iter, rj, slurj);
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

