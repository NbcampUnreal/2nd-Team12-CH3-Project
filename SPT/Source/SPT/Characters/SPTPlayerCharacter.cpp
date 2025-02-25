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
    // 컨트롤러 회전에 따라 스프링 암도 회전하도록 설정
    SpringArmComp->bUsePawnControlRotation = true;
    // 앉기 시 카메라가 자연스럽게 이동하도록 카메라 렉 설정 및 설정 값
    SpringArmComp->bEnableCameraLag = true;
    SpringArmComp->CameraLagSpeed = 20.f;
    SpringArmComp->bEnableCameraRotationLag = true;
    SpringArmComp->CameraRotationLagSpeed = 20.f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void ASPTPlayerCharacter::AddItemToInventory(AItemActor* Item)
{
    // 아이템이 유효한지 확인
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("AddItemToInventory: Item is invalid!"));
        return;
    }

    // 아이템이 장비 아이템인지 소모품 아이템인지 확인
    UEquipmentItem* EquipmentItem = Cast<UEquipmentItem>(Item->GetItemData());
    if (EquipmentItem)
    {
        // 장비 아이템이라면 EquipmentInventory에 추가
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
        // 소모품 아이템이라면 ConsumableInventory에 추가
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

    // 아이템이 장비나 소모품이 아니라면 경고
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
	
    // InventoryManagerClass가 할당되었는지 확인
    if (InventoryManagerClass)
    {
        // 인벤토리 매니저 생성
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

    // 아직 미구현(인벤토리 위젯)
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
    // 현재 장착중인 아이템을 사용한다.
    if (value.Get<bool>())
    {
        
    }
}

void ASPTPlayerCharacter::StartInteract(const FInputActionValue& value)
{
    // 라인트레이스를 통해 찾았던 액터와 상호작용 한다.
    if (value.Get<bool>())
    {
        UE_LOG(LogTemp, Warning, TEXT("Interact Complete"));
        TryPickupItem();
    }
}

void ASPTPlayerCharacter::OnOffInventory(const FInputActionValue& value)
{
    // 인벤토리를 켜거나 끈다.
    if (value.Get<bool>())
    {

    }
}

void ASPTPlayerCharacter::StartReload(const FInputActionValue& value)
{
    // 현재 착용중인 아이템이 총이거나 재장전 할 수 있는 아이템이라면 재장전
    if (value.Get<bool>())
    {
        
    }
}

