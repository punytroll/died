#include "Common.h"

std::string DiED::sStatusToString(const DiED::clientstatus_t & Status)
{
	switch(Status)
	{
	case DiED::Connected:
		return "Connected";
	case DiED::Disconnected:
		return "Disconnected";
	case DiED::Connecting:
		return "Connecting";
	case DiED::Deleted:
		return "Deleted";
	default:
		return "Unknown";
	}
}
