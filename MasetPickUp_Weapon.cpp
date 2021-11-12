// Fill out your copyright notice in the Description page of Project Settings.

#include "MasetPickUp_Weapon.h"
#include <Engine/Classes/Components/SkeletalMeshComponent.h>
#include <Engine/Classes/Components/SphereComponent.h>
#include <Runtime/UMG/Public/Components/WidgetComponent.h>

// Sets default values
AMasetPickUp_Weapon::AMasetPickUp_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//콜리전 위치/회전/스케일/조정//
	const FVector Position = FVector(0.f, 0.f, 0.f);
	const FRotator Rotation = FRotator(0.f, 0.f, 0.f);
	const FVector Scale = FVector(4.f, 4.f, 4.f);
	FTransform Transform = FTransform(Rotation, Position, Scale);
	const FVector2D WidgetDrawSize = FVector2D(180, 70);
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	weaponMesh->SetupAttachment(RootComponent);
	weaponMesh->SetSimulatePhysics(true);
	weaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	overlapCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	overlapCollision->AttachToComponent(weaponMesh, AttachmentRules);
	overlapCollision->SetCollisionProfileName("OverlapAllDynamic");
	overlapCollision->SetSphereRadius(32.f);
	overlapCollision->SetRelativeTransform(Transform);
	overlapCollision->bHiddenInGame = false;

	overlapCollision->OnComponentBeginOverlap.AddDynamic(this, &AMasetPickUp_Weapon::TriggerEnter);
	overlapCollision->OnComponentEndOverlap.AddDynamic(this, &AMasetPickUp_Weapon::TriggerExit);

	/**************************************************************************************************************************************************************************************************
	//ConstructorHelpers::FObjectFinder<UMaterial> WidgetMaterial(TEXT("MaterialInstanceConstant'/Engine/EngineMaterials/Widget3DPassThrough_Masked_OneSided.Widget3DPassThrough_Masked_OneSided'"));//
	//if (WidgetMaterial.Succeeded())																																								 //
	//{																																																 //
	//	widget = NewObject<UWidgetComponent>(this, AMasetPickUp_Weapon::StaticClass());																												 //
	//	widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));																															 //
	//	widget->SetupAttachment(weaponMesh);																																						 //
	//	widget->SetMaterial(0, WidgetMaterial.Object);																																				 //
	//	widget->SetWidgetSpace(EWidgetSpace::Screen);																																				 //
	//	widget->SetDrawSize(WidgetDrawSize);																																						 //
	//}																																																 //
	**************************************************************************************************************************************************************************************************/

	/****************************************************************************
	weaponInfo.weaponType = this->weaponInfo.weaponType;						 //
	weaponInfo.playerMotion = this->weaponInfo.playerMotion;					 //
	weaponInfo.weaponClass = this->weaponInfo.weaponClass;						 //
	weaponInfo.CurrentAmmo = this->weaponInfo.CurrentAmmo;						 //
	weaponInfo.MaxAmmo = this->weaponInfo.MaxAmmo;								 //
	weaponInfo.EquipSocketName = this->weaponInfo.EquipSocketName;				 //
	weaponInfo.UnEquipSocketName = this->weaponInfo.UnEquipSocketName;			 //
	*****************************************************************************/
}

// Called when the game starts or when spawned
void AMasetPickUp_Weapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMasetPickUp_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMasetPickUp_Weapon::TriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<AThirdPersonCharacter>(OtherActor);
	if (Player != nullptr &&(OtherActor!=this))
	{
		if (GEngine) 
		{
			OnOff_VisibiltyCustom_Depth(true, true);
			bCanWePick = true;
			GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Red, bCanWePick ? TEXT("TRUE") : TEXT("FALSE"));
			EnableInput(Player->GetWorld()->GetFirstPlayerController());
		}
	}
}

void AMasetPickUp_Weapon::TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player = Cast<AThirdPersonCharacter>(OtherActor);
	if (Player != nullptr && (OtherActor != this))
	{
		if (GEngine)
		{
			OnOff_VisibiltyCustom_Depth(false, false);
			bCanWePick = false;
			GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Red, bCanWePick ? TEXT("TRUE") : TEXT("FALSE"));
			DisableInput(Player->GetWorld()->GetFirstPlayerController());
		}
	}
}

void AMasetPickUp_Weapon::OnOff_VisibiltyCustom_Depth(bool newVisibilty, bool Value)
{
	//widget->SetVisibility(newVisibilty, false);
	weaponMesh->SetRenderCustomDepth(Value);
}

void AMasetPickUp_Weapon::OffAll()
{
	OnOff_VisibiltyCustom_Depth(false, false);
	overlapCollision->DestroyComponent();
}

void AMasetPickUp_Weapon::FireStart(AThirdPersonCharacter* player)
{
	Player = Cast<AThirdPersonCharacter>(player);
	if (weaponInfo.CurrentAmmo > 0)
	{
		Fire();

		if (weaponInfo.bAuto)
		{
			GetWorldTimerManager().SetTimer(FireTimeHandle, this, &AMasetPickUp_Weapon::Fire, 0.2, true);
		}
	}
}

void AMasetPickUp_Weapon::FireEnd()
{
	if (weaponInfo.CurrentAmmo > 0)
	{
		GetWorldTimerManager().ClearTimer(FireTimeHandle);
	}
}


void AMasetPickUp_Weapon::Fire()
{
}

void AMasetPickUp_Weapon::ReloadStart()
{
}

void AMasetPickUp_Weapon::Reload()
{
}

int AMasetPickUp_Weapon::Range(int min, int max)
{
	int r = rand() % (max - min + 1) + min;
	return r;
}
