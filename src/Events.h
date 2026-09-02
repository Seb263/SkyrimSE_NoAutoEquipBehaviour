#pragma once

#include "DataHandler.hpp"
#include "SettingsIni.hpp"

#include "Core/Main.hpp"

#include "Utils/MiscUtils.hpp"

namespace Events
{
	class ModEventSink :
		public RE::BSTEventSink<RE::TESEquipEvent>
	{
		ModEventSink() = default;
		ModEventSink(const ModEventSink&) = delete;
		ModEventSink(ModEventSink&&) = delete;
		ModEventSink& operator=(const ModEventSink&) = delete;
		ModEventSink& operator=(ModEventSink&&) = delete;

	public:
		#define continueEvent RE::BSEventNotifyControl::kContinue

		static ModEventSink* GetSingleton()
		{
			static ModEventSink singleton;
			return &singleton;
		}

		static void LoadEvents()
		{
			auto* eventSink = GetSingleton();
			auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
			eventSourceHolder->AddEventSink<RE::TESEquipEvent>(eventSink);
		}

		RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent* event, RE::BSTEventSource<RE::TESEquipEvent>*);
	};
};
