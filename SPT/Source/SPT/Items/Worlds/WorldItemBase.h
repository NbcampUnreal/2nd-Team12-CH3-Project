// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/Base/ItemBase.h"
#include "SPT/Interfaces/InteractableInterface.h"
#include "WorldItemBase.generated.h"

UCLASS()
class SPT_API AWorldItemBase : public AItemBase, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	/* �⺻ ������ */
	AWorldItemBase();

	virtual void BeginPlay() override;

	/* ������ �ݱ� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter);

	/* ������ ��� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void OnDrop(ASPTPlayerCharacter* PlayerCharacter);

	/* ��ȣ�ۿ� ���� �Լ�*/
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(ASPTPlayerCharacter* PlayerCharacter) override;

	/* �޽� ������Ʈ */
	void UpdateMesh();

	/* �����Ϳ��� ������ ������ ���̺� ������Ʈ */
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void OnConstruction(const FTransform& Transform);
#endif

	/* ������ ������ ���̺� */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item|Data")
	UDataTable* ItemDataTable;

	/* ������ ID */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item|Data")
	FName ItemID;

protected:

	/* ��� ��ġ ��� */
	FVector GetDropLocation(
		ASPTPlayerCharacter* PlayerCharacter,
		FVector& OutDropLocation,
		FRotator& OutDropRotation);

};
