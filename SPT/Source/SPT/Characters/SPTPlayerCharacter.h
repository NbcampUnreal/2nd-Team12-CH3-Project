// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InventoryManager.h"
#include "ItemWidget/InventoryMainWidget.h"
#include "SPTPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItemBase;
class AEquipmentInventory;
class AEquipmentSlotInventory;
class AConsumableInventory;

struct FInputActionValue;

UCLASS()
class SPT_API ASPTPlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASPTPlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void StartMove(const FInputActionValue& value);
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StopMove(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void StartCrouch(const FInputActionValue& value);
	UFUNCTION()
	void StopCrouch(const FInputActionValue& value);
	UFUNCTION()
	void ItemUse(const FInputActionValue& value);
	UFUNCTION()
	void StartInteract(const FInputActionValue& value);
	UFUNCTION()
	void OnOffInventory(const FInputActionValue& value);
	UFUNCTION()
	void StartReload(const FInputActionValue& value);

private:
	// ī�޶� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComp;




public:
	// �κ��丮 ���� �Լ�
	UFUNCTION()
	void TryPickupItem();
	UFUNCTION()
	void DropItem(UInventoryItem* InventoryItem);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AInventoryManager* GetInventory() const;

	// ĳ���Ϳ� �κ��丮 �Ҵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AInventoryManager> InventoryManagerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AEquipmentInventory> EquipmentInventoryClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AConsumableInventory> ConsumableInventoryClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AEquipmentSlotInventory> EquipmentSlotInventoryClass;
	UPROPERTY()
	AInventoryManager* InventoryManager;
	UPROPERTY()
	AEquipmentInventory* EquipmentInventory;
	UPROPERTY()
	AConsumableInventory* ConsumableInventory;
	UPROPERTY()
	AEquipmentSlotInventory* EquipmentSlotInventory;

	// �κ��丮 ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryMainWidget> InventoryMainWidgetClass;
	UPROPERTY()
	UInventoryMainWidget* InventoryMainWidgetInstance;
};
