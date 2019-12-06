#include <stdio.h>
#include <malloc.h>

struct node {
	int info;
	struct node *next;
}*root;

void creat_nod(int data)
{
	struct node *tmp,*q;

	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->info = data;
	tmp->next = NULL;

	if(root == NULL)
	{
		root = tmp;
	}
}

void addatbegin(int data)
{
	struct node *tmp;
	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->info = data;
	tmp->next = root;

	root = tmp;
}

void addinbetween(int pos,int data)
{
	struct node *tmp,*q;
	int i;

	q = root;

	for(i=0;i<pos;i++)
	{
		q = q->next;
	}
	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->info = data;
	tmp->next = q->next;

	q->next = tmp;
}
void addatend(int data)
{
	struct node *tmp,*q;

	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->info = data;
	tmp->next = NULL;

	q = root;
	while(q->next != NULL)
	{
		q = q->next;
	}
	q->next = tmp;
}

void delete(int data)
{
	struct node *q,*tmp;

	if(root->info == data)
	{
		tmp = root;//root = root->next;
		root = root->next;
		free(tmp);
		return;
	}
	
	q = root;

	while(q->next->next != NULL)
	{
		if(q->next->info == data)
		{
			tmp = q->next;//q->next = q->next->next;
			q->next = tmp->next;
			free(tmp);
			return;
		}
		q = q->next;
	}
	
	if(q->next->info == data)
	{
		//q->next = NULL;
		tmp = q->next;
		free(tmp);
		q->next = NULL;
		return;
	}	
}

void display()
{
	struct node *q;

	q = root;
	while(q != NULL)
	{
		printf("List is %d\n",q->info);
		q = q->next;
	}
}

void search(int data)
{
	struct node *q;

	q = root;
	int pos = 1;

	while(q != NULL)
	{
		if(q->info == data)
		{
			printf("The searching data is %d and position is %d\n",data,pos);

		}
		q = q->next;
		pos++;
	}
}

int main(void)
{
	int a,b,c,d,e,f;
	while(1){
		printf("Select the options:\n");
		printf("1.Create node\n");
		printf("2.Add node at end\n");
		printf("3.Add node at begin\n");
		printf("4.Add node at inbetween\n");
		printf("5.Delete node\n");
		printf("6.Display all the nodes\n");
		printf("7.Search the node\n");
		printf("Enter your choice ");
		scanf("%d",&a);
		switch(a)
		{
			case 1:
				printf("Enter the value ");
				scanf("%d",&b);
				creat_nod(b);
				break;
			case 2:
				printf("Enter the value ");
				scanf("%d",&b);
				addatend(b);
				break;
			case 3:
				printf("Enter the value ");
				scanf("%d",&b);
				addatbegin(b);
				break;
			case 4:
				printf("Enter the position ");
				scanf("%d",&b);
				printf("Enter the value ");
				scanf("%d",&c);
				addinbetween(b,c);
				break;
			case 5:
				printf("Enter the value ");
				scanf("%d",&b);
				delete(b);
				break;
			case 6:
				display();
				break;
			case 7:
				printf("Enter the value ");
				scanf("%d",&b);
				search(b);
				break;
		}
	}
	return 0;
}
