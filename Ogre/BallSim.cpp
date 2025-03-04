#include "BallSim.h"
 
Sphere::Sphere(Ogre::SceneManager* scnMgr, Ogre::Vector3& dir, Ogre::Real& scale, Ogre::Real& radius, Ogre::Real& speed)
{
    if(radius > 100.0f) {
        sEnt = scnMgr->createEntity("Star", "sphere.mesh");
        sEnt->setMaterialName("Examples/Sun");
    }
    else { 
        sEnt = scnMgr->createEntity("Planet", "sphere.mesh");
        sEnt->setMaterialName("Examples/Earth");
    }
    sEnt->setCastShadows(true);
    sNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    sNode->attachObject(sEnt);
    sNode->scale(scale, scale, scale);
    sDirection = dir;
    sDirection.normalise();
    sRadius = radius;
    sSpeed = speed;
}
//-------------------------------------------------------------------------------------
BallSim::BallSim(void)
    : sunNode(0),
    mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    mDetailsPanel(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0)
{
}
//-------------------------------------------------------------------------------------
BallSim::~BallSim(void)
{
    if (mTrayMgr) delete mTrayMgr;
    if (mCameraMan) delete mCameraMan;
 
    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

void Sphere::move(const Ogre::FrameEvent& evt) {  
     
    Ogre::Vector3 bPosition = sNode->getPosition();
    std::cout << bPosition.y << std::endl;
    if(bPosition.y < -1500.0f + sRadius && sDirection.y < 0.0f) { 
        sDirection.y = -sDirection.y;
    }
    if(bPosition.y > 1500.0f - sRadius && sDirection.y > 0.0f) {
        sDirection.y = -sDirection.y;
    }
    if(bPosition.z < -1500.0f + sRadius && sDirection.z < 0.0f) {
        sDirection.z = -sDirection.z;
    }
    if(bPosition.z > 1500.0f - sRadius && sDirection.z > 0.0f) { 
        sDirection.z = -sDirection.z;
    }
    if(bPosition.x < -1500.0f + sRadius && sDirection.x < 0.0f)  {
        sDirection.x = -sDirection.x;
    }
    if(bPosition.x > 1500.0f - sRadius && sDirection.x > 0.0f) {
        sDirection.x = -sDirection.x;
    }

    sNode->translate(sSpeed * evt.timeSinceLastFrame * sDirection);

}

void BallSim::createBox(void)
{
    Ogre::Plane floor(Ogre::Vector3(0, 1, 0), -1500);
    Ogre::Plane ceiling(Ogre::Vector3(0, -1, 0), -1500);
    Ogre::Plane wall1(Ogre::Vector3(1, 0, 0), -1500);
    Ogre::Plane wall2(Ogre::Vector3(-1, 0, 0), -1500);
    Ogre::Plane wall3(Ogre::Vector3(0, 0, 1), -1500);
    Ogre::Plane wall4(Ogre::Vector3(0, 0, -1), -1500);
    Ogre::MeshManager::getSingleton().createPlane("bottom", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, floor, 3000, 3000, 100, 100, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    Ogre::MeshManager::getSingleton().createPlane("top", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, ceiling, 3000, 3000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    Ogre::MeshManager::getSingleton().createPlane("w1", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, wall1, 3000, 3000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
    Ogre::MeshManager::getSingleton().createPlane("w2", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, wall2, 3000, 3000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
    Ogre::MeshManager::getSingleton().createPlane("w3", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, wall3, 3000, 3000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
    Ogre::MeshManager::getSingleton().createPlane("w4", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, wall4, 3000, 3000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
    Ogre::Entity* entFloor = mSceneMgr->createEntity("FloorEntity", "bottom");
    Ogre::Entity* entCeiling = mSceneMgr->createEntity("CeilingEntity", "top");
    Ogre::Entity* entWall1 = mSceneMgr->createEntity("Wall1Entity", "w1");
    Ogre::Entity* entWall2 = mSceneMgr->createEntity("Wall2Entity", "w2");
    Ogre::Entity* entWall3 = mSceneMgr->createEntity("Wall3Entity", "w3");
    Ogre::Entity* entWall4 = mSceneMgr->createEntity("Wall4Entity", "w4");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entFloor);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entCeiling);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entWall1);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entWall2);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entWall3);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entWall4);
    entFloor->setMaterialName("Examples/Flare");
    entCeiling->setMaterialName("Examples/Flare");
    entWall1->setMaterialName("Examples/SpaceSkyPlane");
    entWall2->setMaterialName("Examples/SpaceSkyPlane");
    entWall3->setMaterialName("Examples/SpaceSkyPlane");
    entWall4->setMaterialName("Examples/SpaceSkyPlane");
    entFloor->setCastShadows(false);
    entCeiling->setCastShadows(false);
    entWall1->setCastShadows(false);
    entWall2->setCastShadows(false);
    entWall3->setCastShadows(false);
    entWall4->setCastShadows(false);
}

bool BallSim::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
 
    // construct Ogre::Root
    mRoot = new Ogre::Root(mPluginsCfg);
 
//-------------------------------------------------------------------------------------
    // setup resources
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);
 
    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
