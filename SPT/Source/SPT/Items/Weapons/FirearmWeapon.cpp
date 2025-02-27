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

	// 기본 피직스 애셋 설정 (필요시)
	if (ItemData)
	{
		SkeletalMeshComponent->SetSkeletalMesh(ItemData->GetItemData().AssetData.SkeletalMesh);
		SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset);
	}

	// 초기 상태 설정
	bIsReloading = false;
	bIsFiring = false;
	bIsAiming = false;
	CurrentRecoil = 0.0f;
}

void AFirearmWeapon::Attack()
{
	// TODO : 발사 구현, 인벤토리와 연결
	if (FirearmStats.AmmoCount <= 0 || bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is out of ammo or reloading!"), *GetName());
		return;
	}

	// 사격 상태로 전환
	bIsFiring = true;
	FirearmStats.AmmoCount--;
	CurrentRecoil += FirearmStats.Recoil;  // 반동 누적

	UE_LOG(LogTemp, Log, TEXT("%s fired! Ammo left: %d, Recoil: %.2f"), *GetName(), FirearmStats.AmmoCount, CurrentRecoil);
}

void AFirearmWeapon::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter || !ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirearmWeapon::Equip - PlayerCharacter is null!"));
		return;
	}

	// 기존에 장착된 총기가 있다면 먼저 해제
	/*
	AWeaponBase* EquippedWeapon = PlayerCharacter->GetEquippedWeapon();
	if (EquippedWeapon && EquippedWeapon->IsA(AFirearmWeapon::StaticClass()))
	{
		EquippedWeapon->UnEquip(PlayerCharacter);
	}
	*/
	// 새 총기 장착
	//PlayerCharacter->EquipWeapon(this);

	// 피직스 및 충돌 비활성화
	if (SkeletalMeshComponent)
	{
		// 기본 애셋 설정
		SkeletalMeshComponent->SetSkeletalMeshAsset(ItemData->GetItemData().AssetData.SkeletalMesh);
		SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset);
		SkeletalMeshComponent->SetVisibility(true);

		// 물리 시뮬레이션 비활성화
		SkeletalMeshComponent->SetSimulatePhysics(false);

		// 충돌 비활성화 (장착된 상태에서는 충돌을 비활성화)
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	// 장착된 위치로 이동
	// PlayerCharacter->EquipWeapon(this); 대신
	FName SocketName = ItemData->GetItemData().AssetData.AttachSocketName;
	if (SocketName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("Equip - No AttachSocketName set in ItemData, defaulting to 'hand_r'."));
		SocketName = TEXT("hand_r");
	}
	AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	
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
