#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveRandomLocation.generated.h"

class AAISpawnVolume;

UCLASS()
class SPT_API UBTT_MoveRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_MoveRandomLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FVector GetRandomPointInSpawnVolume(AAISpawnVolume* SpawnVolume);
};
