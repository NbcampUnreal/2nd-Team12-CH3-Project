// Fill out your copyright notice in the Description page of Project Settings.


#include "FirearmWeapon.h"
#include "SPTPlayerCharacter.h"
#include "SPT/Items/Worlds/WorldWeapon.h"
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
	bIsFiring = true;
	Attack();
}

void AFirearmWeapon::End_Fire()
{
	if (!bIsFiring)
	{
		return;
	}

	bIsFiring = false;
}

void AFirearmWeapon::Attack()
{
	// TODO : 발사 구현, 인벤토리와 연결
	if (FirearmStats.AmmoCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is out of ammo!"), *GetName());
		return;
	}

	// 사격 상태로 전환
	FirearmStats.AmmoCount--;
	CurrentRecoil += FirearmStats.Recoil;  // 반동 누적

	UE_LOG(LogTemp, Log, TEXT("%s fired! Ammo left: %d, Recoil: %.2f"), *GetName(), FirearmStats.AmmoCount, CurrentRecoil);
}

bool AFirearmWeapon::CanEquip()
{
	if (ItemState == EItemState::EIS_Equipped || bIsReloading || !bIsFiring)
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
	// 기존에 장착된 총기가 있다면 먼저 해제
	if (AWeaponBase* EquippedWeapon = Owner->GetEquippedWeapon())
	{
		if (EquippedWeapon && EquippedWeapon->IsA(AFirearmWeapon::StaticClass()))
		{
			Drop(Owner);
		}
	}

	// 장착된 위치로 이동
	// PlayerCharacter->EquipWeapon(this); 대신
	if (FirearmStats.RightHandSocketName.IsValid())
	{
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
