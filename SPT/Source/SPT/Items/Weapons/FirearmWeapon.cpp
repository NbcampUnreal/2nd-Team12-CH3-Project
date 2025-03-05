// Fill out your copyright notice in the Description page of Project Settings.


#include "FirearmWeapon.h"
#include "SPTPlayerCharacter.h"
#include "SPTPlayerController.h"
#include "SPT/Items/Worlds/WorldWeapon.h"
#include "SPT/Inventory/ItemWidget/PreviewCharacter.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"

AFirearmWeapon::AFirearmWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	// Skeletal Mesh �߰�
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	if (StaticMeshComponent && SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetupAttachment(RootComponent);
	}

	// �ʱ� ���� ����
	bIsReloading = false;
	bIsFiring = false;
	bIsAiming = false;
	CurrentRecoil = 0.0f;
	bIsAutoFire = FirearmStats.bHasAutomaticFireMode;

}

void AFirearmWeapon::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ASPTPlayerCharacter>(GetOwner());
}

void AFirearmWeapon::Attack()
{
	BeginFire();
}

bool AFirearmWeapon::CanFire()
{
	if (bIsEquipping || bIsFiring || bIsReloading)
	{
		return false;
	}

	return true;;
}

void AFirearmWeapon::BeginFire()
{
	if (CanFire())
	{
		bIsFiring = true;

		if (bIsAutoFire)
		{
			// �߻� �ӵ� ����
			GetWorld()->GetTimerManager().SetTimer(
				FireTimerHandle,
				this,
				&AFirearmWeapon::OnFiring,
				WeaponData.WeaponStats.AttackRate,
				true
			);
		}

		OnFiring();
	}
}

void AFirearmWeapon::EndFire()
{
	if (!bIsFiring)
	{
		return;
	}

	// ���� ����
	if (GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}

	bIsFiring = false;
}

void AFirearmWeapon::OnFiring()
{
	if (FirearmStats.AmmoCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is out of ammo!"), *GetName());
		EndFire();
		return;
	}
	FirearmStats.AmmoCount--;

	UCameraComponent* Camera = Cast<UCameraComponent>(Owner->GetComponentByClass<UCameraComponent>());
	FVector Direction = Camera->GetForwardVector();
	FTransform Transform = Camera->GetComponentToWorld();

	FVector Start = Transform.GetLocation() + Direction;

	// ź���� ����
	Direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Direction, FirearmStats.RecoilAngle);
	FVector End = Transform.GetLocation() + Direction * FirearmStats.HitDistance;

	TArray<AActor*> Ignores;

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		End,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		Ignores,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (bHit)	// �߻��� LineTrace�� Object�� �¾� Block �浹�� �Ǿ��ٸ�
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->CanBeDamaged())  // �ǰ� ������ ������� Ȯ��
		{
			float Damage = WeaponData.WeaponStats.Damage; // ������ �⺻ ���ط�
			FDamageEvent DamageEvent;
			AController* InstigatorController = Owner->GetInstigatorController(); // ���� �� �÷��̾��� ��Ʈ�ѷ�

			float ActualDamage = HitActor->TakeDamage(Damage, DamageEvent, InstigatorController, this);

			UE_LOG(LogTemp, Log, TEXT("%s hit %s! Applied Damage: %.2f"), *GetName(), *HitActor->GetName(), ActualDamage);
		}

		if (FirearmStats.HitDecal)
		{
			FRotator Rotator = HitResult.ImpactNormal.Rotation();	// HitDecal�� ���� ���� (�浹�� Normal)
			UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
				GetWorld(),
				FirearmStats.HitDecal,
				FVector(5),
				HitResult.Location,
				Rotator,
				10
			);
			Decal->SetFadeScreenSize(0);
		}

		if (FirearmStats.HitParticle)
		{
			// Hit �� �������κ��� ���ʷ� �� ����(= �÷��̾�)�� ���ϵ��� ����
			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(HitResult.Location, HitResult.TraceStart);
			//Particle�� �浹 ��ġ���� �� ��ġ�� ���ϵ��� ����
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				FirearmStats.HitParticle,
				HitResult.Location,
				Rotator
			);
		}
	}

	if (FirearmStats.FlashParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(
			FirearmStats.FlashParticle,
			SkeletalMeshComponent,
			"MuzzleFlash",
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset
		);
	}

	if (FirearmStats.EjectParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(
			FirearmStats.EjectParticle,
			SkeletalMeshComponent,
			"EjectAmmo",
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset
		);
	}

	FVector MuzzleLocation = SkeletalMeshComponent->GetSocketLocation("MuzzleFlash");

	if (FirearmStats.FireSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			FirearmStats.FireSound,
			MuzzleLocation
		);
	}

	// ī�޶� ��鸲 
	if (FirearmStats.CameraShakeClass)
	{
		APlayerController* Controller = Owner->GetController<ASPTPlayerController>();
		if (Controller)
		{
			Controller->PlayerCameraManager->StartCameraShake(FirearmStats.CameraShakeClass);
		}
	}
	
	// �ѱ� �ݵ�
	Owner->AddControllerPitchInput(-FirearmStats.RecoilRate * UKismetMathLibrary::RandomFloatInRange(0.8f, 1.2f));

	UE_LOG(LogTemp, Log, TEXT("%s fired! Ammo left: %d, Recoil: %.2f"), *GetName(), FirearmStats.AmmoCount, CurrentRecoil);
}

