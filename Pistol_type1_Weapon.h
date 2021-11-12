// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasetPickUp_Weapon.h"
#include "Pistol_type1_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API APistol_type1_Weapon : public AMasetPickUp_Weapon
{
	GENERATED_BODY()

public:
	APistol_type1_Weapon();

	virtual void Fire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UArrowComponent* arrow;
};
