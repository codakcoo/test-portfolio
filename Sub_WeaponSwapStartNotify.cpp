// Fill out your copyright notice in the Description page of Project Settings.


#include "Sub_WeaponSwapStartNotify.h"
#include "ThirdPersonCharacter.h"
#include "Engine.h"



void USub_WeaponSwapStartNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	GEngine->AddOnScreenDebugMessage(-1, 100, FColor::White, TEXT("Notify Start!!!"));

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		AThirdPersonCharacter* player = Cast<AThirdPersonCharacter>(MeshComp->GetOwner());
		
		if (player != NULL)
		{
			player->SubWeaponOrMotion();
		}

	}
}

FString USub_WeaponSwapStartNotify::GetNotifyName_Implementation() const
{
	return L"Swap_Pistol_Notify";
}
