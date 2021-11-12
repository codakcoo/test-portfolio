// Fill out your copyright notice in the Description page of Project Settings.


#include "M4_Weapon.h"
#include <Engine/Classes/Camera/CameraComponent.h>
#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"

AM4_Weapon::AM4_Weapon()
{
	//메쉬 or 애니메이션 가져오기//
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> M4_MeshObj(TEXT("SkeletalMesh'/Game/WeaponBundle/MarketplaceBlockout/Modern/Weapons/Assets/Rifles/01/SKM_Modern_Weapons_Rifle_01.SKM_Modern_Weapons_Rifle_01'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> M4_ReloadObj(TEXT("AnimSequence'/Game/WeaponBundle/MarketplaceBlockout/Modern/Weapons/Assets/Rifles/01/ANI_Modern_Weapons_Rifle_01_Reload.ANI_Modern_Weapons_Rifle_01_Reload'"));
	if (M4_MeshObj.Succeeded())
	{
		weaponMesh->SetSkeletalMesh(M4_MeshObj.Object, true);
		if (M4_ReloadObj.Succeeded())
		{
			ReloadAnim = M4_ReloadObj.Object;
		}
	}
	/////////////////////////////////
	
	//머즐//
	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrow->SetupAttachment(weaponMesh);
	arrow->bHiddenInGame = false;
	const FVector Location = FVector(-0.028327f, 56.605316f, 9.45694f);
	const FRotator Rotation = FRotator(0.f, 90.f, 0.f);
	arrow->SetRelativeLocation(Location);
	arrow->SetRelativeRotation(Rotation);
	//////

	//무기 상항//
	weaponInfo.weaponType = EWeaponType::MainWeapon_Type;
	weaponInfo.playerMotion = EPlayerMotion::RifleState;
	weaponInfo.weaponClass = this->GetClass();
	weaponInfo.CurrentAmmo = 30;
	weaponInfo.MaxAmmo = Range(30, 60);
	weaponInfo.EquipSocketName = FName("Rifle_r");
	weaponInfo.UnEquipSocketName = FName("MainWeapon_holder");
	weaponInfo.bAuto = true;
	/////////////
}

void AM4_Weapon::Fire()
{
	if (weaponInfo.CurrentAmmo > 0 && !Player->GetReload())
	{
		--weaponInfo.CurrentAmmo;
		FHitResult OutHit;
		FVector Start = arrow->GetComponentLocation();
		FVector Forward = Player->Camera->GetForwardVector();
		FVector End = (Start + (Forward * 10000.f));
		FCollisionQueryParams CollisionParams;

		if (Player->bZoomIn)
		{
			Player->PlayAnimMontage(Player->Zoom_FireWeaponMontage, 0.5f, "Rifle_Fire_Start");
		}
		else
		{
			Player->PlayAnimMontage(Player->FireWeaponMontage, 0.5f, "Rifle_Fire_Start");
		}
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);
		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, FString::Printf(TEXT("current ammo : %d"), weaponInfo.CurrentAmmo));
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, FString::Printf(TEXT("Max ammo : %d"), weaponInfo.MaxAmmo));

		if (isHit)
		{
			if (OutHit.bBlockingHit)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("You are hitting : %s"), *OutHit.GetActor()->GetName()));

				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("Impact Point : %s"), *OutHit.ImpactPoint.ToString()));

				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("You are hitting %s"), *OutHit.ImpactNormal.ToString()));
			}
		}
	}
	else if(weaponInfo.MaxAmmo > 0)
	{
		ReloadStart();
	}
}

void AM4_Weapon::ReloadStart()
{
	if (Player != NULL)
	{
		if (weaponInfo.CurrentAmmo < 30 && weaponInfo.MaxAmmo > 0)
		{
			GetWorldTimerManager().ClearTimer(FireTimeHandle);
			Player->PlayAnimMontage(Player->ReloadWeaponMontage, 1.0f, "Rifle_Reload_Start");
			Player->SetReload(true);
			weaponMesh->PlayAnimation(ReloadAnim, false);
			
			if (weaponInfo.CurrentAmmo + weaponInfo.MaxAmmo <= 30)
			{
				weaponInfo.CurrentAmmo += weaponInfo.MaxAmmo;
				weaponInfo.MaxAmmo = 0;
			}
			else
			{
				weaponInfo.MaxAmmo -= (30 - weaponInfo.CurrentAmmo);
				weaponInfo.CurrentAmmo += (30-weaponInfo.CurrentAmmo);
			}
			
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("Reload success late MaxAmmo : %d"), weaponInfo.MaxAmmo));
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("Reload success late CurrentAmmo : %d"), weaponInfo.CurrentAmmo));
		}
	}
}

void AM4_Weapon::Reload()
{
}
