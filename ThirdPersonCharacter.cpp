// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCharacter.h"
#include "MasetPickUp_Weapon.h"
#include "Foot_IK.h"
#include "IK_Foot.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TimelineComponent.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include <Engine/Classes/Components/CapsuleComponent.h>
#include <Engine/Classes/GameFramework/CharacterMovementComponent.h>
#include <Engine/Classes/Camera/CameraComponent.h>
#include <Engine/Classes/GameFramework/SpringArmComponent.h>

// Sets default values
AThirdPersonCharacter::AThirdPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(30.6f, 94.f);
	GetCapsuleComponent()->bHiddenInGame = false;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	//스켈레톤 메쉬 블러오기//
	ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshObj(TEXT("SkeletalMesh'/Game/Animation/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	ConstructorHelpers::FClassFinder<UAnimInstance> ThirdPersonAnimObj(TEXT("AnimBlueprint'/Game/Animation/PlayerCharacter_Anim_BP.PlayerCharacter_Anim_BP_C'"));
	if (BodyMeshObj.Succeeded())
	{
		const FVector Position = FVector(0.f, 0.f, -97.f);
		const FRotator Rotation = FRotator(0.f, 270.f, 0.f);
		const FVector Scale = FVector(1.f, 1.f, 1.f);
		FTransform Transform = FTransform(Rotation, Position, Scale);

		GetMesh()->SetSkeletalMesh(BodyMeshObj.Object, true);
		GetMesh()->SetRelativeTransform(Transform);
		if (ThirdPersonAnimObj.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(ThirdPersonAnimObj.Class);
		}
	}
	//////////////////////////

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540.f, 0);
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 50.f;
	GetCharacterMovement()->MaxFlySpeed = 150.f;

	//카메라 위치//
	const FVector Location = FVector(0.f, 80.f, 10.f);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPS_SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 350.f;
	//SpringArm->bEnableCameraLag = true;
	//SpringArm->bEnableCameraRotationLag = true;
	//SpringArm->CameraLagSpeed = 70.f;
	//SpringArm->CameraRotationLagSpeed = 50.f;
	//SpringArm->CameraLagMaxDistance = 10.f;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = true;
	Camera->SetRelativeLocation(Location);

	
	FVector TPSArrowLocation = FVector(0.f, 80.f, 10.f);
	TPSCameraLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("TPS_View"));
	TPSCameraLocation->SetupAttachment(RootComponent);
	TPSCameraLocation->SetRelativeLocation(TPSArrowLocation);
	TPSCameraLocation->bHiddenInGame = true;

	FVector FPSArrowLocation = FVector(-70.5f, 74.6f, 56.4f);
	FPSCameraLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("FPS_View"));
	FPSCameraLocation->SetupAttachment(RootComponent);
	FPSCameraLocation->SetRelativeLocation(FPSArrowLocation);
	FPSCameraLocation->bHiddenInGame = true;

	IK_Foot = CreateDefaultSubobject<UIK_Foot>(TEXT("IK_Foot"));
	IK_Foot->Set_SocketName(("foot_l"), ("foot_r"));

	//애님 몽타주//
	/*무기 스왑 몽타지*/
	static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponSwapObj(TEXT("AnimMontage'/Game/Animation/Montage/Swap_Weapon_Montage.Swap_Weapon_Montage'"));
	/*무기 발사 몽타지*/
	static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponFireObj(TEXT("AnimMontage'/Game/Animation/Montage/Fire_Weapon_Montage.Fire_Weapon_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ZoomWeaponFireObj(TEXT("AnimMontage'/Game/Animation/Montage/Zoom_Fire_Weapon_Montage.Zoom_Fire_Weapon_Montage'"));
	/*무기 리로드 몽타지*/
	static ConstructorHelpers::FObjectFinder<UAnimMontage> WeaponReloadObj(TEXT("AnimMontage'/Game/Animation/Montage/Reload_Weapon_Montage.Reload_Weapon_Montage'"));
	/*파쿠르 몽타지*/
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Slim_RoWall_Obj(TEXT("AnimMontage'/Game/Animation/Montage/Vault_Montage.Vault_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Wide_RoWall_Obj(TEXT("AnimMontage'/Game/Animation/Montage/Wall_On_Vault_Montage.Wall_On_Vault_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HiWall_Obj(TEXT("AnimMontage'/Game/Animation/Montage/Wall_Vault_Montage.Wall_Vault_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> JumpWall_Obj(TEXT("AnimMontage'/Game/Animation/Montage/Jump_Wall_Montage.Jump_Wall_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LedgeClimbUp_Obj(TEXT("AnimMontage'/Game/Animation/Montage/Ledge_Climb_Up.Ledge_Climb_Up'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LedgeTurn_Obj(TEXT("AnimMontage'/Game/Animation/Montage/Ledge_Turn_L.Ledge_Turn_L'"));
	if (WeaponSwapObj.Succeeded() && 
		ZoomWeaponFireObj.Succeeded() && 
		WeaponFireObj.Succeeded() && 
		WeaponReloadObj.Succeeded() && 
		Slim_RoWall_Obj.Succeeded() && 
		Wide_RoWall_Obj.Succeeded() && 
		HiWall_Obj.Succeeded() && 
		JumpWall_Obj.Succeeded() &&
		LedgeClimbUp_Obj.Succeeded() &&
		LedgeTurn_Obj.Succeeded() )
	{
		SwapWeaponMontage = WeaponSwapObj.Object;
		FireWeaponMontage = WeaponFireObj.Object;
		Zoom_FireWeaponMontage = ZoomWeaponFireObj.Object;
		ReloadWeaponMontage = WeaponReloadObj.Object;


		Slim_RoWall_Montage = Slim_RoWall_Obj.Object;
		Wide_RoWall_Montage = Wide_RoWall_Obj.Object;
		HiWall_Montage = HiWall_Obj.Object;
		JumpWall_Montage = JumpWall_Obj.Object;

		LedgeClimbUp_Montage = LedgeClimbUp_Obj.Object;
		LedgeTurnLeft_Montage = LedgeTurn_Obj.Object;
	}
	///////////////////

	//카메라 타임라인//
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CustomCurveFloatObj(TEXT("CurveFloat'/Game/Bp/PickupWeapon_BP/CameraChangeCurve.CameraChangeCurve'"));
	if (CustomCurveFloatObj.Succeeded())
	{
		CurveFloat = CustomCurveFloatObj.Object;
	}
	///////////////////

	static ConstructorHelpers::FClassFinder<UUserWidget> CrosshairWidgetObj(TEXT("WidgetBlueprint'/Game/Bp/PlayerWidget.PlayerWidget_C'"));
	if (CrosshairWidgetObj.Succeeded())
	{
		CrosshairWidgetClass = CrosshairWidgetObj.Class;
	}
	

	//변수 설정//
	bZoomIn = false;

	TPSTransform = TPSCameraLocation->GetRelativeTransform();
	FPSTransform = FPSCameraLocation->GetRelativeTransform();

	Main_Weapon = nullptr;
	Sub_Weapon = nullptr;
	PlayerMotionState = EPlayerMotion::NoWeaponState;
	WeaponTypeState = EWeaponType::NoWeapon_Type;
	/////////////


	
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
	ObjectTypes.Add(WorldStatic);

	TEnumAsByte<ECollisionChannel> Ledge = UEngineTypes::ConvertToCollisionChannel(ETraceTypeQuery::TraceTypeQuery3);
	TEnumAsByte<ECollisionChannel> Visibility = UEngineTypes::ConvertToCollisionChannel(ETraceTypeQuery::TraceTypeQuery1);
	//TraceTypes.Add(Ledge);
	//TraceTypes.Add(Visibility);
	//ObjectTypes.Add(WorldDynamic);
}

// Called when the game starts or when spawned
void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurveTimeline.TickTimeline(DeltaTime);

	TraceInfo traceInfo;

	if (bZoomIn)
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}

	if (bLedge)
	{
		FHitResult OutHit;

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		/****************************************올라갈수 있는지 확인함*********************************************/
		FVector CapsuleStart = GetActorLocation() + (GetActorForwardVector() * 35.f) + FVector(0, 0, 120.f);
		FVector CapsuleEnd = CapsuleStart + (GetActorForwardVector() * 120.f);

		bool isHit = UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), CapsuleStart, CapsuleEnd, 30.6, 90.6, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
		if (isHit)
		{
			bClimbOn = false;
		}
		else
		{
			bClimbOn = true;
		}
		/***********************************************************************************************************/

		FVector SideLine_Start = GetActorLocation();
		FVector SideLine_End = SideLine_Start + (-GetActorRightVector() * 45.f);
		
		//isHit = UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), SideLine_Start, SideLine_End, "Ledge", false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
		
		FVector LineStart_Left = GetActorLocation() + (GetActorRightVector() * -36);
		FVector LineEnd_Left = LineStart_Left + GetActorForwardVector() * 90;
		isHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), LineStart_Left, LineEnd_Left, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
		if (isHit)
		{
			bCanLeft = true;
		}
		else
		{
			bCanLeft = false;
			Left = false;
			FVector TurnStart_Left = GetActorLocation() + -GetActorRightVector() * 70.f;
			FVector TurnEnd_Left = TurnStart_Left + GetActorForwardVector() * 80.f;


			/*************************************옆으로 돌수있는지 확인함****************************************************************/
			isHit = UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), TurnStart_Left, TurnEnd_Left, GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
				ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
			if (!isHit)
			{
				bTurnLeft = true;
				FVector HitLocation = OutHit.Location;
				FVector HitNormal = OutHit.Normal;

				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d"), bTurnLeft));

				//FRotator newRotation = FRotator(GetActorRotation().Pitch, UKismetMathLibrary::MakeRotFromX(HitNormal).Yaw + 180.f, GetActorRotation().Roll);
				//SetActorRotation(newRotation);

				//FVector newLocation = FVector(HitNormal.X * 30.f + HitLocation.X, HitNormal.Y * 30.f + HitLocation.Y, GetActorLocation().Z);
				//SetActorLocation(newLocation);
			/**********************************************************************************************************/


			}
		}
		
		FVector LineStart_Right = GetActorLocation() + (GetActorRightVector() * 36);
		FVector LineEnd_Right = LineStart_Right + GetActorForwardVector() * 90;
		isHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), LineStart_Right, LineEnd_Right, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
		
		if (isHit)
		{
			bCanRight = true;
		}
		else
		{
			bCanRight = false;
			Right = false;
			FVector TurnStart_Left = GetActorLocation() + GetActorRightVector() * 70.f;
			FVector TurnEnd_Left = TurnStart_Left + GetActorForwardVector() * 80.f;


			/*************************************옆으로 돌수있는지 확인함****************************************************************/
			isHit = UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), TurnStart_Left, TurnEnd_Left, GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
				ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
			if (!isHit)
			{
				bTurnRight = true;
				FVector HitLocation = OutHit.Location;
				FVector HitNormal = OutHit.Normal;

				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d"), bTurnRight));

				//FRotator newRotation = FRotator(GetActorRotation().Pitch, UKismetMathLibrary::MakeRotFromX(HitNormal).Yaw + 180.f, GetActorRotation().Roll);
				//SetActorRotation(newRotation);

				//FVector newLocation = FVector(HitNormal.X * 30.f + HitLocation.X, HitNormal.Y * 30.f + HitLocation.Y, GetActorLocation().Z);
				//SetActorLocation(newLocation);


			/**********************************************************************************************************/
			}
		}
		
	}


	if (GetCharacterMovement()->IsFalling() == true)
	{
		Parkour(&traceInfo, ETraceTypeQuery::TraceTypeQuery3 ,100, 200, 130);
		if (traceInfo.bBlockHit)
		{
			TestFunction(traceInfo);
		}
	}
}

