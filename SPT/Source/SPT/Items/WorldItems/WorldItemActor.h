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
	/* 기본 생성자 */
	AWorldItemActor();

	/* 아이템 줍기 및 버리기 */
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
	UDataTable* ItemDataTable;		// 아이템 데이터 테이블
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Pickup")
	FName ItemID;					// 아이템 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	UStaticMeshComponent* PickupMesh;	// 픽업 아이템 메쉬

};
