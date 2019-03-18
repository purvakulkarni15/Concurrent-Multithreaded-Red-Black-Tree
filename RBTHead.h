#include<iostream>
#include<stdlib.h>
#include<graphics.h>
#include<pthread.h>
using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct RBT
{
	RBT* left;
	RBT* right;
	RBT* parent;
	int data;
	int colour;
	int x;
	int y;
}RBT;

RBT* root = NULL;
int black = 1, red = 0;

RBT* createNode(int data);
void insertion(int data, int t_id);
void UpdateTree(RBT* node);
RBT* Search(int data, RBT* myroot);
void doubleBlackFixation(RBT* node);
void DeleteData(int data, RBT* myroot);
void UpdateData(int data1, int data2);
void Black_LL(RBT* node, int opt);
void Black_LR(RBT* node, int opt);
void Black_RR(RBT* node, int opt);
void Black_RL(RBT* node, int opt);

int add = 50;
void drawCircles(int x, int y, int num, int c)
{
	int cnt = 0;
	int tmp = num;
	while(tmp != 0)
	{
		tmp = tmp/10;
		cnt++;
	}

	char numstr[cnt];
	cnt = 0;
	while(num != 0)
	{
		numstr[cnt] = num%10 + '0';
		num = num/10;
		cnt++;
	}
	char revnumstr[cnt+1];
	int i;
	for(int i = 0; i < cnt; i++)
	{
		revnumstr[i] = numstr[cnt-i-1];
	}
	revnumstr[cnt] = '\0';
    setcolor(15);
	line(x,y,x+50+add,y+50);
	line(x,y,x-50-add,y+50);
	if(c == red)
	{
        setcolor(14);
		circle(x, y, 15);
		setfillstyle(SOLID_FILL,4);
        floodfill(x,y,14);
    }
	else if(c == black)
	{
        setcolor(14);
		circle(x, y, 15);
        setfillstyle(SOLID_FILL,1);
        floodfill(x,y,14);
    }
    else
    {
        setcolor(6);
		circle(x, y, 15);
        setfillstyle(SOLID_FILL,6);
        floodfill(x,y,6);
    }

    setcolor(15);
	outtextxy(x-10, y-5, revnumstr);
}


typedef struct Queue
{
	RBT* newnode;
	Queue* next;
	int x;
	int y;
}queue;

queue* front = NULL;
queue* rear = NULL;