// Called to bind functionality to input
void AThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &AThirdPersonCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AThirdPersonCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &AThirdPersonCharacter::Turn);
	InputComponent->BindAxis("LookUp", this, &AThirdPersonCharacter::LookUp);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AThirdPersonCharacter::JumpStart);
	InputComponent->BindAction("Jump", IE_Released, this, &AThirdPersonCharacter::JumpEnd);
	InputComponent->BindAction("Run", IE_Pressed, this, &AThirdPersonCharacter::RunStart);
	InputComponent->BindAction("Run", IE_Released, this, &AThirdPersonCharacter::RunEnd);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &AThirdPersonCharacter::DoCrouch);

	InputComponent->BindAction("Interaction", IE_Pressed, this, &AThirdPersonCharacter::Interaction);

	InputComponent->BindAction("MainWeaponSwap", IE_Pressed, this, &AThirdPersonCharacter::SwapMainWeapon);
	InputComponent->BindAction("SubWeaponSwap", IE_Pressed, this, &AThirdPersonCharacter::SwapSubWeapon);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AThirdPersonCharacter::FireStart);
	InputComponent->BindAction("Fire", IE_Released, this, &AThirdPersonCharacter::FireEnd);
	
	InputComponent->BindAction("Zoom", IE_Pressed, this, &AThirdPersonCharacter::ZoomIn);
	InputComponent->BindAction("Zoom", IE_Released, this, &AThirdPersonCharacter::ZoomOut);
	InputComponent->BindAction("Reload", IE_Pressed, this, &AThirdPersonCharacter::Reload);

	//InputComponent->BindAction("TestMapping", IE_Pressed, this, &AThirdPersonCharacter::Parkour);
}

