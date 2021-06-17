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

	//player ��������
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	//player�� ���� State,Status ��������
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	//�浹ü ��������
	GetComponents<UShapeComponent>(ShapeComponents);

	//��� �浹ü ���ε�
	for (UShapeComponent* component : ShapeComponents)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
		component->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
	}

	//ó���� �ø��� Off
	OffCollision();

	//���� �����͸� �ҷ����� ����, �θ� Begine�� �θ���
	//BP ����
	Super::BeginPlay();

}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor); //�ڱ��ڽ� �ǰ� ����
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass()); //�ڱ��ڽŰ� ���� Ÿ�� �ǰ� ���� ex) ���鳢�� ���� X

	if (OnAttachmentBeginOverlap.IsBound())
	{
		//								   //ĳ����        ����     ���
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
	//���� �޽�, ������Ʈ ���� ��Ģ,�����̸�
	//�� �ڽ��� ����ĳ�� �޽ÿ� Ư�� ���Ͽ� ����
	AttachToComponent
	(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName
	);
}

void ACAttachment::AttachToCollision(USceneComponent* InComponent, FName InSocketName)
{
	//�ø����� ���Ͽ� ���̱�
	//InComponent �������� �ϴ� ���
	InComponent->AttachToComponent
	(
		
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName
	);
}