bool AFirearmWeapon::CanEquip()
{
	if (bIsEquipping || bIsReloading || bIsFiring)
	{
		return false;
	}

	return true;
}

void AFirearmWeapon::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("FirearmWeapon : Equip : Start"));
	UpdateMesh();

	Owner = PlayerCharacter;
	if (FirearmStats.RightHandSocketName.IsValid())
	{
		// �ѱ� �տ� ����
		Owner->EquipWeapon(this);
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FirearmStats.RightHandSocketName);
	}

	FirearmStats.BaseData.SetDataByNoneCurve(Owner);

	if (FirearmStats.AimCurve)
	{
		FOnTimelineFloat timeline;
		timeline.BindUFunction(this, "OnAiming");

		FirearmStats.Timeline->AddInterpFloat(FirearmStats.AimCurve, timeline);
		FirearmStats.Timeline->SetLooping(false);
		FirearmStats.Timeline->SetPlayRate(FirearmStats.AimSpeed);
	}

	ItemState = EItemState::EIS_Equipped;
	UE_LOG(LogTemp, Log, TEXT("%s equipped by %s"), *GetName(), *Owner->GetName());

	EndEquip();
	////////////////////////////////////////////////////////////////
	// ������ ĳ���Ϳ� ���� �ݿ��� ���ؼ� ���� �κ��Դϴ�.
	if (Owner && Owner->PreviewCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("PreviewCharacter Update?")); 
		Owner->PreviewCharacter->EquipWeapon(this);
	}
	////////////////////////////////////////////////////////////////
}

void AFirearmWeapon::BeginEquip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter || !ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirearmWeapon::Equip - PlayerCharacter is null!"));
		return;
	}

	bIsEquipping = true;
	Owner = PlayerCharacter;
	/*
	if (FirearmStats.EquipMontage)
	{
		Owner->PlayAnimMontage(FirearmStats.EquipMontage, FirearmStats.EquipMontage_PlayRate);
	}
	*/

	Equip(PlayerCharacter);
}

void AFirearmWeapon::EndEquip()
{
	bIsEquipping = false;
}


bool AFirearmWeapon::CanUnEquip()
{
	if (bIsEquipping || bIsReloading || bIsFiring)
	{
		return false;
	}

	return true;
}

void AFirearmWeapon::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirearmWeapon::UnEquip - PlayerCharacter is null!"));
		return;
	}

	if (FirearmStats.HolsterSocketName.IsValid())
	{
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FirearmStats.HolsterSocketName);
		UE_LOG(LogTemp, Log, TEXT("%s unequipped by %s"), *GetName(), *PlayerCharacter->GetName());
	}

	ItemState = EItemState::EIS_QuickSlot;
}

void AFirearmWeapon::Drop(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirearmWeapon::Drop - PlayerCharacter is null!"));
		return;
	}

	// ��� ��ġ ���
	FVector DropLocation;
	FRotator DropRotation;
	GetDropLocation(PlayerCharacter, DropLocation, DropRotation);

	// AWorldWeapon ����
	AWorldWeapon* DroppedWeapon = GetWorld()->SpawnActor<AWorldWeapon>(AWorldWeapon::StaticClass(), DropLocation, DropRotation);
	if (!DroppedWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn AWorldWeapon!"));
		return;
	}

	// ���� ���� �����͸� AWorldWeapon�� ����
	ItemState = EItemState::EIS_World;
	WeaponData.FirearmStats = FirearmStats;
	ItemData->SetWeaponData(WeaponData);

	UItemDataObject* NewItemData = ItemData->CreateItemCopy();
	DroppedWeapon->SetItemData(NewItemData);
	DroppedWeapon->UpdateMesh();

	if (ItemData->GetItemData().ItemID.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("FirearmWeapon::Faild to load ItemID!!!!!!"));
	}

	// ������ �� �浹 Ȱ��ȭ
	if (USkeletalMeshComponent* WeaponMesh = DroppedWeapon->FindComponentByClass<USkeletalMeshComponent>())
	{
		WeaponMesh->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset); // ������ �ּ� ����
		WeaponMesh->SetSimulatePhysics(true); // ���� �ùķ��̼� Ȱ��ȭ
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // �浹 Ȱ��ȭ
		WeaponMesh->SetCollisionObjectType(ECC_PhysicsBody); // ���� �ٵ� Ÿ�� ����
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block); // ��� ä�ο��� �浹 ó��
		WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // Pawn Ŭ���� �浹 ����
		WeaponMesh->SetEnableGravity(true); // �߷� ����
		// �÷��̾� �������� ������ ���� �߰� (������ ������)
		WeaponMesh->AddImpulse(PlayerCharacter->GetActorForwardVector() * 200.0f, NAME_None, true);
	}

	// ���� ���� ����
	Destroy();
}

