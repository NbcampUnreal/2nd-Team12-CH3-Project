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

	// Skeletal Mesh 추가
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	if (StaticMeshComponent && SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetupAttachment(RootComponent);
	}

	// 초기 상태 설정
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
			// 발사 속도 제어
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

	// 연사 멈춤
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

	// 탄착군 형성
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

	if (bHit)	// 발사한 LineTrace가 Object에 맞아 Block 충돌이 되었다면
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->CanBeDamaged())  // 피격 가능한 대상인지 확인
		{
			float Damage = WeaponData.WeaponStats.Damage; // 무기의 기본 피해량
			FDamageEvent DamageEvent;
			AController* InstigatorController = Owner->GetInstigatorController(); // 총을 쏜 플레이어의 컨트롤러

			float ActualDamage = HitActor->TakeDamage(Damage, DamageEvent, InstigatorController, this);

			UE_LOG(LogTemp, Log, TEXT("%s hit %s! Applied Damage: %.2f"), *GetName(), *HitActor->GetName(), ActualDamage);
		}

		if (FirearmStats.HitDecal)
		{
			FRotator Rotator = HitResult.ImpactNormal.Rotation();	// HitDecal이 붙을 방향 (충돌의 Normal)
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
			// Hit 된 지점으로부터 최초로 쏜 지점(= 플레이어)을 향하도록 설정
			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(HitResult.Location, HitResult.TraceStart);
			//Particle이 충돌 위치에서 쏜 위치를 향하도록 나옴
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

	// 카메라 흔들림 
	if (FirearmStats.CameraShakeClass)
	{
		APlayerController* Controller = Owner->GetController<ASPTPlayerController>();
		if (Controller)
		{
			Controller->PlayerCameraManager->StartCameraShake(FirearmStats.CameraShakeClass);
		}
	}
	
	// 총기 반동
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
		// 총기 손에 부착
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
	// 프리뷰 캐릭터에 무기 반영을 위해서 넣은 부분입니다.
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

	// 드롭 위치 계산
	FVector DropLocation;
	FRotator DropRotation;
	GetDropLocation(PlayerCharacter, DropLocation, DropRotation);

	// AWorldWeapon 생성
	AWorldWeapon* DroppedWeapon = GetWorld()->SpawnActor<AWorldWeapon>(AWorldWeapon::StaticClass(), DropLocation, DropRotation);
	if (!DroppedWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn AWorldWeapon!"));
		return;
	}

	// 기존 무기 데이터를 AWorldWeapon에 복사
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

	// 피직스 및 충돌 활성화
	if (USkeletalMeshComponent* WeaponMesh = DroppedWeapon->FindComponentByClass<USkeletalMeshComponent>())
	{
		WeaponMesh->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset); // 피직스 애셋 연결
		WeaponMesh->SetSimulatePhysics(true); // 물리 시뮬레이션 활성화
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
		WeaponMesh->SetCollisionObjectType(ECC_PhysicsBody); // 물리 바디 타입 설정
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block); // 모든 채널에서 충돌 처리
		WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // Pawn 클래스 충돌 무시
		WeaponMesh->SetEnableGravity(true); // 중력 적용
		// 플레이어 방향으로 물리적 힘을 추가 (아이템 던지기)
		WeaponMesh->AddImpulse(PlayerCharacter->GetActorForwardVector() * 200.0f, NAME_None, true);
	}

	// 원본 무기 제거
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
	// 재장전 후 탄약 충전
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

	// 개별 무기 상태 초기화
	bIsReloading = false;
	bIsFiring = false;
	bIsAiming = false;
	CurrentRecoil = 0.0f;
}

void AFirearmWeapon::UpdateMesh()
{
	// 피직스 및 충돌 비활성화
	if (SkeletalMeshComponent)
	{
		// 기본 애셋 설정
		SkeletalMeshComponent->SetSkeletalMeshAsset(ItemData->GetItemData().AssetData.SkeletalMesh);
		SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset);
		SkeletalMeshComponent->SetVisibility(true);

		SetActorLocation(FVector::ZeroVector);
		SetActorRotation(FRotator::ZeroRotator);

		// 물리 시뮬레이션 비활성화
		SkeletalMeshComponent->SetSimulatePhysics(false);

		// 충돌 비활성화 (장착된 상태에서는 충돌을 비활성화)
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	// 기본 피직스 애셋 설정 (필요시)
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
