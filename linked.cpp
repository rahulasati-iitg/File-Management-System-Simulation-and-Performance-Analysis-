#include <bits/stdc++.h>
#include <unistd.h>
#include <string.h>
#include<chrono>

using namespace std;

#define MAX_FILES 30
// #define MAX_FILE_SIZE 1000
#define total_blocks 100

typedef struct
{
    string name;
    int start_index;
    int size;
} File;

typedef struct BlockNode
{
    int loc;
    char ch;
    int free; // 0 if free, 1 if allocated
    struct BlockNode *next;
} Block;

// int memory[MAX_FILE_SIZE];
// int free_blocks[total_blocks]={0};   //0 if free, 1 if allocated

Block blocks[total_blocks];
File file_table[MAX_FILES];
int num_files = 0;
// int next_free_index = 0;

int create_file(string name, string str)
{
    if (num_files >= MAX_FILES)
    {
        printf("Error: Maximum number of files reached\n");
        return 0;
    }

    int size=str.length();
    int next_free_index;
    bool flag = false;

    int free_blocks[size];
    int free_blocks_count = 0;
    for (int i = 0; i < total_blocks; i++) // this loop finds a contiguous memory location where the given file can be accomodated
    {

        if (blocks[i].free == 0)
        {
            free_blocks[free_blocks_count++] = i;
        }

        if (free_blocks_count == size)
        {
            flag = true;
            break;
        }
    }

    if (flag == false) // if no such memory location could be found where the file can be accomodated
    {
        printf("not enough memory to allocate the file\n");
        return 0;
    }

    int start_index = free_blocks[0];
    File file;
    // strcpy(file.name, name);
    file.name = name;
    file.start_index = start_index;
    file.size = size;

    blocks[free_blocks[0]].free = 1;
    blocks[free_blocks[0]].ch = str[0];

    for (int i = 1; i < file.size; i++)
    {
        blocks[free_blocks[i - 1]].next = &(blocks[free_blocks[i]]);
        blocks[free_blocks[i]].free = 1;

        //if (i < str.length())
            blocks[free_blocks[i]].ch = str[i];
    }

    blocks[free_blocks[file.size - 1]].next = NULL;

    file_table[num_files] = file;
    num_files++;

    cout << "file " << name << " has been created\n";
    //sleep(1);
    return 1;
}

void read_file(string name) // reads the given file content
{
    int i;
    for (i = 0; i < num_files; i++)
    {
        if (file_table[i].name == name)
        {
            cout << "file: " << file_table[i].name << "\nstart_index: " << file_table[i].start_index << "  size: " << file_table[i].size << endl;
            int j;
            Block *temp = &(blocks[file_table[i].start_index]);
            //int count = file_table[i].start_index;

            while (temp)
            {
                cout << "b" << temp->loc << "[" << temp->ch << "] ";
                temp = temp->next;
            }

            printf("\n");
            //sleep(1);
            return;
        }
    }
    printf("Error: File not found\n");
}

void update_file(string name, string data)
{
    int i;
    for (i = 0; i < num_files; i++)
    {
        if (file_table[i].name == name)
        {

            if (data.length() > file_table[i].size)
            {
                printf("Error: File size limit exceeded\n");
                return;
            }

            /*int j;
            for (j = 0; j < file_table[i].size; j++)
            {

                if (j < data.length())
                    blocks[file_table[i].start_index + j].ch = data[j];
                else
                    blocks[file_table[i].start_index + j].ch = '\0';
            }*/
            Block* temp=&(blocks[file_table[i].start_index]);
            int count=0;
            while(temp)
            {
                if(count<data.length())
                   temp->ch=data[count++];
                 else
                    temp->ch='\0';

                    temp=temp->next; 
            }


            cout << name << " file has been updated succesfully\n";
           // sleep(1);
            return;
        }
    }
    printf("Error: File not found\n");
}

