// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IK_Foot.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FHit_Value
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool s_bBlocking_Hit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float s_Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector s_Normal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float s_FootOffset_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float s_FootOffset_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator s_FootRotation_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator s_FootRotation_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float s_HipOffset;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_API UIK_Foot : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
		class ACharacter* m_pCharacter;

public:
	//로테이션값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		FRotator	c_pFootRotation_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		FRotator	c_pFootRotation_Right;

	//발 오프셋값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		float	c_pFootOffset_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values ")
		float	c_pFootOffset_Right;
	
	//힙 오프셋값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		float c_HipOffset;

	//회전율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		float InterpSpeed;

	//본네임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
		FName m_sIKSocketName_LeftFoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
		FName m_sIKSocketName_RightFoot;

public:	
	// Sets default values for this component's properties
	UIK_Foot();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FHit_Value Update_Distance(FName SocketName);
	FHit_Value Update_LineTrace(FName SocketName);

	void Set_SocketName(FString SocketName_Left, FString SocketName_Right);


	FRotator NormalToRotator(FVector pVector);;
	void Update_FootOffset(float* Current, float Target, float DeltaTime);
	void Update_FootRotation(FRotator* Current, FRotator Target, float DeltaTime);
};
