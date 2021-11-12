// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS(config = Game)
class PROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

	

public:
	//ü�� ����//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Info")
		float hp = 100.f;
	//���� ����//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Info")
		bool bDead = false;
	//ü�»��� �Լ�//
	UFUNCTION(BlueprintCallable, Category = "Character Info")
		virtual void CalculateHp(float Delta);
	//�������� �Լ�//
	virtual void CalculateDead();
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
