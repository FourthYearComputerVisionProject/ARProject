#include "stdafx.h"
#include "ControlManager.h"
//Mike's new stuff//


//Things I need:
//Reference to StereoViewer
//Collection of Applications

//Things I need to do:
//Start and spawn an OS app (constructor)
ControlManager::ControlManager(){};
//Switch out apps (either OS to other when other starts, or other to OS when other ends)
void ControlManager::closeApp(){
	if(/*App is not OS*/true){
		//Close app, open OS
	}else{

	}

};
//Pipe to StereoViewer
void ControlManager::display(){};