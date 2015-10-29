/**+---------------------------------------------------------------------------
Bluecadet Interactive 2015
Developers: Paul Rudolph & Stacey Martens
Contents: 
Comments:
1. In setup, create each Path based on a (0,0) coordinate.
2. Move the Path to the desired starting location on screen by setting mPosition.
3. If you are translating the space, you have to update mTranslationPos by calling setTranslationPos in the draw function.
----------------------------------------------------------------------------*/

#pragma once
#include "cinder/app/App.h"
#include "cinder/Timeline.h"
#include "cinder/Shape2d.h"

namespace touchObject {
	
	typedef std::shared_ptr<class		BaseTouchObject>		TouchObjectRef;
	typedef std::shared_ptr<const class BaseTouchObject>		TouchObjectConstRef;
	typedef std::weak_ptr  <class		BaseTouchObject>		TouchObjectWeakRef;

	typedef std::deque     <TouchObjectWeakRef>					TouchObjectList;
	typedef std::map       <int, TouchObjectWeakRef>			TouchObjectMap;

	//! TouchType options. Touch = fingers on a screen, object = fiducial, mouse = mouse 
	enum TouchType { TOUCH, OBJECT, MOUSE };

	class BaseTouchObject : public std::enable_shared_from_this<BaseTouchObject> {
   
	public:

		BaseTouchObject();
		//! Virtual destructor will  call the subclass destructor as well
		virtual ~BaseTouchObject();
		
		//! Setting up base touch object as a rectangle
		void						setupBaseTouchObj(const cinder::vec2 &pos = cinder::vec2(0), const cinder::vec2 &size = cinder::vec2(10.0f, 10.0f), bool registerWithTouchManager = true);
		//! Setting up base touch object as a circle
		void						setupBaseTouchObj(const cinder::vec2 &pos = cinder::vec2(0), float radius = 10.0, bool registerWithTouchManager = true);
		//! Setting up base touch object as random shape
		void						setupBaseTouchObj(const std::vector<cinder::vec2> &coordinates, const cinder::vec2 &pos = cinder::vec2(0), bool registerWithTouchManager = true);
		
		//! Iterate through coordinates passed in. Create Path2d of the touchable area.
		void						createShape(const std::vector<cinder::vec2> &coordinates);
		cinder::Path2d				getPath(){ return mPath; };

		//! Will call do draw the debug shape as placeholder draw() function until you fill the shape with what you want
		virtual void				draw();
		//! Draws an the outer box of the object, and the objects to string in the center, helpful for debugging pourposes.
		virtual void				drawDebugShape();

		//! Sets position
		virtual void				setPosition(const ci::vec2 &pt)						{ mPosition = pt; };
		//! Returns position
		const cinder::vec2&			getPosition()										{ return mPosition; };
		//! Accepts the translation position, if this object or any parent above it is translating their space
		void						setTranslationPos(cinder::vec2 newTranslationPos)	{ mTranslationPos = newTranslationPos; }
		//! Returns width as float
		float						getWidth()											{ return mPath.calcBoundingBox().getWidth(); };
		//! Returns height as float
		float						getHeight()											{ return mPath.calcBoundingBox().getHeight(); };
		//! Sets ColorA of object
		virtual void                setObjectColor(  const cinder::ColorA &color )		{ mObjectColor = color; };
		//! Returns object color
		const cinder::ColorA&       getObjectColor()									{ return mObjectColor; };
	
		//! Set whether or not should currently accept touch. Can be turned on/off
		virtual void                setAcceptTouch(bool state)							{ mAcceptTouch = state; };
		//! Returns if this object is accepting touch
		bool                        isAcceptingTouch()									{ return mAcceptTouch; }
    
		//! Returns the unique ID tag that is set upon creation of the touch object
		const int&                  getUniqueID()										{ return mUniqueID; }

		//! Register with touch manager to receive touches
		virtual void                registerWithTouchMngr();
		//! Unregister with touch manager, will no longer receive touches
		virtual void                unRegisterWithTouchMngr();
		//! Returns whether or not this object should accept the touch point
		virtual bool                hasTouchPoint(  const ci::vec2 &pnt );
		//! Returns the total number of touches currently within the object
		int                         getNumTouches()										{ return mObjectTouchIDs.size(); }
		//! Remove whatever touches are currently within the object
		virtual void				endTouches();
		
		//! 
		virtual	void				touchesBeganHandler(int touchID, const ci::vec2 &touchPnt, TouchType touchType){};
		virtual void				touchesMovedHandler(int touchID, const ci::vec2 &touchPnt, TouchType touchType){};
		virtual void				touchesEndedHandler(int touchID, const ci::vec2 &touchPnt, TouchType touchType){};

		//! Sets scale of touchable object. Everything is scaled around the center.
		void						setScale(const cinder::vec2 &scale);
		//! Returns scale of touchable object
		const ci::vec2				getScale()											{ return mScale; };
		
		//! Returns label including Unique ID
		virtual std::string			getDebugString();
		
		//! Scale as vector 
		cinder::vec2				mScale;

//! Only children of this class have access to these variables, to allow access use "->" acessor (i.e make an accessor method)
protected:
		ci::vec2					mPosition, 
									mTranslationPos;
		float						mWidth,
									mHeight;
		cinder::ColorA				mObjectColor;
		//! String for debug purposes. If set this can be used to identify the object.
		std::string					mToString;
		//! Vector containing the touch IDs of the touches within this object
		std::vector<int>			mObjectTouchIDs;
		//! UniqueIDLookupMap can be used to quickly find an object by ID
		//static TouchObjectMap		UniqueIDLookupMap;
	
//! No one other than this class can access these variables
private:

		bool						mAcceptTouch;
		//! Object identification 
		int							mUniqueID;
		//! Coordinates of the object 
		cinder::Path2d				mPath;
		
		//! STATIC CLASS MEMBERS

		//! TotalObjectCount is used to count the number of Object instances for debugging purposes
		//! Total object count is a value that is incremented when an object is created and decremented when an object is destroyed. This way we always know how many objects exist.
		static int					TotalObjectCount;

		//! ObjectID is used to generate new unique ID's anytime an object is created. 
		static int					ObjectID;
};

}//End touchobject namespace