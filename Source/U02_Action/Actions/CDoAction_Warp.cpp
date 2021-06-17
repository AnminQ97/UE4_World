#include "CDoAction_Warp.h"
#include "Global.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CBehaviorComponent.h"
#include "Components/DecalComponent.h"

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();

	//데칼 얻어오기
	//OwnerCharacter->Children 캐릭터 밑으로 붙어있는 애들 얻어오기
	for (AActor* actor : OwnerCharacter->Children)
	{
		//IsA 특정한 타입을 상속받았는지 확인
		//Attachment_Warp를 찾기
		if (actor->IsA<ACAttachment>() && actor->GetActorLabel().Contains("Warp"))
		{
			//데칼 컴포넌트 얻어오기
			Decal = CHelpers::GetComponent<UDecalComponent>(actor); //actor:Attachment_Warp
			break;
		}

	}
}

void ACDoAction_Warp::DoAction()
{
	CheckFalse(*bEquipped); 

	CheckFalse(State->IsIdleMode());

	if (UserCursorLocation()) //유저 커서 값이 있다면 플레이어 워프
	{
		FRotator rotator;
		CheckFalse(GetCursorLocationAndRotation(Location, rotator));
	}
	else  //아니면 몬스터 워프
	{
		AAIController* controller = OwnerCharacter->GetController<AAIController>();
		UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

		Location = behavior->GetWarpLocation();
		Decal->SetVisibility(false);
	}

	State->SetActionMode();
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Warp::Begin_DoAction()
{
	FTransform transform = Datas[0].EffectTransform;
	//파티클
	UGameplayStatics::SpawnEmitterAttached
	(
		Datas[0].Effect,
		OwnerCharacter->GetMesh(),
		"", 
		transform.GetLocation(), 
		FRotator(transform.GetRotation()),
		transform.GetScale3D()
	);
}

void ACDoAction_Warp::End_DoAction()
{
	OwnerCharacter->SetActorLocation(Location);
	Location = FVector::ZeroVector;

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Equipment에 있는 bEquipped 확인
	CheckFalse(*bEquipped);

	if (UserCursorLocation() == NULL)
	{
		Decal->SetVisibility(false);
		return;
	}

	FVector location;
	FRotator rotator;
	//마우스가갈 수 있는위치
	if (GetCursorLocationAndRotation(location, rotator))
	{
		//데칼 보이기, 위치, 회전
		Decal->SetVisibility(true);
		Decal->SetWorldLocation(location);
		Decal->SetWorldRotation(rotator);
		
	}
	else
		Decal->SetVisibility(false);
}

ACPlayer* ACDoAction_Warp::UserCursorLocation()
{
	return Cast<ACPlayer>(OwnerCharacter);
}

//마우스 충돌이 땅에만 되게
bool ACDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotator)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	//충돌대상, 충돌 타입을 배열로 넣음
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery1); //월드스테틱 타입

	FHitResult hitResult;
	//마우스 위치랑 월드에 있는 3D메쉬랑 충돌체크 후 HitResult 리턴 함수
	if (controller->GetHitResultUnderCursorForObjects(objects, false, hitResult))
	{	
		OutLocation = hitResult.Location;//마우스 충돌 위치
		OutLocation.Z += 20.0f;
		OutRotator = hitResult.ImpactNormal.Rotation(); //-방지

		return true;
	}

	return false;
}
