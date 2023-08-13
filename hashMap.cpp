// Name: Kevin Barrios
// NSHE ID: 2001697903
// Assignment 5 word classification (custom hashMap)
//  implement our custom hashMap structure 
#include "hashMap.h"
#include <cmath>
#include <string>
using namespace std;

template <class t1, class t2>
const hashMap<t1, t2>& hashMap<t1, t2>::operator=(const hashMap<t1, t2>& rhs)
{
	if (this != &rhs)
	{
		deallocateMap();
		copyMap(rhs);
	}
	return *this;
}

template <class t1, class t2>
t2& hashMap<t1, t2>::operator[](t1 key)
{
	// we need to type case all these values to see 
	// if we hit a 20% or greater list 
	double loadFactor1, loadFactor2; 
	double itm1, itm2, cap; 
	itm1 = items1;
	itm2 = items2; 
	cap = capacity;
	loadFactor1 = itm1/cap;
	loadFactor2 = itm2/cap;
	//check if we need to resize >= 20% 
	if(loadFactor1 >= .2 || loadFactor2 >= .2)
		resize(capacity); 
	// define our indexes 
	// set up a node count so we only ever allow
	//  2 nodes per list MAX
	int index1, index2;
	int nodeCount[2]; 
	int collisionCounter = 0; //initially set to 0
	while(1) // while true because we have exit conditions
	{
		//compute indexes 
		index1 = (h1(key) + (collisionCounter * h2(key))) % capacity; 
		index2 = (h2(key) + (collisionCounter * h1(key))) % capacity; 
		
		//search for key in table 1, return value field if found
		nodeCount[0] = 0;
		for(node * u = table1[index1]; u != nullptr; u = u->link)
		{
			nodeCount[0]++;
			if(u->key == key)
				return u->value;
			if(nodeCount[0] == 2)
				break;
		}
		//search for key in table 2, return value field if found
		nodeCount[1] = 0; 
		for(node * u = table2[index2]; u != nullptr; u = u->link)
		{
			nodeCount[1]++;
			if(u->key == key)
				return u->value;
			if(nodeCount[1] == 2)
				break;
		}
		// no keys found in either table 
		
		// see if we can insert in table 1
		if(nodeCount[0] == 0)
		{
			//the list was vacant so we just insert at the head
			table1[index1] = new node;
			table1[index1]->key = key;
			table1[index1]->value = t2();
			//    make sure we set link to null :(   *so many hours of sleep...gone*
			table1[index1]->link = nullptr;
			return table1[index1]->value;
		}
		if(nodeCount[0] == 1)
		{
			//the list has an existing node so we
			//allocate existing nodes link 
			table1[index1]->link = new node;
			table1[index1]->link->key = key;
			table1[index1]->link->value = t2();
			//because only two nodes are allowe we increment items
			items1++;
			return table1[index1]->link->value;
		}
		if(nodeCount[1] == 0)
		{
			//the list was vacant so we just insert at the head
			table2[index2] = new node;
			table2[index2]->key = key;
			table2[index2]->value = t2();
			table2[index2]->link = nullptr;
			return table2[index2]->value;
		}
		if(nodeCount[1] == 1)
		{
			//the list has an existing node so we
			//allocate existing nodes link 
			table2[index2]->link = new node;
			table2[index2]->link->key = key;
			table2[index2]->link->value = t2();
			items2++;
			return table2[index2]->link->value;
		}
		
		// if we reach here we know we didn't find a key initially 
		// AND all possible slots are full 
		
		//increment our collisionCounter to perform the "double hash"
		collisionCounter++;
	}
}

