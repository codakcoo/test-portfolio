// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadAnimNotify.h"
#include "ThirdPersonCharacter.h"
#include "Engine.h"

FString UReloadAnimNotify::GetNotifyName_Implementation() const
{
	return L"Reload_Notify";
}

void UReloadAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	GEngine->AddOnScreenDebugMessage(-1, 100, FColor::White, TEXT("Notify Start!!!"));

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		AThirdPersonCharacter* player = Cast<AThirdPersonCharacter>(MeshComp->GetOwner());

		if (player != NULL)
		{
			if (player->CanMainWeaponMotion() || player->CanSubWeaponMotion())
			{
				player->SetReload(false);
			}
		}

	}
}
