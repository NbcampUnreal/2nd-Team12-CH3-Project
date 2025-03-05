#include "SPTEnemyController.h"
#include "SPTEnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"             // UAISense의 기본 기능 사용
#include "Perception/AISense_Sight.h"       // UAISense_Sight::StaticClass() 사용 가능
#include "Perception/AISense_Hearing.h"     // UAISense_Hearing::StaticClass() 사용 가능
#include "Perception/AISense_Damage.h"      // UAISense_Damage::StaticClass() 사용 가능
#include "UObject/EnumProperty.h"           // 블루프린트 Enum을 가져오기 위한 헤더
#include "Kismet/GameplayStatics.h"

ASPTEnemyController::ASPTEnemyController()
{
    // 블랙보드 생성
    BBC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

    // BehaviorTree 및 BlackboardData를 에디터에서 설정한 기본값으로 설정
    static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("/Game/Blueprints/AI/BB_EnemyAI.BB_EnemyAI"));

    if (BB.Succeeded()) BlackboardAsset = BB.Object;

    // AI Perception 감지 이벤트 연결  
    if (AIPerceptionComp)
    {
        AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &ASPTEnemyController::OnPerceptionUpdated);
    }
}

void ASPTEnemyController::BeginPlay()
{
    Super::BeginPlay();
}

void ASPTEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (!InPawn) return;

    if (AIPerceptionComp && !AIPerceptionComp->OnPerceptionUpdated.IsBound())
    {
        AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &ASPTEnemyController::OnPerceptionUpdated);
    }

    // AI 컨트롤러 블루프린트 클래스명을 기반으로 BehaviorTree 설정
    FString ControllerClassName = GetClass()->GetName();
    bool bNeedsPerception = false;

    if (ControllerClassName.Contains(TEXT("BP_SPTMoveAlongPatrolEnemyController")))
    {
        BehaviorTreeAsset = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/Blueprints/AI/BT_MoveAlongPatrolAI.BT_MoveAlongPatrolAI"));
    }
    else if (ControllerClassName.Contains(TEXT("BP_SPTRandomMoveEnemyController")))
    {
        BehaviorTreeAsset = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/Blueprints/AI/BT_RandomMoveAI.BT_RandomMoveAI"));
    }
    else
    {
        BehaviorTreeAsset = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/Blueprints/AI/BT_EnemyAI.BT_EnemyAI"));

        // BT_EnemyAI만 AIPerceptionComponent 필요
        bNeedsPerception = true;
    }

    // BT_EnemyAI를 사용하는 경우에만 AIPerceptionComponent 설정
    if (bNeedsPerception)
    {
        if (!AIPerceptionComp)
        {
            AIPerceptionComp = FindComponentByClass<UAIPerceptionComponent>();
            if (!AIPerceptionComp)
            {
                UE_LOG(LogTemp, Error, TEXT("[ERROR] OnPossess(): AIPerceptionComponent not found! AI perception disabled"));
                return;
            }
            else
            {
                AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &ASPTEnemyController::OnPerceptionUpdated);
                UE_LOG(LogTemp, Warning, TEXT("[DEBUG] OnPossess(): AIPerceptionComponent initialized successfully"));
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[INFO] AIPerceptionComponent not required, skipping setup"));
    }

    // BehaviorTree 실행
    if (BehaviorTreeAsset && BBC)
    {
        UseBlackboard(BlackboardAsset, BBC);
        RunBehaviorTree(BehaviorTreeAsset);
        UE_LOG(LogTemp, Warning, TEXT("Behavior Tree Started Successfully!: %s"), *BehaviorTreeAsset->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[ERROR] BehaviorTree execution failed! BehaviorTreeAsset is NULL"));
    }

    SetStateAsPassive();

    GetWorldTimerManager().SetTimer(ForgottenActorTimerHandle, this, &ASPTEnemyController::CheckIfForgottenSeenActor, 0.5f, true);
    GetWorldTimerManager().SetTimer(ForgottenDamagedActorTimerHandle, this, &ASPTEnemyController::CheckIfForgottenDamagedActor, 1.5f, true);
}

void ASPTEnemyController::OnUnPossess()
{
    Super::OnUnPossess();

    GetWorldTimerManager().ClearTimer(ForgottenActorTimerHandle);
    GetWorldTimerManager().ClearTimer(ForgottenDamagedActorTimerHandle);
}

void ASPTEnemyController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    UE_LOG(LogTemp, Warning, TEXT("OnPerceptionUpdated executed, number of detected actors: %d"), UpdatedActors.Num());

    if (!AIPerceptionComp || !BBC)
    {
        return;
    }

    for (AActor* Actor : UpdatedActors)
    {
        if (!Actor) continue;

        FAIStimulus Stimulus;

        if (CanSenseActor(Actor, EAISenseType::Sight, Stimulus))
        {
            UE_LOG(LogTemp, Warning, TEXT("CanSenseActor: Sight detected!"));
            HandleSensedSight(Actor);
        }

        if (CanSenseActor(Actor, EAISenseType::Hearing, Stimulus))
        {
            UE_LOG(LogTemp, Warning, TEXT("CanSenseActor: Hearing detected!"));
            HandleSensedSound(Stimulus.StimulusLocation);
        }

        if (CanSenseActor(Actor, EAISenseType::Damage, Stimulus))
        {
            UE_LOG(LogTemp, Warning, TEXT("CanSenseActor: Damage detected!"));
            HandleSensedDamage(Actor);
        }
    }
}


