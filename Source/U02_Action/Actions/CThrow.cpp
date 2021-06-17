#include "CThrow.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACThrow::ACThrow()
{
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle",Sphere);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");
	
	InitialLifeSpan = 10; //����

	Projectile->InitialSpeed = 1000; //�ӵ�
	Projectile->MaxSpeed = 1200; //����
	Projectile->ProjectileGravityScale = 0.0f; //�߷�

}

void ACThrow::BeginPlay()
{
	Super::BeginPlay();

	//�ø��� ������Ʈ ��������
	TArray<UShapeComponent*> components;
	GetComponents<UShapeComponent>(components);

	//�浹�Ǹ� �Լ�ȣ��
	for (UShapeComponent* component : components)
		component->OnComponentBeginOverlap.AddDynamic(this, &ACThrow::OnComponentBeginOverlap);
	

}

void ACThrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

	//OnThrowBeginOverlap�� ����� �Լ��� ���� CDoAction_Throw���� ����
	if (OnThrowBeginOverlap.IsBound())
		OnThrowBeginOverlap.Broadcast(SweepResult);

	if(bDestroy)
		Destroy();
}


