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
	CheckNullResult(Path, false); //Path�� ���̸�, ��ΰ� ������ return false;

	OutLocation = Path->GetSpline()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World); //Index�� ���ö��� ������ġ ����
	return true;
}

void UCPatrolComponent::UpdateNextIndex()
{
	CheckNull(Path);

	int32 count = Path->GetSpline()->GetNumberOfSplinePoints();

	//Reverse 
	if (bReverse)
	{
		//�ڰ� �ִ°��
		if (Index > 0)
		{
			Index--;
			return;
		}

		//�������� �����ϰ� ������ �����ִٸ�
		if (Path->GetSpline()->IsClosedLoop())
		{
			Index = count - 1;
			return;
		}

		//������ �ƴ϶��
		Index = 1;
		bReverse = false;

		return;
	}

	//Forward ������
	if (Index < count - 1)
	{
		Index++;
		return;
	}

	//���������� ���� ������ �����ִٸ�
	if (Path->GetSpline()->IsClosedLoop())
	{
		Index = 0;
		return;
	}

	//������ �����ִٸ�
	Index = count - 2;
	bReverse = true; 
}


void UCPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


