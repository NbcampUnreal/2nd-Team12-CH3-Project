// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "SPT/Interfaces/InteractableInterface.h" //// 추가
#include "SPTPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

///////////////////////////////////////////////////////////////////////

class AWorldItemBase;
class AWeaponBase;
class AConsumableItem;

/* 상호 작용 */
USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY();

	FInteractionData() :
		CurrentInteractable(nullptr),
		LastInteractionCheckTime(0.0f)
	{
	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

///////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////

    /* 장착 및 해제 파트는 현재 무기에 맞춰져 있습니다.
     * 이후 재정비가 필요한 파트입니다.
     */
	/* 무기 장착 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool EquipWeapon(AWeaponBase* NewItem);

	/* 무기 해제 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool UnEquipWeapon();

	/* 무기 드롭 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DropWeapon();

	/* 현재 장착한 무기 반환 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetEquippedWeapon() const;

	/* 현재 장착한 무기 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Equipped")
	AWeaponBase* EquippedWeapon;

	/* 장착할 소켓 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Equipment")
	FName EquippedWeaponSocket;



	/* 상호 작용 관련 함수 */
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
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


///////////////////////////////////////////////////////////////////////

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