// 좌/우, 앞/뒤 이동 관리//
void AThirdPersonCharacter::MoveForward(float Value)
{
	if (bLedge == false)
	{
		if ((Controller != nullptr) && (Value != 0.f))
		{

			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			AddMovementInput(Direction, Value);
		}
	}
	else
	{
		if ((Controller != nullptr) && (Value < 0.f))
		{
			TraceInfo traceInfo;
			//Parkour(&traceInfo, -500.f, 0, 0);
			if (traceInfo.bBlockHit)
			{
				bAcross = true;
			}
			else
			{
				bAcross = false;
			}
		}
	}
}

void AThirdPersonCharacter::MoveRight(float Value)
{
	if (bLedge == false)
	{
		if ((Controller != nullptr) && (Value != 0.f))
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
		}
	}
	else
	{
		if (bCanLeft)
		{
			if ((Controller != nullptr) && (Value == 0.f))
			{
				Left = false;
				Right = false;
				GetCharacterMovement()->StopMovementImmediately();
			}
			else if ((Controller != nullptr) && (Value < 0.f))
			{
				/*
				const FVector Direction = GetActorRightVector();
				AddMovementInput(Direction, Value);
				*/

				const FVector Current = GetActorLocation();
				const FVector Target = Current + GetActorRightVector() * 40.f * Value;
				const FVector Direction = UKismetMathLibrary::VInterpTo(Current, Target, GetWorld()->GetDeltaSeconds(), 2.0f);
				SetActorLocation(Direction);
				Left = true;
				Right = false;
			}
		}
		else if (bTurnLeft)
		{
			if ((Controller != nullptr) && (Value < 0.f))
			{
				Parkour_SetMotion(EMovementMode::MOVE_Flying, ECollisionEnabled::QueryAndPhysics, true, true, bLedge, LedgeTurnLeft_Montage, "Inner_L_Start");
			}
		}
		else
		{
			GetCharacterMovement()->StopMovementImmediately();
			Left = false;
		}

		if (bCanRight)
		{
			if ((Controller != nullptr) && (Value == 0.f))
			{
				Left = false;
				Right = false;
				GetCharacterMovement()->StopMovementImmediately();
			}
			else if ((Controller != nullptr) && (Value > 0.f))
			{
				/*
				const FVector Direction = GetActorRightVector();
				AddMovementInput(Direction, Value);
				*/

				const FVector Current = GetActorLocation();
				const FVector Target = Current + GetActorRightVector() * 40.f * Value;
				const FVector Direction = UKismetMathLibrary::VInterpTo(Current, Target, GetWorld()->GetDeltaSeconds(), 2.0f);
				SetActorLocation(Direction);
				Left = false;
				Right = true;
			}
		}
		else if (bTurnRight)
		{
			if ((Controller != nullptr) && (Value > 0.f))
			{
				Parkour_SetMotion(EMovementMode::MOVE_Flying, ECollisionEnabled::QueryAndPhysics, true, true, bLedge, LedgeTurnLeft_Montage, "Inner_R_Start");
			}
		}
		else
		{
			GetCharacterMovement()->StopMovementImmediately();
			Left = false;
		}
	}
}
/////////////////////

