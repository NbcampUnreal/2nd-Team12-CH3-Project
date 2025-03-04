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
    // ��Ʈ�ѷ� ȸ���� ���� ������ �ϵ� ȸ���ϵ��� ����
    SpringArmComp->bUsePawnControlRotation = true;
    // �ɱ� �� ī�޶� �ڿ������� �̵��ϵ��� ī�޶� �� ���� �� ���� ��
    SpringArmComp->bEnableCameraLag = true;
    SpringArmComp->CameraLagSpeed = 20.f;
    SpringArmComp->bEnableCameraRotationLag = true;
    SpringArmComp->CameraRotationLagSpeed = 20.f;
    SpringArmComp->SocketOffset = FVector(0.f, 75.f, 100.f);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

    bUseControllerRotationYaw = false;

    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);



    // �κ��丮 �߰��� ���� �߰��� �ۼ��� �ڵ�� �Դϴ�.
    // �κ��丮 Ŭ���� �� ���� Ŭ���� �ڵ� �Ҵ�
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

// ����Ʈ���̽� �Լ��� ����Ͽ� ĳ������ �տ� ��ü�� �ִ��� �Ǻ� �� �������� �� �۵�
void ASPTPlayerCharacter::TryPickupItem()
{
    // ĳ���Ͱ� �������� �ִ��� Ž���ϴ� ������ ����
    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + (ForwardVector * 200.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    // �������� Ž�����θ� Ȯ��
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, Params);

    if (bHit)
    {
        AItemBase* ItemBase = Cast<AItemBase>(HitResult.GetActor());
        if (ItemBase && InventoryManager)
        {
            // �������� ���� �� �������� �����͸� �κ��丮 �����ͷ� ����
            UInventoryItem* ItemData = NewObject<UInventoryItem>();
            ItemData->SetItemData(ItemBase->GetItemData());
            // ������ ���� ���� �� �������� �κ��丮�� �߰��ϴ� �Լ� ȣ�� �� �߰��� ������ ����
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

// �������� �����Ͽ� ĳ������ �տ� ����߸��� �Լ�
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
	
    // InventoryManagerClass�� �Ҵ�Ǿ����� Ȯ��
    if (InventoryManagerClass)
    {
        // �κ��丮 �Ŵ��� ����
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
                // Ű �Է� �߿� 1���� ȣ���
                EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartInteract);
            }

            if (PlayerController->InventoryAction)
            {
                // Ű �Է� �߿� 1���� ȣ���
                EnhancedInput->BindAction(PlayerController->InventoryAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::OnOffInventory);
            }

            if (PlayerController->ReloadAction)
            {
                // Ű �Է� �߿� 1���� ȣ���
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
    // ���콺�� X, Y �������� 2D ������ ������
    FVector2D LookInput = value.Get<FVector2D>();

    // �¿� ȸ��
    AddControllerYawInput(LookInput.X);
    // ���� ȸ��
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
    // ���� �������� �������� ����Ѵ�.
    if (value.Get<bool>())
    {

    }
}

void ASPTPlayerCharacter::StartInteract(const FInputActionValue& value)
{
    // ����Ʈ���̽��� ���� ã�Ҵ� ���Ϳ� ��ȣ�ۿ� �Ѵ�.
    // ����� �������� �ݴ� ���۸� ����
    // �ٸ� ��ȣ�ۿ� �߰� �ʿ�(����: �Ʒø�忡�� ����� �Ʒ� �޴� ���ñ�)
    if (value.Get<bool>())
    {
        TryPickupItem();
    }
}

void ASPTPlayerCharacter::OnOffInventory(const FInputActionValue& value)
{
    // �κ��丮�� �Ѱų� ����.
    if (value.Get<bool>())
    {
        if (InventoryMainWidgetInstance)
        {
            // �κ��丮�� ���� ���¸� Ȯ���Ͽ� �Ѱ� ���� ���� �۵�
            bool bIsVisible = InventoryMainWidgetInstance->IsVisible();
            InventoryMainWidgetInstance->SetVisibility(bIsVisible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
            InventoryMainWidgetInstance->FindPreviewCharacter();

            // �κ��丮�� ���¿� ���� �Է¸�带 ��ȯ
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
/// ����, ������, �߻� �߰� ����
/// 
/// </summary>
/// <param name="value"></param>

void ASPTPlayerCharacter::StartReload(const FInputActionValue& value)
{
    // ���� �������� �������� ���̰ų� ������ �� �� �ִ� �������̶�� ������
    if (value.Get<bool>())
    {
        if (EquippedWeapon)
        {
            AFirearmWeapon* FirearmWeapon = Cast<AFirearmWeapon>(EquippedWeapon);
            FirearmWeapon->BeginReload();  //// �߰�
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
            FirearmWeapon->SwitchAiming();  //// �߰�
        }
    }
}

void ASPTPlayerCharacter::StartAttack(const FInputActionValue& value)
{
    if (EquippedWeapon)
    {
        EquippedWeapon->PrimaryAction(this);  //// �߰�
    }
}

void ASPTPlayerCharacter::StopAttack(const FInputActionValue& value)
{
    if (EquippedWeapon)
    {
        AFirearmWeapon* FirearmWeapon = Cast<AFirearmWeapon>(EquippedWeapon);
        FirearmWeapon->EndFire();  //// �߰�
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
        // FirearmWeapon->GetMagazinCapacity();    // ��ü ź�� ����
        // FirearmWeapon->GetCurrentAmmo();        // ���� ź�� ����
        return FirearmWeapon->GetFirearmType();        // �ѱ� ����
    }

    return EFirearmType::EFT_MAX;
}
