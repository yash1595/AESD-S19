#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/list.h>
#include<linux/slab.h>
#include <linux/timer.h>
#define SIZE 	50																//Defines the Maximum array size.

struct Node 																	//Structure for Linked List node.
{
	char NodeName[20];
	int Count;
	struct list_head list; 
};


int size_count_s1 = 0,size_count_s2 = 0;										//Variables for calculationg freed space and number 
int number_of_nodes_s1=0,number_of_nodes_s2=0;									// of nodes.

struct Node NodeList_Set1;
struct Node NodeList_Set2;

char Node_Arr[50][50];
char Node_Old[50][50]={	"Zebra", "Dog", "Fish", "Pig", "Bull", "Cat", "Rat","Lion","Tiger","Sheep",
						"Dragon", "Snake", "Turtle", "Croc", "Hippo", "Puma", "Cheetah","Leopard","Panther","Lizard",
						"Zebra", "Dog", "Fish", "Pig", "Bull", "Cat", "Rat","Lion","Tiger","Sheep",
						"Dragon", "Snake", "Turtle", "Croc", "Hippo", "Puma", "Cheetah","Leopard","Panther","Lizard",
						"Mice", "Ant", "Bee", "Pig", "Bull", "Cat", "Rat","Lion","Tiger","Sheep"};

char MinimumString[10];
char Temp[20];
 
/*******************************************************************************
	@operation:Frees the nodes used by Set2
*******************************************************************************/
void SizeCount(uint8_t size)													
{
	printk("Size occupied by Set2: %d\n",size_count_s2);
	struct Node *TempNode;
	list_for_each_entry(TempNode, &NodeList_Set2.list, list)
	{	
		kfree(TempNode);
	}
	printk("Size freed by Set2: %d\n",size_count_s2);
}

/*******************************************************************************
	@operation:	Sorts the defined array.
	@Ref:	refered from Geeks for Geeks (changed variable names)
*******************************************************************************/
void SortedArray(char Node_Old[][50], int n)
{

	int i,j;
	
	for (i = 0; i < SIZE; i++)
	{
		int minimum_index = i;
		strcpy(MinimumString, Node_Old[i]);
		for (j = i+1; j < SIZE; j++)
		{
			if (strcmp(MinimumString, Node_Old[j]) > 0)
			{
				strcpy(MinimumString, Node_Old[j]);
				minimum_index = j;
			}
		}
		if (minimum_index != i)
		{
			
			strcpy(Temp,Node_Old[i]);
			strcpy(Node_Old[i], Node_Old[minimum_index]);
			strcpy(Node_Old[minimum_index], Temp);
		}
	}
	for(i=0;i<SIZE;++i)
	{
		strcpy(Node_Arr[i],Node_Old[i]);
	}
	
}

/*******************************************************************************
	@operation:	Searches for the nodes with count >= defined value
*******************************************************************************/
void SearchCount(uint8_t search_count)
{
	printk("Applying filter for count\n");
	printk("All nodes with count :%d\n",search_count);
	struct Node *TempNode;
	list_for_each_entry(TempNode, &NodeList_Set2.list, list)
		{	
			if(search_count <= TempNode->Count)
			{
				
				printk(KERN_INFO "Animal name: %s\n", TempNode->NodeName);
			}
		}
		printk("_______________________________________________________");
}

/*******************************************************************************
	@operation:	Searches for the nodes with string same as defined in calling
				function.
*******************************************************************************/
void SearchName(char* search_name)
{
	printk("Applying filter for name\n");
	printk("Nodes with string : %s\n",search_name);
	struct Node *TempNode;
	list_for_each_entry(TempNode, &NodeList_Set2.list, list)
		{	
			if(strcmp(TempNode->NodeName,search_name)==0)
			{
				printk(KERN_INFO "Animal name: %s\n==>Animal Count:%d", TempNode->NodeName,TempNode->Count);
			}
			
		}
		printk("_______________________________________________________");
}

/*******************************************************************************
	@operation:	Searches for the nodes with count >= defined value and string
				matching when the function is search_name.
*******************************************************************************/
void SearchNameAndCount(char* search_name, uint8_t search_count)				
{
	printk("Applying filter for count and name\n");
	printk("Nodes with string : %s and Count:%d\n",search_name,search_count);
	struct Node *TempNode;
	list_for_each_entry(TempNode, &NodeList_Set2.list, list)
		{	
			if(strcmp(TempNode->NodeName,search_name)==0 && (search_count <= TempNode->Count))
			{
				printk(KERN_INFO "Animal name: %s\n==>Animal Count:%d", TempNode->NodeName,TempNode->Count);
			}
			
		}
		printk("_______________________________________________________");
}

/*******************************************************************************
	@operation: Int main for Kernel
********************************************************************************/

