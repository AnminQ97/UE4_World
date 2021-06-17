#include "CGameMode.h"
#include "Global.h"

ACGameMode::ACGameMode()
{

	//게임모드 실행시 플레이어 스타트 위치에 플레이어 소환
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BPCPlayer.BPCPlayer_C'");

}