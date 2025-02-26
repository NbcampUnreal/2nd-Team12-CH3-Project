// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldItemBase.h"
#include "SPTPlayerCharacter.h"
#include "SPT/Items/Data/ItemDataObject.h"
#include "SPT/Items/Weapons/WeaponBase.h"

AWorldItemBase::AWorldItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemState = EItemState::EIS_World;
}

void AWorldItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWorldItemBase::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	// 플레이어의 인벤토리에 추가 (인벤토리 시스템이 구현되어 있어야 함)
	UE_LOG(LogTemp, Log, TEXT("%s picked up by player"), *GetName());

	// 아이템을 제거 (또는 숨김 처리)
	Destroy();
}

void AWorldItemBase::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter || !ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWorldItemBase::OnDrop - Invalid data!"));
		return;
	}

	FVector DropLocation;
	FRotator DropRotation;
	GetDropLocation(PlayerCharacter, DropLocation, DropRotation);

	// 새로운 AWorldItemBase 생성
	AWorldItemBase* DroppedItem = GetWorld()->SpawnActor<AWorldItemBase>(AWorldItemBase::StaticClass(), DropLocation, DropRotation);
	if (!DroppedItem)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn AWorldItemBase!"));
		return;
	}

	// 기존 아이템 데이터 유지
	DroppedItem->SetItemData(GetItemData());

	// 물리 적용
	UStaticMeshComponent* ItemMesh = DroppedItem->FindComponentByClass<UStaticMeshComponent>();
	if (ItemMesh)
	{
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->AddImpulse(FVector(FMath::RandRange(-100.f, 100.f), FMath::RandRange(-100.f, 100.f), 200.f), NAME_None, true);
	}

	/* 플레이어 인벤토리에서 제거 (필요의 경우) */
	// PlayerCharacter->RemoveItemFromInventory(this);

	// 원본 객체 제거
	Destroy();
	UE_LOG(LogTemp, Log, TEXT("%s dropped successfully"), *GetName());
}

void AWorldItemBase::BeginFocus()
{
	UE_LOG(LogTemp, Log, TEXT("WorldItemBase BeginFocus : % s"), *GetName());
}

void AWorldItemBase::EndFocus()
{
	UE_LOG(LogTemp, Log, TEXT("WorldItemBase EndFocus: %s"), *GetName());
}

void AWorldItemBase::BeginInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on interface test actor."));
}

void AWorldItemBase::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on interface test actor."));
}

void AWorldItemBase::Interact(ASPTPlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on interface test actor."));
		OnPickup(PlayerCharacter);
	}
}

void AWorldItemBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 변경된 프로퍼티가 존재하는지 확인
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? FName(*PropertyChangedEvent.Property->GetName()) : NAME_None;

	// ItemID가 변경된 경우에만 데이터 테이블을 업데이트
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AWorldItemBase, ItemID))
	{
		if (!ItemData)
		{
			ItemData = NewObject<UItemDataObject>(this, UItemDataObject::StaticClass());
		}

		ItemData->InitializeItemData(ItemID);
		UpdateMesh();
		UE_LOG(LogTemp, Log, TEXT("PostEditChangeProperty - ItemData updated for %s"), *ItemID.ToString());
	}
}

void AWorldItemBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!ItemDataTable || ItemID.IsNone())
	{
		UE_LOG(LogTemp, Log, TEXT("AWorldItemBase::OnConstruction: There is No ItemDataTable or ItemID for %s"), *GetName());
		return;
	}

	if (!ItemData)
	{
		ItemData = NewObject<UItemDataObject>(this, UItemDataObject::StaticClass());
	}

	ItemData->SetItemData(ItemData->GetItemData());
	UE_LOG(LogTemp, Log, TEXT("OnConstruction: Item data loaded for %s"), *GetName());

	UpdateMesh();
}

FVector AWorldItemBase::GetDropLocation(ASPTPlayerCharacter* PlayerCharacter, FVector& OutDropLocation, FRotator& OutDropRotation)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("GetDropLocation: World is null"));
		return PlayerCharacter->GetActorLocation();
	}

	// 현재 위치를 기준으로 드랍
	FVector ItemLocation = PlayerCharacter->GetActorLocation();
	OutDropLocation = ItemLocation;
	OutDropRotation = PlayerCharacter->GetActorRotation();

	// 바닥 감지를 위한 LineTrace 설정
	FHitResult HitResult;
	FVector TraceStart = ItemLocation;
	FVector TraceEnd = TraceStart + FVector(0.f, 0.f, -500.f);
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
		OutDropLocation = ItemLocation + FVector(0.f, 0.f, 0.f);
		UE_LOG(LogTemp, Warning, TEXT("GetDropLocation: No ground found, dropping at default location %s"), *OutDropLocation.ToString());
	}

	// 랜덤 회전 적용
	OutDropRotation += FRotator(FMath::RandRange(-20.f, 20.f), FMath::RandRange(-180.f, 180.f), 0.f);

	return OutDropLocation;
}

void AWorldItemBase::UpdateMesh()
{
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateMesh: ItemData is null for %s"), *GetName());
		return;
	}

	// 메쉬 업데이트 로직
	const FItemData& FoundItemData = ItemData->GetItemData();

	// 스태틱 메쉬 업데이트
	if (FoundItemData.AssetData.StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(FoundItemData.AssetData.StaticMesh);
		StaticMeshComponent->SetVisibility(true);
	}
}