static int __init INIT_NodeModule(void)
{
	struct timespec begin;
	struct timespec end;

	getnstimeofday (&begin);

	SortedArray(Node_Old,SIZE);

	int flag_animal = 0; 
	
	struct Node *NewAnimal_Set1, *TempNode_Set1;

	INIT_LIST_HEAD(&NodeList_Set1.list);

	int i=0;
	for(i=0; i < SIZE; ++i)
	{
		list_for_each_entry(TempNode_Set1, &NodeList_Set1.list, list)
		{	
			if( strcmp(TempNode_Set1->NodeName, Node_Arr[i]) == 0 )
			{
				TempNode_Set1->Count++;
				flag_animal = 1; 
				break;
			}
		}
		
		if(!flag_animal)
		{
			NewAnimal_Set1 = kmalloc(sizeof(*NewAnimal_Set1), GFP_KERNEL);
			size_count_s1+=sizeof(*NewAnimal_Set1);
			number_of_nodes_s1+=1;

			strcpy(NewAnimal_Set1->NodeName, Node_Arr[i]);

			NewAnimal_Set1->Count = 1;

			INIT_LIST_HEAD(&NewAnimal_Set1->list);

			list_add_tail(&(NewAnimal_Set1->list), &(NodeList_Set1.list));
		}
		flag_animal = 0; 
	}
/*******************************************************************************
	@operation: Output for Set-1 ==> 1.List 2.Node count 3.Size occupied byt the 
				nodes
********************************************************************************/
		printk("List of nodes with count for Set-1\n");
		list_for_each_entry(TempNode_Set1, &NodeList_Set1.list, list)
		{		
			printk(KERN_INFO "%s--%d\n", TempNode_Set1->NodeName,TempNode_Set1->Count);
		}
		printk("Number of nodes in Set-1:%d",number_of_nodes_s1);

		printk("Size occupied by Set1: %d\n",size_count_s1);
		list_for_each_entry(TempNode_Set1, &NodeList_Set1.list, list)
		{	
			kfree(TempNode_Set1);
		}
		printk("Size freed by Set1: %d\n",size_count_s1);

/*******************************************************************************
	@operation: Set-2 assigning nodes
********************************************************************************/
	
	struct Node *NewAnimal_Set2, *TempNode_Set2;
	INIT_LIST_HEAD(&NodeList_Set2.list);

	for(i=0; i < SIZE; ++i)
	{
		list_for_each_entry(TempNode_Set2, &NodeList_Set2.list, list)
		{	
			if( strcmp(TempNode_Set2->NodeName, Node_Arr[i]) == 0 )
			{
				TempNode_Set2->Count++;
				flag_animal = 1; 
				break;
			}
		}
		
		if(!flag_animal)
		{
			NewAnimal_Set2 = kmalloc(sizeof(*NewAnimal_Set2), GFP_KERNEL);
			size_count_s2+=sizeof(*NewAnimal_Set2);
			number_of_nodes_s2+=1;
			
			strcpy(NewAnimal_Set2->NodeName, Node_Arr[i]);

			NewAnimal_Set2->Count = 1;

			INIT_LIST_HEAD(&NewAnimal_Set2->list);

			list_add_tail(&(NewAnimal_Set2->list), &(NodeList_Set2.list));
		}
		flag_animal = 0; 
	}

	
/*******************************************************************************
	@operation: Output for Set-2
********************************************************************************/
	printk("List of nodes for Set-2 without any filter\n");
	list_for_each_entry(TempNode_Set2, &NodeList_Set2.list, list)
		{	
	
			printk(KERN_INFO "%s--%d\n", TempNode_Set2->NodeName,TempNode_Set2->Count);
		}
	SearchCount(2);
	SearchName("Dog");
	SearchNameAndCount("Cat",2);
	printk(KERN_ALERT "Traversing the list using list_for_each_entry()\n");
	list_for_each_entry(TempNode_Set2, &NodeList_Set2.list, list)
	{
		printk(KERN_INFO "Animal name: %s - count: %d\n", TempNode_Set2->NodeName, TempNode_Set2->Count);
	}		

	getnstimeofday (&end);
	struct timespec diff;	
	diff = timespec_sub(end, begin); 
	printk(KERN_ALERT "Time elapsed: %luns\n", diff.tv_nsec );
	SizeCount(1);

	return 0; 

}

static void __exit EXIT_NodeModule(void)
{
	
	struct timespec begin,end;

	getnstimeofday (&begin);

	printk(KERN_ALERT "Uninstalling..\n");
	getnstimeofday (&end);
	struct timespec diff;
	diff = timespec_sub(end, begin); 
	printk(KERN_ALERT "Time elapsed: %luns\n", diff.tv_nsec );

}

module_init(INIT_NodeModule);
module_exit(EXIT_NodeModule);
MODULE_DESCRIPTION("kernel funtions with Set1 and Set2");
MODULE_AUTHOR("Yash Gupte");
MODULE_LICENSE("GPL");