//WHATS THE POINT OF HAVING A DUMMY HEAD NODE IN A CIRCULAR DOUBLY LINKED LIST?
//TEST OVERLOADED DELETEITEM AND INSERTITEM

//Includes
#include <iostream.h>
#include <conio.h   >

//Defines
#define ERR_FLAG NULL

template <class type>
class CList
{	
private:
	struct node
	{
		type Value;
		node *Next;
		node *Prev;
	};

	node* Head;   //Pointer to the top    of the Head

	node* GetNode(long Number);
	node* GetNode(long Number, bool Success);

	long Length;

public: //TO ADD: Enqueue, Dequeue, Save, Load
	CList (); //Calls Create()
	~CList(); //Calls Delete()
	
	void Create(); //Initiallize Head (called on constructor)
	void Delete(); //Delete      Head (called on destructor )

	long  GetLength();

	bool  InList   (type Item);

	bool  AddItem   (type Value             ); //Add node to begining
	type* GetItem   (long Number            ); //Overload for GetItem
	bool  DeleteItem(long Number            ); //Overload for DeleteItem
	bool  InsertItem(long Number, type Value); //Overload for InsertItem

	//void  DeleteItems(); //Deletes all nodes other than the dummy header

	//void  DeleteElement(type element);

	bool Push(type  Value  ); //Pushes a node to the front of the Head
	type Pop (bool &Success);

	type Pop ();

	bool Enqueue(type Value   );
	type Dequeue(bool &Success);

	type Dequeue();
	//void ModifyElement(node **Head, int element, int element2);
	//void FindElement(node *Head, int element, node **location);
	//void InsertElement(node **Head, int element);
};

/***************************************************
Constructor()
 Inputs nothing
 Calls Delete().
***************************************************/

template <class type>
CList<type>::CList()
{
	Create();
}

/***************************************************
Destructor()
 Calls Delete().
***************************************************/

template <class type>
CList<type>::~CList()
{
	Delete();
}

/***************************************************
Create()
 Initializes the Head by setting bottom to the dummy
 and pointing Head to the dummy as well.

 Inputs:
  nothing, but Head MUST equal NULL. If it does 
  not, we will have a memory leak. Head is
  always NULL after Delete().
 Output:
  The dummy header was created and now you may use
  functions without errors. The list is also
  made circular.
***************************************************/

template <class type>
void CList<type>::Create()
{
	if (Head == NULL)
	{
		Head       = new node;
		Head->Next = Head;
		Head->Prev = Head;

		Length = 0;
	}
}

/***************************************************
Delete()
 Deletes each node and sets Head to NULL, deleting
 the "Dummy Head Node".

 Inputs: 
  Nothing
 Output:
  The list is equal to NULL and EVERY node is 
  deleted.
 Note:
  After a call to Delete(), DO NOT use ANY list
  functions other than Create. This is because the
  dummy head has been deleted, and if you call a
  function you have a great chance of having an
  error. Create() recreates the dummy head. If 
  you wish to simply clear the list, call
  DeleteItems().
***************************************************/

template <class type>
void CList<type>::Delete()
{
	if (Head != NULL)
	{
		node *temp = Head->Next;

		while (temp != Head)
		{
			temp = temp->Next;
			delete temp->Prev;
		}
		delete temp;

		Head   = NULL;
		Length = 0;
	}
}

/***************************************************
GetNode()
 Gets node n. The list starts at 0.
 Inputs: Node index to get
 Output: Returns a pointer to the node with the item
***************************************************/

template <class type>
CList<type>::node* CList<type>::GetNode(long Number)
{
	node* Temp = Head;

	for (int i=0; i<Number+1; i++)
	{
		if (Temp == NULL || Temp == Head->Prev)
		{
			return NULL;
		}

		Temp = Temp->Next;
	}

	if (Temp == NULL)
	{
		return NULL;
	}
	else
		return Temp;
}

/***************************************************
AddItem()
 Adds a node to the begining.
 Inputs the Value of the new node (in template type)
 Outputs (returns) a value indicating success
***************************************************/

template <class type>
bool CList<type>::AddItem(type Value)
{
	node *NewNode = new node; //Allocate dynamic memory

	if (NewNode == NULL) 
		return 0;

	NewNode->Value = Value;
	NewNode->Next = Head->Next; //Next = begining of old Head (after dummy header)
	NewNode->Prev = Head; //The previous node is the dummy header
	
	(NewNode->Next)->Prev = NewNode; //The old begining of the Head's Prev is the new node

	Head->Next = NewNode; //The dummy header links to the new node

	if (Head->Prev == Head)
		Head->Prev = NewNode;

	++Length;

	return true;
}

/***************************************************
GetLength()
 Gets the number of nodes.
 Inputs: NONE
 Output: a long integer containing the length
***************************************************/

template <class type>
long CList<type>::GetLength()
{
	long  len=0;
	node* cur = Head->Next;

	while (cur != Head)
	{
		++len;
		cur = cur->Next;
	}

	return len;
}

/***************************************************
InList()
 Searches for the item in the list and returns it
 Inputs: the item to look for
 Output: a boolean indicating if it was found
***************************************************/

