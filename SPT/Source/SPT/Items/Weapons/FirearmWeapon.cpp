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
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	// 기본 피직스 애셋 설정 (필요시)
	SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset);

	// 초기 상태 설정
	bIsReloading = false;
	bIsFiring = false;
	bIsAiming = false;
	CurrentRecoil = 0.0f;
}

void AFirearmWeapon::Attack()
{
	// TODO : 발사 구현, 인벤토리와 연결
	if (CurrentAmmo <= 0 || bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is out of ammo or reloading!"), *GetName());
		return;
	}

	// 사격 상태로 전환
	bIsFiring = true;
	CurrentAmmo--;
	CurrentRecoil += FirearmStats.Recoil;  // 반동 누적

	UE_LOG(LogTemp, Log, TEXT("%s fired! Ammo left: %d, Recoil: %.2f"), *GetName(), CurrentAmmo, CurrentRecoil);
}

void AFirearmWeapon::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirearmWeapon::Equip - PlayerCharacter is null!"));
		return;
	}

	/* 캐릭터의 일부 함수 필요
	// 기존에 장착된 총기가 있다면 먼저 해제
	AWeaponBase* EquippedWeapon = PlayerCharacter->GetEquippedWeapon();
	if (EquippedWeapon && EquippedWeapon->IsA(AFirearmWeapon::StaticClass()))
	{
		EquippedWeapon->UnEquip(PlayerCharacter);
	}

	// 새 총기 장착
	PlayerCharacter->EquipWeapon(this);
	*/

	// 피직스 및 충돌 비활성화
	if (SkeletalMeshComponent)
	{
		// 물리 시뮬레이션 비활성화
		SkeletalMeshComponent->SetSimulatePhysics(false);

		// 충돌 비활성화 (장착된 상태에서는 충돌을 비활성화)
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	// 장착된 위치로 이동
	// PlayerCharacter->EquipWeapon(this); 대신
	AttachToActor(PlayerCharacter, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemData->GetItemData().AssetData.AttachSocketName);
	ItemState = EItemState::EIS_Equipped;
	UE_LOG(LogTemp, Log, TEXT("%s equipped by %s"), *GetName(), *PlayerCharacter->GetName());
}


void AFirearmWeapon::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirearmWeapon::UnEquip - PlayerCharacter is null!"));
		return;
	}

	// 플레이어의 현재 장착 무기 해제
	// PlayerCharacter->UnEquipWeapon();
	UE_LOG(LogTemp, Log, TEXT("%s unequipped by %s"), *GetName(), *PlayerCharacter->GetName());

	// 무기를 바닥에 드롭
	Drop(PlayerCharacter);
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
	DroppedWeapon->SetItemData(ItemData);

	// 피직스 및 충돌 활성화
	if (USkeletalMeshComponent* WeaponMesh = DroppedWeapon->FindComponentByClass<USkeletalMeshComponent>())
	{
		WeaponMesh->SetSimulatePhysics(true); // 물리 시뮬레이션 활성화
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
		WeaponMesh->SetCollisionObjectType(ECC_PhysicsBody); // 물리 바디 타입 설정
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block); // 모든 채널에서 충돌 처리

		// 플레이어 방향으로 물리적 힘을 추가 (아이템 던지기)
		WeaponMesh->AddImpulse(PlayerCharacter->GetActorForwardVector() * 200.0f, NAME_None, true);
	}

	// 원본 무기 제거
	ItemState = EItemState::EIS_World;
	Destroy();
}

void AFirearmWeapon::Reload()
{
	if (bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is already reloading!"), *GetName());
		return;
	}

	bIsReloading = true;
	UE_LOG(LogTemp, Log, TEXT("%s reloading..."), *GetName());

	// 재장전 후 탄약 충전
	CurrentAmmo = FirearmStats.MagazineCapacity;
	bIsReloading = false;

	UE_LOG(LogTemp, Log, TEXT("%s reloaded. Ammo: %d/%d"), *GetName(), CurrentAmmo, FirearmStats.MagazineCapacity);
}

void AFirearmWeapon::AimDownSights()
{
	// TODO: 에임
	UE_LOG(LogTemp, Log, TEXT("FirearmWeapon: Aiming Down Sights!"));
}

void AFirearmWeapon::StopAiming()
{
	// TODO: 에임 해제
	UE_LOG(LogTemp, Log, TEXT("FirearmWeapon: %s stopped aiming"), *GetName());
}

int32 AFirearmWeapon::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

void AFirearmWeapon::InitializeFirearmData(const FWeaponItemData& NewWeaponData)
{
	FirearmStats = ItemData->GetWeaponData().FirearmStats;

	// 개별 무기 상태 초기화
	CurrentAmmo = FirearmStats.MagazineCapacity;
	bIsReloading = false;
	bIsFiring = false;
	bIsAiming = false;
	CurrentRecoil = 0.0f;
}
