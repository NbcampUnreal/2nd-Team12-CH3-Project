// Fill out your copyright notice in the Description page of Project Settings.


#include "SPTPlayerCharacter.h"
#include "InventoryManager.h"
#include "SPT/Inventory/ItemData/InventoryItem.h"
#include "SPT/Items/Base/Itembase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "SPTPlayerController.h"
#include "SPT/Items/Weapons/WeaponBase.h"
#include "SPT/Items/Worlds/WorldWeapon.h"
#include "SPT/Items/Weapons/FirearmWeapon.h"

ASPTPlayerCharacter::ASPTPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
    // 컨트롤러 회전에 따라 스프링 암도 회전하도록 설정
    SpringArmComp->bUsePawnControlRotation = true;
    // 앉기 시 카메라가 자연스럽게 이동하도록 카메라 렉 설정 및 설정 값
    SpringArmComp->bEnableCameraLag = true;
    SpringArmComp->CameraLagSpeed = 20.f;
    SpringArmComp->bEnableCameraRotationLag = true;
    SpringArmComp->CameraRotationLagSpeed = 20.f;
    SpringArmComp->SocketOffset = FVector(0.f, 75.f, 100.f);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

    bUseControllerRotationYaw = false;

    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);



    // 인벤토리 추가를 위해 추가로 작성된 코드들 입니다.
    // 인벤토리 클래스 및 위젯 클래스 자동 할당
    static ConstructorHelpers::FClassFinder<AInventoryManager> BP_InventoryManager(TEXT("/Game/Blueprints/Inventory/BP_InventoryManager"));
    static ConstructorHelpers::FClassFinder<UInventoryMainWidget> WBP_InventoryMainWidget(TEXT("/Game/Blueprints/Inventory/UI/WBP_InventoryMainWidget"));
    
    if (BP_InventoryManager.Succeeded())
    {
        InventoryManagerClass = BP_InventoryManager.Class;
    }
    if (WBP_InventoryMainWidget.Succeeded())
    {
        InventoryMainWidgetClass = WBP_InventoryMainWidget.Class;
    }

}

// 라인트레이스 함수를 사용하여 캐릭터의 앞에 물체가 있는지 판별 후 아이템일 시 작동
void ASPTPlayerCharacter::TryPickupItem()
{
    // 캐릭터가 아이템이 있는지 탐색하는 범위를 설정
    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + (ForwardVector * 200.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    // 아이템의 탐색여부를 확인
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, Params);

    if (bHit)
    {
        AItemBase* ItemBase = Cast<AItemBase>(HitResult.GetActor());
        if (ItemBase && InventoryManager)
        {
            // 아이템이 있을 시 아이템의 데이터를 인벤토리 데이터로 복사
            UInventoryItem* ItemData = NewObject<UInventoryItem>();
            ItemData->SetItemData(ItemBase->GetItemData());
            // 아이템 복사 성공 시 아이템을 인벤토리로 추가하는 함수 호출 후 추가된 아이템 제거
            if (ItemData)
            {
                if (ItemData->IsWeapon())
                {
                    AWorldWeapon* WorldWeapon = Cast<AWorldWeapon>(ItemBase);
                    if (WorldWeapon && ItemBase->GetItemData()->WeaponData.WeaponType == EWeaponType::EWT_Firearm)
                    {
                        WorldWeapon->OnPickup(this);
                    }
                }
                else
                {
                    InventoryManager->AddItemToInventory(ItemData);
                }

                ItemBase->Destroy();
            }
        }
    }
}

bool ASPTPlayerCharacter::EquipWeapon(AWeaponBase* NewItem)
{
    if (!NewItem) return false;
    if (EquippedWeapon)
    {
        UnEquipWeapon();
    }

    EquippedWeapon = NewItem;

    if (!EquippedWeapon)
    {
        return false;
    }

    return true;
}

bool ASPTPlayerCharacter::UnEquipWeapon()
{
    if (EquippedWeapon) {
        EquippedWeapon->UnEquip(this);
        EquippedWeapon = nullptr;
    }

    return false;
}

// 아이템을 생성하여 캐릭터의 앞에 떨어뜨리는 함수
void ASPTPlayerCharacter::DropItem(UInventoryItem* InventoryItem)
{
    if (InventoryManager)
    {
        FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 200.0f;
        InventoryManager->DropItem(InventoryItem, DropLocation);
    }
}

