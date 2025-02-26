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
	// AI�� �����Ѵ�.
	void SpawnAI();

	// ���� ��Ų AI�� ����� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void Respawn();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<ACharacter> SpawnAIClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<APatrolRoute> PatrolRoute;
	
	// ������ ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float RespawnDelay;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = true))
	USceneComponent* Scene;

	// Ư�� �ð� ���� ��������Ű�� Ÿ�̸�
	FTimerHandle RespawnTimerHandle;
};