// Yaw / Pitch 회전 //
void AThirdPersonCharacter::Turn(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonCharacter::LookUp(float Rate)
{
	AddControllerPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
////////////////////

/*******인풋 액션*********/

//점프//
void AThirdPersonCharacter::JumpStart()
{
	if (bLedge)
	{
		if (bClimbOn)
		{
			TraceInfo traceInfo;
			//Parkour(&traceInfo,ETraceTypeQuery::TraceTypeQuery3);
			bClimbOn = false;
			bLedge = false;
			Parkour_SetMotion(EMovementMode::MOVE_Flying, ECollisionEnabled::QueryOnly, true, true, bLedge, LedgeClimbUp_Montage);

			FVector newLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), traceInfo.s_HitLocation, GetWorld()->GetDeltaSeconds(), 1.f);

				//Parkour_SetTransform(traceInfo.s_TraceLocation, traceInfo.s_TraceNormal);
		}
	}
	else
	{
		if (GetCharacterMovement()->IsFalling() == false)
		{

			TraceInfo traceInfo;
			Parkour(&traceInfo, ETraceTypeQuery::TraceTypeQuery4, 70.f, 100.f, 100.f, 44.f);
			if (traceInfo.bBlockHit)
			{
				if (traceInfo.ValutOn)
				{
					Parkour_SetMotion(EMovementMode::MOVE_Flying, ECollisionEnabled::QueryOnly, true, true, bLedge, Wide_RoWall_Montage);
					Parkour_SetTransform(traceInfo.s_HitLocation, traceInfo.s_HitNormal, 30.f);
				}
				else
				{
					Parkour_SetMotion(EMovementMode::MOVE_Flying, ECollisionEnabled::QueryOnly, true, true, bLedge, Slim_RoWall_Montage);
					Parkour_SetTransform(traceInfo.s_HitLocation, traceInfo.s_HitNormal, 60.f);
				}
			}
			else
			{
				Parkour_SetMotion(EMovementMode::MOVE_Walking, ECollisionEnabled::QueryAndPhysics, traceInfo.bBlockHit);
			}
		}

	}
}

void AThirdPersonCharacter::JumpEnd()
{
	bPressedJump = false;
}
///////////

//달리기//
void AThirdPersonCharacter::RunStart()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AThirdPersonCharacter::RunEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}
/////////

//앉기//
void AThirdPersonCharacter::DoCrouch()
{
	CanCrouch() ? Crouch() : UnCrouch();
}
///////

//주변의 물체를 감지함//
void AThirdPersonCharacter::Interaction()
{
	TArray<AActor*> InRangeWeapon;
	GetCapsuleComponent()->GetOverlappingActors(InRangeWeapon);

	for (int i = 0; i < InRangeWeapon.Num(); i++)
	{
		AMasetPickUp_Weapon* const Pickup = Cast<AMasetPickUp_Weapon>(InRangeWeapon[i]);
		if (Pickup->bCanWePick)
		{
			PickUp(Pickup);
		}
		break;
	}
}
//////////////////////

