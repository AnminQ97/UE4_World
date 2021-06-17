#include "CSpawn.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACSpawn::ACSpawn()
{
	
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle",Sphere);

	InitialLifeSpan = 3.0f; //����

}

void ACSpawn::BeginPlay()
{
	Super::BeginPlay();

	//�ø��� ������Ʈ ��������
	TArray<UShapeComponent*> components;
	GetComponents<UShapeComponent>(components);

	//�浹�Ǹ� �Լ�ȣ��
	for (UShapeComponent* component : components)
		component->OnComponentBeginOverlap.AddDynamic(this, &ACSpawn::OnComponentBeginOverlap);

}

void ACSpawn::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�ڱ��ڽ� �浹 ����
	CheckTrue(OtherActor == GetOwner());

	if (!!Explostion)
	{
		FTransform transform = ExplosionTransform;
		//bFromSweep ������ �ִٸ� �� ��ġ���� �浹
		//SweepResult.Location ���ŷ ������ ���� ��ġ
		transform.AddToTranslation(bFromSweep ? SweepResult.Location : GetActorLocation());
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));

		//�浹�Ǹ� ������ Hit��ƼŬ
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explostion, transform);
	}

	if (OnSpawnBeginOverlap.IsBound())
		OnSpawnBeginOverlap.Broadcast(SweepResult);

	Destroy();
}



