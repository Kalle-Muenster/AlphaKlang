#ifndef _OBJECTMANAGEMENT_H_
#define _OBJECTMANAGEMENT_H_



class ObjectManagement
{
private:
	static ObjectManagement* This;
	ObjectManagement(void);
public:
	static ObjectManagement* getInstance(void);
	~ObjectManagement(void);
};

#endif