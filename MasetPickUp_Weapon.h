// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThirdPersonCharacter.h"
#include "WeaponInfo.h"
#include "MasetPickUp_Weapon.generated.h"

UCLASS()
class PROJECT_API AMasetPickUp_Weapon : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AMasetPickUp_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	FTimerHandle FireTimeHandle;

public:
	//웨폰 구조체//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo", meta = (AllowPrivateAccess = "true"))
	FWeaponInfo weaponInfo;
	///////////////

public:
	//픽업 웨폰//
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* weaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* overlapCollision;
	
	/********************************************************************
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UWidgetComponent* widget;
	********************************************************************/

	/*매개변수*/
	//캐릭터 ref//
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AThirdPersonCharacter* Player;

	//픽업 불리언//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanWePick;
	///////////////


public:
	UFUNCTION()
		void TriggerEnter(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void TriggerExit(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void OnOff_VisibiltyCustom_Depth(bool newVisibilty, bool Value);
	void OffAll();
	
	virtual void FireStart(AThirdPersonCharacter* player);
	virtual void FireEnd();
	virtual void Fire();
	virtual void ReloadStart();
	virtual void Reload();

	//랜덤 함수 : Range(최소, 최대)
	int Range(int min, int max);
};