template <class type>
bool CList<type>::InList(type Item)
{
	node* temp = Head->Next;

	while(temp != Head)
	{
		if (temp->Value.x == Item.x && temp->Value.y == Item.y)
			return true;

		temp = temp->Next;
	}

	return false;
}

/***************************************************
GetItem()
 Gets the node at index X where the Head starts at 0
 Inputs: the index of the node to get
 Output: a pointer of type CNode<Headtype> or NULL
 if failure
***************************************************/

template <class type>
type* CList<type>::GetItem(long Number)
{ 
	node *temp = GetNode(Number);

	if (temp != NULL)
		return &temp->Value;
	else
	{
		return NULL;
	}
}

/***************************************************
DeleteItem()
 Deletes item n where the Head starts at 0.
 Inputs: Index to delete and a boolean varible
 Output: Returns a boolean value indicating success
***************************************************/

template <class type>
bool CList<type>::DeleteItem(long Number)
{
	node *temp = GetNode(Number);

	if (temp != NULL)
	{
		(temp->Prev)->Next = temp->Next;
		(temp->Next)->Prev = temp->Prev;
		delete temp;
		
		--Length;

		return true;
	}
	else
	{
		return false;
	}
}

/***************************************************
Insert Node()
 Inserts a node with specified Value before Number
 Inputs: the place Number (the Head starts at 0) and
         the Value for the new node
 Output: a pointer to the new node or NULL on 
	     failure
***************************************************/

template<class type>
bool CList<type>::InsertItem(long Number, type Value) //Inserts before [Number]th item (from 0)
{
	node* temp = GetNode(Number);

	if (temp != NULL)
	{
		node *NewNode = new node;
		NewNode->Value = Value;
		NewNode->Next = temp;
		NewNode->Prev = temp->Prev;

		(temp->Prev)->Next = NewNode;
		temp->Prev         = NewNode;

		++Length;

		return true;
	}
	else
	{
		return false;
	}
}

/***************************************************
Push()
 See Add() above
***************************************************/

template<class type>
bool CList<type>::Push(type Value)
{
	return AddItem(Value);
}

/***************************************************
Pop()
 Deletes and returns the top node Value.
 No input.
 Outputs the top node Value.
***************************************************/

template<class type>
type CList<type>::Pop(bool& Success)
{
	if (Head == NULL || Head->Next == NULL) //If no Head or no node to pop
	{
		return Head->Value;
	}

	type* Point = GetItem(0);

	if (Point == NULL)
	{
		Success = false;
		return Head->Value;
	}

	type Value = *Point;

	DeleteItem(0);

	return Value;
}

/* Overload bypassing Success */

template<class type>
type CList<type>::Pop()
{
	bool Success;
	return Pop(Success);
}

/***************************************************
Enqueue()
 Adds a node the the end of the list.
 Inputs: The value of the node to insert
 Output: Returns a boolean value indicating success
***************************************************/

template<class type>
bool CList<type>::Enqueue(type Value)
{
	node* NewNode = new node;

	if (Head == NULL || NewNode == NULL)
		return false;

	NewNode->Value   = Value;
	NewNode->Next    = Head;
	NewNode->Prev    = Head->Prev;

	(Head->Prev)->Next = NewNode; //(Head->Prev) is the last node
	Head->Prev         = NewNode; //Head's prev is the last node

	++Length;

	return true;
}

/***************************************************
Dequeue()
 Retrieves and removes the value at the top of the 
 list.
 Inputs: The value of the node to retrieve
 Output: Returns a boolean value indicating success
***************************************************/

template<class type>
type CList<type>::Dequeue(bool& Success)
{
	return Pop(Success);
}

/* Overload bypassing Success */

template<class type>
type CList<type>::Dequeue()
{
	bool Success;
	return Pop(Success);
}

/***************************************************
Delete Element()
 Deletes the first element with the specified Value.
 Inputs the Value of the node to be deleted.
 Outputs nothing.
***************************************************

template<class type>
void CList<type>::DeleteElement(type Value)
{
	node *temp1=NULL, *temp2=NULL;
	bool deleted = false;

	temp1 = Head;
	temp2 = Head;

	if (temp1->Value == Value)
	{
		Head = temp1->Next;
		delete temp1;
		deleted = true;
	}
	else
	{
		temp2 = temp2->Next;
		while(temp2!=NULL && !deleted)
		{
			if (temp2->Value == Value)
			{
				temp1->Next = temp2->Next; //or temp1->Next->Next
				delete temp2;
				deleted = true;
			}
			else
			{
				temp1 = temp1->Next;
				temp2 = temp2->Next;
			}
		}
	}

	if (!deleted)
		cout << "unable to delete that element..."<<endl;
}
*/

/*
void CList::ModifyElement(node **Head, int element, int element2)
{
	node *temp;
	bool changed = false;

	temp = *Head;

	while(temp!=NULL && !changed)
	{
		if (temp->Value == element)
		{
			temp->Value = element2;
			changed = true;
		}
		else
		{
			temp = temp->Next;
		}
	}
	if (!changed)
		cout << "that element could not be changed..."<<endl;
}

void CList::FindElement(node *Head, int element, node **location)
{
	*location = Head;

	if ((*location) != NULL)
	{
		while ((*location)->Value != element && (*location)->Next != NULL)
		{
			*location = (*location)->Next;
		}
	}
}
*/