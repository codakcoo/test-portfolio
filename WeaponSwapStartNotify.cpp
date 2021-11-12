// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSwapStartNotify.h"
#include "ThirdPersonCharacter.h"
#include "Engine.h"




void UMain_WeaponSwapStartNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Yellow, TEXT("NotifyStart"));

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		AThirdPersonCharacter* player = Cast<AThirdPersonCharacter>(MeshComp->GetOwner());
		
		if (player != NULL)
		{
			player->MainWeaponOrMotion();
		}
	}
}

FString UMain_WeaponSwapStartNotify::GetNotifyName_Implementation() const
{
	return L"Swap_Rifle_Notify";
}