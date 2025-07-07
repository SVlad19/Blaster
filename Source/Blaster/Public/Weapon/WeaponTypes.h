#pragma once

constexpr float TRACE_LENGTH = 80000.f;
constexpr uint32 CUSTOM_DEPTH_PURPLE = 250;
constexpr uint32 CUSTOM_DEPTH_BLUE = 251;
constexpr uint32 CUSTOM_DEPTH_TAN = 252;

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),
	EWT_SubmachineGun UMETA(DisplayName = "Submachine Gun"),
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),
	EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),
	EWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),

	EWT_MAX UMETA(DisplayName = "DefaultMax")
};