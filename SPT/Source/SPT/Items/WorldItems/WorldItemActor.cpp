// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldItemActor.h"
#include "SPTPlayerCharacter.h"
#include "SPT/Items/Data/ItemDataObject.h"
#include "SPT/Items/Weapons/WeaponActor.h"
// #include "InventoryComponent.h"

AWorldItemActor::AWorldItemActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AWorldItemActor::BeginPlay()
{
    Super::BeginPlay();

    // 아이템 데이터를 DataTable에서 로드
    InitializeItemFromDataTable();

    if (ItemData.ItemType == EItemType::EIT_Weapon && SkeletalMeshComponent)
    {
        // 무기 물리 설정
        SkeletalMeshComponent->SetPhysicsAsset(ItemData.WeaponData.PhysicsAsset);
        SkeletalMeshComponent->SetSimulatePhysics(true);
        SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        SkeletalMeshComponent->SetVisibility(true);

        // 플레이어와의 충돌을 무시
        SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

        // 환경과의 충돌 유지
        SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
        SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

        // 라인트레이스가 감지를 위한 설정
        SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

        // 무기 이동 제한
        SkeletalMeshComponent->BodyInstance.bLockXTranslation = true;
        SkeletalMeshComponent->BodyInstance.bLockYTranslation = true;
        SkeletalMeshComponent->BodyInstance.bLockZTranslation = false; // Z축만 허용 (떨어지는 동작 가능)

        SkeletalMeshComponent->BodyInstance.bLockXRotation = true;
        SkeletalMeshComponent->BodyInstance.bLockYRotation = true;
        SkeletalMeshComponent->BodyInstance.bLockZRotation = true;

        UE_LOG(LogTemp, Log, TEXT("Weapon Initialized in BeginPlay with Physics Constraints: %s"), *GetName());
    }
}

void AWorldItemActor::InitializeItemFromDataTable()
{
    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemDataTable is not assigned in WorldItemActor!"));
        return;
    }

    if (ItemID.IsNone())
    {
        UE_LOG(LogTemp, Error, TEXT("WorldItemActor has no valid ItemID!"));
        return;
    }

    const FString ContextString = TEXT("ItemDataTable Lookup");
    FItemData* FoundItemData = ItemDataTable->FindRow<FItemData>(ItemID, ContextString);
    if (!FoundItemData)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemID '%s' not found in ItemDataTable!"), *ItemID.ToString());
        return;
    }

    // 데이터 가져오기
    SetItemData(*FoundItemData);

    // 무기인 경우 Physics Asset 적용
    if (ItemData.ItemType == EItemType::EIT_Weapon)
    {
        if (SkeletalMeshComponent)
        {
            SkeletalMeshComponent->SetSkeletalMesh(ItemData.AssetData.SkeletalMesh);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Item '%s' initialized with ItemType: %d"), *ItemID.ToString(), static_cast<int32>(ItemData.ItemType));
}

void AWorldItemActor::InitializeItem(const FItemData& NewItemData)
{
    SetItemData(NewItemData);

    UE_LOG(LogTemp, Log, TEXT("AWorldItemActor Initialized with ItemType: %d (Expected Weapon: %d, Consumable: %d)"),
        static_cast<int32>(GetItemData().ItemType),
        static_cast<int32>(EItemType::EIT_Weapon),
        static_cast<int32>(EItemType::EIT_Consumable));
}

void AWorldItemActor::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnPickup: PlayerCharacter is null"));
        return;
    }

    if (ItemData.ItemID.IsNone())
    {
        UE_LOG(LogTemp, Error, TEXT("OnPickup: Invalid ItemID for %s"), *GetName());
        return;
    }

    if (ItemData.ItemType == EItemType::EIT_Weapon)
    {
        AWeaponActor* Weapon = Cast<AWeaponActor>(this);
        if (Weapon)
        {
            Weapon->OnPickup(PlayerCharacter);
            return;
        }
    }

    // 인벤토리에 추가
    /*
    if (PlayerCharacter->AddItemToInventory(this))
    {
        UE_LOG(LogTemp, Log, TEXT("OnPickup: %s added to %s's inventory"), *ItemData.TextData.Name.ToString(), *PlayerCharacter->GetName());
        Destroy();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnPickup: Inventory full, unable to add %s"), *ItemData.TextData.Name.ToString());
    }
    */
}

