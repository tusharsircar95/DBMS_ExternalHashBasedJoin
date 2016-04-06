//TUSHAR SIRCAR
//130123038
//DBMS - CS344
//Implementation Of Join Using External Hashing Under Memory Constraints
#include<iostream>
#include<stdio.h>
#include<map>
#include<vector>
#include<cmath>
#include<math.h>
#include<algorithm>
#include<sstream>
#include<string>
# define M_PI   3.14159265358979323846
using namespace std;
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


int getLSB(int key,int bits)
{
    int ans = 0;
    for(int i=0; i<bits; i++)
    {
        ans += (key&1)*pow(2,i);
        key = key>>1;
    }
    return ans;
}

int hashValue(int key,int I,int noOfBuckets)
{
    int temp = getLSB(key,noOfBuckets+I);
    return temp%noOfBuckets;
	//return ((int)floor(x*r*M_PI))%noOfBuckets;
}

int IO = 0;
vector<int> resultValues;
FILE *outputLog;
FILE *examOutputLog;
bool joinPossible = true;

int getPages(int noOfRecords,int recordSize,int pageSize)
{
    int recordsPerPage = pageSize/recordSize;
    int pages = noOfRecords/recordsPerPage;
    if(noOfRecords%recordsPerPage != 0)
        pages++;
    return pages;
}

int getNoOfRecords(string r)
{
    FILE *relation = fopen(r.c_str(),"r");
    int count = 0;
    char str[500];
    while(fgets(str,500,relation))
        count++;
    fclose(relation);
    return count;
}

int getNoOfPages(string r,int recordSize,int pageSize)
{
    FILE *relation = fopen(r.c_str(),"r");
    int recordsPerPage = pageSize/recordSize;
    int count = 0;
    int pages = 0;
    char str[500];
    while(fgets(str,500,relation))
    {
        count++;
        if(count == recordsPerPage)
        {
            count = 0;
            pages++;
        }
    }
    fclose(relation);
    if(count != 0)
        pages++;
    return pages;
}

void performInMemoryJoin(string r1,string r2)
{

    printf("Performing in memory join for %s and %s...\n",r1.c_str(),r2.c_str());
    fprintf(outputLog,"Performing in memory join for %s and %s...\n",r1.c_str(),r2.c_str());

    FILE *relation1 = fopen(r1.c_str(),"r");
    FILE *relation2 = fopen(r2.c_str(),"r");
    char str[500];
    vector<int> records1,records2;

    while(fgets(str,500,relation1))
    {
        stringstream ss(str);
        int value;
        ss>>value;
        records1.push_back(value);
    }
    while(fgets(str,500,relation2))
    {
        stringstream ss(str);
        int value;
        ss>>value;
        records2.push_back(value);
    }

    sort(records1.begin(),records1.end());
    sort(records2.begin(),records2.end());

    int index1=0,index2=0;
    int l1=records1.size(),l2=records2.size();
    int counter = 0;

    while(index1<l1 && index2<l2)
    {
        if(records1[index1] == records2[index2])
        {
            int c1 = 0,c2 = 0;
            while(records1[index1] == records1[index1+c1])
                c1++;
            while(records2[index2] == records2[index2+c2])
                c2++;

            int freq = c1*c2;
            while(freq--)
            {
                counter++;
                cout<<records1[index1]<<endl;
                fprintf(outputLog,"%d\n",records1[index1]);
                resultValues.push_back(records1[index1]);
            }


            index1 += c1;
            index2 += c2;
        }
        else if(records1[index1] < records2[index2])
            index1++;
        else index2++;
    }
    fprintf(outputLog,"\n");
    cout<<endl;

    /*for(int i=0,l1=records1.size(); i<l1; i++)
        for(int j=0,l2=records2.size(); j<l2; j++)
                if(records1[i] == records2[j])
                {
                    cout<<records1[i]<<endl;
                    resultValues.push_back(records1[i]);
                    counter++;
                }
    */
    if(counter == 0)
    {
        cout<<"No matching tuples found! No further processing is required!\n\n";
        fprintf(outputLog,"No matching tuples found! No further processing is required!\n\n");
    }

}

