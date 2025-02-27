// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "SPT/Items/Worlds/WorldWeapon.h"
#include "SPTPlayerCharacter.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetSimulatePhysics(false); // ���� ��Ȱ��ȭ
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �浹 ��Ȱ��ȭ
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
	UItemDataObject* NewItemData = ItemData->CreateItemCopy();
	DroppedWeapon->SetItemData(NewItemData);
	DroppedWeapon->UpdateMesh();

	// ���� ȿ�� �����Ͽ� �ڿ������� �������� ����
	if (UStaticMeshComponent* WeaponMesh = DroppedWeapon->FindComponentByClass<UStaticMeshComponent>())
	{
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetCollisionObjectType(ECC_PhysicsBody);
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
		WeaponMesh->AddImpulse(PlayerCharacter->GetActorForwardVector() * 200.0f, NAME_None, true); // �÷��̾� �������� ����
	}

	// ���� ���� ����
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

	// ���� ��ġ�� �������� ���
	FVector ItemLocation = PlayerCharacter->GetActorLocation();
	FVector DropLocation = ItemLocation;
	FRotator DropRotation = PlayerCharacter->GetActorRotation();

	// �ٴ� ������ ���� LineTrace ����
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
