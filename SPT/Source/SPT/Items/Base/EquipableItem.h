// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBaseActor.h"
#include "SPT/Interfaces/EquipableInterface.h"
#include "EquipableItem.generated.h"

UCLASS()
class SPT_API AEquipableItem : public AItemBaseActor, public IEquipableInterface
{
	GENERATED_BODY()
	
public:
    AEquipableItem();

    void DetermineAttachSocket();

    /* ���� ���� ���� üũ */
    bool CanEquip(ASPTPlayerCharacter* PlayerCharacter) const;
     
    /* ������ ���� */
    virtual bool Equip(ASPTPlayerCharacter* PlayerCharacter);

    /* ������ ���� */
    virtual bool UnEquip(ASPTPlayerCharacter* PlayerCharacter);

    /* ���� �� ������ ���� �̸� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Equipable")
    FName AttachSocketName;
};
