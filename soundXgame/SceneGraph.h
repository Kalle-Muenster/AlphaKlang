#ifndef __SCENEGRAPH__
#define __SCENEGRAPH__

#define MAXIMUM_SXCENE_OBJECT (3000)
typedef unsigned int GobID;
typedef char* string;

class IGObject;
class Cam;


class SceneGraph
{
private:
	SceneGraph(void);
	List<IGObject*,MAXIMUM_SXCENE_OBJECT> *drawables;
	float r,g,b;
	void UpdateSky(void);

public:
	~SceneGraph(void);
	static SceneGraph* GetInstance();
	void DrawAll(void);
	void DrawSky(void);

	//Activate Colorflashing Sky drawer !...
	bool SkyFlasherActive;

	//Add's an object to the scene and generate a new ID that will be returned... 
	unsigned int Add(IGObject* object);

	// removes an object from the scene, but does not distroy it.
	// use "Destruct(IGObject*)" to delete an object permanently...
	void Remove(IGObject* object);

	//removes the object with the given ID from the scene and calls it's destructor... 
	void Destruct(GobID ID);

	//removes the object named "Name" from the scene and calls it's destructor... 
	void Destruct(string Name);

	//if the Object can be found in the scene, it will be removed and destroyed... 
	void Destruct(IGObject* Obj);

	// accses to the scene's main-camera...
	Cam* camera;

	// Find an object by it's name...
	IGObject* Object(const char* name);  

	// Find an object by it's ID...
	IGObject* Object(unsigned int ID);

	// total count on objects in scene...
	unsigned ObjectsCount(void);

	// returns the GobID with the highest value that is contained in the scenegraph...
	// its's the ID of the last Object at the end of the scene-draw-List...
	GobID HighestID(void);
};

#endif