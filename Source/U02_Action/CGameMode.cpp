#include "CGameMode.h"
#include "Global.h"

ACGameMode::ACGameMode()
{

	//���Ӹ�� ����� �÷��̾� ��ŸƮ ��ġ�� �÷��̾� ��ȯ
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BPCPlayer.BPCPlayer_C'");

}