bool AFirearmWeapon::CanReload()
{
	if (bIsEquipping || bIsReloading || bIsFiring)
	{
		return false;
	}

	return true;
}

void AFirearmWeapon::BeginReload()
{
	if (bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is already reloading!"), *GetName());
		return;
	}

	bIsReloading = true;
	UE_LOG(LogTemp, Log, TEXT("%s reloading..."), *GetName());

	GetWorldTimerManager().SetTimer(ReloadTimerHandle,
		this,
		&AFirearmWeapon::Reload,
		FirearmStats.ReloadTime,
		false
	);
}

void AFirearmWeapon::Reload()
{
	// ������ �� ź�� ����
	FirearmStats.AmmoCount = FirearmStats.MagazineCapacity;
	UE_LOG(LogTemp, Log, TEXT("%s reloaded. Ammo: %d/%d"), *GetName(), FirearmStats.AmmoCount, FirearmStats.MagazineCapacity);

	EndReload();
}

void AFirearmWeapon::EndReload()
{
	bIsReloading = false;
}

bool AFirearmWeapon::bIsInAim()
{
	return bIsAiming;
}

bool AFirearmWeapon::CanAim()
{
	if (bIsEquipping || bIsReloading || bIsAiming)
	{
		return false;
	}

	return true;
}

void AFirearmWeapon::SwitchAiming()
{
	if (!bIsAiming)
	{
		BeginAiming();
	}
	else
	{
		EndAiming();
	}
}

void AFirearmWeapon::BeginAiming()
{
	bIsAiming = true;

	if (FirearmStats.AimCurve) {
		FirearmStats.Timeline->PlayFromStart();
		FirearmStats.AimData.SetData(Owner);

		return;
	}

	FirearmStats.AimData.SetDataByNoneCurve(Owner);

	return;
}

void AFirearmWeapon::AimDownSights(float Output)
{
	bIsAiming = true;

	UCameraComponent* Camera = Owner->FindComponentByClass<UCameraComponent>();
	if (Camera)
	{
		Camera->FieldOfView = FMath::Lerp(FirearmStats.AimData.FieldOfView, FirearmStats.BaseData.FieldOfView, Output);
	}

	return;
}

void AFirearmWeapon::EndAiming()
{
	if (!bIsAiming)
	{
		return;
	}

	if (FirearmStats.AimCurve) {
		FirearmStats.Timeline->PlayFromStart();
		FirearmStats.BaseData.SetData(Owner);

		return;
	}

	FirearmStats.BaseData.SetDataByNoneCurve(Owner);
	bIsAiming = false;

	return;
}

void AFirearmWeapon::ToggleAutoFire()
{
	if (FirearmStats.bHasAutomaticFireMode)
	{
		bIsAutoFire = !bIsAutoFire;
	}
}

int32 AFirearmWeapon::GetMagazinCapacity() const
{
	return FirearmStats.MagazineCapacity;
}

int32 AFirearmWeapon::GetCurrentAmmo() const
{
	return FirearmStats.AmmoCount;
}

EFirearmType AFirearmWeapon::GetFirearmType() const
{
	return FirearmStats.FirearmType;
}

void AFirearmWeapon::SetWeaponData(const FWeaponItemData& NewWeaponData)
{
	Super::SetWeaponData(NewWeaponData);

	FirearmStats = NewWeaponData.FirearmStats;

	// ���� ���� ���� �ʱ�ȭ
	bIsReloading = false;
	bIsFiring = false;
	bIsAiming = false;
	CurrentRecoil = 0.0f;
}

void AFirearmWeapon::UpdateMesh()
{
	// ������ �� �浹 ��Ȱ��ȭ
	if (SkeletalMeshComponent)
	{
		// �⺻ �ּ� ����
		SkeletalMeshComponent->SetSkeletalMeshAsset(ItemData->GetItemData().AssetData.SkeletalMesh);
		SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset);
		SkeletalMeshComponent->SetVisibility(true);

		SetActorLocation(FVector::ZeroVector);
		SetActorRotation(FRotator::ZeroRotator);

		// ���� �ùķ��̼� ��Ȱ��ȭ
		SkeletalMeshComponent->SetSimulatePhysics(false);

		// �浹 ��Ȱ��ȭ (������ ���¿����� �浹�� ��Ȱ��ȭ)
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	// �⺻ ������ �ּ� ���� (�ʿ��)
	if (ItemData)
	{
		SkeletalMeshComponent->SetSkeletalMesh(ItemData->GetItemData().AssetData.SkeletalMesh);
		SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset);
	}
}

USkeletalMeshComponent* AFirearmWeapon::GetWeaponMesh()
{
	return SkeletalMeshComponent;
}
