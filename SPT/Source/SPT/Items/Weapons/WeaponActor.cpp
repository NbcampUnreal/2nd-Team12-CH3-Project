// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "SPT/Characters/SPTPlayerCharacter.h"
// #include "SPT/Inventory/InventoryComponent.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;

	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetSimulatePhysics(false); // 물리 비활성화
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
	}

}

void AWeaponActor::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter) return;

	if (ItemData.ItemID.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("OnPickup: Invalid ItemID for %s"), *GetName());
		return;
	}

	// 물리 및 충돌 해제 (장착 상태이므로 필요 없음)
	if (SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetSimulatePhysics(false);
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 플레이어의 손에 무기를 장착
	PlayerCharacter->EquipItem(this);

	// 장착 상태로 설정
	UpdateItemState(EItemState::EIS_Equipped);
	UE_LOG(LogTemp, Log, TEXT("WeaponActor: %s picked up %s"), *PlayerCharacter->GetName(), *GetName());
}

void AWeaponActor::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
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
    FVector HandLocation = PlayerCharacter->GetMesh()->GetSocketLocation(FName("hand_r"));
    FVector DropLocation = HandLocation + PlayerCharacter->GetActorForwardVector() * 30.f + FVector(0.f, 0.f, -10.f);
    FRotator DropRotation = GetActorRotation();

    // 바닥 감지를 위한 LineTrace 설정
    FHitResult HitResult;
    FVector TraceStart = DropLocation;
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
        DropLocation = HandLocation + FVector(0.f, 0.f, -30.f);
        UE_LOG(LogTemp, Warning, TEXT("OnDrop: No ground found, dropping at default location %s"), *DropLocation.ToString());
    }

    // 캐릭터와의 연결 해제
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    if (UPrimitiveComponent* MeshComp = GetMeshComponent())
    {
        SetActorHiddenInGame(false);
        MeshComp->SetVisibility(true);

        MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
        MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        MeshComp->SetCollisionObjectType(ECC_PhysicsBody);

        MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        MeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
        MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
        SetActorEnableCollision(true);

        MeshComp->SetSimulatePhysics(true);
        MeshComp->SetAngularDamping(0.1f);

        UE_LOG(LogTemp, Log, TEXT("OnDrop: Dropped Item Successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("OnDrop: No valid mesh component found!"));
    }

    SetActorLocation(DropLocation);
    UpdateItemState(EItemState::EIS_World);
}

EWeaponType AWeaponActor::GetWeaponType() const
{
	return GetWeaponData().WeaponType;
}

FWeaponItemData AWeaponActor::GetWeaponData() const
{
	return ItemData.WeaponData;
}
