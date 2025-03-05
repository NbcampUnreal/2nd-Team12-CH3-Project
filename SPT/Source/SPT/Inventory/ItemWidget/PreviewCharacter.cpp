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

    // ĳ���� �޽�
    PreviewCharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewCharacterMesh"));
    RootComponent = PreviewCharacterMesh;

    // �������� (ī�޶� �Ÿ� ����)
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 100.0f;
    SpringArmComp->bEnableCameraLag = false;
    SpringArmComp->bUsePawnControlRotation = false;
    SpringArmComp->bInheritPitch = false;
    SpringArmComp->bInheritYaw = false; 
    SpringArmComp->bInheritRoll = false;

    // SceneCaptureComponent2D (UI ��¿�)
    CaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComponent"));
    CaptureComp->SetupAttachment(SpringArmComp);

    // ������ ���� �޽� ������Ʈ ���� �� ������ ĳ������ �޽��� ���� (��: ������ ����)
    PreviewWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewWeaponMesh"));
    // ������ ĳ������ ���� �޽��� �����ϰų� ������ ��ġ
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

    // ���� ������ �޽� ������Ʈ ��������
    USkeletalMeshComponent* WeaponMeshComp = EquippedWeapon->GetWeaponMesh();
    if (!WeaponMeshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("PreviewCharacter: EquipWeaponPreview - WeaponMeshComponent is null"));
        return;
    }

    // ������� ���� �޽� ������Ʈ�� ���� �޽ø� �Ҵ�
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

