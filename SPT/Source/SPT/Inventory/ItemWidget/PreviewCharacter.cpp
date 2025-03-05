// Fill out your copyright notice in the Description page of Project Settings.


#include "PreviewCharacter.h"
#include "SPT/Items/Weapons/WeaponBase.h"
#include "SPT/Items/Weapons/FirearmWeapon.h"
#include "SPT/Items/Data/ItemDataObject.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
APreviewCharacter::APreviewCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

    // 캐릭터 메쉬
    PreviewCharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewCharacterMesh"));
    RootComponent = PreviewCharacterMesh;

    // 스프링암 (카메라 거리 조정)
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 100.0f;
    SpringArmComp->bEnableCameraLag = false;
    SpringArmComp->bUsePawnControlRotation = false;
    SpringArmComp->bInheritPitch = false;
    SpringArmComp->bInheritYaw = false; 
    SpringArmComp->bInheritRoll = false;

    // SceneCaptureComponent2D (UI 출력용)
    CaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComponent"));
    CaptureComp->SetupAttachment(SpringArmComp);

    // 프리뷰 무기 메시 컴포넌트 생성 및 프리뷰 캐릭터의 메쉬에 부착 (예: 오른손 소켓)
    PreviewWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewWeaponMesh"));
    // 프리뷰 캐릭터의 메인 메쉬에 부착하거나 별도로 배치
    PreviewWeaponMesh->SetupAttachment(PreviewCharacterMesh, TEXT("AR_RightHand"));

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/Resources/Characters/Player/Meshes/SK_Bandit"));
    if (SkeletalMeshAsset.Succeeded())
    {
        PreviewCharacterMesh->SetSkeletalMesh(SkeletalMeshAsset.Object);
    }


}

void APreviewCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APreviewCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APreviewCharacter::RotatePreview(float DeltaYaw)
{
    FRotator NewRotation = PreviewCharacterMesh->GetComponentRotation();
    NewRotation.Yaw -= DeltaYaw;
    PreviewCharacterMesh->SetWorldRotation(NewRotation);
}

void APreviewCharacter::EquipWeapon(AFirearmWeapon* EquippedWeapon)
{
    if (!EquippedWeapon)
    {
        UE_LOG(LogTemp, Warning, TEXT("PreviewCharacter: EquipWeaponPreview - EquippedWeapon is null"));
        return;
    }

    // 무기 액터의 메시 컴포넌트 가져오기
    USkeletalMeshComponent* WeaponMeshComp = EquippedWeapon->GetWeaponMesh();
    if (!WeaponMeshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("PreviewCharacter: EquipWeaponPreview - WeaponMeshComponent is null"));
        return;
    }

    // 프리뷰용 무기 메시 컴포넌트에 무기 메시를 할당
    USkeletalMesh* NewMesh = WeaponMeshComp->SkeletalMesh;
    if (NewMesh)
    {
        PreviewWeaponMesh->SetSkeletalMesh(NewMesh);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PreviewCharacter: EquipWeaponPreview - No SkeletalMesh found on WeaponMeshComponent"));
    }

    PreviewWeaponMesh->AttachToComponent(PreviewCharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("AR_RightHand"));
}

