// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasetPickUp_Weapon.h"
#include "M4_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AM4_Weapon : public AMasetPickUp_Weapon
{
	GENERATED_BODY()
	
public:
	AM4_Weapon();

	virtual void Fire() override;

	virtual void ReloadStart() override;
	virtual void Reload() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UArrowComponent* arrow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimationAsset* ReloadAnim;
};
