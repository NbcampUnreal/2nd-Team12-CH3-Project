// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIPatrolPath.h"

ABossAIPatrolPath::ABossAIPatrolPath()
{
    PrimaryActorTick.bCanEverTick = false;  // �� ������ ���� ���ʿ�
}

AActor* ABossAIPatrolPath::GetWaypoint(int32 Index) const
{
    if (Waypoints.IsValidIndex(Index))
    {
        return Waypoints[Index];
    }
    return nullptr;
}

int32 ABossAIPatrolPath::Num() const
{
    return Waypoints.Num();
}