#pragma once

#include "DataHandler.hpp"
#include "SettingsIni.hpp"

namespace ModCore
{
	class Main
	{
	public:

		static void Process(RE::FormID itemFormID = 0x0)
		{
			if (!SettingsIni::bEnabled) return;

			auto* player = RE::PlayerCharacter::GetSingleton();
			if (!player) return;

			auto* equippedForm = itemFormID ? RE::TESForm::LookupByID<RE::TESForm>(itemFormID) : nullptr;
			auto* equippedWeapon = equippedForm ? equippedForm->As<RE::TESObjectWEAP>() : nullptr;
			if (!equippedWeapon) return;

			TRACE("Equipped weapon was <\"{}\" [BASE:{:08X}]>", equippedWeapon->GetName(), equippedWeapon->formID);

			const auto weaponType = equippedWeapon->GetWeaponType();
			const bool isTwoHanded = weaponType == RE::WEAPON_TYPE::kTwoHandSword || weaponType == RE::WEAPON_TYPE::kTwoHandAxe;
			const bool isBow = weaponType == RE::WEAPON_TYPE::kBow || weaponType == RE::WEAPON_TYPE::kCrossbow;

			if (isTwoHanded && !SettingsIni::bApplyOnTwoHanded) return;
			if (isBow && !SettingsIni::bApplyOnBow) return;

			auto* lastOneHandItems = GetLastOneHandItems(player);
			if (!lastOneHandItems) return;

			// 0 = left hand, 1 = right hand
			for (int handIndex = 0; handIndex <= 1; ++handIndex) {
				auto* item = lastOneHandItems[handIndex];
				if (!item) continue;

				const bool isRightHand = (handIndex == 1);

				TRACE("Checking cached {} hand item <\"{}\" [BASE:{:08X}]>",
					isRightHand ? "right" : "left", item->GetName(), item->formID);

				if (isRightHand && !SettingsIni::bHandleRightHand) continue;
				if (!isRightHand && !SettingsIni::bHandleLeftHand) continue;

				if (item->IsWeapon() && !SettingsIni::bHandleWeapons) continue;
				if (item->IsArmor() && !SettingsIni::bHandleShields) continue;
				if (item->IsMagicItem() && !SettingsIni::bHandleSpells) continue;

				TRACE("  -> Clearing cached {} hand item", isRightHand ? "right" : "left");

				lastOneHandItems[handIndex] = nullptr;
			}
		}

	private:

		static RE::TESBoundObject** GetLastOneHandItems(RE::PlayerCharacter* player)
		{
			if (REL::Module::IsVR()) {
				auto* runtime = player->GetVRInfoRuntimeData();
				return runtime ? runtime->lastOneHandItems : nullptr;
			} else {
				auto& runtime = player->GetPlayerRuntimeData();
				return runtime.lastOneHandItems;
			}
		}
	};
};