//-------------------------------------------------------------------------------------
    // configure
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->restoreConfig() || mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "BallSim Render Window");
    }
    else
    {
        return false;
    }
//-------------------------------------------------------------------------------------
    // choose scenemanager
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
//-------------------------------------------------------------------------------------
    // create camera
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");
 
    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,0,1000));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);
 
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
//-------------------------------------------------------------------------------------
    // create viewports
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
 
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
//-------------------------------------------------------------------------------------
    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
//-------------------------------------------------------------------------------------
    // Create any resource listeners (for loading screens)
    //createResourceListener();
//-------------------------------------------------------------------------------------
    // load resources
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
//-------------------------------------------------------------------------------------
    // Create the scene
    createBox();

    Ogre::Vector3 direction = Ogre::Vector3(1.0f, 2.0f, -2.0f);
    Ogre::Real scale = 3.0f;
    Ogre::Real radius = 300.0f;
    Ogre::Real speed = 200.0f;
    Sphere* s = new Sphere(mSceneMgr, direction, scale, radius, speed);
    spheres.push_back(s);

    direction = Ogre::Vector3(-1.0f, -2.0f, -2.0f);
    scale = 1.0f;
    radius = 100.0f;
    speed = 600.0f;
    Sphere* s2 = new Sphere(mSceneMgr, direction, scale, radius, speed);
    spheres.push_back(s2);
    /*
    Ogre::Entity* sun = mSceneMgr->createEntity("Star", "sphere.mesh");
    sun->setMaterialName("Examples/Sun");
    sunNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    
    sunNode->attachObject(sun);
    sunNode->scale(2.0f, 2.0f, 2.0f);
    */
    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
 
    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);
//-------------------------------------------------------------------------------------
    //create FrameListener
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
 
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
    mInputManager = OIS::InputManager::createInputSystem( pl );
 
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
 
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
 
    //Set initial mouse clipping size
    windowResized(mWindow);
 
    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
 
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();
 
    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");
 
    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();
 
    mRoot->addFrameListener(this);
//-------------------------------------------------------------------------------------
    mRoot->startRendering();
 
    return true;
}
 
bool BallSim::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
 
    if(mShutDown)
        return false;
 
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
 
    mTrayMgr->frameRenderingQueued(evt);
    
    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
        for(int i = 0; i < spheres.size(); ++i) {
            spheres[i]->move(evt);
        }

        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }
 
    return true;
}
//-------------------------------------------------------------------------------------
bool BallSim::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
 
    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_T)   // cycle texture filtering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;
 
        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }
 
        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;
 
        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }
 
        mCamera->setPolygonMode(pm);
        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }
 
    mCameraMan->injectKeyDown(arg);
    return true;
}
 
bool BallSim::keyReleased( const OIS::KeyEvent &arg )
{
    mCameraMan->injectKeyUp(arg);
    return true;
}
 
bool BallSim::mouseMoved( const OIS::MouseEvent &arg )
{
    if (mTrayMgr->injectMouseMove(arg)) return true;
    mCameraMan->injectMouseMove(arg);
    return true;
}
 
bool BallSim::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    mCameraMan->injectMouseDown(arg, id);
    return true;
}
 
bool BallSim::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    mCameraMan->injectMouseUp(arg, id);
    return true;
}
 
//Adjust mouse clipping area
void BallSim::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
 
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
 
//Unattach OIS before window shutdown (very important under Linux)
void BallSim::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
 
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
 
 
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        BallSim app;
 
        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif