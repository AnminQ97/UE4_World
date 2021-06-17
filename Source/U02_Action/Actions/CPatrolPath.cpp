#include "CPatrolPath.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

ACPatrolPath::ACPatrolPath()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<USplineComponent>(this, &Spline, "Spline", Scene);
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Scene);

	Spline->SetRelativeLocation(FVector(0, 0, 30));

	Text->SetRelativeLocation(FVector(0, 0, 30));
	Text->SetRelativeRotation(FRotator(0, 180, 0));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Black;
	 
}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	Text->Text = FText::FromString(GetActorLabel());

	Spline->SetClosedLoop(bLoop); //에디터상에서 Loop On/Off
}

void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
	Text->SetVisibility(false); //게임 시작시 글씨 가리기
}

void ACPatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