void ASPTEnemyController::SetStateAsPassive()
{
    if (BBC)
    {
        BBC->SetValueAsEnum(FName("State"), 0); // 0 = Passive
        UE_LOG(LogTemp, Warning, TEXT("%s AIState Set to Passive!"), *GetName());
    }
}

void ASPTEnemyController::SetStateAsAttacking(AActor* AttackTarget)
{
    if (BBC)
    {
        BBC->SetValueAsObject(TEXT("AttackTarget"), AttackTarget);
        UE_LOG(LogTemp, Warning, TEXT("AttackTarget Set in Blackboard: %s"), *AttackTarget->GetName());

        BBC->SetValueAsEnum(FName("State"), 1); // 1 = Attacking
        UE_LOG(LogTemp, Warning, TEXT("%s AIState Set to Attacking!"), *GetName());
    }
}

void ASPTEnemyController::SetStateAsInvestigating(FVector Location)
{
    if (BBC)
    {
        BBC->SetValueAsEnum(TEXT("State"), 2); // 2 == Investigating

        BBC->SetValueAsVector(TEXT("PointOfInterest"), Location);
    }
}

bool ASPTEnemyController::CanSenseActor(AActor* Actor, EAISenseType Sense, FAIStimulus& OutStimulus) const
{
    if (!AIPerceptionComp || !Actor)
    {
        return false;
    }

    // 감각 ID 가져오기 (정수 변환 필요)
    int32 SenseID = -1;
    if (Sense == EAISenseType::Sight) {
        SenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass()).Index;
    }
    else if (Sense == EAISenseType::Hearing) {
        SenseID = UAISense::GetSenseID(UAISense_Hearing::StaticClass()).Index;
    }
    else if (Sense == EAISenseType::Damage) {
        SenseID = UAISense::GetSenseID(UAISense_Damage::StaticClass()).Index;
    }
    else {
        return false;
    }

    // 감지된 Actor의 모든 감지 정보를 저장하는 구조체
    FActorPerceptionBlueprintInfo PerceptionInfo;
    // 감지된 Actor의 Stimulus 정보를 가져와서 PerceptionInfo 구조체에 저장
    if (AIPerceptionComp->GetActorsPerception(Actor, PerceptionInfo))
    {
        // PerceptionInfo.LastSensedStimuli 배열 - 저장된 감지된 Actor의 Stimulus 정보
        // FAIStimulus - 감지된 Actor의 감지한 개별 감각(Stimulus)을 저장하는 구조체
        for (const FAIStimulus& Stimulus : PerceptionInfo.LastSensedStimuli)
        {
            int32 StimulusID = Stimulus.Type.Index;

            UE_LOG(LogTemp, Warning, TEXT("Stimulus dected: Type %d (Expect: %d), Success or failure: %d"),
                StimulusID, SenseID, Stimulus.WasSuccessfullySensed());

            // 현재 Stimulus의 감각 유형이 SenseClass와 일치하는지 확인
            // Stimulus.WasSuccessfullySensed() - AI가 해당 Stimulus를 성공적으로 감지했는지를 반환하는 함수
            if (Stimulus.WasSuccessfullySensed() && Stimulus.Type == SenseID)
            {
                OutStimulus = Stimulus;
                return true;
            }
        }
    }

    return false;
}

int32 ASPTEnemyController::GetCurrentState() const
{
    if (!BBC) return 0; // 기본값 (Passive)

    // 블랙보드에서 State 값을 가져옴
    int32 EnumValue = BBC->GetValueAsEnum(TEXT("State"));

    return EnumValue;
}