void performJoin(string r1,string r2,int recordSize1,int recordSize2,int pageSize,int availablePages,int hashingRounds,int roundNo,int noOfPages_r1,int noOfPages_r2)
{
    int noOfBuckets = availablePages - 1;
    int pagesForRelation = 1;
    int recordsPerPage1 = pageSize/recordSize1;
    int recordsPerPage2 = pageSize/recordSize2;

    if(noOfPages_r1 == -1)
        noOfPages_r1 = getNoOfPages(r1,recordSize1,pageSize);
    if(noOfPages_r2 == -1)
        noOfPages_r2 = getNoOfPages(r2,recordSize2,pageSize);


    printf("READING %s AND %s\n",r1.c_str(),r2.c_str());
    printf("SIZE OF %s: %d pages\n",r1.c_str(),noOfPages_r1);
	printf("SIZE OF %s: %d pages\n",r2.c_str(),noOfPages_r2);
	printf("TOTAL NUMBER OF AVAILABLE PAGES: %d\n",availablePages);
    printf("NUMBER OF BUCKETS IN HASH TABLE: %d\n\n",availablePages-1);

    fprintf(outputLog,"READING %s AND %s\n",r1.c_str(),r2.c_str());
    fprintf(outputLog,"SIZE OF %s: %d pages\n",r1.c_str(),noOfPages_r1);
	fprintf(outputLog,"SIZE OF %s: %d pages\n",r2.c_str(),noOfPages_r2);
	fprintf(outputLog,"TOTAL NUMBER OF AVAILABLE PAGES: %d\n",availablePages);
    fprintf(outputLog,"NUMBER OF BUCKETS IN HASH TABLE: %d\n\n",availablePages-1);


    if(availablePages-1 >= noOfPages_r1 + noOfPages_r2)
    {
        IO += noOfPages_r1;
        IO += noOfPages_r2;
        performInMemoryJoin(r1,r2);
    }
	else if(hashingRounds == 0)
    {
        printf("Join not possible with given constraints!\n");
        fprintf(outputLog,"Join not possible with given constraints!\n");
        joinPossible = false;
    }
	else
	{
	    printf("Cannot perform in-memory join for %s and %s...\n",r1.c_str(),r2.c_str());
	    fprintf(outputLog,"Cannot perform in-memory join for %s and %s...\n",r1.c_str(),r2.c_str());
	    printf("HASHING ROUND %d...\n\n",roundNo);
	    fprintf(outputLog,"HASHING ROUND %d...\n\n",roundNo);
        fprintf(examOutputLog,"HASHING ROUND %d...\n\n",roundNo);

	    FILE *relation1 = fopen(r1.c_str(),"r");
        FILE *relation2 = fopen(r2.c_str(),"r");
        vector<vector<int> > hashTable;
        int recordsInBucket_r1[noOfBuckets];
        int recordsInBucket_r2[noOfBuckets];
        FILE *flushFiles1[noOfBuckets];
        FILE *flushFiles2[noOfBuckets];

        for(int i=0; i<noOfBuckets; i++)
        {
            vector<int> temp;
            hashTable.push_back(temp);
            recordsInBucket_r1[i] = 0;
            recordsInBucket_r2[i] = 0;
            string filename = "_bucket" + patch::to_string(i);
            flushFiles1[i] = fopen((r1.substr(0,r1.length()-4)+filename+".txt").c_str(),"w");
            flushFiles2[i] = fopen((r2.substr(0,r2.length()-4)+filename+".txt").c_str(),"w");
        }

        IO += noOfPages_r1;
        IO += noOfPages_r2;

        vector<int> rel1Buckets[noOfBuckets];
        vector<int> rel2Buckets[noOfBuckets];


        //reading relation1
        printf("READING %s\n",r1.c_str());
        fprintf(outputLog,"READING %s\n",r1.c_str());

        char str[500];
        int count = 0;




        while(fgets(str,500,relation1))
        {
            stringstream ss(str);
            int value;
            ss>>value;


            int h = hashValue(value,roundNo,noOfBuckets);
            rel1Buckets[h].push_back(value);
            hashTable[h].push_back(value);

            count++;
            printf("Tuple %d: %d Mapped to bucket: %d\n",(count),value,h);
            fprintf(outputLog,"Tuple %d: %d Mapped to bucket: %d\n",(count),value,h);
            recordsInBucket_r1[h]++;

            if(hashTable[h].size() == recordsPerPage1)
            {
                for(int i=0; i<recordsPerPage1; i++)
                    fprintf(flushFiles1[h],"%d\n",hashTable[h][i]);
                printf("Page for bucket %d full. Flushed to secondary storage\n",h);
                fprintf(outputLog,"Page for bucket %d full. Flushed to secondary storage\n",h);
                IO++;
                hashTable[h].clear();
            }
        }
        for(int i=0; i<noOfBuckets; i++)
            if(hashTable[i].size() != 0)
        {
            for(int j=0,l=hashTable[i].size(); j<l; j++)
                fprintf(flushFiles1[i],"%d\n",hashTable[i][j]);
            printf("Page for bucket %d is flushed to secondary storage\n",i);
            fprintf(outputLog,"Page for bucket %d is flushed to secondary storage\n",i);
            IO++;
            hashTable[i].clear();
        }

        printf("Done with %s\n\nCreated the following files:\n",r1.c_str());
        fprintf(outputLog,"Done with %s\n\nCreated the following files:\n",r1.c_str());
        for(int i=0; i<noOfBuckets; i++)
        {
            if(recordsInBucket_r1[i] == 0)
                continue;
            string filename = "_bucket" + patch::to_string(i);
            cout<<r1.substr(0,r1.length()-4)+filename+".txt"<<endl;
            fprintf(outputLog,"%s\n",(r1.substr(0,r1.length()-4)+filename+".txt").c_str());
        }
        cout<<endl;
        fprintf(outputLog,"\n");


        //reading relation2
        count = 0;
        printf("READING %s\n",r2.c_str());
        fprintf(outputLog,"READING %s\n",r2.c_str());
        while(fgets(str,500,relation2))
        {
            stringstream ss(str);
            int value;
            ss>>value;

            int h = hashValue(value,roundNo,noOfBuckets);
            rel2Buckets[h].push_back(value);
            hashTable[h].push_back(value);
            recordsInBucket_r2[h]++;

            count++;
            printf("Tuple %d: %d Mapped to bucket: %d\n",(count),value,h);
            fprintf(outputLog,"Tuple %d: %d Mapped to bucket: %d\n",(count),value,h);

            if(hashTable[h].size() == recordsPerPage2)
            {
                for(int i=0; i<recordsPerPage2; i++)
                    fprintf(flushFiles2[h],"%d\n",hashTable[h][i]);
                printf("Page for bucket %d full. Flushed to secondary storage\n",h);
                fprintf(outputLog,"Page for bucket %d full. Flushed to secondary storage\n",h);
                IO++;
                hashTable[h].clear();
            }
        }
        for(int i=0; i<noOfBuckets; i++)
            if(hashTable[i].size() != 0)
        {
            for(int j=0,l=hashTable[i].size(); j<l; j++)
                fprintf(flushFiles2[i],"%d\n",hashTable[i][j]);
            printf("Page for bucket %d is flushed to secondary storage\n",i);
            fprintf(outputLog,"Page for bucket %d is flushed to secondary storage\n",i);
            IO++;
            hashTable[i].clear();
        }

        printf("Done with %s\n\nCreated the following files:\n",r2.c_str());
        fprintf(outputLog,"Done with %s\n\nCreated the following files:\n",r2.c_str());
        for(int i=0; i<noOfBuckets; i++)
        {
            if(recordsInBucket_r2[i] == 0)
                continue;
            string filename = "_bucket" + patch::to_string(i);
            cout<<r2.substr(0,r1.length()-4)+filename+".txt"<<endl;
            fprintf(outputLog,"%s\n",(r2.substr(0,r1.length()-4)+filename+".txt").c_str());
        }
        cout<<endl;
        fprintf(outputLog,"\n");

        fclose(relation1);
        fclose(relation2);

        printf("Processed %s\n",r1.c_str());
        printf("Processed %s\n",r2.c_str());
        fprintf(examOutputLog,"Processed %s\n",r1.c_str());
        fprintf(examOutputLog,"Processed %s\n",r2.c_str());

        for(int i=0; i<noOfBuckets; i++)
        {
            printf("Bucket %d:\n",i);
            printf("R1 keys: ");
            fprintf(examOutputLog,"Bucket %d:\n",i);
            fprintf(examOutputLog,"R1 keys: ");

            for(int j=0,l=rel1Buckets[i].size(); j<l; j++)
            {
                printf("%d ",rel1Buckets[i][j]);
                fprintf(examOutputLog,"%d ",rel1Buckets[i][j]);
            }
            printf("\nR1 pages: %d\n",getPages(rel1Buckets[i].size(),recordSize1,pageSize));
            fprintf(examOutputLog,"\nR1 pages: %d\n",getPages(rel1Buckets[i].size(),recordSize1,pageSize));

            printf("R2 keys: ");
            fprintf(examOutputLog,"R2 keys: ");
            for(int j=0,l=rel2Buckets[i].size(); j<l; j++)
            {
                printf("%d ",rel2Buckets[i][j]);
                fprintf(examOutputLog,"%d ",rel2Buckets[i][j]);
            }
            printf("\nR2 pages: %d\n",getPages(rel2Buckets[i].size(),recordSize2,pageSize));
            fprintf(examOutputLog,"\nR2 pages: %d\n",getPages(rel2Buckets[i].size(),recordSize2,pageSize));

            string inMemoryPossible = "No";
            if(availablePages-1 >= getPages(rel1Buckets[i].size(),recordSize1,pageSize) + getPages(rel2Buckets[i].size(),recordSize2,pageSize))
                inMemoryPossible = "Yes";
            printf("In Memory Join: %s\n\n",inMemoryPossible.c_str());
            fprintf(examOutputLog,"In Memory Join: %s\n\n",inMemoryPossible.c_str());

        }


        for(int i=0; i<noOfBuckets; i++)
        {
            fclose(flushFiles1[i]);
            fclose(flushFiles2[i]);

            string filename = "_bucket" + patch::to_string(i);
            int pages1 = getPages(recordsInBucket_r1[i],recordSize1,pageSize);
            int pages2 = getPages(recordsInBucket_r2[i],recordSize2,pageSize);
            if(pages1 != 0 && pages2 != 0)
                performJoin(r1.substr(0,r1.length()-4)+filename+".txt",r2.substr(0,r2.length()-4)+filename+".txt",recordSize1,recordSize2,pageSize,availablePages,hashingRounds-1,roundNo+1,pages1,pages2);
        }

	}

}

