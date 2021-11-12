// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol_type1_Weapon.h"
#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include <Engine/Classes/Camera/CameraComponent.h>

APistol_type1_Weapon::APistol_type1_Weapon()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> Pistol_type1(TEXT("SkeletalMesh'/Game/WeaponBundle/MarketplaceBlockout/Modern/Weapons/Assets/Pistols/01/SKM_Modern_Weapons_Pistol_01.SKM_Modern_Weapons_Pistol_01'"));
	if (Pistol_type1.Succeeded())
	{
		weaponMesh->SetSkeletalMesh(Pistol_type1.Object);
		//overlapCollision->SetGenerateOverlapEvents(true);
	}

	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrow->SetupAttachment(weaponMesh);
	arrow->bHiddenInGame = false;
	const FVector Location = FVector(0.044816f, 15.865077f, 7.105377f);
	const FRotator Rotation = FRotator(0.f, 90.f, 0.f);
	arrow->SetRelativeLocation(Location);
	arrow->SetRelativeRotation(Rotation);

	//무기 상항//
	weaponInfo.weaponType = EWeaponType::SubWeapon_Type;
	weaponInfo.playerMotion = EPlayerMotion::PistolState;
	weaponInfo.weaponClass = this->GetClass();
	weaponInfo.CurrentAmmo = 10;
	weaponInfo.MaxAmmo = Range(10, 25);
	weaponInfo.EquipSocketName = FName("Pistol_r");
	weaponInfo.UnEquipSocketName = FName("SubWeapon_holder");
	weaponInfo.bAuto = false;
	/////////////
}

void APistol_type1_Weapon::Fire()
{
	if (weaponInfo.CurrentAmmo > 0)
	{
		--weaponInfo.CurrentAmmo;
		FHitResult OutHit;
		FVector Start = arrow->GetComponentLocation();
		FVector Forward = Player->Camera->GetForwardVector();
		FVector End = (Start + (Forward * 10000.f));
		FCollisionQueryParams CollisionParams;

		if (Player->bZoomIn)
		{
			Player->PlayAnimMontage(Player->Zoom_FireWeaponMontage, 2.0f, "Pistol_Fire_Start");
		}
		else
		{
			//Player->PlayAnimMontage(Player->FireWeaponMontage, 2.0f)
		}
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);

		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

		GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Yellow, FString::Printf(TEXT("current ammo : %d"), weaponInfo.CurrentAmmo));

		if (isHit)
		{
			if (OutHit.bBlockingHit)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("You are hitting : %s"), *OutHit.GetActor()->GetName()));

				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("Impact Point : %s"), *OutHit.ImpactPoint.ToString()));

				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("You are hitting %s"), *OutHit.ImpactNormal.ToString()));
			}
		}
	}
	else
	{
		FireEnd();
	}
}