//픽업//
void AThirdPersonCharacter::PickUp(AActor* MasterPickUp)
{

	if (MasterPickUp != NULL) 
	{
		AMasetPickUp_Weapon* Pickup = Cast<AMasetPickUp_Weapon>(MasterPickUp);
		if (Pickup->weaponInfo.weaponType == EWeaponType::MainWeapon_Type)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Pickup;
			const FTransform  spawnTransform = GetMesh()->GetSocketTransform(Pickup->weaponInfo.UnEquipSocketName, RTS_World);
			const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

			if (Main_Weapon == nullptr)
			{
				Main_Weapon = GetWorld()->SpawnActor<AMasetPickUp_Weapon>(Pickup->weaponInfo.weaponClass, spawnTransform, SpawnParams);
				Main_Weapon->AttachToComponent(GetMesh(), AttachmentRules, Pickup->weaponInfo.UnEquipSocketName);
				Main_Weapon->weaponMesh->SetSimulatePhysics(false);
				Main_Weapon->weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Main_Weapon->OffAll();
				Pickup->Destroy();
			}
		}
		if (Pickup->weaponInfo.weaponType == EWeaponType::SubWeapon_Type)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Pickup;
			const FTransform spawnTransform = GetMesh()->GetSocketTransform(Pickup->weaponInfo.UnEquipSocketName, RTS_World);
			const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

			if (Sub_Weapon == nullptr)
			{
				Sub_Weapon = GetWorld()->SpawnActor<AMasetPickUp_Weapon>(Pickup->weaponInfo.weaponClass, spawnTransform, SpawnParams);
				Sub_Weapon->AttachToComponent(GetMesh(), AttachmentRules, Pickup->weaponInfo.UnEquipSocketName);
				Sub_Weapon->weaponMesh->SetSimulatePhysics(false);
				Sub_Weapon->weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Sub_Weapon->OffAll();
				Pickup->Destroy();
			}
		}
	}
}
/////////

//무기 체인지 함수//
void AThirdPersonCharacter::SwapMainWeapon()
{
	if (Main_Weapon != NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Yellow, TEXT("MainWeapon_SwapAnimMontage Play!"));
		PlayAnimMontage(SwapWeaponMontage, 1.5f, "Rifle_Swap_Start");
	}
}

void AThirdPersonCharacter::SwapSubWeapon()
{
	if (Sub_Weapon != NULL)
	{
		GEngine->AddOnScreenDebugMessage(01, 100, FColor::Yellow, TEXT("SubWeapon_SwapAnimMotage Play!"));
		PlayAnimMontage(SwapWeaponMontage, 1.5f, "Pistol_Swap_Start");
	}
}
////////////////////

//바꿀수있는지를 확인함//
bool AThirdPersonCharacter::CanMainWeaponMotion()
{
	return (Main_Weapon != NULL && Main_Weapon->weaponInfo.weaponType == EWeaponType::MainWeapon_Type);
}


bool AThirdPersonCharacter::CanSubWeaponMotion()
{
	return (Sub_Weapon != NULL && Sub_Weapon->weaponInfo.weaponType == EWeaponType::SubWeapon_Type);
}
////////////////////////

//메인 웨폰 바꾸는모션//
void AThirdPersonCharacter::MainWeaponOrMotion()
{
	if (Main_Weapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Black, TEXT("Attach Success!!"));
		if (WeaponTypeState == Main_Weapon->weaponInfo.weaponType)
		{
			ZoomOut();

			PlayerMotionState = EPlayerMotion::NoWeaponState;
			WeaponTypeState = EWeaponType::NoWeapon_Type;
			const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
			Main_Weapon->AttachToComponent(GetMesh(), AttachmentRules, Main_Weapon->weaponInfo.UnEquipSocketName);


		}
		else
		{
			PlayerMotionState = Main_Weapon->weaponInfo.playerMotion;
			WeaponTypeState = Main_Weapon->weaponInfo.weaponType;
			const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
			Main_Weapon->AttachToComponent(GetMesh(), AttachmentRules, Main_Weapon->weaponInfo.EquipSocketName);
			if (IsValid(Sub_Weapon))
			{
				Sub_Weapon->AttachToComponent(GetMesh(), AttachmentRules, Sub_Weapon->weaponInfo.UnEquipSocketName);
			}
		}
	}
}
///////////////////////

void AThirdPersonCharacter::SubWeaponOrMotion()
{
	if (Sub_Weapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Black, TEXT("Attach Success!!"));
		if (WeaponTypeState == Sub_Weapon->weaponInfo.weaponType)
		{
			ZoomOut();

			PlayerMotionState = EPlayerMotion::NoWeaponState;
			WeaponTypeState = EWeaponType::NoWeapon_Type;
			const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
			Sub_Weapon->AttachToComponent(GetMesh(), AttachmentRules, Sub_Weapon->weaponInfo.UnEquipSocketName);

	
		}
		else
		{
			PlayerMotionState = Sub_Weapon->weaponInfo.playerMotion;
			WeaponTypeState = Sub_Weapon->weaponInfo.weaponType;
			const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
			Sub_Weapon->AttachToComponent(GetMesh(), AttachmentRules, Sub_Weapon->weaponInfo.EquipSocketName);
			if (IsValid(Main_Weapon))
			{
				Main_Weapon->AttachToComponent(GetMesh(), AttachmentRules, Main_Weapon->weaponInfo.UnEquipSocketName);
			}
		}
	}
}

void AThirdPersonCharacter::FireStart()
{
	if (IsValid(Main_Weapon) && WeaponTypeState == Main_Weapon->weaponInfo.weaponType && !bReload)
	{
		Main_Weapon->FireStart(this);
	}
	 if (IsValid(Sub_Weapon) && WeaponTypeState == Sub_Weapon->weaponInfo.weaponType && !bReload)
	{
		Sub_Weapon->FireStart(this);
	}
}