void AWorldItemActor::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnDrop: PlayerCharacter is null"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("OnDrop: World is null"));
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
    QueryParams.AddIgnoredActor(this);  // 자기 자신을 무시
    QueryParams.AddIgnoredActor(PlayerCharacter);  // 플레이어 무시

    bool bHit = World->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_Visibility,
        QueryParams);

    if (bHit)
    {
        DropLocation = HitResult.ImpactPoint + FVector(0.f, 0.f, 10.f);
        FRotator RandomRotation = FRotator(FMath::RandRange(-20.f, 20.f), FMath::RandRange(-180.f, 180.f), 0.f);
        DropRotation += RandomRotation;
        UE_LOG(LogTemp, Log, TEXT("OnDrop: Found ground at %s"), *DropLocation.ToString());
    }
    else
    {
        DropLocation = ItemLocation + FVector(0.f, 0.f, -30.f);
        UE_LOG(LogTemp, Warning, TEXT("OnDrop: No ground found, dropping at default location %s"), *DropLocation.ToString());
    }

    SpawnDroppedItem(DropLocation, DropRotation);
    UpdateItemState(EItemState::EIS_World);
}

void AWorldItemActor::SpawnDroppedItem(FVector SpawnLocation, FRotator SpawnRotation)
{
    if (Quantity <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnDroppedItem: Invalid drop amount"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnDroppedItem: World is null"));
        return;
    }

    // 월드에 아이템 생성
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    AWorldItemActor* DroppedItem = World->SpawnActor<AWorldItemActor>(
        GetClass(),
        SpawnLocation,
        SpawnRotation,
        SpawnParams);

    if (DroppedItem)
    {
        DroppedItem->SetItemData(ItemData);
        UE_LOG(LogTemp, Log, TEXT("SpawnDroppedItem: Dropped %d of %s"), Quantity, *ItemData.ItemID.ToString());
    }
}

void AWorldItemActor::BeginFocus()
{
    UE_LOG(LogTemp, Log, TEXT("WorldItemActor BeginFocus : % s"), *GetName());

    if (SkeletalMeshComponent)
    {
        SkeletalMeshComponent->SetRenderCustomDepth(true);
        SkeletalMeshComponent->CustomDepthStencilValue = 1; // 하이라이트 효과 추가 가능
    }
}

void AWorldItemActor::EndFocus()
{
    UE_LOG(LogTemp, Log, TEXT("WorldItemActor EndFocus: %s"), *GetName());

    if (SkeletalMeshComponent)
    {
        SkeletalMeshComponent->SetRenderCustomDepth(false);
    }
}

void AWorldItemActor::BeginInteract()
{
    UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on interface test actor."));
}

void AWorldItemActor::EndInteract()
{
    UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on interface test actor."));
}

void AWorldItemActor::Interact(ASPTPlayerCharacter* PlayerCharacter)
{
    if (PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on interface test actor."));
        OnPickup(PlayerCharacter);
    }
}

void AWorldItemActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FName ChangedPropertyName = PropertyChangedEvent.Property ? FName(*PropertyChangedEvent.Property->GetName()) : NAME_None;

    if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AWorldItemActor, ItemID))
    {
        if (ItemDataTable)
        {
            const FString ContextString{ ItemID.ToString() };

            if (const FItemData* FoundItemData = ItemDataTable->FindRow<FItemData>(ItemID, ItemID.ToString()))
            {
                SetItemData(*FoundItemData);
            }
        }
    }
}

#if WITH_EDITOR  // 에디터에서만 실행
void AWorldItemActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);


    if (ItemDataTable && !ItemID.IsNone())
    {
        const FString ContextString = TEXT("ItemDataTable Lookup");
        FItemData* FoundItemData = ItemDataTable->FindRow<FItemData>(ItemID, ContextString);
        if (FoundItemData)
        {
            SetItemData(*FoundItemData);

            if (ItemData.ItemType == EItemType::EIT_Weapon && SkeletalMeshComponent)
            {
                SkeletalMeshComponent->SetSkeletalMesh(ItemData.AssetData.SkeletalMesh);
                SkeletalMeshComponent->SetVisibility(true);
                UE_LOG(LogTemp, Log, TEXT("OnConstruction: Skeletal Mesh Set for %s"), *GetName());
            }
        }
    }
}
#endif