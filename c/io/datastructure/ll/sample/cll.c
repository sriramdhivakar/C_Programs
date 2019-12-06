#include <stdio.h>
#include <malloc.h>

struct node {
	struct node *prev;
	int info;
	struct node *next;
}*root;

void creat_node(int value)
{
	struct node *tmp;

	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->prev = NULL;
	tmp->info = value;
	tmp->next = NULL;

	if(root == NULL)
	{
		root = tmp;
		root->prev = root;
		root->next = root;
	}
}

void addatbegin(int value)
{
	struct node *tmp;

	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->prev = NULL;
	tmp->info = value;
	tmp->next = NULL;

	tmp->next = root;
	tmp->prev = root->prev;
	root->prev = tmp;
	root->next = tmp;
	root = tmp;
}

void addinbetween(int pos,int value)
{
	struct node *q,*tmp;

	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->next = NULL;
	tmp->info = value;
	tmp->prev = NULL;

	int i;

	q = root;

	for(i=0;i<pos-1;i++)
	{
		q = q->next;
	}
	
	q->next->prev = tmp;
	tmp->next = q->next;
	tmp->prev = q;
	q->next = tmp;
}

void addatend(int value)
{
	struct node *tmp,*q;

	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->prev = NULL;
	tmp->info = value;
	tmp->next = NULL;

	q = root;

	while(q->next != root)
	{
		q = q->next;
	}
	q->next = tmp;
	tmp->prev = q;
	tmp->next = root;
}

void delete(int value)
{
	struct node *q,*tmp;
	
	/*q = root;
	tmp = NULL;

	while(q->info != value)
	{
		tmp = q;
		q = q->next;
	}
	
	if(q == root)
	{
		tmp = root->prev;
		root = root->next;
		tmp->next = root;
		root->prev = tmp;
		free(q);
	}*/

/*	if(root == NULL)
	{
		printf("List is Empty\n");
	}
	else
	{
		tmp = root;

		while(tmp->next != root)
		{
			tmp = tmp->next;
		}
		tmp->next = root->next;
		root->next->prev = tmp;
		free(root);
		root = tmp->next;
	}
*/
	tmp = root;

	while(tmp->next != root)
	{
		tmp = tmp->next;
	}
	if(root->info == value)
	{
		root->next->prev = tmp;
		root = root->next;
		tmp->next = root;
		free(tmp);
		return;
	}
	//free(tmp);

	//printf("list is %d\n",root->info);
	
	q = root;

	/*while(q->next->info == value)
	{
		tmp = q->next;
		q->next = tmp->next;
		tmp->prev = q;
	}*/
	while(q->next->next != root)
	{
		if(q->next->info == value)
		{
			tmp = q->next;
			q->next = tmp->next;
			tmp->prev = q;
			return;
		}

		q = q->next;
	}


	//printf("list is %d\n",tmp->info);
	//printf("delete list is %d\n",q->info);
	
	if(q->next->info == value)
	{
		tmp = q->next;
		tmp->prev = NULL;
		q->next = tmp->next;
	}
		
}

void displayfrombegin()
{
	struct node *q;

	if(root == NULL)
		printf("List is Empty\n");

	q = root;

	while(q->next != root)
	{
		printf("List is %d\n",q->info);
		q = q->next;
	}
	printf("List is %d\n",q->info);
}

void displayfromend()
{
	struct node *q,*tmp;

	q = root;

	while(q->next != root)
	{
	/*	if(q->next == root)
		{
			tmp = q;
		}*/
		q = q->next;
	}

//	printf("Display from end is %d\n",q->info);

	tmp = q;
	//free(q);
	//printf("Display from end is %d\n",tmp->info);

	while(tmp->prev != q)
	{
	//	getchar();
		//printf("7\n");
		printf("List is %d\n",tmp->info);
		tmp = tmp->prev;
		if(tmp == root)
		{
			//free(tmp);
			break;
		}
	}
	//printf("8\n");
	//getchar();
	//printf("9\n");
	printf("List is %d\n",tmp->info);
	//getchar();
	return;
	//free(q);
	//free(tmp);	

	//	printf("10\n");
}

void search(int value)
{
	struct node *q;
	
	int count = 0;

	q = root;

	while(q->info != value)
	{
		q = q->next;
		count++;
	}
	printf("value %d is found in %d node\n",q->info,count);
}

void update(int value,int nvalue)
{
	struct node *q;

	int count = 0;

	q = root;

	while(q->info != value)
	{
		q = q->next;
		count++;
	}
	printf("value %d is found in %d node\n",q->info,count);
	q->info = nvalue;
	printf("value %d is changed as value %d in %d node\n",value,q->info,count);
}

int main(void)
{
	int a,b,c,d;

	while(1)
	{
		printf("Choose the process:\n");
		printf("1.create the node\n");
		printf("2.Add at begin\n");
		printf("3.Add at inbetween\n");
		printf("4.Add at end\n");
		printf("5.Delete the node\n");
		printf("6.Display from begin\n");
		printf("7.Display from end\n");
		printf("8.Search the node\n");
		printf("9.Update the node\n");
		printf("10.Clear the screen\n");
		printf("Enter the option: ");
		scanf("%d",&a);
		switch(a)
		{
			case 1:
				printf("Enter the value: ");
				scanf("%d",&a);
				creat_node(a);
				break;
			case 2:
				printf("Enter the value: ");
				scanf("%d",&a);
				addatbegin(a);
				break;
			case 3:
				printf("Enter the pos: ");
				scanf("%d",&a);
				printf("Enter the value: ");
				scanf("%d",&b);
				addinbetween(a,b);
				break;
			case 4:
				printf("Enter the value: ");
				scanf("%d",&a);
				addatend(a);
				break;
			case 5:
				printf("Enter the value: ");
				scanf("%d",&a);
				delete(a);
				break;
			case 6:
				displayfrombegin();
				break;
			case 7:
				displayfromend();
				break;
			case 8:
				printf("Enter the value: ");
				scanf("%d",&a);
				search(a);
				break;
			case 9:
				printf("Enter the value to be change: ");
				scanf("%d",&a);
				printf("Enter the new value to the node: ");
				scanf("%d",&b);
				update(a,b);
				break;
			case 10:
				system("clear");
				break;
			default:
				printf("Enrer the wrong option\n");
				break;
		}
	}
	return 0;
}