void AThirdPersonCharacter::FireEnd()
{
	if (IsValid(Main_Weapon) && WeaponTypeState == Main_Weapon->weaponInfo.weaponType)
	{
		Main_Weapon->FireEnd();
	}
	 if (IsValid(Sub_Weapon) && WeaponTypeState == Sub_Weapon->weaponInfo.weaponType)
	{
		Sub_Weapon->FireEnd();
	}
}

void AThirdPersonCharacter::ZoomIn()
{
	if ((IsValid(Main_Weapon) && WeaponTypeState == Main_Weapon->weaponInfo.weaponType && GetCharacterMovement()->IsFalling() == false) ||
		(IsValid(Sub_Weapon) && WeaponTypeState == Sub_Weapon->weaponInfo.weaponType && GetCharacterMovement()->IsFalling() == false))
	{
		if (CurveFloat)
		{
			FOnTimelineFloat TimelineProgress;
			TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
			CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
			CurveTimeline.SetLooping(false);

			bZoomIn = true;
			bUseControllerRotationYaw = true;
			SpringArm->TargetArmLength = 0.f;

			if (CrosshairWidgetClass != nullptr)
			{
				CurrentWidget = CreateWidget(GetWorld(), CrosshairWidgetClass);
				if (CurrentWidget != nullptr)
				{
					CurrentWidget->AddToViewport();
				}
			}

			CurveTimeline.Play();
		}
	}
}

void AThirdPersonCharacter::ZoomOut()
{
	if ((IsValid(Main_Weapon) && WeaponTypeState == Main_Weapon->weaponInfo.weaponType && GetCharacterMovement()->IsFalling() == false)||
		(IsValid(Sub_Weapon) && WeaponTypeState == Sub_Weapon->weaponInfo.weaponType && GetCharacterMovement()->IsFalling() == false))
	{
		if (CurveFloat)
		{
			FOnTimelineFloat TimelineProgress;
			TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
			CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
			CurveTimeline.SetLooping(false);

			bZoomIn = false;
			bUseControllerRotationYaw = false;
			SpringArm->TargetArmLength = 350.f;

			
			CurrentWidget->RemoveFromViewport();
			

			CurveTimeline.Reverse();
		}
	}
}

void AThirdPersonCharacter::Reload()
{
	if (IsValid(Main_Weapon) && WeaponTypeState == Main_Weapon->weaponInfo.weaponType && Main_Weapon->weaponInfo.MaxAmmo > 0)
	{
		Main_Weapon->ReloadStart();
	}
	else if (IsValid(Sub_Weapon) && WeaponTypeState == Sub_Weapon->weaponInfo.weaponType && Sub_Weapon->weaponInfo.MaxAmmo > 0)
	{
		Sub_Weapon->ReloadStart();
	}
}

void AThirdPersonCharacter::SetReload(bool boolean)
{
	this->bReload = boolean;
}

bool AThirdPersonCharacter::GetReload()
{
	return bReload;
}

/*************************************************************************************************************************************/
                                                 //ETraceTypeQuery::TraceTypeQuery4 = ValutBlock// : 뛰어넘기 블럭
												//ETraceTypeQuery::TraceTypeQuery3 = LedgeBlock// : 매달리기 블럭

/************************************************************************************************************************************/

