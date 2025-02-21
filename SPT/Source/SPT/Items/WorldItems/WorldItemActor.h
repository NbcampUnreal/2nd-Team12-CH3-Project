// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/Base/ItemBaseActor.h"
#include "SPT/Interfaces/InteractableInterface.h"
#include "WorldItemActor.generated.h"

UCLASS()
class SPT_API AWorldItemActor : public AItemBaseActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	/* �⺻ ������ */
	AWorldItemActor();

	/* ������ �ݱ� �� ������ */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter);
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void OnDrop(ASPTPlayerCharacter* PlayerCharacter);

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(ASPTPlayerCharacter* PlayerCharacter) override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Pickup")
	UDataTable* ItemDataTable;		// ������ ������ ���̺�
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Pickup")
	FName ItemID;					// ������ ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	UStaticMeshComponent* PickupMesh;	// �Ⱦ� ������ �޽�

};
