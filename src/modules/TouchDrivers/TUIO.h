//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2014  
//	Developers: Paul Rudolph & Stacey Martens
//  Contents: Code for TUIO driver to be used with the touch manager class. Accepts touchevents and 2d object tracking.
//  Comments: This TUIO driver was based off concepts in Cinder's listenerBasic sample 
//----------------------------------------------------------------------------

#pragma once
#include "cinder/app/App.h"
#include "TuioClient.h"
#include "../../core/TouchManager.h"

class Tuio{
public:
	Tuio();
	~Tuio();
	void connect();
	void disconnect();

private:
	 //TUIO Event Callbacks
	cinder::tuio::Client	mTuioConnection;

	cinder::CallbackId		mTuioTouchBeganConnection,	//connections for touch events
							mTuioTouchMovedConnection,
							mTuioTouchEndedConnection,

							//connections for 2d object events
							mTuioObjectBeganConnection,	
							mTuioObjectMovedConnection,
							mTuioObjectEndedConnection;

   //any touches
	void					tuioTouchesBegan(cinder::app::TouchEvent event);
    void					tuioTouchesMoved(cinder::app::TouchEvent event);
    void					tuioTouchesEnded(cinder::app::TouchEvent event);
	
	//any 2d objects (for example a fiducial marker)
	void					tuioObjectBegan(cinder::tuio::Object event);
	void					tuioObjectMoved(cinder::tuio::Object event);
	void					tuioObjectEnded(cinder::tuio::Object event);


	//
	std::shared_ptr<class TouchManager>	mTouchManagerPntr;  
};