int counter = 0;
void enqueue(RBT* Node)
{
	//pthread_mutex_lock(&mutex);
	queue* newnode = (queue*)malloc(sizeof(queue));
	newnode->newnode = Node;
	newnode->next = NULL;
	counter++;

	if(front == NULL)
	{
		front = newnode;
		rear = front;
	}
	else
	{
		queue* temp = front;
		while(temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = newnode;
		rear = newnode;
	}
    //pthread_mutex_unlock(&mutex);
}

RBT* dequeue()
{
	//pthread_mutex_lock(&mutex);
	queue* temp = front;
	front = front->next;
	RBT* returnN = temp->newnode;
	free(temp);
	counter--;
	temp = NULL;
	return(returnN);
    //pthread_mutex_unlock(&mutex);
}

int IsEmpty()
{
    //pthread_mutex_lock(&mutex);
	if(front == NULL)
	{
		return 1;
	}
	else return 0;
    //pthread_mutex_unlock(&mutex);
}



void PrintTree(int ele)
{
    pthread_mutex_lock(&mutex);
	enqueue(root);
	RBT* node;
	int x = getmaxx() / 2;
    int y = 50;
	root->x = x;
	root->y = 50;
	add = 75;
	while(!IsEmpty())
	{
		int cnt = counter;
		while(cnt > 0)
		{
			node = dequeue();
			if(ele == node->data)
			{
                drawCircles(node->x,node->y,node->data, 5);
            }
			else
			{
               drawCircles(node->x,node->y,node->data, node->colour);
            }

			if(node->left != NULL)
			{
				node->left->x = node->x-50-add;
				node->left->y = node->y+50;
				enqueue(node->left);
			}
			if(node->right != NULL)
			{
				node->right->x = node->x+50+add;
				node->right->y = node->y+50;
				enqueue(node->right);
			}
			cnt--;
		}
		add = add-30;
	}
    pthread_mutex_unlock(&mutex);
}


void doubleBlackFixation(RBT* node)
{
	cout<<"			Executing Fixation for double black"<<endl;
	if(node == root)
	{
		root->colour = black;
		return;
	}
	RBT* sibling;
	int sib_child;
	if(node->parent->right == node)
	{
		sib_child = 1;
		sibling = node->parent->left;
	}
	else
	{
		sib_child = 0;
		sibling = node->parent->right;
	}
	int sib_colour;
	if(sibling != NULL)
		sib_colour = sibling->colour;

	if(sib_colour == black)
	{
		cout<<"Colour of sibling is black"<<endl;
		if(sibling->right != NULL && sibling->right->colour == red && sib_child == 0)
		{
			cout<<"Here"<<endl;
			Black_RR(sibling->right,0);
			cout<<"RR Rotation has been performed"<<endl;
			return;
		}
		else if(sibling->left != NULL && sibling->left->colour == red && sib_child == 0)
		{
			Black_RL(sibling->left,0);
			cout<<"RL Rotation has been performed"<<endl;
			return;
		}
		else if(sibling->right != NULL && sibling->right->colour == red && sib_child == 1)
		{
			Black_LR(sibling->right,0);
			cout<<"LL Rotation has been performed"<<endl;
			return;
		}
		else if(sibling->left != NULL && sibling->left->colour == red && sib_child == 1)
		{
			Black_LL(sibling->left,0);
			cout<<"LR Rotation has been performed"<<endl;
			return;
		}
		else
		{
			cout<<"None of the children are red"<<endl;

			if(node->parent->colour == red)
			{
				cout<<"Colour of parent is red"<<endl;
				node->parent->colour = black;
				sibling->colour = red;
				return;
			}
			else
			{
				sibling->colour = red;
				doubleBlackFixation(node->parent);
			}
		}
	}
	else
	{
		cout<<"Colour of sibling is red"<<endl;
		sibling->colour = black;
		node->parent->colour = red;
		if(node->parent->parent->left == node->parent)
			node->parent->parent->left = sibling;
		else
			node->parent->parent->right = sibling;
		if(node->parent->right == sibling)
		{
			RBT* temp = sibling->left;
			sibling->left = node->parent;
			sibling->parent = sibling->parent->parent;
			sibling->left->parent = sibling;

			sibling->left->right = temp;
			if(temp != NULL)
			{
				temp->parent = sibling->left;
			}
		}
		else
		{
			RBT* temp = sibling->right;
			sibling->right = sibling->parent;
			sibling->parent = sibling->parent->parent;
			sibling->right->parent = sibling;

			sibling->right->left = temp;
			if(temp != NULL)
			{
				temp->parent = sibling->right;
			}
		}
		doubleBlackFixation(node);

	}

}

void DeleteData(int data, RBT* myroot)
{
	RBT* delNode = Search(data, myroot);
	RBT* parent = delNode->parent;


	cout<<"Deleting "<<delNode->data<<endl;

	int colour_v = delNode->colour;
	int colour_u;

	if(delNode->left == NULL && delNode->right == NULL)
	{
		colour_u = black;
		delNode->data = -9999;

		if(colour_v != red)
		{
			cout<<"Encountered double black"<<endl;
			doubleBlackFixation(delNode);
		}
		else
			cout<<"Implementing case 1: Either of u or v are red"<<endl;
		if(parent->left == delNode)
		{
			parent->left = NULL;
		}
		else if(parent->right == delNode)
		{
			parent->right = NULL;
		}

		free(delNode);
	}
	else if(delNode->left == NULL || delNode->right == NULL)
	{
		int child_pos;
		if(delNode->right == NULL)
		{
			delNode->data = delNode->left->data;
			delNode->colour = delNode->left->colour;
			free(delNode->left);
			delNode->left = NULL;
			colour_u = delNode->colour;
			child_pos = 10;
		}
		else if(delNode->left == NULL)
		{
			delNode->data = delNode->right->data;
			delNode->colour = delNode->right->colour;
			free(delNode->right);
			delNode->right = NULL;
			colour_u = delNode->colour;
			child_pos = 0;
		}
		if(colour_u == red || colour_v == red)
		{
			delNode->colour = black;
		}
		else
		{
			cout<<"Encountered double black"<<endl;
			doubleBlackFixation(delNode);
		}
	}
	else if(delNode->left != NULL && delNode->right != NULL)
	{

		RBT* temp = delNode->right;

		while(temp->left != NULL)
		{
			temp = temp->left;
		}
		delNode->data = temp->data;

		cout<<"Recursive call for deletion of "<<temp->data<<endl;
		DeleteData(temp->data, delNode->right);
	}
}

RBT* Search(int data, RBT* myroot)
{
	RBT* temp = myroot;
	while(temp != NULL)
	{
		if(data > temp->data)
		{
			temp = temp->right;
		}
		else if(data < temp->data)
		{
			temp = temp->left;
		}
		else if(data == temp->data)
		{
			return temp;
		}
	}
	return temp;
}

RBT* createNode(int data)
{
	RBT* newnode = (RBT*)malloc(sizeof(RBT));
	newnode->data = data;
	newnode->parent = NULL;
	newnode->left = NULL;
	newnode->right = NULL;
	newnode->colour = red;
	return newnode;
}

void insertion(int data, int t_id)
{
	RBT* newnode = createNode(data);
	pthread_mutex_lock(&mutex);
	if(root == NULL)
	{
		root = newnode;
		root->colour = black;
		cout<<"The given data "<<data<<" is inserted as root"<<endl;
		pthread_mutex_unlock(&mutex);
	}
	else
	{
		pthread_mutex_unlock(&mutex);

		RBT* temp = root;
		while(temp != NULL)
		{
			if(temp->data > data)
			{
				pthread_mutex_lock(&mutex);
				if(temp->data > data)
				{
					if(temp->left != NULL)
					{
						temp = temp->left;
					}
					else
					{
						temp->left = newnode;
						newnode->parent = temp;
						cout<<"The node "<<newnode->data<<" is inserted to the left of "<<temp->data<<endl;
						pthread_mutex_unlock(&mutex);
						break;
					}
				}
				pthread_mutex_unlock(&mutex);
			}

			else if(temp->data < data)
			{
				pthread_mutex_lock(&mutex);
				if(temp->data < data)
				{
					if(temp->right != NULL)
					{
						temp = temp->right;
					}
					else
					{
						temp->right = newnode;
						newnode->parent = temp;
						cout<<"The node "<<newnode->data<<" is inserted to the right of "<<temp->data<<endl;
						pthread_mutex_unlock(&mutex);
						break;
					}
				}
				pthread_mutex_unlock(&mutex);
			}
		}
	}
	pthread_mutex_lock(&mutex);
	UpdateTree(newnode);
	//PrintTree();
	pthread_mutex_unlock(&mutex);
}

void UpdateTree(RBT* node)
{
	if(node == root || node == NULL)
		return;
	else if(node->colour == red && node->parent->colour == red)
	{
		if(node->parent->parent->left == node->parent)
		{
			if(node->parent->parent->right == NULL || node->parent->parent->right->colour == black)
			{
				//Uncle is at right and is black and parent is at left;
				if(node->parent->left == node)
				{
					Black_LL(node,1);
					cout<<"LL Rotation is done on "<<node->data<<endl;
					UpdateTree(node->parent);
				}
				else if(node->parent->right == node)
				{
					Black_LR(node,1);
					cout<<"LR Rotation is done on "<<node->data<<endl;
					UpdateTree(node->parent);
				}
			}
			else
			{
				//Uncle is at right and is red Func;
				if(node->parent->parent != root)
				{
					node->parent->parent->colour = red;
				}
				node->parent->colour = black;
				node->parent->parent->right->colour = black;

				UpdateTree(node->parent->parent);
			}
		}
		else
		{
			if(node->parent->parent->left == NULL || node->parent->parent->left->colour == black)
			{
				//Uncle is at left and is black and parent is at right;
				if(node->parent->right == node)
				{
					Black_RR(node,1);
					cout<<"RR Rotation is done on "<<node->data<<endl;
					UpdateTree(node->parent);

				}
				else if(node->parent->left == node)
				{
					Black_RL(node,1);
					cout<<"RL Rotation is done on "<<node->data<<endl;
					UpdateTree(node->parent);
				}
			}
			else
			{
				//Uncle is at the left and is red Func;
				if(node->parent->parent != root)
				{
					node->parent->parent->colour = red;
				}
				node->parent->colour = black;
				node->parent->parent->left->colour = black;
				UpdateTree(node->parent->parent);
			}
		}
	}
}

void UpdateData(int data1, int data2)
{
    RBT* node = Search(data1, root);
    if(node != NULL)
    {
        DeleteData(data1, root);
        insertion(data2,1);
    }
}

void Black_LL(RBT* node, int opt)
{
	RBT* parent = node->parent;
	RBT* grandparent = parent->parent;

	//Swap colours of grandparent and parent;
	if(opt == 1)
	{
		int colour= grandparent->colour;
		grandparent->colour = parent->colour;
		parent->colour = colour;
	}
	else
	{
		int colour= grandparent->colour;
		grandparent->colour = parent->colour;
		parent->colour = colour;
		node->colour = black;
	}

	RBT* temp = node->parent->right;
	parent->right = grandparent;

	grandparent->left = temp;
	if(temp != NULL)
		temp->parent = grandparent;

	if(grandparent != root)
	{

		if(grandparent->parent->left == grandparent)
		{
			parent->right = grandparent;
			parent->parent = grandparent->parent;
			grandparent->parent->left = parent;
			grandparent->parent = parent;
		}
		else if(grandparent->parent->right == grandparent)
		{
			parent->right = grandparent;
			parent->parent = grandparent->parent;
			grandparent->parent->right = parent;
			grandparent->parent = parent;
		}
	}
	else
	{
		root =  parent;
		grandparent->parent = parent;
		root->parent = NULL;
	}
}


void Black_RR(RBT* node, int opt)
{
	RBT* parent = node->parent;
	RBT* grandparent = parent->parent;

	//Swap colours of grandparent and parent;
	if(opt == 1)
	{
		int colour= grandparent->colour;
		grandparent->colour = parent->colour;
		parent->colour = colour;
	}
	else
	{
		int colour= grandparent->colour;
		grandparent->colour = parent->colour;
		parent->colour = colour;
		node->colour = black;
	}

	RBT* temp = node->parent->left;
	parent->left = grandparent;

	grandparent->right = temp;
	if(temp != NULL)
		temp->parent = grandparent;

	if(grandparent != root)
	{

		if(grandparent->parent->left == grandparent)
		{
			parent->left = grandparent;
			parent->parent = grandparent->parent;
			grandparent->parent->left = parent;
			grandparent->parent = parent;
		}
		else if(grandparent->parent->right == grandparent)
		{
			parent->left = grandparent;
			parent->parent = grandparent->parent;
			grandparent->parent->right = parent;
			grandparent->parent = parent;
		}
	}
	else
	{
		root = parent;
		grandparent->parent = parent;
		root->parent = NULL;
	}
}


void Black_LR(RBT* node, int opt)
{
	RBT* parent = node->parent;
	RBT* grandparent = parent->parent;

	//Left rotation about parent
	RBT* temp = node->left;

	node->left = parent;
	parent->parent = node;

	parent->right = temp;
	if(temp != NULL)
		temp->parent = parent;

	grandparent->left= node;
	node->parent = grandparent;

	//Swap colours of grandparent and parent;
	if(opt == 1)
	{
		int colour= grandparent->colour;
		grandparent->colour = node->colour;
		node->colour = colour;
	}
	else
	{
		node->colour = black;
		int colour= grandparent->colour;
		grandparent->colour = node->colour;
		node->colour = colour;
	}
	//Right rotation about grandparent
	RBT* temp2 = node->right;
	grandparent->left = temp2;
	if(temp2 != NULL)
		temp2->parent = grandparent;

	if(grandparent != root)
	{

		if(grandparent->parent->left == grandparent)
		{
			node->right = grandparent;
			node->parent = grandparent->parent;
			grandparent->parent->left = node;
			grandparent->parent = node;
		}
		else if(grandparent->parent->right == grandparent)
		{
			node->right = grandparent;
			node->parent = grandparent->parent;
			grandparent->parent->right =node;
			grandparent->parent = node;
		}
	}
	else
	{
		root =  node;
		grandparent->parent = node;
		root->parent = NULL;
	}
}

void Black_RL(RBT* node, int opt)
{
	RBT* parent = node->parent;
	RBT* grandparent = parent->parent;

	//Right rotation about parent
	RBT* temp = node->right;
	node->right = parent;
	parent->parent = node;

	parent->left = temp;
	if(temp != NULL)
		temp->parent = parent;

	grandparent->right = node;
	node->parent = grandparent;

	//Swap colours of grandparent and parent;
	if(opt == 1)
	{
		int colour= grandparent->colour;
		grandparent->colour = node->colour;
		node->colour = colour;
	}
	else
	{
		node->colour = black;
		int colour= grandparent->colour;
		grandparent->colour = node->colour;
		node->colour = colour;
		parent->colour = black;
	}

	//Left rotation about grandparent
	RBT* temp2 = node->left;
	grandparent->right = temp2;
	if(temp2 != NULL)
		temp2->parent = grandparent;

	if(grandparent != root)
	{

		if(grandparent->parent->left == grandparent)
		{
			node->left = grandparent;
			node->parent = grandparent->parent;
			grandparent->parent->left = node;
			grandparent->parent = node;
		}
		else if(grandparent->parent->right == grandparent)
		{
			node->left = grandparent;
			node->parent = grandparent->parent;
			grandparent->parent->right =node;
			grandparent->parent = node;
		}
	}
	else
	{
		root =  parent;
		grandparent->parent = node;
		root->parent = NULL;
	}

}
