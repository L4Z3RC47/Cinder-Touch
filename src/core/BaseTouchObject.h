//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2014
//	Developers: Paul Rudolph & Stacey Martens
//  Contents: 
//  Comments: 
//				-Setup function is no longer virtual since any subclass with create their own anyway
//				-Removed color from being passed in setup, leaving the due to debugging purposes
//				-Removed set offset function
//				-Removed Axis Locking
//				-Removed Visibility variable
//----------------------------------------------------------------------------

#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/Timeline.h" //allows for position/size variables can be animated easily

namespace touchObject {
	
	typedef std::shared_ptr<class		BaseTouchObject>		TouchObjectRef;
	typedef std::shared_ptr<const class BaseTouchObject>		TouchObjectConstRef;
	typedef std::weak_ptr  <class		BaseTouchObject>		TouchObjectWeakRef;

	typedef std::deque     <TouchObjectRef>						TouchObjectList;
	typedef std::map       <int, TouchObjectWeakRef>			TouchObjectMap;

	enum TouchType { TOUCH, OBJECT, MOUSE };
	enum CoordinateSpace { LOCAL, GLOBAL };
	class BaseTouchObject : public std::enable_shared_from_this<BaseTouchObject> {
   
	public:

		BaseTouchObject();
		virtual ~BaseTouchObject(); //Virtual destructor will  call the subclass dtor aswell
		
		void						setup(const cinder::Vec2f &pos, const cinder::Vec2f &size);
		
		
		//Drawing Functions
		virtual void	draw(cinder::Vec2f translationOffset = cinder::Vec2f::zero());
		//Draws an the outer box of the object, and the objects to string in the center, helpful for debugging pourposes.
		virtual void				drawDebugBox();
    

		//Positioning Functions
		virtual void				setPosition ( const cinder::Vec2f &pt);
		const cinder::Vec2f&		getPosition()										{ return mPosition; };
		const cinder::Vec2f		    getGlobalPosition()									{ return mPosition + mParentPosition; };

		void						setParentPosition(cinder::Vec2f parentPoint)		{ mParentPosition = parentPoint; };
		const cinder::Vec2f&		getParentPosition()									{ return mParentPosition; };

		//Size Functions
		virtual void                setSize(const cinder::Vec2f &size)					{ mWidth = size.x; mHeight = size.y; };
		const cinder::Vec2f			getSize()											{ return ci::Vec2f(mWidth, mHeight); };
		

		virtual float				getWidth()											{ return mWidth; };
		virtual float				getHeight()											{ return mHeight; };
		
		//Get position and size as a rectObject
		const cinder::Rectf 		getRect(CoordinateSpace coordinateSpace){
			//for drawing, we need to be able to draw these differently depening on if the object is already being translated. 
			//But for finding if a touch point exists within this space, we need to know the global location -- so that is handled separately
			if (coordinateSpace == LOCAL){
				return  cinder::Rectf(
					mPosition.x ,
					mPosition.y ,
					mPosition.x + mWidth*mScale.x,
					mPosition.y + mHeight*mScale.y);
			}else{
				return  cinder::Rectf(
					mPosition.x + mParentPosition.x,
					mPosition.y + mParentPosition.y,
					mPosition.x + mParentPosition.x + mWidth*mScale.x,
					mPosition.y + mParentPosition.y + mHeight*mScale.y);
			}
		}
	
		const cinder::Vec2f			getCenter()											{ return getRect(LOCAL).getCenter();}
    
		//Color
		virtual void                setObjectColor(  const cinder::ColorA &color )		{ mObjectColor = color; };
		const cinder::ColorA&       getObjectColor()									{ return mObjectColor; };
	
		//Accept Touch
		virtual void                setAcceptTouch(bool state)							{ mAcceptTouch = state; };
		bool                        isAcceptingTouch()									{ return mAcceptTouch; }
    
		//ID TAG
		const int&                  getUniqueID()										{ return mUniqueID; }

		//Register with Touch manager to recieve touches
		virtual void                registerWithTouchMngr();
		virtual void                unRegisterWithTouchMngr();
	 
		virtual bool                hasTouchPoint(  const cinder::Vec2f &pnt );
		int                         getNumTouches()										{ return mObjectTouchIDs.size(); }
	
		virtual void				endTouches();
		
		
		virtual	void				touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType ){};
		virtual void				touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){};
		virtual void				touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){};

		const cinder::Vec2f			getScale()											{ return mScale; };
		void						setScale(cinder::Vec2f scale)						{ mScale = scale; };

		//TOString - prints what the object actually is 
		virtual std::string			getDebugString();

		

protected://Only children of this class have access to these variables, to allow access use "->" acessor(i.e make an accessor method)
    

		cinder::Vec2f				mPosition, mParentPosition, mScale;

		float						mWidth,
									mHeight;
    
		//Object color 
		cinder::ColorA				mObjectColor;

		
		//String for debug purposes if set, this can be used to identify the object
		std::string					mToString;

		//A vector containing the touch ids of the touches objects that are touching the guiObject
		std::vector<int>			mObjectTouchIDs;
		
		// uniqueIDLookup allows us to quickly find a Node by id
		static TouchObjectMap		UniqueIDLookupMap;
	
private://No one other than this class can access these variables


		bool						mAcceptTouch;
	
		//Object Identification 
		int							mUniqueID,
									mTouchesCallbackId;
	
		
	
		//STATIC CLASS MEMBERS
		//TotalObjectCount is used to count the number of Object instances for debugging purposes
		static int					TotalObjectCount;
		// ObjectID is used to generate new unique id's any time a gui object is created. 
		static int					ObjectID;
};

}//End BaseGuiObject namespace