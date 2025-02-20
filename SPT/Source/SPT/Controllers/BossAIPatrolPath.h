// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossAIPatrolPath.generated.h"

UCLASS()
class SPT_API ABossAIPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossAIPatrolPath();

    // ������ ��ġ�� ��������Ʈ ���
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Patrol")
    TArray<AActor*> Waypoints;

    // �ε����� ��������Ʈ�� ��� �Լ�
    AActor* GetWaypoint(int32 Index) const;

    // ��������Ʈ ���� ��ȯ
    int32 Num() const;
};
