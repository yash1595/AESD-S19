#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>		
#include <linux/list.h>
#include<linux/slab.h>
#include <linux/timer.h>
#define SIZE 	50																//Defines the Maximum array size.

/******************************************************************************
	@operation:	Data Structure used is Linked List
*******************************************************************************/

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
char* search_name="";
int search_count;
int filter;

module_param(filter, int , S_IRUSR | S_IWUSR);
module_param(search_name, charp , S_IRUSR | S_IWUSR);
module_param(search_count, int , S_IRUSR | S_IWUSR);


char Node_Arr[50][50];
char Node_Old[50][50]={	"Zebra", "Dog", "Fish", "Pig", "Bull", "Cat", "Rat","Lion","Tiger","Sheep",
						"Dragon", "Snake", "Turtle", "Croc", "Hippo", "Puma", "Cheetah","Leopard","Panther","Lizard",
						"Zebra", "Dog", "Fish", "Pig", "Bull", "Cat", "Rat","Lion","Tiger","Sheep",
						"Dragon", "Snake", "Turtle", "Croc", "Hippo", "Puma", "Cheetah","Leopard","Panther","Lizard",
						"Mice", "Ant", "Bee", "Pig", "Bull", "Cat", "Rat","Lion","Tiger","Sheep"};

char MinimumString[10];
char Temp[20];
 
 //int filter = 2;

 struct Node *TempNode,*NewNode;
 struct Node *NewNode;
