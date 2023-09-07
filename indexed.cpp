#include <bits/stdc++.h>
#include <unistd.h>
#include <string.h>
#include<chrono>

using namespace std;

#define MAX_FILES 30
// #define MAX_FILE_SIZE 1000
#define total_blocks 100



typedef struct BlockNode
{
    int loc;
    char ch;
    int free; // 0 if free, 1 if allocated
} Block;

typedef struct IndexBlock
{
    int size;
   Block* disk_blk[total_blocks];
}Index_Block;

typedef struct
{
    string name;
    Index_Block *ind_blk;
} File;

// int memory[MAX_FILE_SIZE];
// int free_blocks[total_blocks]={0};   //0 if free, 1 if allocated

Block blocks[total_blocks];
File file_table[MAX_FILES];
int num_files = 0;
// int next_free_index = 0;

int create_file(string name,string str)
{
    if (num_files >= MAX_FILES)
    {
        printf("Error: Maximum number of files reached\n");
        return 0;
    }

    int next_free_index;
    bool flag = false;


    int size=str.length();
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

    File file;
    file.name = name;
    
    //cout<<"11\n";
    Index_Block* tmp_ind;
    tmp_ind=(Index_Block*)malloc(sizeof(Index_Block));
     tmp_ind->size=size;
    // cout<<"11\n";
    
    for (int i = 0; i <tmp_ind->size; i++)
    {
        blocks[free_blocks[i]].free = 1;
            blocks[free_blocks[i]].ch = str[i];

        tmp_ind->disk_blk[i]=&(blocks[free_blocks[i]]);
    }

    file.ind_blk=tmp_ind;
    file_table[num_files] = file;
    num_files++;

    cout << "file " << name << " has been created succesfully\n";
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
            cout << "file: " << file_table[i].name<< endl;

            Index_Block* temp=file_table[i].ind_blk;
            for(int j=0;j<temp->size;j++)
               cout << "b" << temp->disk_blk[j]->loc << "[" << temp->disk_blk[j]->ch << "] ";

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

            if (data.length() > file_table[i].ind_blk->size)
            {
                printf("Error: File size limit exceeded\n");
                return;
            }

            
          /*  Block* temp=&(blocks[file_table[i].start_index]);
            int count=0;
            while(temp)
            {
                if(count<data.length())
                   temp->ch=data[count++];
                 else
                    temp->ch='\0';

                    temp=temp->next; 
            }*/

            Index_Block* temp_ind=file_table[i].ind_blk;
            for(int j=0;j<temp_ind->size;j++)
            {
                if(j<data.length())
                   temp_ind->disk_blk[j]->ch=data[j];
                else
                      temp_ind->disk_blk[j]->ch='\0';
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
    //cout<<"11\n";
    int i;
    for (i = 0; i < num_files; i++)
    {
        if (file_table[i].name == name)
        {
            Index_Block* temp_ind=file_table[i].ind_blk;
            for(int j=0;j<temp_ind->size;j++)
              {
                 temp_ind->disk_blk[j]->ch='\0';
                 temp_ind->disk_blk[j]->free=0;
              }


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
