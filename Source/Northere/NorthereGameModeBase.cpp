#include "NorthereGameModeBase.h"
#include "MyPlayerController.h"

ANorthereGameModeBase::ANorthereGameModeBase()
{
	PlayerControllerClass = AMyPlayerController::StaticClass();

}