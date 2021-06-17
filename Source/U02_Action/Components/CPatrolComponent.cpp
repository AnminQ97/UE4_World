#include "CPatrolComponent.h"
#include "Global.h"
#include "Actions/CPatrolPath.h"
#include "Components/SplineComponent.h"

UCPatrolComponent::UCPatrolComponent()
{
	

}

bool UCPatrolComponent::GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadiuse)
{
	OutLocation = FVector::ZeroVector;
	OutAcceptanceRadiuse = AcceptanceRadiuse;
	CheckNullResult(Path, false); //Path가 널이면, 경로가 없으면 return false;

	OutLocation = Path->GetSpline()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World); //Index의 스플라인 월드위치 얻어옴
	return true;
}

void UCPatrolComponent::UpdateNextIndex()
{
	CheckNull(Path);

	int32 count = Path->GetSpline()->GetNumberOfSplinePoints();

	//Reverse 
	if (bReverse)
	{
		//뒤가 있는경우
		if (Index > 0)
		{
			Index--;
			return;
		}

		//마지막에 도착하고 루프가 켜져있다면
		if (Path->GetSpline()->IsClosedLoop())
		{
			Index = count - 1;
			return;
		}

		//루프가 아니라면
		Index = 1;
		bReverse = false;

		return;
	}

	//Forward 정주행
	if (Index < count - 1)
	{
		Index++;
		return;
	}

	//마지막까지 오고 루프가 켜져있다면
	if (Path->GetSpline()->IsClosedLoop())
	{
		Index = 0;
		return;
	}

	//루프가 꺼져있다면
	Index = count - 2;
	bReverse = true; 
}


void UCPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