AInventoryManager* ASPTPlayerCharacter::GetInventory() const
{
    return InventoryManager;
}

void ASPTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    // InventoryManagerClass가 할당되었는지 확인
    if (InventoryManagerClass)
    {
        // 인벤토리 매니저 생성
        InventoryManager = GetWorld()->SpawnActor<AInventoryManager>(InventoryManagerClass);
    }

    if (InventoryMainWidgetClass)
    {
        InventoryMainWidgetInstance = CreateWidget<UInventoryMainWidget>(GetWorld(), InventoryMainWidgetClass);
        if (InventoryMainWidgetInstance)
        {
            InventoryMainWidgetInstance->AddToViewport();
            InventoryMainWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (InventoryManager)
    {
        InventoryManager->SetInventoryWidget(InventoryMainWidgetInstance);
    }

    SetHealth(100.f);
    MaxHealth = Health;
}

void ASPTPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (ASPTPlayerController* PlayerController = Cast<ASPTPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Started, this, &ASPTPlayerCharacter::StartMove);
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::Move);
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Completed, this, &ASPTPlayerCharacter::StopMove);
            }

            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartJump);

                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ASPTPlayerCharacter::StopJump);
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::Look);
            }

            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartSprint);
                EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &ASPTPlayerCharacter::StopSprint);
            }

            if (PlayerController->CrouchAction)
            {
                EnhancedInput->BindAction(PlayerController->CrouchAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartCrouch);
                EnhancedInput->BindAction(PlayerController->CrouchAction, ETriggerEvent::Completed, this, &ASPTPlayerCharacter::StopCrouch);
            }

            if (PlayerController->ItemUseAction)
            {
                EnhancedInput->BindAction(PlayerController->ItemUseAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::ItemUse);
            }

            if (PlayerController->InteractAction)
            {
                // 키 입력 중에 1번만 호출됨
                EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartInteract);
            }

            if (PlayerController->InventoryAction)
            {
                // 키 입력 중에 1번만 호출됨
                EnhancedInput->BindAction(PlayerController->InventoryAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::OnOffInventory);
            }

            if (PlayerController->ReloadAction)
            {
                // 키 입력 중에 1번만 호출됨
                EnhancedInput->BindAction(PlayerController->ReloadAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartReload);
            }

            if (PlayerController->AimingAction)
            {
                EnhancedInput->BindAction(PlayerController->AimingAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::SwitchAiming);
            }

            if (PlayerController->AttackAction)
            {
                EnhancedInput->BindAction(PlayerController->AttackAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartAttack);
                EnhancedInput->BindAction(PlayerController->AttackAction, ETriggerEvent::Completed, this, &ASPTPlayerCharacter::StopAttack);
            }
        }
    }
}

void ASPTPlayerCharacter::StartMove(const FInputActionValue& value)
{
    if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
    {
        CharacterMovementComp->bUseControllerDesiredRotation = true;
    }
}

void ASPTPlayerCharacter::Move(const FInputActionValue& value)
{
    if (Controller)
    {
        const FVector2D MoveInput = value.Get<FVector2D>();
        const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

        if (MoveInput.X != 0.0f)
        {
            const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
            AddMovementInput(MovementDirection, MoveInput.X);
        }

        if (MoveInput.Y != 0.0f)
        {
            const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
            AddMovementInput(MovementDirection, MoveInput.Y);
        }
    }
}

void ASPTPlayerCharacter::StopMove(const FInputActionValue& value)
{
    if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
    {
        CharacterMovementComp->bUseControllerDesiredRotation = false;
    }
}

void ASPTPlayerCharacter::Look(const FInputActionValue& value)
{
    // 마우스의 X, Y 움직임을 2D 축으로 가져옴
    FVector2D LookInput = value.Get<FVector2D>();

    // 좌우 회전
    AddControllerYawInput(LookInput.X);
    // 상하 회전
    AddControllerPitchInput(LookInput.Y);
}

void ASPTPlayerCharacter::StartJump(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        Jump();
    }
}

