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
/* Interaction �߰� */
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

	/* ������ ����*/
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool EquipItem(AWorldItemActor* NewItem);

	/* ������ ���� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool UnEquipItem();

	/* ������ ���? */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void DropItem();

	/* �Һ� ������ ��� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void UseConsumable();

	/* ���� ������ ������ ��ȯ */
	UFUNCTION(BlueprintCallable, Category = "Item")
	AWorldItemActor* GetEquippedItem() const;

	/* ���� �տ� ��� �ִ� ������ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Equipped")
	AWorldItemActor* EquippedItem;

	/* ������ ���� */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Equipment")
	FName EquippedItemSocket;  // �տ� �鸮�� ������

	/* �κ��丮 �ý��� */

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
	/* ��ȣ �ۿ� ���� �Լ� */
	void PerformInteractionCheck();		// �÷��̾��� �þ� �������� ����Ʈ���̽� ����
	void FoundInteractable(AActor* NewInteractable);	// ���ο� ��ȣ�ۿ� ��� ���� ��, ���ο� ���� ��Ŀ��
	void NoInteractableFound();			// �÷��̾ ��ȣ�ۿ� ������ ���͸� ã�� ������ �� ����
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
	
////////////////////////////////////////////////////////////////////////////////


private:
	// ī�޶� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComp;
};
