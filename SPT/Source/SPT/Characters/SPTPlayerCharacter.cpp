// Fill out your copyright notice in the Description page of Project Settings.


#include "SPTPlayerCharacter.h"
#include "EquipmentInventory.h"
#include "ConsumableInventory.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "SPTPlayerController.h"

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

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void ASPTPlayerCharacter::AddItemToInventory(AItemActor* Item)
{
    // �������� ��ȿ���� Ȯ��
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("AddItemToInventory: Item is invalid!"));
        return;
    }

    // �������� ��� ���������� �Ҹ�ǰ ���������� Ȯ��
    UEquipmentItem* EquipmentItem = Cast<UEquipmentItem>(Item->GetItemData());
    if (EquipmentItem)
    {
        // ��� �������̶�� EquipmentInventory�� �߰�
        if (EquipmentInventory)
        {
            UE_LOG(LogTemp, Warning, TEXT("Added EquipmentItem to Equipment Inventory: %s"), *Item->GetItemName());
            EquipmentInventory->AddItem(EquipmentItem);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Equipment Inventory is null!"));
        }
        return;
    }

    UConsumableItem* ConsumableItem = Cast<UConsumableItem>(Item->GetItemData());
    if (ConsumableItem)
    {
        // �Ҹ�ǰ �������̶�� ConsumableInventory�� �߰�
        if (ConsumableInventory)
        {
            ConsumableInventory->AddItem(ConsumableItem);
            UE_LOG(LogTemp, Log, TEXT("Added ConsumableItem to Consumable Inventory: %s"), *Item->GetItemName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Consumable Inventory is null!"));
        }
        return;
    }

    // �������� ��� �Ҹ�ǰ�� �ƴ϶�� ���
    UE_LOG(LogTemp, Warning, TEXT("AddItemToInventory: Unknown item type for item: %s"), *Item->GetName());
}

void ASPTPlayerCharacter::ToggleInventory()
{
}

void ASPTPlayerCharacter::TryPickupItem()
{
    UE_LOG(LogTemp, Warning, TEXT("PickUp Start"));
    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + (ForwardVector * 200.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, Params);


    if (bHit) {
        AItemActor* Item = Cast<AItemActor>(HitResult.GetActor());
        if (Item) {
            UE_LOG(LogTemp, Warning, TEXT("PickUp This"));
            AddItemToInventory(Item);
            Item->SetActorHiddenInGame(true);
            Item->SetActorEnableCollision(false);
        }

    }
}

void ASPTPlayerCharacter::DropItem(AInventoryManager* Inventory, AItemActor* Item)
{
}

AInventoryManager* ASPTPlayerCharacter::GetInventory() const
{
    return nullptr;
}

void ASPTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    // InventoryManagerClass�� �Ҵ�Ǿ����� Ȯ��
    if (InventoryManagerClass)
    {
        // �κ��丮 �Ŵ��� ����
        InventoryManager = GetWorld()->SpawnActor<AInventoryManager>(InventoryManagerClass);
        if (InventoryManager)
        {
            if (EquipmentInventoryClass)
            {
                EquipmentInventory = GetWorld()->SpawnActor<AEquipmentInventory>(EquipmentInventoryClass);
                InventoryManager->RegisterInventory(EquipmentInventory);
            }

            if (ConsumableInventoryClass)
            {
                ConsumableInventory = GetWorld()->SpawnActor<AConsumableInventory>(ConsumableInventoryClass);
                InventoryManager->RegisterInventory(ConsumableInventory);
            }
        }
    }

    // ���� �̱���(�κ��丮 ����)
    //if (InventoryMainWidgetClass)
    //{
    //    InventoryMainWidgetInstance = CreateWidget<UInventoryMainWidget>(GetWorld(), InventoryMainWidgetClass);
    //    if (InventoryMainWidgetInstance)
    //    {
    //        InventoryMainWidgetInstance->AddToViewport();
    //    }

    //    if (InventoryManager)
    //    {
    //        InventoryManager->PlayerCharacter = this;
    //    }
    //}

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
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::Move);
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
        }
    }
}

void ASPTPlayerCharacter::Move(const FInputActionValue& value)
{
    if (!Controller) return;

    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
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
    if (value.Get<bool>())
    {
        Crouch();
    }
}

void ASPTPlayerCharacter::StopCrouch(const FInputActionValue& value)
{
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
    if (value.Get<bool>())
    {
        UE_LOG(LogTemp, Warning, TEXT("Interact Complete"));
        TryPickupItem();
    }
}

void ASPTPlayerCharacter::OnOffInventory(const FInputActionValue& value)
{
    // �κ��丮�� �Ѱų� ����.
    if (value.Get<bool>())
    {

    }
}

void ASPTPlayerCharacter::StartReload(const FInputActionValue& value)
{
    // ���� �������� �������� ���̰ų� ������ �� �� �ִ� �������̶�� ������
    if (value.Get<bool>())
    {
        
    }
}

