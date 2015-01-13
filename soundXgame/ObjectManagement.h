#ifndef _OBJECTMANAGEMENT_H_
#define _OBJECTMANAGEMENT_H_



class ObjectManagement
{
private:
	static bool NotIsInstanciated;
	ObjectManagement(void);
	std::vector<unsigned> usedIDs;
public:
	static const char* start(void);
	static ObjectManagement* getInstance(void);
	~ObjectManagement(void);
	unsigned GenerateID(void);
	void FreeID(unsigned);
};

#endif