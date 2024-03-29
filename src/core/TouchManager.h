//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2014
//	Developers: Paul Rudolph & Stacey Martens
//  Contents: 
//  Comments: 
//----------------------------------------------------------------------------

#pragma once
#include "cinder/app/App.h"
#include "BaseTouchObject.h"
#include <mutex>
class TouchManager{    
public:	
	
	enum TouchEventType{ BEGAN, MOVED, ENDED };

	//A struct is created for each touch that comes in
	struct TouchObject{
		int							touchId;
		ci::vec2					touchPoint;
		touchObject::TouchType		touchType;
		touchObject::TouchObjectWeakRef touchingObjectPntr;
	};

	//Will return a static pointer to this one touch manager
	static std::shared_ptr<TouchManager> getInstance();
	~TouchManager();

	void					update();
    void					draw(); 
    
	//Touch functions
	void					touchEvent(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType, TouchEventType eventType);

	//TUIO touches aren't always on the main thread. So when we receive touches through TUIO, we'll want to hold onto the functions in the mRenderFunctionQueue, 
	//and on each update we will call all of the functions in that deque
	void					mainThreadTouchesBegan(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType);
	void					mainThreadTouchesMoved(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType);
	void					mainThreadTouchesEnded(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType);

	//Registering the object for input adds it to deque of the entire application's registered objects. 
	//If a touch comes through, it will check the deque and see which registered object was hit.
	void					registerObjectForInput(touchObject::TouchObjectWeakRef obj);
	void					unregisterObjectForInput(touchObject::TouchObjectWeakRef obj);

	//Find the current point location of the ID that is passed in
	ci::vec2					getCurrentTouchPoint(int touchId);
	
	void					endTouch(int touchID);

    float                   getLatestTouchTime(){return mLatestTouchTime;};
	void					setTouchScale(ci::vec2 scale){ mScale = scale; };

	int						mTotalRegisteredObjectCount;

private:
    //private to make the TouchManager a singleton
    TouchManager();

	//find the object that the current touch is hitting
	touchObject::TouchObjectWeakRef findTouchingObject(const ci::vec2 &touchPoint);
    
	//local variables
	static std::shared_ptr<class TouchManager>	mTouchManagerInstance;
	std::mutex									mTouchMapLock;
	std::map<int,TouchObject>					mTouchMap;
	std::deque<touchObject::TouchObjectWeakRef>	mRegisteredObjectsDeque;
	std::mutex									mRegisteredObjectLock;


	//RenderQueue - this pushes all touches (TUIO or otherwise) to the main thread
	std::deque<std::pair<TouchEventType, TouchObject>>		mTouchUpdateQueue;
	std::mutex									mUpdateMutex;
	void										updateTouches();
    
    //keep track of the most recent touch time
    float                                       mLatestTouchTime;
	ci::vec2									mScale;
};