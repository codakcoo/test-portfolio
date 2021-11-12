// Fill out your copyright notice in the Description page of Project Settings.


#include "IK_Foot.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UIK_Foot::UIK_Foot()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	m_pCharacter = Cast<ACharacter>(GetOwner());
	if (m_pCharacter == NULL) return;

	InterpSpeed = 0.f;
}


// Called when the game starts
void UIK_Foot::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UIK_Foot::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	m_pCharacter = NULL;
}


// Called every frame
void UIK_Foot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (m_pCharacter->GetCharacterMovement()->IsFalling() == false)
	{
		FHit_Value Hip_LeftValue = Update_Distance(m_sIKSocketName_LeftFoot);

		FHit_Value Hip_RightValue = Update_Distance(m_sIKSocketName_RightFoot);

		if (Hip_LeftValue.s_bBlocking_Hit || Hip_LeftValue.s_bBlocking_Hit)
		{
			//뭐가 더 높은지 측정//
			bool PickA = Hip_LeftValue.s_Distance >= Hip_RightValue.s_Distance;

			if (Hip_LeftValue.s_Distance >= Hip_RightValue.s_Distance)
			{
				float ReturnValue = Hip_LeftValue.s_Distance;
				float Target = (ReturnValue - 48.f) * -1;
				///////////////////////
				

				//왼발 로테이션 오프셋값 측정//
				c_HipOffset = UKismetMathLibrary::FInterpTo(c_HipOffset, Target, DeltaTime, InterpSpeed);
		
				FHit_Value Foot_LeftValue = Update_LineTrace(m_sIKSocketName_LeftFoot);

				Update_FootRotation(&c_pFootRotation_Left, NormalToRotator(Foot_LeftValue.s_Normal), DeltaTime);
				float TargetOffset_Left = (Foot_LeftValue.s_Distance - 110.f) / 45.f;
				Update_FootOffset(&c_pFootOffset_Left, TargetOffset_Left, DeltaTime);
				///////////////////////////////

				//오른발 로테이션 오프셋값 측정//
				FHit_Value Foot_RightValue = Update_LineTrace(m_sIKSocketName_RightFoot);

				Update_FootRotation(&c_pFootRotation_Right, NormalToRotator(Foot_LeftValue.s_Normal), DeltaTime);

				float TargetOffset_Right = (Foot_RightValue.s_Distance - 110.f) / 45.f;
				Update_FootOffset(&c_pFootOffset_Right, TargetOffset_Right, DeltaTime);
				/////////////////////////////////
			}
			else
			{
				float ReturnValue = Hip_RightValue.s_Distance;
				float Target = (ReturnValue - 48.f) * -1;
				///////////////////////
				

				//왼발 로테이션 오프셋값 측정//
				c_HipOffset = UKismetMathLibrary::FInterpTo(c_HipOffset, Target, DeltaTime, InterpSpeed);

				FHit_Value Foot_LeftValue = Update_LineTrace(m_sIKSocketName_LeftFoot);

				Update_FootRotation(&c_pFootRotation_Left, NormalToRotator(Foot_LeftValue.s_Normal), DeltaTime);
				float TargetOffset_Left = (Foot_LeftValue.s_Distance - 110.f) / 45.f;
				Update_FootOffset(&c_pFootOffset_Left, TargetOffset_Left, DeltaTime);
				///////////////////////////////

				//오른발 로테이션 오프셋값 측정//
				FHit_Value Foot_RightValue = Update_LineTrace(m_sIKSocketName_RightFoot);

				Update_FootRotation(&c_pFootRotation_Right, NormalToRotator(Foot_LeftValue.s_Normal), DeltaTime);

				float TargetOffset_Right = (Foot_RightValue.s_Distance - 110.f) / 45.f;
				Update_FootOffset(&c_pFootOffset_Right, TargetOffset_Right, DeltaTime);
				/////////////////////////////////
			}
			
		}
	}

}

//힙오프셋 측정//
FHit_Value UIK_Foot::Update_Distance(FName SocketName)
{
	FHit_Value Hit_Result;
	FVector Start = FVector(m_pCharacter->GetMesh()->GetSocketLocation(SocketName).X,
							m_pCharacter->GetMesh()->GetSocketLocation(SocketName).Y,
							(m_pCharacter->GetMesh()->GetComponentLocation().Z + 48.f));
	FVector End = Start - FVector(0, 0, 151.f);

	FHitResult OutHit;

	TArray<AActor*> IgnoreActors; //무시할 액터들.

	bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
	
	Hit_Result.s_bBlocking_Hit = OutHit.bBlockingHit;
	Hit_Result.s_Distance = OutHit.Distance;
	//Hit_Result.Normal = FVector(0.f, 0.f, 0.f);

	return Hit_Result;
}
//////////////////


//라인트레이스 측정//
FHit_Value UIK_Foot::Update_LineTrace(FName SocketName)
{
	FHit_Value Hit_Result;
	FVector Pos = FVector(m_pCharacter->GetMesh()->GetSocketLocation(SocketName).X,
						  m_pCharacter->GetMesh()->GetSocketLocation(SocketName).Y,
						  m_pCharacter->GetMesh()->GetSocketLocation("root").Z);
	FVector Start = Pos + FVector(0.f, 0.f, 100.f);
	FVector End = Pos - FVector(0.f, 0.f, 100.f);

	FHitResult OutHit;
	TArray<AActor*> IgnoreActors; //무시할 액터들.

	bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
	if (isHit)
	{
		Hit_Result.s_bBlocking_Hit = OutHit.bBlockingHit;
		Hit_Result.s_Distance = OutHit.Distance;
		Hit_Result.s_Normal = OutHit.Normal;
	}
	else
	{
		Hit_Result.s_bBlocking_Hit = OutHit.bBlockingHit;
		Hit_Result.s_Distance = 0.f;
		Hit_Result.s_Normal = FVector(0.f, 0.f, 0.f);
	}
	return Hit_Result;
}
///////////////////

//소켓 지정//
void UIK_Foot::Set_SocketName(FString SocketName_Left, FString SocketName_Right)
{
	m_sIKSocketName_LeftFoot = FName(*SocketName_Left);
	m_sIKSocketName_RightFoot = FName(*SocketName_Right);
}
////////////

FRotator UIK_Foot::NormalToRotator(FVector Vector)
{
	float Atan_Roll = UKismetMathLibrary::DegAtan2(Vector.Y, Vector.Z);
	float Atan_Picth = UKismetMathLibrary::DegAtan2(Vector.X, Vector.Z);
	Atan_Picth *= -1.f;

	FRotator Result = FRotator(Atan_Picth, 0.f, Atan_Roll);

	return Result;
}

void UIK_Foot::Update_FootOffset(float* Current, float Target, float DeltaTime)
{
	float InterpOffset = UKismetMathLibrary::FInterpTo(*Current, Target, DeltaTime, InterpSpeed);
	*Current = InterpOffset;
}

void UIK_Foot::Update_FootRotation(FRotator* Current, FRotator Target, float DeltaTime)
{
	FRotator InterpRotator = UKismetMathLibrary::RInterpTo(*Current, Target, DeltaTime, InterpSpeed);
	*Current = InterpRotator;
}
