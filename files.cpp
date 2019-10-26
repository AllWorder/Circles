#include <iostream>
#include <fstream>


main()
{
    char line[25];
    char anline[25];
    char ananline[25];

    
    std::ifstream file;
    file.open("file.txt");
    
    file.getline(line, 20); // - 1 symbols
    std::cout << line << "\n";

    file.getline(anline, 20);
    std::cout << anline << "\n";

    file.getline(ananline, 20); 
    std::cout << ananline << "\n";


    file.seekg(0, file.end);
    int len = file.tellg();
    file.seekg(0, file.beg);

    std::cout << len << "\n";







    /*std::ofstream file;
    file.open("file.txt");
    file << "Jenya sasat";
    file.close;
    */
}