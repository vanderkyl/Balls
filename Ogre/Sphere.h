#ifndef __Sphere_h_
#define __Sphere_h_

//---------
// includes
//---------

#include <Ogre.h>

class Sphere {
public:
	Sphere(Ogre::SceneManager* scnMgr, Ogre::Vector3& dir, Ogre::Real& scale, Ogre::Real& radius, Ogre::Real& speed);
	~Sphere();
	void move(const Ogre::FrameEvent& evt);
	Ogre::SceneNode* getNode() {return sNode; }
protected:
	Ogre::SceneNode* sNode;
	Ogre::Entity* sEnt;
	Ogre::Vector3 sDirection;
	Ogre::Real sRadius;
	Ogre::Real sSpeed;
};

#endif // #ifndef __Sphere_h_