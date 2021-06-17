#include "CAIController.h"
#include "Global.h"
#include "CEnemy_AI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "CPlayer.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UCBehaviorComponent>(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600.0f; //�þ߹���
	Sight->LoseSightRadius = 800.0f; //�ν� �Ÿ�
	Sight->PeripheralVisionAngleDegrees = 90.0f; //�þ߰�
	Sight->SetMaxAge(2.0f); 

	Sight->DetectionByAffiliation.bDetectEnemies = true; //�� ������
	Sight->DetectionByAffiliation.bDetectFriendlies = false; //�Ʊ� ���� ����
	Sight->DetectionByAffiliation.bDetectNeutrals = false; //�߸� ���� ����

	Perception->ConfigureSense(*Sight); //Sense�� �þ߰��� 
	Perception->SetDominantSense(*Sight->GetSenseImplementation()); //���� �켱����, �þ߹ۿ� ��� �þ߰� ������
}


void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDrawDebug)
	{
		FVector center = OwnerEnemy->GetActorLocation();
		center.Z -= AdjustCirclHeight;

		//���ݹ���
		DrawDebugCircle(GetWorld(), center, Sight->SightRadius, 300, FColor::Green, false, -1.0f, 0, 0, FVector::RightVector, FVector::ForwardVector);
		//��������
		DrawDebugCircle(GetWorld(), center, BehaviorRange, 300, FColor::Red, false, -1.0f, 0, 0, FVector::RightVector, FVector::ForwardVector);
	}
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

//���� �Ǵ°�
void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerEnemy = Cast<ACEnemy_AI>(InPawn); //����ü
	CheckNull(OwnerEnemy);
				//����ü�� �ִ� ������							//������� ����
	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset,Blackboard); //����� ������(�� ��ü �ȿ� �ִ� ������)
	SetGenericTeamId(OwnerEnemy->GetTeamID()); //TeamID

	//�÷��̾ ������ �Ǹ� �߻��� �Լ�(OnPerceptionUpdated)
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);

	//BehaviorComponent�� ������ �־��ֱ� 
	Behavior->SetBlackboard(Blackboard); 

	//������ Ʈ�� ����
	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());

}

//���ǰ� Ǯ����
void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	Perception->OnPerceptionUpdated.Clear(); //���ε���(��ϵ� AddDynamic) �Լ��� ������
}
											  //������ �� �ֵ��� �Ϸ� ����
void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray <AActor*> actors;
	//������ ���� ����
	Perception->GetCurrentlyPerceivedActors(NULL, actors); //������ Ǯ���� �迭 �ʱ�ȭ
	//Perception->GetPerceivedActors(NULL, ); //������ Ǯ���� �迭�� ��������

	ACPlayer* player = NULL;
	//�÷��̾� Ÿ�Ը� �߸���
	for (AActor* actor : actors)
	{
		player = Cast<ACPlayer>(actor);
		
		//�÷��̾�� ĳ������ ������ ����������
		if (!!player)
			break;
	}

	//������ �÷��̾� ���� ������ �÷��̾� ����
	Blackboard->SetValueAsObject("Player", player);
}