template <class t1, class t2>
void hashMap<t1, t2>::resize(size_t amount)
{
	//create two pointers 
	// node **x node **y
	// copy address of table 1 to x 
	// copy address of table 2 to y 
	node **x;
	node **y; 
	x = table1; 
	y = table2;
	
	// allocate a new array for table 1 and table 2 [newlength] 
	// set items1 and items2 to 0 
	// make our capacity += amount 
	items1 = 0;
	items2 = 0;
	capacity += amount;
	table1 = new node*[capacity];
	table2 = new node*[capacity];

	for (int i = 0; i < capacity; i++)
	{
		table1[i] = nullptr; //paranoia made me set it up this way
		table2[i] = nullptr;
	}
	// have a pointer get the nodes from x and y and call the [] operator 
	//(*this)[u->key] = u->value; 
	int nodeCount[2];
	for (int i = 0; i < amount; i++)
	{
		nodeCount[0] = 0;
		for(node * u = x[i]; u != nullptr; u = u->link)
		{
			nodeCount[0]++;
			(*this)[u->key] = u->value; //insert 
			// if we ever hit 2 nodes we know list is full so stop trying to trav further
			if(nodeCount[0] == 2) 
				break;
		}
		nodeCount[1] = 0;
		for(node * u = y[i]; u != nullptr; u = u->link)
		{
			nodeCount[1]++;
			(*this)[u->key] = u->value; //insert 
			// if we ever hit 2 nodes we know list is full so stop trying to trav further
			if(nodeCount[1] == 2)
				break;
		}
	}
	
	// after make sure to deallocate x and y 
	for (int k = 0;  k < (capacity-amount) ; k++) // k for kevin
	{
		if(x[k] != nullptr)
		{
			if(x[k]->link != nullptr)
				delete x[k]->link;
			delete x[k];
		}
		if(y[k] != nullptr)
		{
			if(y[k]->link != nullptr)
				delete y[k]->link;
			delete y[k];
		}
	}
	delete[] x;
	delete[] y;
}

template <class t1, class t2>
void hashMap<t1, t2>::copyMap(const hashMap<t1, t2>& map)
{
	capacity = map.capacity; 
	items1 = 0;
	items2 = 0; 
	table1 = new node*[capacity];
	table2 = new node*[capacity];
	for (int i = 0; i < capacity; i++)
		table1[i] = table2[i] = nullptr;
	
	int nodeCount[2];
	for (int i = 0; i < capacity; i++)
	{
		nodeCount[0] = 0;
		for(node * u = map.table1[i]; u != nullptr; u = u->link)
		{
			nodeCount[0]++;
			(*this)[u->key] = u->value; //insert 
			if(nodeCount[0] == 2)
				break;
		}
		nodeCount[1] = 0;
		for(node * u = map.table2[i]; u != nullptr; u = u->link)
		{
			nodeCount[1]++;
			(*this)[u->key] = u->value; //insert 
			if(nodeCount[1] == 2)
				break;
		}
	}
	
	// is this a deep copy ? :'(
}

template <class t1, class t2>
void hashMap<t1, t2>::deallocateMap()
{
	// this logic really only works because 
	// of how we allocated map in [] operator 
	for (int i = 0; i < capacity; i++)
	{
		if(table1[i] != nullptr)
		{
			if(table1[i]->link != nullptr)
				delete table1[i]->link;
			delete table1[i];
		}
		if(table2[i] != nullptr)
		{
			if(table2[i]->link != nullptr)
				delete table2[i]->link;
			delete table2[i];
		}
	}
	
	delete[] table1;
	delete[] table2; 
}

template <class t1, class t2>
size_t hashMap<t1, t2>::h1(const string& s) const
{
	// implemented formula from pdf
	size_t toReturn = 0;
	for(int i = 0; i < s.length() -1; i++)
		toReturn += (pow(10, i) * s[i]); 
	return toReturn;
}

template <class t1, class t2>
size_t hashMap<t1, t2>::h2(const string& s) const
{
	// implemented formula from pdf
	size_t toReturn = 0;
	for(int i = 0; i < s.length()-1; i++)
		toReturn += (pow(10,i) * s[s.length()-1-i]);
	return toReturn;
}