int delete_file(string name)
{
    int i;
    for (i = 0; i < num_files; i++)
    {
        if (file_table[i].name == name)
        {
            int j;
            Block *temp = &(blocks[file_table[i].start_index]);
            Block *temp1;
            while (temp)
            {
                temp1 = temp;
                temp->ch = '\0';
                temp->free = 0;

                temp = temp->next;
                temp1->next = NULL;
            }

            free(&(file_table[i])); // free the file structure

            for (; i < num_files - 1; i++)
            {
                file_table[i] = file_table[i + 1];
            }
            num_files--;

            cout << name << " file has been deleted succesfully\n";
           // sleep(1);
            return 1;
        }
    }
    printf("Error: File not found\n");
    return 0;
}

int main()
{
    srand(time(NULL));

    for(int i=0;i<total_blocks;i++)
    {
        blocks[i].loc=i;
        blocks[i].next=NULL;
        blocks[i].free=0;
         blocks[i].ch='\0';
    }

    int n;
    int allocated_files=0;

    cout << "how many files do you want to create?: ";
    cin >> n;

   chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        string name, str;
         /*cout << "enter the name of the file"<<i+1<<": ";
         cin >> name;

         cout << "enter the content of the file"<<i+1<<": ";
         cin >> str;*/

        for (int j = 0; j < 5; j++) // file name will always be of size=%+4
            name += char(rand() % 26 + 97);
        name = name + ".txt";

        for (int j = 0; j < (rand() % 30 + 1); j++) // the file content will be of size<=20
            str += char(rand() % 26 + 97);

        int size = str.length();
        int check = create_file(name, str);
        if(check==1)
          allocated_files++;

        // cout<<endl;
    }
    cout << endl << endl;
    for (int i = 0; i < num_files; i++)
    {
        read_file(file_table[i].name);
        cout << endl;
    }


    for(int i=0;i<7;i++)  //here some files are being deleted
    {
        int x=rand()%n+1;
        int chk=delete_file(file_table[x].name);
        if(chk==1)
          allocated_files--;
    }

    cout << endl;

    cout<<endl<<endl<<"------------------------------------------------------blocks------------------------------------------------\n";
    for (int i = 0; i < total_blocks; i++)
        {
            cout << "b" << i << "[" << blocks[i].ch << "] ";
            if((i+1)%10==0)
              cout<<endl;
        }

    cout << endl<<endl;
    // update_file(file_table[1].name,"mahas");

    for (int i = 0; i < 4; i++)
    {
        string name, str;
        /*cout << "enter the name of the file"<<i+1<<": ";
         cin >> name;
         cout << "enter the content of the file"<<i+1<<": ";
         cin >> str;*/
        for (int j = 0; j < 5; j++) // file name will always be of size=%+4
            name += char(rand() % 26 + 97);
        name = name + ".txt";

        for (int j = 0; j < (rand() % 20 + 11); j++) // the file content will be of size<=11
            str += char(rand() % 26 + 97);

        int size = str.length();
        int check = create_file(name, str);
        if(check==1)
          allocated_files++;

        // cout<<endl;
    }

    cout << endl;
    for (int i = 0; i < num_files; i++)
    {
        read_file(file_table[i].name);
        cout << endl;
    }

    cout<<endl<<endl<<"------------------------------------------------------blocks--------------------------------------------------\n";
    for (int i = 0; i < total_blocks; i++)
        {
            cout << "b" << i << "[" << blocks[i].ch << "] ";
            if((i+1)%10==0)
              cout<<endl;
        }

    cout << endl;

    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
   // fclose(fp);

   int free_blocks=0;
   for(int i=0;i<total_blocks;i++)
     if(blocks[i].free==0)
        free_blocks++;

        long long duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        cout<<"no. of total allocated files: "<<allocated_files<<"\nno. of total free memory blocks after allocation: "<<free_blocks<<endl;
        cout<<"total time duration of all the operations:"<<duration<<" micro seconds"<<endl;

    return 0;
}
