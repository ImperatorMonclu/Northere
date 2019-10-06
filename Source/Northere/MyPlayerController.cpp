#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;

}

void AMyPlayerController::MainMap()
{
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableTouchEvents = false;

}