// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "SPT/Interfaces/InteractableInterface.h" //// �߰�
#include "SPTPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

///////////////////////////////////////////////////////////////////////

class AWorldItemBase;
class AWeaponBase;
class AConsumableItem;

/* ��ȣ �ۿ� */
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

    /* ���� �� ���� ��Ʈ�� ���� ���⿡ ������ �ֽ��ϴ�.
     * ���� ������ �ʿ��� ��Ʈ�Դϴ�.
     */
	/* ���� ���� */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool EquipWeapon(AWeaponBase* NewItem);

	/* ���� ���� */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool UnEquipWeapon();

	/* ���� ��� */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DropWeapon();

	/* ���� ������ ���� ��ȯ */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AWeaponBase* GetEquippedWeapon() const;

	/* ���� ������ ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Equipped")
	AWeaponBase* EquippedWeapon;

	/* ������ ���� */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Equipment")
	FName EquippedWeaponSocket;



	/* ��ȣ �ۿ� ���� �Լ� */
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

	FORCEINLINE bool IsInteracting() const;

	/* ��ȣ �ۿ� ���� ���� */
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
	// ī�޶� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComp;
};
