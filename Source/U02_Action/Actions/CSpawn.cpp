#include "CSpawn.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACSpawn::ACSpawn()
{
	
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle",Sphere);

	InitialLifeSpan = 3.0f; //생명

}

void ACSpawn::BeginPlay()
{
	Super::BeginPlay();

	//컬리젼 컴포넌트 가져오기
	TArray<UShapeComponent*> components;
	GetComponents<UShapeComponent>(components);

	//충돌되면 함수호출
	for (UShapeComponent* component : components)
		component->OnComponentBeginOverlap.AddDynamic(this, &ACSpawn::OnComponentBeginOverlap);

}

void ACSpawn::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//자기자신 충돌 방지
	CheckTrue(OtherActor == GetOwner());

	if (!!Explostion)
	{
		FTransform transform = ExplosionTransform;
		//bFromSweep 막힌게 있다면 그 위치에서 충돌
		//SweepResult.Location 블록킹 볼륨에 막힌 위치
		transform.AddToTranslation(bFromSweep ? SweepResult.Location : GetActorLocation());
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));

		//충돌되면 나오는 Hit파티클
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explostion, transform);
	}

	if (OnSpawnBeginOverlap.IsBound())
		OnSpawnBeginOverlap.Broadcast(SweepResult);

	Destroy();
}



