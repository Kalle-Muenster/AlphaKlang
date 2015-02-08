/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
| file: list.h                         |
| class: List					       |
| description: 			               |	
|		 a tiny list-type, very fast   |
|		at Iterrating , but slower at  |
|		adding or searching elements.  |
|		by name. it provides Foreach-  |
|       like iterration-style but with
|       advantage of possy
|
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef _LUECKLIST_H_
#define _LUECKLIST_H_

#ifndef EMPTY
#define	EMPTY (4294967295u)
#endif

#include <stdio.h>

//A List of variables of type <ListMember> , with a maximum amount of <MAXIMUM_SIZE> 
template<class ListMember,const unsigned MAXIMUM_SIZE> class List
{
typedef unsigned ListMemberID;

protected:
	ListMember		list[MAXIMUM_SIZE];
	int				numberOfMember;
	ListMemberID	highestSlotNumberInUse;
	ListMember		Nulled;
	bool			CyclingActive;
	int             CyclingCounter;
	unsigned		CycleCheck;
public:
	bool			MemberArePointers;
	// Constructs a new list-instance..
	List(void)
	{
		numberOfMember = 0;
		highestSlotNumberInUse = 0;
		CyclingActive = false;

		//check if ListMember are class-pointers or valuetypes
		MemberArePointers = Utility::StringCompareRecursive( typeid(ListMember).name(),"class ") >= 6; 

		//if it' a pointer-list set each slot to NULL
		if(MemberArePointers)
			Nulled = NULL;
		else try
		{	//try instanciating a variable of ListMembertype by using it's standardconstructor, for using it as NULL-sign..
			Nulled = ListMember();
		}
		catch(...)
		{	//if does'nt work: Allert!
			std::cout<<"ERROR: when using valuetypes as listmember, use the \"List<type,count>(type NullInstance)\" - constructor !...";
			throw"LueckList: error while initializing. use a different constructor...";
		}

		//Initialize the List's dataarray with the desired Nulled value
		//for signing it as unused empty...
		for(int i=0;i<MAXIMUM_SIZE;i++)
			list[i] = Nulled;	
	}

	//Construct a new Listinstance and initialize it's data-array to the given value...
	List(ListMember NullValue)
	{
		numberOfMember = 0;
		highestSlotNumberInUse = 0;
		MemberArePointers = MemberArePointers = Utility::StringCompareRecursive( typeid(ListMember).name(),"class ") >= 6;
		for(int i=0;i<MAXIMUM_SIZE;i++)
			list[i] = NullValue;	 

	}


	// Distroy's all the list's members when the list is beeing distroyed... 
	virtual ~List(void)
	{
		if(MemberArePointers)
		{
			for(int i=0;i<MAXIMUM_SIZE;i++)
			{
				if(list[i] != Nulled)
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
				if(list[index] == Nulled) // and look for free slot to add it there later..
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
				if(list[index]==Nulled)
				{	
					list[index]=member;
					numberOfMember++; 
					return (highestSlotNumberInUse = index);
				}
			}//if maximum count on members reached !!
			throw "Liste voll!..\n";
		}
		catch(char* exeption)
		{
			std::cout<<"Exeption in LuecklList<";
			std::cout<<typeid(ListMember).name();
			std::cout<<"> ... ";
			std::cout<<exeption;
		}
		return NULL;
	}

	//retrurn's a copy of the member at List[index] .. 
	ListMember operator[](ListMemberID index)
	{
		return list[index];
	}

	//retrieve's a pointer to the ListMember.at("index")...
	ListMember* at(ListMemberID index)
	{
		return &list[index];
	}

	// Howmany members are contained in the list ?...
	int Count(void)
	{
		return numberOfMember;
	}

	int StartCycle(int ID)
	{
	    CycleCheck = Next(ID-1);
		CyclingActive=true;
		CyclingCounter=0;
		return CycleCheck;
	}

	// returns the Next member's ID - the next used slot-number in the list
	ListMemberID Next(int current)
	{	
		//return if list is empty.
		if(numberOfMember==0)
			return EMPTY;
		
		//fastforward until finding next used slot.
		while(list[++current]==Nulled);
		
		//If in cycle-mode jump to first if reached end...
		if(CyclingActive)
		{
			CycleCheck = (current = current >= MAXIMUM_SIZE ? 0 : current);

			if(++CyclingCounter>=numberOfMember)
			{
			   CyclingActive=false;
			   CycleCheck = EMPTY;
			}
		}
		//and return it's position.
		return (ListMemberID)current;
	}

	// returns the previous member's ID - the first used slot-number found before [current]
	int Prev(int current)
	{	
		if(current<=0)
		{
			if(CyclingActive)
				current = Last()+1;
			else
				return EMPTY;
		}

		//return if already first element.
		//if(current <= First())  
		//{
		//	if(CyclingActive)
		//	{
		//		Last() : -1;
		//	}
		//}

		//rewind the list until finding a used slot.
		while(list[--current] == Nulled);

		if(CyclingActive)
		{
			CycleCheck = current;
			if(++CyclingCounter>=numberOfMember)
			{
				  CyclingActive=false;
				  CycleCheck = EMPTY;
			}
		}

		//return the found position.
		return (ListMemberID)current;
	}

	unsigned Cycle(void)
	{
		return CyclingActive? CycleCheck : EMPTY;
	}

	// return the ID of the first member in the list..
	ListMemberID First(void)
	{
		//list-slot indexes can't be negativ, so getting the next of [-1] will be the first...
		return Next(-1);
	}

	// return the ID of the last member at the end of the list..
	ListMemberID Last(void)
	{
		//return a "peak"-value, wich's updated everytime an objects is Addet or Removed
		return highestSlotNumberInUse;
	}

	// remove's the member at slot-number [ListMemberID]...
	bool Remove(ListMemberID id)
	{
		//check	if slot at "id" is in use..
		if(list[id]!=Nulled)
		{
			list[id] = Nulled; //erease the pointer,

			//if it's the last element:
			if(id==highestSlotNumberInUse) //decrease the peak-value... 
				highestSlotNumberInUse = Prev(id); // to it's preceding ellement.

			--numberOfMember;// and decrease the member-count by 1.
			return true;
		}
		return false;
	}

	// remove's the given member from the list, if it's contained in it..
	bool Remove(ListMember member)
	{
		for(unsigned ID = First(); ID <= Last() ; ID = Next(ID))
		{
			if(list[ID]==member)
			{
				highestSlotNumberInUse = (ID==highestSlotNumberInUse)? Prev(ID) : highestSlotNumberInUse;

				list[ID] = Nulled;
				--numberOfMember;
				return true;
			}
		}
		return false;
	}

	// remove's the element at slot-number [ListMemberID] and calls it's destructor...
	void Distruct(ListMemberID id)
	{
		if(list[id]!=Nulled)
		{
			if((id==highestSlotNumberInUse)&&(id!=First()))
				highestSlotNumberInUse = Prev(id);

			if(MemberArePointers)
				delete list[id];  // same as "Remove", but also kill's the object itself befor ereasing the pointer to it...
			
			list[id] = Nulled;
			--numberOfMember;
		}
	}
};

#endif