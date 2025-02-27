// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineAISpawnPoint.generated.h"

class APatrolRoute;

UCLASS()
class SPT_API ASplineAISpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ASplineAISpawnPoint();

protected:
	virtual void BeginPlay() override;

public:	
	// AI를 스폰한다.
	void SpawnAI();

	// 스폰 시킨 AI가 사망된 후 호출되는 함수
	UFUNCTION()
	void Respawn();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<ACharacter> SpawnAIClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<APatrolRoute> PatrolRoute;
	
	// 리스폰 딜레이
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float RespawnDelay;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = true))
	USceneComponent* Scene;

	// 특정 시간 이후 리스폰시키는 타이머
	FTimerHandle RespawnTimerHandle;
};
