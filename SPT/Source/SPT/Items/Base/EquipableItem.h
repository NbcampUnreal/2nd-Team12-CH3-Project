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

    /* 장착 가능 여부 체크 */
    bool CanEquip(ASPTPlayerCharacter* PlayerCharacter) const;
     
    /* 아이템 장착 */
    virtual bool Equip(ASPTPlayerCharacter* PlayerCharacter);

    /* 아이템 해제 */
    virtual bool UnEquip(ASPTPlayerCharacter* PlayerCharacter);

    /* 장착 시 연결할 소켓 이름 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Equipable")
    FName AttachSocketName;
};
