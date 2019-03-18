#include<graphics.h>
#include "RBTFunctions.h"
#include<pthread.h>

void* threadInsertion1(void* parameters);
void* threadInsertion2(void* parameters);

void* threadSearch1(void* parameters);
void* threadSearch2(void* parameters);


int main()
{
int gd = DETECT,gm;
	initgraph(&gd,&gm,"C:\\TC\\BGI");
	//int wid = initwindow(640,640);
    //setbkcolor(5);
	pthread_t t1, t2, t3, t4;
	int retVal1 = pthread_create(&t1, NULL, threadInsertion1, NULL);
	//int retVal2 = pthread_create(&t2, NULL, threadInsertion2, NULL);

	if(retVal1 != 0)
	{
		cout<<"The creation of T1 has failed.."<<endl;
	}

	/*if(retVal2 != 0)
	{
		cout<<"The creation of T2 has failed.."<<endl;
	}*/

	pthread_join(t1, NULL);
//	pthread_join(t2, NULL);

	int retVal3 = pthread_create(&t3, NULL, threadSearch1, NULL);
	int retVal4 = pthread_create(&t4, NULL, threadSearch2, NULL);

	pthread_join(t3, NULL);
	pthread_join(t4, NULL);

	delay(5000);

    PrintTree(-9999);
    delay(5000);
	int del;
    cout<<"Enter the element to delete"<<endl;
    cin>>del;
    cleardevice();
    DeleteData(del,root);
    PrintTree(-9999);
    delay(5000);
    cleardevice();
    cout<<"Enter the element to delete"<<endl;
    cin>>del;
    cleardevice();
    DeleteData(del,root);

    PrintTree(-9999);
    delay(5000);
    cleardevice();

    int updateData, updateVal;
    PrintTree(-9999);
    delay(5000);
    cout<<"Enter the data to update"<<endl;
    cin>>updateData;
    cout<<"Enter update value"<<endl;
    cin>>updateVal;
    cleardevice();
    UpdateData(updateData,updateVal);

    PrintTree(-9999);
    delay(5000);
    cleardevice();

	closegraph();

   return 0;
}

void* threadInsertion1(void* parameters)
{
	int arr[10] = {10,20,30,40,50,60,70,80,90,100};
	for(int i = 0; i < 10; i++)
	{
		insertion(arr[i], 1);
		PrintTree(-9999);
	    delay(2500);
		cleardevice();
	}
	pthread_exit(NULL);
}

void* threadInsertion2(void* parameters)
{
	int arr[5] = {60,70,80,20,100};
	for(int i = 0; i < 5; i++)
	{
		insertion(arr[i], 2);
		PrintTree(-9999);
	    delay(2500);
		cleardevice();
	}
	pthread_exit(NULL);
}


void* threadSearch1(void* parameters)
{
	int arr[4] = {10,15,50,14};
	for(int i = 0; i < 4; i++)
	{
		cout<<"___________"<<endl;
		RBT* node = Search(arr[i], root);
		if(node == NULL)
		{
			cout<<"The node "<<arr[i]<<" is not found"<<endl;
		}
		else
		{
			cout<<"The node "<<arr[i]<<" is present in the given tree"<<endl;
			PrintTree(arr[i]);
			delay(3500);
		    cleardevice();
		}
	}
	pthread_exit(NULL);
}

void* threadSearch2(void* parameters)
{
	int arr[4] = {60,88,90,84};
	for(int i = 0; i < 4; i++)
	{
		RBT* node = Search(arr[i], root);
		cout<<"___________"<<endl;
		if(node == NULL)
		{
			cout<<"The node "<<arr[i]<<" is not found"<<endl;
		}
		else
		{
			cout<<"The node "<<arr[i]<<" is present in the given tree"<<endl;
			PrintTree(arr[i]);
			delay(3500);
		    cleardevice();
		}
	}
	pthread_exit(NULL);
}


