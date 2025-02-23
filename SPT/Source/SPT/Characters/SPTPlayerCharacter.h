// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "SPT/Interfaces/InteractableInterface.h"
#include "SPT/Items/Data/WeaponDataStructs.h"
#include "SPTPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AWorldItemActor;
class AWeaponActor;
class AConsumableItemActor;
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

	/* 아이템 장착*/
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool EquipItem(AWorldItemActor* NewItem);

	/* 아이템 해제 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool UnEquipItem();

	/* 아이템 드롭? */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void DropItem();

	/* 소비 아이템 사용 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void UseConsumable();

	/* 현재 장착한 아이템 반환 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	AWorldItemActor* GetEquippedItem() const;

	/* 현재 손에 들고 있는 아이템 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Equipped")
	AWorldItemActor* EquippedItem;

	/* 장착할 소켓 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Equipment")
	FName EquippedItemSocket;  // 손에 들리는 아이템

	/* 인벤토리 시스템 */

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
	
////////////////////////////////////////////////////////////////////////////////


private:
	// 카메라 관련 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComp;
};