void AThirdPersonCharacter::Parkour(TraceInfo* traceInfo, ETraceTypeQuery TraceCannel, float LineLength, float LineHeight, float ParkourHeight, float StartLinePos)
{
	if (bLedge == false)
	{
		if (TraceCannel == ETraceTypeQuery::TraceTypeQuery4)
		{

			//벽유무 라인트레이스//
			FHitResult OutHit;
			FVector Start = GetActorLocation() - FVector(0.f, 0.f, StartLinePos);
			FVector End = Start + (GetActorForwardVector() * LineLength);
			FCollisionQueryParams CollisionParams;

			//정면 라인트레이스//
			bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceCannel, false, IgnoreActors, EDrawDebugTrace::ForDuration, OutHit, true);
			///////////////////////
			if (isHit)
			{
				//벽넘기 라인트레이스//
				FVector HitLocation = OutHit.Location;
				FVector HitNormal = OutHit.Normal;

				FVector VaultStart = HitLocation + UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(HitNormal)) * -10.f + FVector(0.f, 0.f, LineHeight);
				FVector VaultEnd = VaultStart - FVector(0.f, 0.f, LineHeight);

				//처음 벽넘기라인트레이스//
				isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), VaultStart, VaultEnd, TraceCannel, false, IgnoreActors, EDrawDebugTrace::ForDuration, OutHit, true);
				//////////////////////////

				if (isHit)
				{
					//벽넘을 위치 가져오기//
					FVector HitHeight = OutHit.Location;
					float ValutHeight = HitHeight.Z - HitLocation.Z;
					///////////////////////

					if (ValutHeight < ParkourHeight)
					{
						//블록 올라가기 라인트레이스//
						VaultStart = MakeLocation(HitLocation, HitNormal, 50, 0, LineHeight + 50.f);
						VaultEnd = VaultStart - FVector(0.f, 0.f, LineHeight + 100.f);

						isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), VaultStart, VaultEnd, TraceCannel, false, IgnoreActors, EDrawDebugTrace::ForDuration, OutHit, true);
						/////////////////////////////
						if (isHit)
						{
							//넘어서 올라가기//

							/*
							FVector newLocation = GetActorLocation() + (UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(HitNormal)) * 50);
							SetActorLocation(newLocation);
							*/

							(*traceInfo).s_HitLocation = HitHeight;
							(*traceInfo).s_HitNormal = HitNormal;
							(*traceInfo).bBlockHit = true;
							(*traceInfo).ValutOn = isHit;
						}
						else
						{

							/*
							FVector newLocation = GetActorLocation() + (UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(HitNormal)) * 50);
							SetActorLocation(newLocation);
							*/
							(*traceInfo).s_HitLocation = HitLocation;
							(*traceInfo).s_HitNormal = HitNormal;
							(*traceInfo).bBlockHit = true;
							(*traceInfo).ValutOn = isHit;
						}
					}
					else
					{
						(*traceInfo).bBlockHit = false;
					}
				}
			}
			else
			{
				(*traceInfo).bBlockHit = false;
			}
		}
		else if (TraceCannel == ETraceTypeQuery::TraceTypeQuery3)
		{
			//벽유무 라인트레이스//
			FHitResult OutHit;
			FVector Start = GetActorLocation() - FVector(0, 0, StartLinePos);
			FVector End = Start + (GetActorForwardVector() * LineLength);

			bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceCannel, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
			if (isHit)
			{
				FVector HitLocation = OutHit.Location;
				FVector HitNormal = OutHit.Normal;

				FVector ClimbStart = MakeLocation(HitLocation, HitNormal, 10.f, 0, LineHeight);
				FVector ClimbEnd = ClimbStart - FVector(0.f, 0.f, LineHeight);

				isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), ClimbStart, ClimbEnd, TraceCannel, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, false);
				if (isHit)
				{
					FVector LedgeLocation = OutHit.Location;

					float CanHeight = LedgeLocation.Z - HitLocation.Z;
					if (CanHeight < ParkourHeight)
					{
						GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
						FRotator newRotation = FRotator(GetActorRotation().Pitch, UKismetMathLibrary::MakeRotFromX(HitNormal).Yaw + 180.f, GetActorRotation().Roll);
						SetActorRotation(newRotation);

						FVector newLocation = FVector(HitNormal.X * 30.f + HitLocation.X, HitNormal.Y * 30.f + HitLocation.Y, LedgeLocation.Z - 22.f);
						SetActorLocation(newLocation);
						GetCharacterMovement()->StopMovementImmediately();

						(*traceInfo).s_HitLocation = LedgeLocation;
						(*traceInfo).s_HitNormal = HitNormal;
						(*traceInfo).bBlockHit = isHit;
						GetCharacterMovement()->bOrientRotationToMovement = false;
						bLedge = true;
					}
					else
					{
						bLedge = false;
					}
				}
				else
				{
					bLedge = false;
				}
			}
		}
	}
	else
	{
		if (TraceCannel == ETraceTypeQuery::TraceTypeQuery3)
		{

			//벽유무 라인트레이스//
			FHitResult OutHit;
			FVector Start = GetActorLocation() - FVector(0.f, 0.f, StartLinePos);
			FVector End = Start + (GetActorForwardVector() * LineLength);
			FCollisionQueryParams CollisionParams;

			//정면 라인트레이스//
			bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceCannel, false, IgnoreActors, EDrawDebugTrace::ForDuration, OutHit, true);
			///////////////////////
			if (isHit)
			{
				//벽넘기 라인트레이스//
				FVector HitLocation = OutHit.Location;
				FVector HitNormal = OutHit.Normal;

				FVector VaultStart = HitLocation + UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(HitNormal)) * -10.f + FVector(0.f, 0.f, LineHeight);
				FVector VaultEnd = VaultStart - FVector(0.f, 0.f, LineHeight);

				//처음 벽넘기라인트레이스//
				isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), VaultStart, VaultEnd, TraceCannel, false, IgnoreActors, EDrawDebugTrace::ForDuration, OutHit, true);
				//////////////////////////

				if (isHit)
				{
					//벽넘을 위치 가져오기//
					FVector HitHeight = OutHit.Location;
					float ValutHeight = HitHeight.Z - HitLocation.Z;
					///////////////////////

					if (ValutHeight < ParkourHeight)
					{
						(*traceInfo).s_HitLocation = HitLocation;
						(*traceInfo).s_HitNormal = HitNormal;
						(*traceInfo).bBlockHit = true;
						(*traceInfo).ValutOn = isHit;

					}
					else
					{
						(*traceInfo).bBlockHit = false;
					}
				}
				else
				{
					(*traceInfo).bBlockHit = false;
				}
			}
			else
			{
				(*traceInfo).bBlockHit = false;
			}
		}
		/*else if (TraceCannel == ETraceTypeQuery::TraceTypeQuery3)
		{
			//벽유무 라인트레이스//
			FHitResult OutHit;
			FVector Start = GetActorLocation() - FVector(0, 0, StartLinePos);
			FVector End = Start + (GetActorForwardVector() * LineLength);

			bool isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceCannel, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
			if (isHit)
			{
				FVector HitLocation = OutHit.Location;
				FVector HitNormal = OutHit.Normal;

				FVector ClimbStart = MakeLocation(HitLocation, HitNormal, 10.f, 0, LineHeight);
				FVector ClimbEnd = ClimbStart - FVector(0.f, 0.f, LineHeight);

				isHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), ClimbStart, ClimbEnd, TraceCannel, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, false);
				if (isHit)
				{
					FVector LedgeLocation = OutHit.Location;

					float CanHeight = LedgeLocation.Z - HitLocation.Z;
					if (bClimbOn)
					{
						Parkour_SetMotion(EMovementMode::MOVE_Flying, ECollisionEnabled::QueryAndPhysics, bClimbOn, true, bLedge, LedgeClimbUp_Montage);
					}
					else if (CanHeight < ParkourHeight)
					{
						FRotator newRotation = FRotator(GetActorRotation().Pitch, UKismetMathLibrary::MakeRotFromX(HitNormal).Yaw + 180.f, GetActorRotation().Roll);
						SetActorRotation(newRotation);

						FVector newLocation = FVector(HitNormal.X * 30.f + HitLocation.X, HitNormal.Y * 30.f + HitLocation.Y, LedgeLocation.Z - 22.f);
						SetActorLocation(newLocation);
						GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
						GetCharacterMovement()->StopMovementImmediately();

						(*traceInfo).s_TraceLocation = LedgeLocation;
						(*traceInfo).s_TraceNormal = HitNormal;
						GetCharacterMovement()->bOrientRotationToMovement = false;
						bLedge = true;
					}
					else
					{
						bLedge = false;
					}
				}
				else
				{
					bLedge = false;
				}
			}
		}
		*/
	}
}

