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

    /* ���� ���� ���� üũ */
    bool CanEquip(ASPTPlayerCharacter* PlayerCharacter) const;
     
    /* ������ ���� */
    virtual bool Equip(ASPTPlayerCharacter* PlayerCharacter);

    /* ������ ���� */
    virtual bool Unequip(ASPTPlayerCharacter* PlayerCharacter);

protected:
    /* ������ SkeletalMesh */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipable")
    USkeletalMeshComponent* ItemMesh;

    /* ���� �� ������ ���� �̸� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipable")
    FName AttachSocketName;
};