/*******************************************************************************
	@operation:Frees the nodes used by Set2
*******************************************************************************/
void SizeCount(uint8_t size)													
{
	printk("Size occupied by Set2: %d\n",size_count_s2);
//	struct Node *TempNode;
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


static int __init INIT_NodeModule(void)
{
	struct timespec begin;
	struct timespec end;

	getnstimeofday (&begin);

	SortedArray(Node_Old,SIZE);

	INIT_LIST_HEAD(&NodeList_Set1.list);
	INIT_LIST_HEAD(&NodeList_Set2.list);

	int flag_Node = 0; 

	int i=0;
	for(i=0; i < SIZE; ++i)
	{
		list_for_each_entry(TempNode, &NodeList_Set1.list, list)
		{	
			if( strcmp(TempNode->NodeName, Node_Arr[i]) == 0 )
			{
				TempNode->Count++;
				flag_Node = 1; 
				break;
			}
		}
		
		if(!flag_Node)
		{
			NewNode = kmalloc(sizeof(*NewNode), GFP_KERNEL);
			size_count_s1+=sizeof(*NewNode);
			number_of_nodes_s1+=1;

			strcpy(NewNode->NodeName, Node_Arr[i]);

			NewNode->Count = 1;

			INIT_LIST_HEAD(&NewNode->list);

			list_add_tail(&(NewNode->list), &(NodeList_Set1.list));
		}
		flag_Node = 0; 
	}
/*******************************************************************************
	@operation: Output for Set-1 ==> 1.List 2.Node count 3.Size occupied byt the 
				nodes
********************************************************************************/
		printk("List of nodes with count for Set-1\n");
		list_for_each_entry(TempNode, &NodeList_Set1.list, list)
		{		
			printk(KERN_INFO "%s--%d\n", TempNode->NodeName,TempNode->Count);
		}
		printk("Number of nodes in Set-1:%d",number_of_nodes_s1);

		printk("Size occupied by Set1: %d\n",size_count_s1);
		// list_for_each_entry(TempNode, &NodeList_Set1.list, list)
		// {	
		// 	kfree(TempNode);
		// }
		// printk("Size freed by Set1: %d\n",size_count_s1);

/*******************************************************************************
	@operation: Set-2 assigning nodes
********************************************************************************/
//if(filter == 0) //SearchCount();//print list1.
if(filter == 1)
{	
	
	printk("Applying filter for name\n");
	printk("Nodes with string : %s\n",search_name);
//	struct Node *TempNode;
	list_for_each_entry(TempNode,&NodeList_Set1.list,list)
		{	
			if(strcmp(TempNode->NodeName,search_name)==0)
			{
				
				NewNode = kmalloc(sizeof(*NewNode), GFP_KERNEL);
				printk(KERN_INFO "Node name: %s\n==>Node Count:%d", TempNode->NodeName,TempNode->Count);
				size_count_s2 += sizeof(*NewNode);
				number_of_nodes_s2+=1;
				/* Copy info */
				strcpy(NewNode->NodeName , TempNode->NodeName);
				NewNode->Count = TempNode->Count;

				INIT_LIST_HEAD(&NewNode->list);

				/* Add the new node to NodeList_Set2 */
				list_add_tail(&(NewNode->list), &(NodeList_Set2.list));
				//printk(KERN_INFO "Node name: %s\n", TempNode->NodeName);
			}
		}
		printk("_______________________________________________________");
		printk("Sizecount_s2:%d",size_count_s2);
		printk("Sizecount_s2:%d",number_of_nodes_s2);
}

	//SearchName("Dog",&NodeList_Set1.list);//print list2 with count



else if(filter == 2)
{
	int search_count=3;
	printk("Applying filter for count\n");
	printk("All nodes with count :%d\n",search_count);
	//struct Node *TempNode;
	list_for_each_entry(TempNode, &NodeList_Set1.list, list)
		{	
			if(search_count > TempNode->Count)
			{
				
				NewNode = kmalloc(sizeof(*NewNode), GFP_KERNEL);
				printk(KERN_INFO "Animal name: %s\n", TempNode->NodeName);
				size_count_s2 += sizeof(*NewNode);
				number_of_nodes_s2+=1;
				/* Copy info */
				strcpy(NewNode->NodeName , TempNode->NodeName);
				NewNode->Count = TempNode->Count;

				INIT_LIST_HEAD(&NewNode->list);

				/* Add the new node to NodeList_Set2 */
				list_add_tail(&(NewNode->list), &(NodeList_Set2.list));
				//printk(KERN_INFO "Node name: %s\n", TempNode->NodeName);
			}
		}
		printk("_______________________________________________________");
		printk("Sizecount_s2:%d",size_count_s2);
		printk("Nodes:%d",number_of_nodes_s2);
}
else if(filter > 2)
{
	int search_count=2;
	char* search_name = "Dog";

	printk("Applying filter for count and name\n");
	printk("Nodes with string : %s and Count:%d\n",search_name,search_count);
//	struct Node *TempNode;
	list_for_each_entry(TempNode, &NodeList_Set1.list, list)
		{	
			if(strcmp(TempNode->NodeName,search_name)==0 && (search_count <= TempNode->Count))
			{
				
				NewNode = kmalloc(sizeof(*NewNode), GFP_KERNEL);
				
				size_count_s2 += sizeof(*NewNode);
				number_of_nodes_s2+=1;
				/* Copy info */
				strcpy(NewNode->NodeName , TempNode->NodeName);
				NewNode->Count = TempNode->Count;

				INIT_LIST_HEAD(&NewNode->list);

				/* Add the new node to NodeList_Set2 */
				list_add_tail(&(NewNode->list), &(NodeList_Set2.list));
				//printk(KERN_INFO "Node name: %s\n", TempNode->NodeName);
			}
		}
		printk("_______________________________________________________");
		printk("Sizecount_s2:%d",size_count_s2);
		printk("Nodes:%d",number_of_nodes_s2);
}
else if(filter<1)
{
	printk("List of nodes for Set-2 without any filter\n");
	list_for_each_entry(TempNode, &NodeList_Set2.list, list)
		{	
			printk(KERN_INFO "%s--%d\n", TempNode->NodeName,TempNode->Count);
		}
		printk("Nodes:%d",number_of_nodes_s1);
}
	
/*******************************************************************************
	@operation: Output for Set-2
********************************************************************************/
		

	getnstimeofday (&end);
	struct timespec diff;	
	diff = timespec_sub(end, begin); 
	printk(KERN_ALERT "Time elapsed: %luns\n", diff.tv_nsec );
//	SizeCount(1);

	struct Node *TempNode2;
	

	printk(KERN_INFO "Freeing the %d bytes allocated for Set2", size_count_s2);
	list_for_each_entry(TempNode2, &NodeList_Set2.list, list)
	{	
		kfree(TempNode2);
	}
	printk(KERN_INFO "Freeing the %d bytes allocated for Set1", size_count_s1);
	list_for_each_entry(TempNode2, &NodeList_Set1.list, list)
	{	
		kfree(TempNode2);
	}

	return 0; 

}

static void __exit EXIT_NodeModule(void)
{
	
	struct timespec begin,end;

	struct Nodes *TempNode;

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
