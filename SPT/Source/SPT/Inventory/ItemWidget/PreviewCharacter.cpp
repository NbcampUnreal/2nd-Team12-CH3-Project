// Fill out your copyright notice in the Description page of Project Settings.


#include "PreviewCharacter.h"
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

