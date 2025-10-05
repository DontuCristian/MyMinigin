#include "MuteCommand.h"
#include "ServiceLocator.h"
#include "SoundService.h"
#include "Null_SoundService.h"
#include "SDL_SoundSystem.h"
#include "Logging_SoundSystem.h"

void dae::MuteCommand::Execute()
{
	//ServiceLocator::GetSoundService().ToggleMute();
}
