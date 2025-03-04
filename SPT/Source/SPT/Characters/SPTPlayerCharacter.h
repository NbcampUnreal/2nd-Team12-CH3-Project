// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InventoryManager.h"
#include "ItemWidget/InventoryMainWidget.h"
#include "SPTPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

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
	UFUNCTION()
	void SwitchAiming(const FInputActionValue& value);
	UFUNCTION()
	void StartAttack(const FInputActionValue& value);
	UFUNCTION()
	void StopAttack(const FInputActionValue& value);
	UFUNCTION()
	void SwitchAutoFire(const FInputActionValue& value);

private:
	// 카메라 관련 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComp;

public:
	// 인벤토리 관련 함수
	UFUNCTION()
	void TryPickupItem();
	UFUNCTION()
	bool EquipWeapon(AWeaponBase* NewItem);		//// 추가
	UFUNCTION()
	bool UnEquipWeapon();	//// 추가
	UFUNCTION()
	void DropItem(UInventoryItem* InventoryItem);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AInventoryManager* GetInventory() const;

	// 캐릭터에 인벤토리 할당
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AInventoryManager> InventoryManagerClass;
	UPROPERTY()
	AInventoryManager* InventoryManager;

	// 장착한 무기 종류 반환
	UFUNCTION(BlueprintCallable, Category = "Inventory|Equipped")
	AWeaponBase* GetEquippedWeapon() const;
	UFUNCTION(BlueprintCallable, Category = "Inventory|Equipped")
	enum EFirearmType GetEquippedFirearmType() const;

	// 인벤토리 위젯 적용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UInventoryMainWidget> InventoryMainWidgetClass;
	UPROPERTY()
	UInventoryMainWidget* InventoryMainWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	APreviewCharacter* PreviewCharacter;


private:
	/* 현재 장착한 무기 */
	AWeaponBase* EquippedWeapon;
};
