#include "Events.h"

namespace Events
{
	RE::BSEventNotifyControl ModEventSink::ProcessEvent(const RE::TESEquipEvent* event, RE::BSTEventSource<RE::TESEquipEvent>*)
	{
		if (!event || !event->equipped) return continueEvent;

		auto* actor = MiscUtils::ResolveHandle<RE::Actor>(event->actor);
		if (!actor || !actor->IsPlayerRef()) return continueEvent;

		ModCore::Main::Process(event->baseObject);

		return continueEvent;
	}
}