void ASPTEnemyController::HandleSensedSight(AActor* Actor)
{
    if (!Actor || !BBC) return;
    UE_LOG(LogTemp, Warning, TEXT("HandleSensedSight excute: %s"), *Actor->GetName());

    // 감지된 액터를 KnownSeenActors 배열에 추가 (중복 방지)
    KnownSeenActors.AddUnique(Actor);

    // 현재 AI 상태 가져오기
    int32 StateValue = GetCurrentState();

    // 플레이어 캐릭터 가져오기
    AActor* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    // 현재 상태가 Passive이고, 감지된 Actor가 플레이어라면 Attacking 상태로 변경
    if ((StateValue == 0 || StateValue == 2) && Actor == PlayerCharacter) // 0 == Passive, 2 == Investigating
    {
        SetStateAsAttacking(Actor);
    }
}

void ASPTEnemyController::HandleSensedSound(FVector Location)
{
    if (!BBC) return;
    UE_LOG(LogTemp, Warning, TEXT("HandleSensedSound excute"));

    int32 StateValue = GetCurrentState();

    // 현재 상태가 Passive이고, 감지된 Actor가 플레이어라면 Attacking 상태로 변경
    if (StateValue == 0 || StateValue == 2)
    {
        SetStateAsInvestigating(Location);
    }
}

void ASPTEnemyController::HandleSensedDamage(AActor* Actor)
{
    if (!Actor || !BBC) return;
    UE_LOG(LogTemp, Warning, TEXT("HandleSensedDamage excute"));

    // 감지된 액터를 KnownDamagedActors 배열에 추가 (중복 방지)
    KnownDamagedActors.AddUnique(Actor);

    int32 StateValue = GetCurrentState();

    if ((StateValue == 0 || StateValue == 2))
    {
        SetStateAsAttacking(Actor);
    }
}

void ASPTEnemyController::CheckIfForgottenSeenActor()
{
    if (!AIPerceptionComp) return;

    TArray<AActor*> CurrentlyPerceivedActors;
    AIPerceptionComp->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), CurrentlyPerceivedActors);
    TSet<AActor*> CurrentlyPerceivedSet(CurrentlyPerceivedActors); // 성능 최적화 - 배열(TArray)보다 해시 기반의 TSet이 더 빠르게 검색 가능

    for (int32 i = KnownSeenActors.Num() - 1; i >= 0; i--)
    {
        AActor* SeenActor = KnownSeenActors[i];

        // 더 이상 감지되지 않는다면
        if (!CurrentlyPerceivedSet.Contains(SeenActor))
        {
            UE_LOG(LogTemp, Warning, TEXT("Forgetting Actor: %s"), *SeenActor->GetName());
            HandleForgotActor(SeenActor);
        }
    }
}


void ASPTEnemyController::CheckIfForgottenDamagedActor()
{
    if (!AIPerceptionComp) return;

    TArray<AActor*> CurrentlyDamagedActors;
    AIPerceptionComp->GetKnownPerceivedActors(UAISense_Damage::StaticClass(), CurrentlyDamagedActors);
    TSet<AActor*> CurrentlyDamagedSet(CurrentlyDamagedActors);

    for (int32 i = KnownDamagedActors.Num() - 1; i >= 0; i--)
    {
        if (!CurrentlyDamagedSet.Contains(KnownDamagedActors[i]))
        {
            UE_LOG(LogTemp, Warning, TEXT("Forgetting Damaged Actor: %s"), *KnownDamagedActors[i]->GetName());
            HandleForgotActor(KnownDamagedActors[i]);
            KnownDamagedActors.RemoveAt(i);
        }
    }
}

void ASPTEnemyController::HandleForgotActor(AActor* Actor)
{
    if (!Actor || !BBC) return;

    UE_LOG(LogTemp, Warning, TEXT("HandleForgotActor: %s"), *Actor->GetName());

    KnownSeenActors.Remove(Actor);
    KnownDamagedActors.Remove(Actor);

    if (!BBC) return;

    // 블랙보드에서 현재 공격 대상 가져오기
    AActor* CurrentAttackTarget = Cast<AActor>(BBC->GetValueAsObject(TEXT("AttackTarget")));

    // 현재 공격 대상과 비교하여 같다면 상태를 Passive로 변경
    if (CurrentAttackTarget == Actor)
    {
        BBC->SetValueAsObject(TEXT("AttackTarget"), nullptr);
        SetStateAsPassive();
    }
}
