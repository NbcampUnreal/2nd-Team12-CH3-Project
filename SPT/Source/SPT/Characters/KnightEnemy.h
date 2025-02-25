#pragma once

#include "CoreMinimal.h"
#include "SPTEnemyCharacter.h"
#include "KnightEnemy.generated.h"

UCLASS()
class SPT_API AKnightEnemy : public ASPTEnemyCharacter
{
	GENERATED_BODY()

public:
	AKnightEnemy();

protected:
	virtual void BeginPlay() override;

public:

	void Attack() override;
};
