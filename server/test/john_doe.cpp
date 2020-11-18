#include <iostream>
using namespace std;

int main() {
   
   int trunkHeight;
   int trunkWidth;
   int leavesWidth;
   
   cout << "Enter height of trunk: ";
   cin >> trunkHeight;
   cout << endl;
   
   while (trunkHeight <= 0) {
      cout << "Please enter a positive number for height of trunk: ";
      cin >> trunkHeight;
      cout << endl;
   }
   
   cout << "Enter width of trunk: ";
   cin >> trunkWidth;
   cout << endl;
   
   while (((trunkWidth % 2) == 0) && trunkWidth > 0) {
      cout << "Please enter an odd number for width of trunk: ";
      cin >> trunkWidth;
      cout << endl;
   }
   
   cout << "Enter width of leaves: ";
   cin >> leavesWidth;
   cout << endl << endl;
   
   while (((leavesWidth % 2) == 0) || (leavesWidth <= (trunkWidth+1))) {
      cout << "Please enter a number for width of leaves larger than " << trunkWidth+1 <<": ";
      cin >> leavesWidth;
      cout << endl;
   }
   
   
   int a;
   int b;
   int c;
   for (a = 1; a <= leavesWidth; a = a + 2) {
      for (b = 0; b < (leavesWidth - a) / 2; ++b) {
         cout << " ";
      }
      for (c = 0; c < a; ++c) {
         cout << "*";
      }
      cout << endl;
   }
   
   
   int i;
   int j;
   int k;
   for (i = 0; i < trunkHeight; ++i) {
      for (j = 0; j < (leavesWidth - trunkWidth) / 2; ++j) {
         cout << " ";
      }
      for (k = 0; k < trunkWidth; ++k) {
         cout << "*";
      }
      cout << endl;
   }
   
   


    return 0;
}