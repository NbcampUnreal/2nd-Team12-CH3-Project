// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InventoryManager.h"
#include "ItemWidget/InventoryMainWidget.h"
#include "SPT/ItemActor.h"		// 테스트용 아이템 액터 정식 아이템 추가시 폐기
#include "SPTPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AEquipmentInventory;
class AConsumableInventory;

struct FInputActionValue;

UCLASS()
class SPT_API ASPTPlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASPTPlayerCharacter();

	// 인벤토리 관련 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToInventory(AItemActor* Item);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();
	UFUNCTION()
	void TryPickupItem();
	UFUNCTION()
	void DropItem(UInventoryItem* InventoryItem);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AInventoryManager* GetInventory() const;

	// 캐릭터에 인벤토리 할당
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AInventoryManager> InventoryManagerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AEquipmentInventory> EquipmentInventoryClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AConsumableInventory> ConsumableInventoryClass;
	UPROPERTY()
	AInventoryManager* InventoryManager;
	UPROPERTY()
	AEquipmentInventory* EquipmentInventory;
	UPROPERTY()
	AConsumableInventory* ConsumableInventory;

	// 인벤토리 위젯 적용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryMainWidget> InventoryMainWidgetClass;
	UPROPERTY()
	UInventoryMainWidget* InventoryMainWidgetInstance;



protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void Move(const FInputActionValue& value);
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
	// 카메라 관련 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComp;
};
