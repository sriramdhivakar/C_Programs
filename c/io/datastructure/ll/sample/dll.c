#include <stdio.h>
#include <malloc.h>

struct node {
	struct node *prev;
	int info;
	struct node *next;
}*root;

void creat_nod(int value)
{
	struct node *tmp;

	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->prev = NULL;
	tmp->info = value;
	tmp->next = NULL;

	if(root == NULL)
		root = tmp;
}

void addatbegin(int value)
{
	struct node *tmp;

	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->prev = NULL;
	tmp->info = value;
	tmp->next = NULL;

	root->prev = tmp->prev;
	tmp->next = root;

	root = tmp;
}

void addinbetween(int pos,int value)
{
	struct node *tmp,*q;
	
	int i;

	q = root;

	for(i=0;i<pos-1;i++)
	{
		q = q->next;
	}

	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->prev = NULL;/*q;*//*q->prev;*/
	tmp->info = value;
	tmp->next = NULL;/*q->next;*/

	tmp->next = q->next;
	tmp->prev = q;

	tmp->next->prev = tmp;
	q->next = tmp;

}

void addatend(int value)
{
	struct node *tmp,*q;

	q = root;

	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->prev = NULL;
	tmp->info = value;
	tmp->next = NULL;

	while(q->next != NULL)
	{
		q = q->next;
	}

	q->next = tmp;
	tmp->prev = q;
}

void delete(int value)
{
	struct node *q,*tmp;

	/*Delete at begin*/
	if(root->info == value)
	{
		root = root->next;
		root->prev = NULL;
		return;
	}

	q = root;

	/*Delete at inbetween*/
	while(q->next->next != NULL)
	{
		if(q->next->info == value)
		{
			tmp = q->next;
			q->next = tmp->next;
			tmp->prev = q->prev;
		/*	free(tmp);*/
			return;
		}
		q = q->next;
	}

	/*Delete at end*/
	if(q->next->info == value)
	{
		q->next = NULL;
	}
}

void search(int value)
{
	struct node *q;
	
	int pos = 1;

	q = root;

	while(q->info != value)
	{
		q = q->next;
		pos++;
	}

	printf("Entered value %d is in position %d\n",value,pos);
}

void displayfrombegin()
{
	struct node *q;
      
	int count = 0;

	if(root == NULL)
	{
		printf("List is Empty\n");
		return;
	}

	q = root;

	while(q != NULL)
	{
		printf("List is %d\n",q->info);
		q = q->next;
		count++;
	}

	printf("Total number of node present in the list is %d\n",count);
}

void displayfromend()
{
	struct node *q,*tmp;

	int count = 0;

	if(root == NULL)
	{
		printf("List is Empty\n");
		return;
	}

	q = root;

	while(q != NULL)
	{
		if(q->next == NULL)
		{
			tmp = q;
		}
		q = q->next;
	}

	while(tmp != NULL)
	{
		printf("List is %d\n",tmp->info);
		tmp = tmp->prev;
		count++;
	}

	printf("Total number of node present in the list is %d\n",count);
}

void update(int value,int nvalue)
{
	struct node *q;

	int x;

	q = root;

	while(q->info != value)
	{
		q = q->next;
	}

	printf("Previous value is %d\n",q->info);

	q->info = nvalue;

	/*q = root;
	
	while(q->info != value)
	{
		q = q->next;
	}*/

	printf("New value is %d\n",q->info);
	//return;
}


int main(void)
{
	int a,b,c,d;
	while(1)
	{
		printf("\n");
		printf("Select the process to perform\n");
		printf("\n");
		printf("1.Create a node\n");
		printf("2.Add at begin\n");
		printf("3.Add at inbetween\n");
		printf("4.Add at end\n");
		printf("5.Delete the node\n");
		printf("6.Search the node\n");
		printf("7.Display from begin\n");
		printf("8.Display from end\n");
		printf("9.Update the node\n");
		printf("\n");
		printf("Choose the option: ");
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
				addatbegin(b);
				break;
			case 3:
				printf("Enter the position ");
				scanf("%d",&c);
				printf("Enter the value ");
				scanf("%d",&d);
				addinbetween(c,d);
				break;
			case 4:
				printf("Enter the value ");
				scanf("%d",&b);
				addatend(b);
				break;
			case 5:
				printf("Enter the value ");
				scanf("%d",&b);
				delete(b);
				break;
			case 6:
				printf("Enter the value ");
				scanf("%d",&b);
				search(b);
				break;
			case 7:
				displayfrombegin();
				break;
			case 8:
				displayfromend();
				break;
			case 9:
				printf("Enter the value to change ");
				scanf("%d",&b);
				printf("Enter the value to update ");
				scanf("%d",&c);
				update(b,c);
				break;
			default:
				printf("wrong selection\n");
				break;
		}
	}
	return 0;
}
