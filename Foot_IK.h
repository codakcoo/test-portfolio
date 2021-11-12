// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Foot_IK.generated.h"

typedef struct ST_IK_TraceInfo
{
	float	fOffset;
	FVector pImpactLocation;
};

USTRUCT(Atomic, BlueprintType)
struct FST_IK_AnimValue
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float		m_fEffectorLocation_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float		m_fEffectorLocation_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator	m_pFootRotation_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator	m_pFootRotation_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float		m_fHipOffset;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_API UFoot_IK : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties

private:
	UPROPERTY()
		class ACharacter* m_pCharacter;
private:
	//! Owner Characters capsule height
	float	m_fIKCapsuleHalkHeight;

	//! IK Anim Instance Value struct
	FST_IK_AnimValue m_stIKAnimValuse;

	//! IK Active state
	bool m_bActive = false;

	float m_fDeltaTime = 0.0f;

public:
	//! Default Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
		FRotator	m_pFootRotation_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
		FRotator	m_pFootRotation_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
		float	m_pFootOffset_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
		float	m_pFootOffset_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
		float	m_fHipOffset;

	//! Left foot bone name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
		FName m_sIKSocketName_LeftFoot;
	//! Right foot bone name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
		FName m_sIKSocketName_RightFoot;

	//! Distance between the floor and the foot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		float m_IKAdjustOffset;

	//! Max Linetrace distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		float m_fIKTraceDistance;

	//! Hip(pelvis) move speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		float m_fIKHipsInterpSpeed;

	//! Leg joing move speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
		float m_fIKFeetInterpSpeed;

	//! Enable debug mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Debug")
		bool m_bDebug;

public:	
	UFoot_IK();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//! Set foot bone name from skeletalmesh
	void Set_IKSocketName(FString sSocName_LeftFoot, FString sSocName_RightFoot);

	//! Reset IK State
	void IK_ResetVars();

	//! Set IK Active state
	void SetIKActive(bool bActive);

	//! Set IK Debug state
	void SetIKDebug(bool bActive);

private:
	//! IK process update
	void IK_Update(float fDeltaTime);

	//! IK Debug mode update
	void IK_Debug();

	//! process IK Foot,leg Z Offset
	void IK_Update_FootOffset(float fDeltaTime, float fTargetValue, float* fEffectorValue, float fInterpSpeed);

	//! Process IK Foot rotation from ground normal
	void IK_Update_FootRotation(float fDeltaTime, FRotator pTargetValue, FRotator* pFootRotatorValue, float fInterpSpeed);

	//! Process IK characters capsule component Z Offset
	void IK_Update_CapsuleHalfHeight(float fDeltaTime, float fHipsShifts, bool bResetDefault);

	//! Get foot line trace info
	ST_IK_TraceInfo IK_FootTrace(float fTraceDistance, FName sSocket);

	//! Get ground normal
	FRotator NormalToRotator(FVector pVector);

	//! Check owner character is moving
	bool IsMoving();

public:
	FORCEINLINE FST_IK_AnimValue GetIKAnimValue() { return m_stIKAnimValuse; }
	FORCEINLINE bool GetIKDebugState() { return m_bDebug; }
};
