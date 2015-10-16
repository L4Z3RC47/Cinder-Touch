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
//				-Adds 2 ways to create a shape2d
//YOU CAN CREATE SHAPES FROM LOADING SVG!!!!! -- THIS IS NOT BUILT INTO THIS YET
//auto svg = svg::Doc::create( fsPath );
//Shape2d shape = svg->getShape();
//----------------------------------------------------------------------------

#pragma once
#include "cinder/app/App.h"
#include "cinder/Timeline.h" //allows for position/size variables can be animated easily
#include "cinder/Shape2d.h"

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
		
		//rect
		void						setup(bool registerWithTouchManager, const cinder::vec2 &pos, const cinder::vec2 &size = cinder::vec2(10.0f, 10.0f));
		//circle
		void						setup(bool registerWithTouchManager, const cinder::vec2 &pos, float radius = 10.0f);
		//random shape
		void						setup(bool registerWithTouchManager, const std::vector<cinder::vec2> &coordinates);
		
		void						createShape2d(const std::vector<cinder::vec2> &coordinates);
		cinder::Shape2d				getShape2d(){ return mShape2d; };

		//Drawing Functions
		virtual void				draw();
		//Draws an the outer box of the object, and the objects to string in the center, helpful for debugging pourposes.
		virtual void				drawDebugShape();


		//Positioning Functions
		//virtual void				setPosition ( const ci::vec2 &pt);
		//const cinder::vec2&			getPosition()										{ return mPosition; };

		void						setTranslationPosition(cinder::vec2 translatePoint){ mTranslationPosition = translatePoint; };

		//Size Functions
		virtual void                setSize(const ci::vec2 &size)						{ mWidth = size.x; mHeight = size.y; };
		const cinder::vec2			getSize()											{ return ci::vec2(mWidth, mHeight); };
		
		//Leave these in , they are not hurting anything
		float						getWidth()											{ return mWidth; };
		float						getHeight()											{ return mHeight; };
		
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

		const ci::vec2				getScale()											{ return mScale; };
		void						setScale(const cinder::vec2 &scale);

		//TOString - prints what the object actually is 
		virtual std::string			getDebugString();

		cinder::vec2				mScale;

protected://Only children of this class have access to these variables, to allow access use "->" acessor(i.e make an accessor method)
    

		ci::vec2					mTranslationPosition;

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
		//bool						mTranslating;
		//Object Identification 
		int							mUniqueID,
									mTouchesCallbackId;
	
		//std::shared_ptr<cinder::Shape2d> mShape2d; --try
		cinder::Shape2d				mShape2d;
		
		//STATIC CLASS MEMBERS
		//TotalObjectCount is used to count the number of Object instances for debugging purposes
		static int					TotalObjectCount;
		// ObjectID is used to generate new unique id's any time a gui object is created. 
		static int					ObjectID;
};

}//End BaseGuiObject namespace