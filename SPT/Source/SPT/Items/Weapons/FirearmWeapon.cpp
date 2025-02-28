// Fill out your copyright notice in the Description page of Project Settings.


#include "FirearmWeapon.h"
#include "SPTPlayerCharacter.h"
#include "SPTPlayerController.h"
#include "SPT/Items/Worlds/WorldWeapon.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"

AFirearmWeapon::AFirearmWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

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
}

void AFirearmWeapon::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ASPTPlayerCharacter>(GetOwner());
	/*
	if (FirearmStats.HolsterSocketName.IsValid())
	{
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FirearmStats.HolsterSocketName);
	}
	*/
}

void AFirearmWeapon::Attack()
{
	Begin_Fire();
}

bool AFirearmWeapon::CanFire()
{
	if (bIsEquipping || bIsFiring || bIsReloading)
	{
		return false;
	}

	return true;;
}

void AFirearmWeapon::Begin_Fire()
{
	if (CanFire())
	{
		bIsFiring = true;

		// 발사 속도 제어
		GetWorld()->GetTimerManager().SetTimer(
			FireTimerHandle,
			this,
			&AFirearmWeapon::Attack,
			WeaponData.WeaponStats.AttackRate, 
			true);

		OnFiring();
	}
}

void AFirearmWeapon::End_Fire()
{
	if (!bIsFiring)
	{
		return;
	}

	bIsFiring = false;

	// 타이머 정지 (연사 멈춤, 미구현)
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void AFirearmWeapon::OnFiring()
{
	if (FirearmStats.AmmoCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is out of ammo!"), *GetName());
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
	UKismetSystemLibrary::LineTraceSingle(
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

	if (HitResult.bBlockingHit)	// 발사한 LineTrace가 Object에 맞아 Block 충돌이 되었다면
	{
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

		/* 카메라 흔들림 (미구현)
		if (FirearmStats.CameraShakeClass)
		{
			APlayerController* Controller = Owner->GetController<ASPTPlayerController>();
			if (Controller)
			{
				ConstructorHelpers::FClassFinder<UMatineeCameraShake>(FirearmStats.CameraShakeClass, );
				Controller->PlayerCameraManager->StartCameraShake(FirearmStats.CameraShakeClass);
			}
		}
		*/
	}

	UE_LOG(LogTemp, Log, TEXT("%s fired! Ammo left: %d, Recoil: %.2f"), *GetName(), FirearmStats.AmmoCount, CurrentRecoil);
	End_Fire();
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

	Begin_Equip();
}

void AFirearmWeapon::Begin_Equip()
{
	// Equip 함수와 반대가 된 거 같기도 합니다만,
	// 캐릭터에서 Equip(this)로 넘기고 있기에 그냥 사용하였습니다.
	//Owner->EquipWeapon(this);
	UpdateMesh();

	if (FirearmStats.RightHandSocketName.IsValid())
	{
		// 총기 손에 부착
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FirearmStats.RightHandSocketName);
	}

	ItemState = EItemState::EIS_Equipped;
	UE_LOG(LogTemp, Log, TEXT("%s equipped by %s"), *GetName(), *Owner->GetName());

	End_Equip();
}

void AFirearmWeapon::End_Equip()
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

bool AFirearmWeapon::CanRelad()
{
	if (bIsEquipping || bIsReloading || bIsFiring)
	{
		return false;
	}

	return true;
}

void AFirearmWeapon::Begin_Reload()
{
	if (bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is already reloading!"), *GetName());
		return;
	}

	bIsReloading = true;
	UE_LOG(LogTemp, Log, TEXT("%s reloading..."), *GetName());
}

void AFirearmWeapon::Reload()
{
	// 재장전 후 탄약 충전
	FirearmStats.AmmoCount = FirearmStats.MagazineCapacity;
	UE_LOG(LogTemp, Log, TEXT("%s reloaded. Ammo: %d/%d"), *GetName(), FirearmStats.AmmoCount, FirearmStats.MagazineCapacity);
}

void AFirearmWeapon::End_Reload()
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

void AFirearmWeapon::AimDownSights()
{
	bIsAiming = true;
	UE_LOG(LogTemp, Log, TEXT("FirearmWeapon: Aiming Down Sights!"));

	// TODO: 에임

	return;
}

void AFirearmWeapon::StopAiming()
{
	if (!bIsAiming)
	{
		return;
	}

	bIsAiming = false;
	UE_LOG(LogTemp, Log, TEXT("FirearmWeapon: %s stopped aiming"), *GetName());

	// TODO: 에임 해제
	
	return;
}

int32 AFirearmWeapon::GetCurrentAmmo() const
{
	return FirearmStats.AmmoCount;
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