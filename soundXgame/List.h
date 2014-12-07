#ifndef _LUECKLIST_H_
#define _LUECKLIST_H_

#include <stdio.h>

//A List of variables from type [ListMember] , with a maximum amount of [MAXIMUM_SIZE] 
template<class ListMember,const unsigned MAXIMUM_SIZE> class List
{
public:
	typedef unsigned ListMemberID;
	
	// Constructs a new list-instance..
	List(void)
	{
		numberOfMember = 0;
		highestSlotNumberInUse = 0;
		for(int i=0;i<MAXIMUM_SIZE;i++)
			list[i] = NULL;
	}

	// Distroy's all the list's members when the list is beeing distroyed... 
	virtual ~List(void)
	{
		if(typeid(ListMember).name()=="pointer-to")
		{
			for(int i=0;i<MAXIMUM_SIZE;i++)
			{
				if(list[i] != NULL)
					delete (ListMember*)&list[i];
			}
		}
	}

	// Add a member to the list and return the memberID 
	// (slot-number) at wich the object has been addet to...
	unsigned Add(ListMember member)
	{
		int FirstEmptySlotFound = -1;
		unsigned counter = 0;
		unsigned index = 0;
		try
		{
			while(counter<numberOfMember)
			{//check's if the object has been addet to the list allready...
				if(list[index] == NULL) // and look for free slot to add it there later..
					FirstEmptySlotFound = FirstEmptySlotFound<0? index : FirstEmptySlotFound;
				else if(list[index] == member)
					return index; //return if list already contain the object...
				else 
					counter++;
				index++;
			}

			if(FirstEmptySlotFound>=0)
			{// if a Free slot was found before, add the object there and return it's index.
				list[FirstEmptySlotFound]=member;
				numberOfMember++; 
				return (ListMemberID)FirstEmptySlotFound;
			}
			else for(index = numberOfMember; index < MAXIMUM_SIZE ;index++)
			{// no slot found before: go ahead finding one.. 
				if(list[index]==NULL)
				{	
					list[index]=member;
					numberOfMember++; 
					highestSlotNumberInUse = index;
					return highestSlotNumberInUse;
				}
			}//if maximum count on members reached !!
			throw "Liste voll!..";
		}
		catch(char* exeption)
		{
			std::cout<<"Exeption in LuecklList<";
			std::cout<<typeid(ListMember).name();
			std::cout<<"> ... \n";
			std::cout<<exeption;
		}
	}

	// make the members accsessable by list[memberID]... 
	ListMember operator[](ListMemberID index)
	{
		return list[index];
	}

	// return the member at position [index] - (the memberID)...
	ListMember at(ListMemberID index)
	{
		return list[index];
	}

	// Howmany members are contained in the list ?...
	int Count(void)
	{
		return numberOfMember;
	}

	// returns the Next member's ID - the next used slot-number in the list
	ListMemberID Next(int current)
	{	
		if(numberOfMember>0)
			while(list[++current]==NULL);
		else
			return -1;

		return (ListMemberID)current;
	}

	// returns the previous member's ID - the first used slot-number found before [current]
	ListMemberID Prev(int current)
	{
		if(current == 0)
			return 0;
		while(list[--current]==NULL);
			return (ListMemberID)current;
	}

	// return the ID of the first member in the list..
	ListMemberID First(void)
	{
		return Next(-1);
	}

	// return the ID of the last member at the end of the list..
	ListMemberID Last(void)
	{
		return highestSlotNumberInUse;
	}

	// remove's the member at slot-number [ListMemberID]...
	void Remove(ListMemberID id)
	{
		if(list[id]!=NULL)
		{
			if(id==highestSlotNumberInUse)
				highestSlotNumberInUse = Prev(id);


			list[id] = NULL;
			--numberOfMember;
		}
	}

	// remove's the element at slot-number [ListMemberID] and calls it's destructor...
	void Distruct(ListMemberID id)
	{
		if(list[id]!=NULL)
		{
			if((id==highestSlotNumberInUse)&&(id!=First()))
				highestSlotNumberInUse = Prev(id);

			delete list[id];
			list[id] = NULL;
			--numberOfMember;
		}
	}

	// remove's the given member from the list, if it's contained in it..
	void Remove(ListMember member)
	{
		for(int i = 0 ; i<MAXIMUM_SIZE ; i++)
		{
			if(list[i]==member)
			{
				if(i==highestSlotNumberInUse)
					highestSlotNumberInUse=Prev(i);

				list[i] = NULL;
				--numberOfMember;
				return;
			}
		}
	}

protected:
	ListMember list[MAXIMUM_SIZE];
	int numberOfMember;
	ListMemberID highestSlotNumberInUse;
};

#endif