void printResults()
{
    printf("TOTAL IO'S: %d\n",IO);
    printf("RESULT OF JOIN\n");
    fprintf(outputLog,"TOTAL IO'S: %d\n",IO);
    fprintf(outputLog,"RESULT OF JOIN\n");
    FILE *output = fopen("output.txt","w");
    for(int i=0,l=resultValues.size(); i<l; i++)
    {
        fprintf(output,"%d\n",resultValues[i]);
        fprintf(outputLog,"%d\n",resultValues[i]);
        fprintf(examOutputLog,"%d\n",resultValues[i]);
        cout<<resultValues[i]<<endl;
    }
    if(resultValues.size() == 0)
    {
        printf("No matching values found!\n");
        fprintf(outputLog,"No matching values found!\n");
    }
    if(!joinPossible)
    {
        printf("\nJoin could not be performed completely with the given constraints!\n");
        fprintf(outputLog,"\nJoin could not be performed completely with the given constraints!\n");
        fprintf(examOutputLog,"\nJoin could not be performed completely with the given constraints!\n");
    }
}

int main()
{
	string relation1,relation2;
	int recordSize1,recordSize2,pageSize,availablePages,hashingRounds;

	cout<<"Enter input parameters:\n";
	cout<<"Enter name of relations: ";
	cin>>relation1>>relation2;
	cout<<"Enter record size of relation1: "; cin>>recordSize1;
	cout<<"Enter record size of relation2: "; cin>>recordSize2;
	cout<<"Enter page size: "; cin>>pageSize;
	cout<<"Enter number of available pages: "; cin>>availablePages;
	cout<<"Enter number of hashing rounds: "; cin>>hashingRounds;

	if(availablePages <= 1)
    {
        printf("\nJoin not possible with just %d pages in memory!\n",availablePages);
        return 1;
    }

    outputLog = fopen("outputLog.txt","w");
    examOutputLog = fopen("examOutputLog.txt","w");
	performJoin(relation1,relation2,recordSize1,recordSize2,pageSize,availablePages,hashingRounds,1,-1,-1);
    printResults();
    fclose(outputLog);
    fclose(examOutputLog);

	return 0;
}
