#include <fstream>
#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include<time.h>

using namespace std;

const int MAX_FILE_SIZE = 1000000;  // Maximum file size in bytes
const int MAX_STRING_LENGTH = 1000; // Maximum length of each string in bytes

// Generate a random string of characters
string generate_random_string()
{
    int length = rand() % MAX_STRING_LENGTH + 1;
    string str;
    for (int i = 0; i < length; i++)
    {
        char c = 'a' + rand() % 26;
        str += c;
    }
    return str;
}

// Generate a random file with random strings
void generate_random_file(const string &filename)
{
    ofstream outfile(filename, ios::binary);
    if (!outfile)
    {
        cerr << "Error: could not open file for writing\n";
        return;
    }

    int file_size = rand() % MAX_FILE_SIZE + 1;
    int bytes_written = 0;

    while (bytes_written < file_size)
    {
        string str = generate_random_string();
        int str_size = str.size();
        if (bytes_written + str_size > file_size)
        {
            // Truncate the string if it would make the file too big
            str_size = file_size - bytes_written;
            str = str.substr(0, str_size);
        }
        outfile.write(str.c_str(), str_size);
        bytes_written += str_size;
    }

    outfile.close();
}

int main()
{
    srand(time(NULL));
    string name , str;
    for(int j=0;j<5;j++)
          {
            name+=char(rand()%26+97);
            //cout<<name[j]<<endl;
            //cout<<char(rand()%26+97)<<endl;

          }
           name=name+".txt";

           cout<<name;

    return 0;
}
