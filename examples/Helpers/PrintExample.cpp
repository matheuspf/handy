#include "Helpers/Print.h"
#include <vector>


void PrintExampleClass ()
{
//! [Print Class Snippet]
    handy::Print printer(", ", "");   // Output to std::cout, separated by ", " and without line break ("")

    printer(10, 20, 30);  // "10, 20, 30"

    printer(std::vector<int>{10, 20, 30});  // Same as bove


    std::ostream myFile("myFile.txt");

    handy::Print filePrinter(myFile, ", ", "");   // Output set to 'myFile'

    filePrinter(10, 20, 30);    // Print to 'myFile'

    filePrinter(std::cout, 10, 20, 30);  // Here the output is to std::cout, not to 'myFile'

    filePrinter(10, 20, 30);    // Print to 'myFile' again
//! [Print Class Snippet]
}


void PrintExampleTest ()
{
//! [Print Function Snippet]
    handy::print(10, 20, 30);   // Exactly the same as handy::Print(std::cout, " ", "\n").operator()

    std::ostream myFile("myFile.txt");

    handy::print(myFile, 10, 20, 30);   // Print to 'myFile' -> "10, 20, 30\n"

    handy::print(myFile, std::vector<int>{10, 20, 30}); // Same as above
//! [Print Function Snippet]
}



int main ()
{
    return 0;
}