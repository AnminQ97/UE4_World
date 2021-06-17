#include "CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/ShapeComponent.h"

ACAttachment::ACAttachment()
{
	//Scene
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
}

void ACAttachment::OnCollision()
{
	for (UShapeComponent* component : ShapeComponents)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACAttachment::OffCollision()
{
	for (UShapeComponent* component : ShapeComponents)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACAttachment::BeginPlay()
{
	//UShapeComponent* collision;

	//player 가져오기
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	//player에 붙은 State,Status 가져오기
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	//충돌체 가져오기
	GetComponents<UShapeComponent>(ShapeComponents);

	//모든 충돌체 바인딩
	for (UShapeComponent* component : ShapeComponents)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
		component->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
	}

	//처음엔 컬리젼 Off
	OffCollision();

	//위에 데이터를 불러오고 실행, 부모 Begine을 부르기
	//BP 실행
	Super::BeginPlay();

}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor); //자기자신 피격 막기
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass()); //자기자신과 같은 타입 피격 금지 ex) 적들끼리 공격 X

	if (OnAttachmentBeginOverlap.IsBound())
	{
		//								   //캐릭터        무기     대상
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
	}

}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
}

void ACAttachment::AttachTo(FName InSocketName)
{
	//붙을 메쉬, 컴포넌트 붙힐 규칙,소켓이름
	//나 자신이 오너캐릭 메시에 특정 소켓에 붙음
	AttachToComponent
	(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName
	);
}

void ACAttachment::AttachToCollision(USceneComponent* InComponent, FName InSocketName)
{
	//컬리젼을 소켓에 붙이기
	//InComponent 붙히고자 하는 대상
	InComponent->AttachToComponent
	(
		
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName
	);
}



