// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "SPT/Interfaces/InteractableInterface.h"
#include "SPTPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AEquipableItem;

struct FInputActionValue;


////////////////////////////////////////////////////////////////////////////////
/* Interaction 추가 */
USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY();

	FInteractionData() :
		CurrentInteractable(nullptr),
		LastIneractionCheckTime(0.0f)
	{
	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastIneractionCheckTime;
};
////////////////////////////////////////////////////////////////////////////////


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

	/** 아이템 장착 함수 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool EquipItem(AEquipableItem* NewItem);

	/** 아이템 장착 함수 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool UnEquipItem();

	/** 현재 장착 중인 무기를 반환 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	AEquipableItem* GetEquippedItem() const;

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

////////////////////////////////////////////////////////////////////////////////
	/* 상호 작용 관련 함수 */
	void PerformInteractionCheck();		// 플레이어의 시야 방향으로 라인트레이스 수행
	void FoundInteractable(AActor* NewInteractable);	// 새로운 상호작용 대상 감지 시, 새로운 액터 포커스
	void NoInteractableFound();			// 플레이어가 상호작용 가능한 액터를 찾지 못했을 때 실행
	void BeginInteract();
	void EndInteract();
	void Interact();					

	FORCEINLINE bool IsInteracting() const;

	/* 상호 작용 관련 변수 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Interaction")
	TScriptInterface<IInteractableInterface> TargetInteractable;

	float InteractionCheckFrequency;
	float InteractionCheckDistance;
	FTimerHandle TimerHandle_Interaction;
	FInteractionData InteractionData;

	/* 현재 장착 중인 아이템 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Weapon")
	AEquipableItem* EquippedItem;

	/* 장착할 소켓 이름 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Weapon")
	FName ItemAttachSocket = "HandSocket"; // 손에 장착할 소켓 이름

	/* 인벤토리 관련 함수
	void OpenInventory();
	void CloseInventory();
	void PickupItem();
	void DropItem(UItemObject* ItemToDrop);

	/* 인벤토리 시스템 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Inventory")
	UInventoryComponent* InventoryComponent;
	*/
////////////////////////////////////////////////////////////////////////////////


private:
	// 카메라 관련 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComp;
};
