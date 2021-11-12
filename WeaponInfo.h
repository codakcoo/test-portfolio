// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "WeaponInfo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerMotion : uint8
{
	RifleState UMETA(DisplayName = "RifleMotion"),
	PistolState UMETA(DisplayName = "PistolMotion"),
	ShootgunState UMETA(DisplayName = "ShootgunMotion"),
	NoWeaponState UMETA(DisplayName = "NoWeaponMotion")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	MainWeapon_Type UMETA(DisplayName = "MainWeapon"),
	SubWeapon_Type UMETA(DisplayName = "SubWeapon"),
	NoWeapon_Type UMETA(DisplayName = "NoWeapon")
};


class PROJECT_API WeaponInfo
{
public:
	WeaponInfo();
	~WeaponInfo();
};

USTRUCT(Atomic, BlueprintType)
struct FWeaponInfo
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo")
		EWeaponType weaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo")
		EPlayerMotion playerMotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo")
		TSubclassOf<class AMasetPickUp_Weapon> weaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo")
		UTexture2D* weaponImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo")
		int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo")
		int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo")
		FName EquipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo")
		FName UnEquipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo")
		bool bAuto;
};
