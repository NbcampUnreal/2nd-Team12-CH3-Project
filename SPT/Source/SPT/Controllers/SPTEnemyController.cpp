#include "SPTEnemyController.h"
#include "SPTEnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"             // UAISense�� �⺻ ��� ���
#include "Perception/AISense_Sight.h"       // UAISense_Sight::StaticClass() ��� ����
#include "Perception/AISense_Hearing.h"     // UAISense_Hearing::StaticClass() ��� ����
#include "Perception/AISense_Damage.h"      // UAISense_Damage::StaticClass() ��� ����
#include "UObject/EnumProperty.h"           // �������Ʈ Enum�� �������� ���� ���
#include "Kismet/GameplayStatics.h"

ASPTEnemyController::ASPTEnemyController()
{
    // ������ ����
    BBC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

    // BehaviorTree �� BlackboardData�� �����Ϳ��� ������ �⺻������ ����
    static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("/Game/Blueprints/AI/BB_EnemyAI.BB_EnemyAI"));

    if (BB.Succeeded()) BlackboardAsset = BB.Object;

    // AI Perception ���� �̺�Ʈ ����  
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

    // AI ��Ʈ�ѷ� �������Ʈ Ŭ�������� ������� BehaviorTree ����
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

        // BT_EnemyAI�� AIPerceptionComponent �ʿ�
        bNeedsPerception = true;
    }

    // BT_EnemyAI�� ����ϴ� ��쿡�� AIPerceptionComponent ����
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

    // BehaviorTree ����
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

    // ���� ID �������� (���� ��ȯ �ʿ�)
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

    // ������ Actor�� ��� ���� ������ �����ϴ� ����ü
    FActorPerceptionBlueprintInfo PerceptionInfo;
    // ������ Actor�� Stimulus ������ �����ͼ� PerceptionInfo ����ü�� ����
    if (AIPerceptionComp->GetActorsPerception(Actor, PerceptionInfo))
    {
        // PerceptionInfo.LastSensedStimuli �迭 - ����� ������ Actor�� Stimulus ����
        // FAIStimulus - ������ Actor�� ������ ���� ����(Stimulus)�� �����ϴ� ����ü
        for (const FAIStimulus& Stimulus : PerceptionInfo.LastSensedStimuli)
        {
            int32 StimulusID = Stimulus.Type.Index;

            UE_LOG(LogTemp, Warning, TEXT("Stimulus dected: Type %d (Expect: %d), Success or failure: %d"),
                StimulusID, SenseID, Stimulus.WasSuccessfullySensed());

            // ���� Stimulus�� ���� ������ SenseClass�� ��ġ�ϴ��� Ȯ��
            // Stimulus.WasSuccessfullySensed() - AI�� �ش� Stimulus�� ���������� �����ߴ����� ��ȯ�ϴ� �Լ�
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
    if (!BBC) return 0; // �⺻�� (Passive)

    // �����忡�� State ���� ������
    int32 EnumValue = BBC->GetValueAsEnum(TEXT("State"));

    return EnumValue;
}

void ASPTEnemyController::HandleSensedSight(AActor* Actor)
{
    if (!Actor || !BBC) return;
    UE_LOG(LogTemp, Warning, TEXT("HandleSensedSight excute: %s"), *Actor->GetName());

    // ������ ���͸� KnownSeenActors �迭�� �߰� (�ߺ� ����)
    KnownSeenActors.AddUnique(Actor);

    // ���� AI ���� ��������
    int32 StateValue = GetCurrentState();

    // �÷��̾� ĳ���� ��������
    AActor* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    // ���� ���°� Passive�̰�, ������ Actor�� �÷��̾��� Attacking ���·� ����
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

    // ���� ���°� Passive�̰�, ������ Actor�� �÷��̾��� Attacking ���·� ����
    if (StateValue == 0 || StateValue == 2)
    {
        SetStateAsInvestigating(Location);
    }
}

void ASPTEnemyController::HandleSensedDamage(AActor* Actor)
{
    if (!Actor || !BBC) return;
    UE_LOG(LogTemp, Warning, TEXT("HandleSensedDamage excute"));

    // ������ ���͸� KnownDamagedActors �迭�� �߰� (�ߺ� ����)
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
    TSet<AActor*> CurrentlyPerceivedSet(CurrentlyPerceivedActors); // ���� ����ȭ - �迭(TArray)���� �ؽ� ����� TSet�� �� ������ �˻� ����

    for (int32 i = KnownSeenActors.Num() - 1; i >= 0; i--)
    {
        AActor* SeenActor = KnownSeenActors[i];

        // �� �̻� �������� �ʴ´ٸ�
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

    // �����忡�� ���� ���� ��� ��������
    AActor* CurrentAttackTarget = Cast<AActor>(BBC->GetValueAsObject(TEXT("AttackTarget")));

    // ���� ���� ���� ���Ͽ� ���ٸ� ���¸� Passive�� ����
    if (CurrentAttackTarget == Actor)
    {
        BBC->SetValueAsObject(TEXT("AttackTarget"), nullptr);
        SetStateAsPassive();
    }
}
