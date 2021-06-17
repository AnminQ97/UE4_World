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

	//��Į ������
	//OwnerCharacter->Children ĳ���� ������ �پ��ִ� �ֵ� ������
	for (AActor* actor : OwnerCharacter->Children)
	{
		//IsA Ư���� Ÿ���� ��ӹ޾Ҵ��� Ȯ��
		//Attachment_Warp�� ã��
		if (actor->IsA<ACAttachment>() && actor->GetActorLabel().Contains("Warp"))
		{
			//��Į ������Ʈ ������
			Decal = CHelpers::GetComponent<UDecalComponent>(actor); //actor:Attachment_Warp
			break;
		}

	}
}

void ACDoAction_Warp::DoAction()
{
	CheckFalse(*bEquipped); 

	CheckFalse(State->IsIdleMode());

	if (UserCursorLocation()) //���� Ŀ�� ���� �ִٸ� �÷��̾� ����
	{
		FRotator rotator;
		CheckFalse(GetCursorLocationAndRotation(Location, rotator));
	}
	else  //�ƴϸ� ���� ����
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
	//��ƼŬ
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

	//Equipment�� �ִ� bEquipped Ȯ��
	CheckFalse(*bEquipped);

	if (UserCursorLocation() == NULL)
	{
		Decal->SetVisibility(false);
		return;
	}

	FVector location;
	FRotator rotator;
	//���콺���� �� �ִ���ġ
	if (GetCursorLocationAndRotation(location, rotator))
	{
		//��Į ���̱�, ��ġ, ȸ��
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

//���콺 �浹�� ������ �ǰ�
bool ACDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotator)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	//�浹���, �浹 Ÿ���� �迭�� ����
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery1); //���彺��ƽ Ÿ��

	FHitResult hitResult;
	//���콺 ��ġ�� ���忡 �ִ� 3D�޽��� �浹üũ �� HitResult ���� �Լ�
	if (controller->GetHitResultUnderCursorForObjects(objects, false, hitResult))
	{	
		OutLocation = hitResult.Location;//���콺 �浹 ��ġ
		OutLocation.Z += 20.0f;
		OutRotator = hitResult.ImpactNormal.Rotation(); //-����

		return true;
	}

	return false;
}
