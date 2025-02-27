// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "SPT/Items/Worlds/WorldWeapon.h"
#include "SPTPlayerCharacter.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetSimulatePhysics(false); // 물리 비활성화
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
	}
}

void AWeaponBase::PrimaryAction(ASPTPlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		Attack();
	}
}

void AWeaponBase::Drop(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWeaponBase::Drop - PlayerCharacter is null!"));
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
	UItemDataObject* NewItemData = ItemData->CreateItemCopy();
	DroppedWeapon->SetItemData(NewItemData);
	DroppedWeapon->UpdateMesh();

	// 물리 효과 적용하여 자연스럽게 떨어지게 설정
	if (UStaticMeshComponent* WeaponMesh = DroppedWeapon->FindComponentByClass<UStaticMeshComponent>())
	{
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetCollisionObjectType(ECC_PhysicsBody);
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
		WeaponMesh->AddImpulse(PlayerCharacter->GetActorForwardVector() * 200.0f, NAME_None, true); // 플레이어 방향으로 던짐
	}

	// 원본 무기 제거
	Destroy();
}

const FWeaponItemData& AWeaponBase::GetWeaponData() const
{
	return WeaponData;
}

void AWeaponBase::SetWeaponData(const FWeaponItemData& NewWeaponData)
{
	WeaponData = NewWeaponData;
}

void AWeaponBase::GetDropLocation(ASPTPlayerCharacter* PlayerCharacter, FVector& OutDropLocation, FRotator& OutDropRotation)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("GetDropLocation: World is null"));
		return;
	}

	// 현재 위치를 기준으로 드랍
	FVector ItemLocation = PlayerCharacter->GetActorLocation();
	FVector DropLocation = ItemLocation;
	FRotator DropRotation = PlayerCharacter->GetActorRotation();

	// 바닥 감지를 위한 LineTrace 설정
	FHitResult HitResult;
	FVector TraceStart = ItemLocation;
	FVector TraceEnd = TraceStart + FVector(0.f, 0.f, -1000.f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(PlayerCharacter);

	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_WorldStatic,
		QueryParams);

	if (bHit)
	{
		OutDropLocation = HitResult.ImpactPoint + FVector(0.f, 0.f, 10.f);
		UE_LOG(LogTemp, Log, TEXT("GetDropLocation: Found ground at %s"), *OutDropLocation.ToString());
	}
	else
	{
		OutDropLocation = ItemLocation + FVector(0.f, 0.f, -30.f);
		UE_LOG(LogTemp, Warning, TEXT("GetDropLocation: No ground found, dropping at default location %s"), *OutDropLocation.ToString());
	}
}
