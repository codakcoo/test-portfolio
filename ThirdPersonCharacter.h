// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponInfo.h"
#include "Components/TimelineComponent.h"
#include "ThirdPersonCharacter.generated.h"

class UCurveFloat;

typedef struct TraceInfo
{
	//히트액터 위치, 기준점따오기//
	FVector s_HitLocation;
	FVector s_HitNormal;

	FVector s_LedgeLocation;
	///////////////////////////////

	bool bBlockHit;
	bool ValutOn;
};

UCLASS()
class PROJECT_API AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdPersonCharacter();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;	//히트 가능한 오브젝트 유형들
	TArray<TEnumAsByte<ETraceTypeQuery>> TraceTypes;
	TArray<AActor*> IgnoreActors; //무시할 액터들.

public:
	//tps카메라 컴포넌트//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
	///////////////////////

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
		class UArrowComponent* TPSCameraLocation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
		class UArrowComponent* FPSCameraLocation;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AloowPrivateAccess = "true"))
		class UAnimMontage* Zoom_FireWeaponMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AloowPrivateAccess = "true"))
		class UAnimMontage* FireWeaponMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AloowPrivateAccess = "true"))
		class UAnimMontage* ReloadWeaponMontage;

	//낮은벽 몽타주//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AloowPrivateAccess = "true"))
		class UAnimMontage* Slim_RoWall_Montage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AloowPrivateAccess = "true"))
		class UAnimMontage* Wide_RoWall_Montage;
	////////////////

	//높은벽 몽타주//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AloowPrivateAccess = "true"))
		class UAnimMontage* HiWall_Montage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AloowPrivateAccess = "true"))
		class UAnimMontage* JumpWall_Montage;
	/////////////////


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AloowPrivateAccess = "true"))
		class UAnimMontage* LedgeClimbUp_Montage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AloowPrivateAccess = "true"))
		class UAnimMontage* LedgeTurnLeft_Montage;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK_Foot", meta = (AllowPrivateAccess = "true"))
		//class UFoot_IK* m_pIK_Foot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK_Foot", meta = (AllowPrivateAccess = "true"))
		class UIK_Foot* IK_Foot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool bZoomIn;

	UPROPERTY(VisibleAnywhere, Category = "PlayerUI")
		TSubclassOf<class UUserWidget> CrosshairWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "PlayerUI")
		class UUserWidget* CurrentWidget;



	//웨폰 변수//
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AMasetPickUp_Weapon* Main_Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AMasetPickUp_Weapon* Sub_Weapon;
	////////////

	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		EWeaponType WeaponTypeState;
	//플레이어 모션//
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		EPlayerMotion PlayerMotionState;
	/////////////////

public:
	//재장전 불리언//
	bool bReload;
	//클라임 불리언//
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool bLedge;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool bClimbOn;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool bCanLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool bCanRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool Left;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool Right;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool bTurnLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool bTurnRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool bAcross;
protected:
	//회전값//
	float BaseTurnRate;
	float BaseLookUpRate;
	//////////

	float InputZ;
	float InputX;

	//float Direction;
	
	FTimerHandle PlayerTimeHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* SwapWeaponMontage;

	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;

	UPROPERTY()
	 FTransform TPSTransform;
	UPROPERTY()
	 FTransform FPSTransform;


	//축 방향 이동/ 마우스포지션 회전//
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Rate);
	void LookUp(float Rate);
	///////////////////////////////////

	//점프//
	void JumpStart();
	void JumpEnd();
	////////

	//달리기//
	void RunStart();
	void RunEnd();

	//앉기//
	void DoCrouch();

public:
	//상호작용//
	void Interaction();
	///////////

	//웨폰 집기//
	void PickUp(AActor* MasterPickUp);
	void SwapMainWeapon();
	void SwapSubWeapon();

	bool CanMainWeaponMotion();
	bool CanSubWeaponMotion();

	void MainWeaponOrMotion();
	void SubWeaponOrMotion();
	/////////////

	void FireStart();
	void FireEnd();

	void ZoomIn();
	void ZoomOut();

	void Reload();
	void SetReload(bool boolean);
	bool GetReload();

	void Parkour(TraceInfo* traceInfo, ETraceTypeQuery TraceCannel, float LineLength, float LineHeight, float ParkourHeight, float StartLinePos = 0);
	void Parkour_SetTransform(FVector HitLocation, FVector HitNormal, float ZOffset = 0);
	void Parkour_SetMotion(EMovementMode MovementMove, ECollisionEnabled::Type NewType, bool bHitBlock, bool bAnim = false, bool bLedge = false, UAnimMontage* AnimMontage = NULL, FName Section = NAME_None);

	void OntheGround();
	void HoldingLedge();

	void Update_LedgeMoveCheck(FVector HitLocation);
	FVector MakeLocation(FVector HitLocation, FVector HitNormal, float NormalAngle, float XYDistance, float ZDistance);
	
	UFUNCTION()
	void TimelineProgress(float Value);


	void TestFunction(TraceInfo traceInfo);
};