void ASPTPlayerCharacter::StopJump(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void ASPTPlayerCharacter::StartSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void ASPTPlayerCharacter::StopSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void ASPTPlayerCharacter::StartCrouch(const FInputActionValue& value)
{
    if (GetCharacterMovement()->IsFalling()) return;

    if (value.Get<bool>())
    {
        Crouch();
    }
}

void ASPTPlayerCharacter::StopCrouch(const FInputActionValue& value)
{
    if (GetCharacterMovement()->IsFalling()) return;

    if (!value.Get<bool>())
    {
        UnCrouch();
    }
}

void ASPTPlayerCharacter::ItemUse(const FInputActionValue& value)
{
    // 현재 장착중인 아이템을 사용한다.
    if (value.Get<bool>())
    {

    }
}

void ASPTPlayerCharacter::StartInteract(const FInputActionValue& value)
{
    // 라인트레이스를 통해 찾았던 액터와 상호작용 한다.
    // 현재는 아이템을 줍는 동작만 존재
    // 다른 상호작용 추가 필요(예시: 훈련모드에서 사용할 훈련 메뉴 선택기)
    if (value.Get<bool>())
    {
        TryPickupItem();
    }
}

void ASPTPlayerCharacter::OnOffInventory(const FInputActionValue& value)
{
    // 인벤토리를 켜거나 끈다.
    if (value.Get<bool>())
    {
        if (InventoryMainWidgetInstance)
        {
            // 인벤토리의 현재 상태를 확인하여 켜고 끄는 동작 작동
            bool bIsVisible = InventoryMainWidgetInstance->IsVisible();
            InventoryMainWidgetInstance->SetVisibility(bIsVisible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
            InventoryMainWidgetInstance->FindPreviewCharacter();

            // 인벤토리의 상태에 따라 입력모드를 전환
            if (!bIsVisible)
            {
                APlayerController* PlayerController = Cast<APlayerController>(GetController());
                if (PlayerController)
                {
                    FInputModeGameAndUI InputMode;
                    InputMode.SetWidgetToFocus(InventoryMainWidgetInstance->TakeWidget());
                    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

                    PlayerController->SetInputMode(InputMode);
                    PlayerController->bShowMouseCursor = true;
                    /*PlayerController->SetPause(true);*/
                }
            }
            else
            {
                APlayerController* PlayerController = Cast<APlayerController>(GetController());
                if (PlayerController)
                {
                    PlayerController->SetInputMode(FInputModeGameOnly());
                    PlayerController->bShowMouseCursor = false;
                    /*PlayerController->SetPause(false);*/
                }
            }
        }
    }
}

/// <summary>
/// 조준, 재장전, 발사 추가 구현
/// 
/// </summary>
/// <param name="value"></param>

void ASPTPlayerCharacter::StartReload(const FInputActionValue& value)
{
    // 현재 착용중인 아이템이 총이거나 재장전 할 수 있는 아이템이라면 재장전
    if (value.Get<bool>())
    {
        if (EquippedWeapon)
        {
            AFirearmWeapon* FirearmWeapon = Cast<AFirearmWeapon>(EquippedWeapon);
            FirearmWeapon->BeginReload();  //// 추가
        }
    }
}

void ASPTPlayerCharacter::SwitchAiming(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        if (EquippedWeapon)
        {
            AFirearmWeapon* FirearmWeapon = Cast<AFirearmWeapon>(EquippedWeapon);
            FirearmWeapon->SwitchAiming();  //// 추가
        }
    }
}

void ASPTPlayerCharacter::StartAttack(const FInputActionValue& value)
{
    if (EquippedWeapon)
    {
        EquippedWeapon->PrimaryAction(this);  //// 추가
    }
}

void ASPTPlayerCharacter::StopAttack(const FInputActionValue& value)
{
    if (EquippedWeapon)
    {
        AFirearmWeapon* FirearmWeapon = Cast<AFirearmWeapon>(EquippedWeapon);
        FirearmWeapon->EndFire();  //// 추가
    }
}

AWeaponBase* ASPTPlayerCharacter::GetEquippedWeapon() const
{
    if (!EquippedWeapon)
    {
        return nullptr;
    }
    
    return EquippedWeapon;
}

EFirearmType ASPTPlayerCharacter::GetEquippedFirearmType() const
{
    if (AFirearmWeapon* FirearmWeapon = Cast<AFirearmWeapon>(EquippedWeapon))
    {
        // FirearmWeapon->GetMagazinCapacity();    // 전체 탄약 개수
        // FirearmWeapon->GetCurrentAmmo();        // 현재 탄약 개수
        return FirearmWeapon->GetFirearmType();        // 총기 종류
    }

    return EFirearmType::EFT_MAX;
}
