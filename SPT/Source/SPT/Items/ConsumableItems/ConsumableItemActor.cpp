// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItemActor.h"

AConsumableItemActor::AConsumableItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AConsumableItemActor::Use(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("ConsumableItemActor: Cannot Use - Character is null!"));
        return;
    }

    // �տ� ��� �ִٰ� ����ϸ� ����Ǵ� ����
    UE_LOG(LogTemp, Log, TEXT("Using item: %s"), *GetName());

    // TODO: ü�� ȸ��, ���� ���� ���� ���� �߰�

    // ��� �� ������ ����
    Destroy();
}
