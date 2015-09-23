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
#include "cinder/app/App.h"
#include "cinder/Timeline.h" //allows for position/size variables can be animated easily

namespace touchObject {
	
	typedef std::shared_ptr<class		BaseTouchObject>		TouchObjectRef;
	typedef std::shared_ptr<const class BaseTouchObject>		TouchObjectConstRef;
	typedef std::weak_ptr  <class		BaseTouchObject>		TouchObjectWeakRef;

	typedef std::deque     <TouchObjectRef>						TouchObjectList;
	typedef std::map       <int, TouchObjectWeakRef>			TouchObjectMap;

	enum TouchType { TOUCH, OBJECT, MOUSE };

	class BaseTouchObject : public std::enable_shared_from_this<BaseTouchObject> {
   
	public:

		BaseTouchObject();
		virtual ~BaseTouchObject(); //Virtual destructor will  call the subclass dtor aswell
		
		void						setup(const ci::vec2 &pos, const ci::vec2 &size);
		
		
		//Drawing Functions
		virtual void				draw();
		//Draws an the outer box of the object, and the objects to string in the center, helpful for debugging pourposes.
		virtual void				drawDebugBox(bool isTranslating = false);
    

		//Positioning Functions
		virtual void				setPosition ( const ci::vec2 &pt);
		const cinder::vec2&			getPosition()										{ return mPosition; };

		void						setParentTranslatePosition(ci::vec2 translatePoint){ mParentTranslatePosition = translatePoint; };

		//Size Functions
		virtual void                setSize(const ci::vec2 &size)					{ mWidth = size.x; mHeight = size.y; };
		const cinder::vec2			getSize()											{ return ci::vec2(mWidth, mHeight); };
		
		//Leave these in , they are not hurting anything
		float						getWidth()											{ return mWidth; };
		float						getHeight()											{ return mHeight; };
		
		//Get position and size as a rectObject
		const cinder::Rectf 		getRect(){ 
			//for drawing, we need to be able to draw these differently depening on if the object is already being translated. 
			//But for finding if a touch point exists within this space, we need to know the global location -- so that is handled separately
			if (mTranslating){
				return  cinder::Rectf(
					0.0f,
					0.0f,
					mWidth,
					mHeight);
			}
			else{
				return  cinder::Rectf(
					mPosition.x,
					mPosition.y,
					mPosition.x + mWidth,
					mPosition.y + mHeight);
			}
		}
		const void					setTranslating(bool isTranslating)					{ mTranslating = isTranslating; };

		const ci::vec2				getCenter()											{ return getRect().getCenter();}
    
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
	 
		virtual bool                hasTouchPoint(  const ci::vec2 &pnt );
		int                         getNumTouches()										{ return mObjectTouchIDs.size(); }
	
		virtual void				endTouches();
		
		
		virtual	void				touchesBeganHandler(int touchID, const ci::vec2 &touchPnt, TouchType touchType){};
		virtual void				touchesMovedHandler(int touchID, const ci::vec2 &touchPnt, TouchType touchType){};
		virtual void				touchesEndedHandler(int touchID, const ci::vec2 &touchPnt, TouchType touchType){};

		const ci::vec2					getScale()									{ return mScale; };
		void						setScale(ci::vec2 scale)						{ mScale = scale; };

		//TOString - prints what the object actually is 
		virtual std::string			getDebugString();


protected://Only children of this class have access to these variables, to allow access use "->" acessor(i.e make an accessor method)
    

		ci::vec2					mPosition, mParentTranslatePosition, mScale;

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
		bool						mTranslating;
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