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
	Sight->SightRadius = 600.0f; //시야범위
	Sight->LoseSightRadius = 800.0f; //인식 거리
	Sight->PeripheralVisionAngleDegrees = 90.0f; //시야각
	Sight->SetMaxAge(2.0f); 

	Sight->DetectionByAffiliation.bDetectEnemies = true; //적 감지함
	Sight->DetectionByAffiliation.bDetectFriendlies = false; //아군 감지 안함
	Sight->DetectionByAffiliation.bDetectNeutrals = false; //중립 감지 안함

	Perception->ConfigureSense(*Sight); //Sense에 시야감지 
	Perception->SetDominantSense(*Sight->GetSenseImplementation()); //감지 우선순위, 시야밖에 없어서 시야가 젤높음
}


void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDrawDebug)
	{
		FVector center = OwnerEnemy->GetActorLocation();
		center.Z -= AdjustCirclHeight;

		//공격범위
		DrawDebugCircle(GetWorld(), center, Sight->SightRadius, 300, FColor::Green, false, -1.0f, 0, 0, FVector::RightVector, FVector::ForwardVector);
		//도망범위
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

//빙의 되는곳
void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerEnemy = Cast<ACEnemy_AI>(InPawn); //적개체
	CheckNull(OwnerEnemy);
				//적개체에 있는 블랙보드							//저장받을 변수
	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset,Blackboard); //사용할 블랙보드(적 개체 안에 있는 블랙보드)
	SetGenericTeamId(OwnerEnemy->GetTeamID()); //TeamID

	//플레이어가 감지가 되면 발생할 함수(OnPerceptionUpdated)
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);

	//BehaviorComponent에 블랙보드 넣어주기 
	Behavior->SetBlackboard(Blackboard); 

	//비헤비어 트리 실행
	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());

}

//빙의가 풀릴때
void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	Perception->OnPerceptionUpdated.Clear(); //바인딩된(등록된 AddDynamic) 함수들 날리기
}
											  //감지가 된 애들이 일로 들어옴
void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray <AActor*> actors;
	//감지된 적들 저장
	Perception->GetCurrentlyPerceivedActors(NULL, actors); //감지가 풀리면 배열 초기화
	//Perception->GetPerceivedActors(NULL, ); //감지가 풀려도 배열에 남아있음

	ACPlayer* player = NULL;
	//플레이어 타입만 추리기
	for (AActor* actor : actors)
	{
		player = Cast<ACPlayer>(actor);
		
		//플레이어로 캐스팅이 됐으면 빠져나오기
		if (!!player)
			break;
	}

	//블랙보드 플레이어 값에 감지된 플레이어 저장
	Blackboard->SetValueAsObject("Player", player);
}
