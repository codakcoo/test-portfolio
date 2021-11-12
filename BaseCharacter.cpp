// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

void ABaseCharacter::CalculateHp(float Delta)
{
	hp += Delta;
	CalculateDead();
}

void ABaseCharacter::CalculateDead()
{
	if (hp <= 0)
	{
		bDead = true;
	}
	else
	{
		bDead = false;
	}
}

#if WITH_EDITOR
void ABaseCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	bDead = false;
	hp = 100.f;

	Super::PostEditChangeProperty(PropertyChangedEvent);

	CalculateDead();
}
#endif

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

