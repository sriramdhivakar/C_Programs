#include <stdio.h>
#include <malloc.h>

struct node {
	int info;
	struct node *link;
}*root;

void creat_list(int value){
	struct node *tmp,*q;

	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->info = value;
	tmp->link = NULL;

	if(root == NULL)
	{
		root = tmp;
	}
	else
	{
		q = root;//node add at end
		while(q->link != 0)
		{
			q = q->link;
		}
		q->link = tmp;
	}
}

void addatbegin(int value)
{
	struct node *tmp;
	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->info = value;
	tmp->link = root;
	root = tmp;
}

void addafter(int value,int pos)
{
	struct node *tmp,*q;

	int i;

	q = root;

	for(i=0;i<pos-1;i++)
	{
		q = q->link;
		if(q == NULL)
		{
			printf("There are less than %d elements",pos);
			return;
		}
	}

	tmp = (struct node*)malloc(sizeof(struct node));
	tmp->link = q->link;
	tmp->info = value;

	q->link = tmp;
}

void delete(int value)
{
	struct node *tmp,*q;

	if(root->info == value)
	{
		tmp = root;
		root = root->link;//First element deleted
		free(tmp);
		return;
	}

	q = root;

	while(q->link->link != NULL)
	{
		if(q->link->info == value)//Element deleted in between
		{
			tmp = q->link;
			q->link = tmp->link;
			free(tmp);
			return ;
		}

		q = q->link;
	}

	if(q->link->info == value)//Last Element is deleted
	{
		tmp = q->link;
		free(tmp);
		q->link = NULL;
		return;
	}

	printf("Element %d is not found\n",value);
}

void search(int value)
{
	struct node *q;

	q = root;

	int pos = 1;

	while(q != NULL)
	{
		if(q->info == value)
		{
			printf("Item %d found at position %d\n",value,pos);
			return;
		}

		q = q->link;
		pos++;
	}

	if(q == NULL)
	{
		printf("Items %d not found in list\n",value);
	}
}

void display()
{
	struct node *q;

	if(root == NULL)
	{
		printf("List is not Empty\n");
		return;
	}

	q = root;
	while( q != NULL)
	{
		printf("List is ");
		printf("%d\n",q->info);
		q = q->link;
	}
}

int main(void){
	int i,n,m,o;

	printf("Enter the number of Elements: ");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		printf("Enter the value: ");
		scanf("%d",&m);
		creat_list(m);
	}
	display();
//	printf("Enter the value: ");
//	scanf("%d",&m);
//	addatbegin(m);
//	display();
//	printf("Enter the value: ");
//	scanf("%d",&m);
//	printf("Enter the pos: ");
//	scanf("%d",&n);
//	addafter(m,n);
//	display();
	printf("Enter the pos: ");
	scanf("%d",&m);
	delete(m);
	display();
//	printf("Enter the pos: ");
//	scanf("%d",&m);
//	search(m);
//	display();
	return 0;
}