void AThirdPersonCharacter::Parkour_SetTransform(FVector HitLocation, FVector HitNormal, float ZOffset)
{
	FRotator newRotation = FRotator(GetActorRotation().Pitch, UKismetMathLibrary::MakeRotFromX(HitNormal).Yaw + 180.f, GetActorRotation().Roll);
	SetActorRotation(newRotation);

	//FVector newLocation = FVector(GetActorLocation().X, GetActorLocation().Y, (HitLocation.Z - ZOffset));
	FVector newLocation = UKismetMathLibrary::VInterpTo(GetActorLocation() + FVector(0.f, 0.f, ZOffset), HitLocation, GetWorld()->GetDeltaSeconds(), 1.f);
	SetActorLocation(newLocation);
}

void AThirdPersonCharacter::Parkour_SetMotion(EMovementMode MovementMove, ECollisionEnabled::Type NewType, bool bHitBlock, bool bAnim, bool bLedged , UAnimMontage* AnimMontage, FName Section)
{
	if (bHitBlock)
	{

		GetCharacterMovement()->SetMovementMode(MovementMove);
		GetCapsuleComponent()->SetCollisionEnabled(NewType);
		if (bAnim)
		{
			if (bLedged)
			{
				DisableInput(GetWorld()->GetFirstPlayerController());
				float Delay = PlayAnimMontage(AnimMontage, 1.0f, Section);
				GetWorld()->GetTimerManager().SetTimer(PlayerTimeHandle, this, &AThirdPersonCharacter::HoldingLedge, Delay, false);
			}
			else
			{
				DisableInput(GetWorld()->GetFirstPlayerController());
				float Delay = PlayAnimMontage(AnimMontage, 1.0f, Section);
				GetWorld()->GetTimerManager().SetTimer(PlayerTimeHandle, this, &AThirdPersonCharacter::OntheGround, Delay, false);
			}
		}
	}
	else
	{
		bPressedJump = true;
		if (bZoomIn)
		{
			ZoomOut();
			bZoomIn ? false : false;
		}
	}
}

void AThirdPersonCharacter::OntheGround()
{
	EnableInput(GetWorld()->GetFirstPlayerController());
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bLedge = false;
}

void AThirdPersonCharacter::HoldingLedge()
{
	EnableInput(GetWorld()->GetFirstPlayerController());
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void AThirdPersonCharacter::Update_LedgeMoveCheck(FVector HitLocation)
{
	FVector Start = FVector(HitLocation.X, HitLocation.Y + 15.3f, HitLocation.Z + 100.f);
	FVector End = Start + FVector(0, 0, 100.f);
	FHitResult OutHit;

	bool isHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
}

FVector AThirdPersonCharacter::MakeLocation(FVector HitLocation, FVector HitNormal, float NormalAngle, float XYDistance, float ZDistance)
{
	FVector newLocation = HitLocation + UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::MakeRotFromX(HitNormal)) * -NormalAngle + FVector(0, 0, ZDistance);
	return newLocation;
}

void AThirdPersonCharacter::TimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(TPSTransform.GetLocation(), FPSTransform.GetLocation(), Value);

	Camera->SetRelativeLocation(NewLocation);
}

//테스트함수//
void AThirdPersonCharacter::TestFunction(TraceInfo traceInfo)
{
	//Parkour_SetMotion(EMovementMode::MOVE_Flying, ECollisionEnabled::QueryAndPhysics, traceInfo.bBlockHit);
	
}
//////////////////
