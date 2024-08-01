// Fill out your copyright notice in the Description page of Project Settings.


#include "Rollback_1v1_GameState.h"


GGPOSession* ggpo;
UGGPOGameInstance* gameInst;
ABaseCharacter *P1Character, *P2Character;
GGPOPlayer p1, p2;
GGPOPlayerHandle player_handles[] = {0, 1};
GGPOPlayerHandle* localHandle, *opHandle;
UFGJ_GameState* curGameState;
int inputs[2] = { 0 };
int disconnect_flags;

#define SYNC_TEST

enum PlayerConnectState {
	Connecting = 0,
	Synchronizing,
	Running,
	Disconnected,
	Disconnecting,
};

//void SetConnectState(GGPOPlayerHandle handle, PlayerConnectState state) {
//
//	if (player_handles[0] == handle) {
//		
//	}
//}

bool cb_begin_game(const char* game)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Beginning Game");
	return true;
}

bool cb_save_game_state(unsigned char** buffer, int* len, int* checksum, int frame)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Saving Game State");
	UpdateGameStateFromWorld();

	*len = sizeof(&curGameState);
	*buffer = (unsigned char*)malloc(*len);
	if (!*buffer) {
		return false;
	}

	memcpy(*buffer, curGameState, *len);

	return true;
}

bool cb_load_game_state(unsigned char* buffer, int len)
{
	memcpy(curGameState, buffer, len);
	UpdateWorldFromGameState();
	return true;
}

bool cb_log_game_state(char* filename, unsigned char* buffer, int len)
{
	FILE* fp = nullptr;
	fopen_s(&fp, filename, "w");
	if (fp) {
		UFGJ_GameState* gameState = (UFGJ_GameState*)buffer;

		fprintf(fp, "Player 1 Position: %.4f, %.4f", gameState->Player1State.Position.X, gameState->Player1State.Position.Y);
		fprintf(fp, "Player 2 Position: %.4f, %.4f", gameState->Player2State.Position.X, gameState->Player2State.Position.Y);

		fclose(fp);
	}
	return true;
}

void cb_free_buffer(void* buffer)
{
	free(buffer);
}

bool cb_advance_frame(int flags)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Advancing Frame");
	disconnect_flags = 0;

	// Make sure we fetch new inputs from GGPO and use those to update
	// the game state instead of reading from the keyboard.
	GGPONet::ggpo_synchronize_input(ggpo, (void*)inputs, sizeof(int) * 2, &disconnect_flags);
	FGJ_AdvanceFrame(inputs, disconnect_flags);
	return true;
}

bool cb_on_event(GGPOEvent* info)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Event!");
	int progress;
	switch (info->code) {
	case GGPO_EVENTCODE_CONNECTED_TO_PEER:
		//SetConnectState(info->u.connected.player, Synchronizing);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Connected");
		break;
	case GGPO_EVENTCODE_SYNCHRONIZING_WITH_PEER:
		progress = 100 * info->u.synchronizing.count / info->u.synchronizing.total;
		//ngs.UpdateConnectProgress(info->u.synchronizing.player, progress);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Sync Progress: " + progress);

		break;
	case GGPO_EVENTCODE_SYNCHRONIZED_WITH_PEER:
		//ngs.UpdateConnectProgress(info->u.synchronized.player, 100);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Synced!");
		break;
	case GGPO_EVENTCODE_RUNNING:
		//ngs.SetConnectState(Running);
		//renderer->SetStatusText("");
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Running");
		break;
	case GGPO_EVENTCODE_CONNECTION_INTERRUPTED:
		//ngs.SetDisconnectTimeout(info->u.connection_interrupted.player,
			//timeGetTime(),
			//info->u.connection_interrupted.disconnect_timeout);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Disconnected");
		break;
	case GGPO_EVENTCODE_CONNECTION_RESUMED:
		//ngs.SetConnectState(info->u.connection_resumed.player, Running);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Reconnected");
		break;
	case GGPO_EVENTCODE_DISCONNECTED_FROM_PEER:
		//ngs.SetConnectState(info->u.disconnected.player, Disconnected);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Disconnected");
		break;
	case GGPO_EVENTCODE_TIMESYNC:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Resyncing");

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Frames Ahead : %i "), info->u.timesync.frames_ahead));

		FPlatformProcess::Sleep(info->u.timesync.frames_ahead / 60);
		break;
	}
	return true;
}

bool ARollback_1v1_GameState::StartSession(int localPlayerIndex, int port, int opPort, const char* opIp) {

	startCount++;
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Start Session Called");

	GGPOErrorCode result = GGPOErrorCode();
	GGPOSessionCallbacks cb = { 0 };

	/* fill in all callback functions */
	cb.begin_game = cb_begin_game;
	cb.advance_frame = cb_advance_frame;
	cb.load_game_state = cb_load_game_state;
	cb.log_game_state = cb_log_game_state;
	cb.save_game_state = cb_save_game_state;
	cb.free_buffer = cb_free_buffer;
	cb.on_event = cb_on_event;

	//setup player objects
	p1.size = p2.size = sizeof(GGPOPlayer);

	p1.player_num = 1;
	p2.player_num = 2;

	if (localPlayerIndex == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Player 1 start!");
		p1.type = EGGPOPlayerType::LOCAL;
		p2.type = EGGPOPlayerType::REMOTE;

		strcpy_s(p2.u.remote.ip_address, opIp);
		p2.u.remote.port = opPort;
	}

	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Player 2 start!");
		p2.type = EGGPOPlayerType::LOCAL;
		p1.type = EGGPOPlayerType::REMOTE;

		strcpy_s(p1.u.remote.ip_address, opIp);
		p1.u.remote.port = opPort;
	}


#if defined(SYNC_TEST)
	char appName[] = "test_fight";

	result = GGPONet::ggpo_start_synctest(&ggpo, &cb, appName, 2, sizeof(int), 1);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "SyncTest Start Called");
#else
	/* Start a new session */
	result = GGPONet::ggpo_start_session(&ggpo,         // the new session object
		&cb,           // our callbacks
		"test_fight",    // application name
		2,             // 2 players
		sizeof(int),   // size of an input packet
		port);         // our local udp port
#endif
	if (result != 0){
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Creating Session Failed");
		Active = false;
		return false;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Created Session!");

	// automatically disconnect clients after 3000 ms and start our count-down timer
	// for disconnects after 1000 ms.   To completely disable disconnects, simply use
	// a value of 0 for ggpo_set_disconnect_timeout.
	GGPONet::ggpo_set_disconnect_timeout(ggpo, 3000);
	GGPONet::ggpo_set_disconnect_notify_start(ggpo, 1000);

	GGPOPlayerHandle handle1, handle2;

	GGPONet::ggpo_add_player(ggpo, &p1, &handle1);
	player_handles[0] = handle1;

	GGPONet::ggpo_add_player(ggpo, &p2, &handle2);
	player_handles[1] = handle2;
	
	// set handles
	if (localPlayerIndex == 0) {
		localHandle = &player_handles[0];
		opHandle = &player_handles[1];
	}

	else {
		localHandle = &player_handles[1];
		opHandle = &player_handles[0];
	}

	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("P1 Handle : %i "), player_handles[0]));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("P2 Handle : %i "), player_handles[1]));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Local Handle : %i "), *localHandle));*/

	GGPONet::ggpo_set_frame_delay(ggpo, *localHandle, 3);

	LocalController = Cast<ARollbackPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	return true;
		
}

ARollback_1v1_GameState::ARollback_1v1_GameState()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	LocalController = Cast<ARollbackPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ARollback_1v1_GameState::StartConnection(UClass* playerClass)
{
	if (!Active)
	{
		Active = true;

		gameInst = Cast<UGGPOGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (gameInst) {
			if (gameInst->NetworkAddresses) {
				int localIndex = gameInst->NetworkAddresses->GetPlayerIndex();
				int localPort = gameInst->NetworkAddresses->GetLocalPort();

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Player Number : %i "), (gameInst->NetworkAddresses->GetPlayerIndex())));

				UGGPONetworkAddress* opAddress;

				if (localIndex == 0) {
					opAddress = gameInst->NetworkAddresses->GetAddress(1);
				}

				else {
					opAddress = gameInst->NetworkAddresses->GetAddress(0);
				}

				//spawn characters
				FVector pScale = FVector(1, 1, 1);

				FActorSpawnParameters p1Params = FActorSpawnParameters();
				p1Params.Name = "Player 1";
				FVector p1Loc = FVector(0, 250, 10);
				FRotator p1Rot = FRotator(0, -90, 0);
				FTransform p1Trans = FTransform(p1Rot, p1Loc, pScale);

				P1Character = (ABaseCharacter*)GetWorld()->SpawnActor(playerClass, &p1Trans, p1Params);

				FActorSpawnParameters p2Params = FActorSpawnParameters();
				p2Params.Name = "Player 2";
				FVector p2Loc = FVector(0, -250, 10);
				FRotator p2Rot = FRotator(0, 90, 0);
				FTransform p2Trans = FTransform(p2Rot, p2Loc, pScale);

				P2Character = (ABaseCharacter*)GetWorld()->SpawnActor(playerClass, &p2Trans, p2Params);

				//set opponent variable
				P1Character->opponent = P2Character;
				P2Character->opponent = P1Character;

				if (P1Character) {
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Valid Player 1");
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Player 1 : %s "), (*P1Character->GetName())));
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Player 1 Opponent : %s "), (*P1Character->opponent->GetName())));
				}
				if (P2Character) {
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Valid Player 2");
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Player 2 : %s "), (*P2Character->GetName())));
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Player 2 Opponent: %s "), (*P2Character->opponent->GetName())));
				}

				bool sessionSuccess = StartSession(localIndex, localPort, opAddress->GetPort(), TCHAR_TO_ANSI(*opAddress->GetIpAddressString()));

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Sending message from port %i "), localPort));

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Sending message to port %i "), opAddress->GetPort()));

				if (sessionSuccess) {
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Started Session");
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Session failed to start.");
					Active = false;
				}

			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Invalid GGPO Game Instance Address Book");
				Active = false;
			}

		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Invalid GGPO Game Instance");
			Active = false;
		}
	}
}

void ARollback_1v1_GameState::BeginPlay()
{
	Super::BeginPlay();

	//Initialize variables
	curGameState = (UFGJ_GameState*)malloc(sizeof(UFGJ_GameState));
}

/*
 * FGJ_AdvanceFrame --
 *
 * Advances the game state by exactly 1 frame using the inputs specified
 * for player 1 and player 2.
 */
void FGJ_AdvanceFrame(int inputArray[], int disconnection_flags)
{
	FUPlayerInputStruct Player1TempInput = FUPlayerInputStruct();
	FUPlayerInputStruct Player2TempInput = FUPlayerInputStruct();

	if (disconnect_flags & (1 << 0)) {
		Player1TempInput.FromInt(0);
	}
	else {
		Player1TempInput.FromInt(inputArray[0]);
	}
		
	if (disconnect_flags & (1 << 1)) {
		Player2TempInput.FromInt(0);
	}
	else {
		Player2TempInput.FromInt(inputArray[1]);
	}
	
	P1Character->HandleInput(Player1TempInput);
	P2Character->HandleInput(Player2TempInput);

	// Notify ggpo that we've moved forward exactly 1 frame.
	GGPONet::ggpo_advance_frame(ggpo);
}

void UpdateGameStateFromWorld()
{
	//curGameState->Player1State = P1Character->GetState();
	//curGameState->Player2State = P2Character->GetState();
	
	if (IsValid(P1Character)) {
		/*AFGJ_PlayerState* p1State;
		P1Character->GetState(p1State);
		int len = sizeof(p1State);
		memcpy(curGameState->Player1State, &p1State, len);*/
		P1Character->GetState(curGameState->Player1State);
	}
	if (IsValid(P2Character)) {
		
		/*AFGJ_PlayerState* p2State;
		P2Character->GetState(p2State);
		int len = sizeof(p2State);
		memcpy(curGameState->Player2State, &p2State, len);*/
		P2Character->GetState(curGameState->Player2State);
	}
}

void UpdateWorldFromGameState()
{
	if (IsValid(P1Character)) {
		P1Character->SetState(curGameState->Player1State);
	}
	if (IsValid(P2Character)) {
		P2Character->SetState(curGameState->Player2State);
	}
}

void ARollback_1v1_GameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GGPOErrorCode result = GGPO_OK;

	if (ggpo)
	{
		int preInputTime = clock();
		disconnect_flags = 0;

		if (LocalController) {
			//get Local inputs
			LocalInput = LocalController->currentInput;
			int localInputInt = LocalInput.ToInt();

			FUPlayerInputStruct NoInput = FUPlayerInputStruct();
			int noInputInt = NoInput.ToInt();

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Local Player Input : %i "), localInputInt));

			result = GGPONet::ggpo_add_local_input(ggpo, *localHandle, &localInputInt, sizeof(int));

			if (GGPO_SUCCEEDED(result)) {
				result = GGPONet::ggpo_synchronize_input(ggpo, inputs, sizeof(int) * 2, &disconnect_flags);

				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Player 1 Input : %i "), inputs[0]));
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Player 2 Input : %i "), inputs[1]));

				if (GGPO_SUCCEEDED(result)) {
					FGJ_AdvanceFrame(inputs, disconnect_flags);
				}
			}

			//if (p1.type == EGGPOPlayerType::LOCAL) {
			//	P1Character->HandleInput(LocalInput);

			//	//P2Character->HandleInput(NoInput);
			//}
			//else {
			//	P2Character->HandleInput(LocalInput);

			//	//P1Character->HandleInput(NoInput);
			//}
			
		}

		int postInputTime = clock();

		int duration = postInputTime - preInputTime;

		int idleTime = (DeltaTime * 1000) - duration;

		GGPONet::ggpo_idle(ggpo, idleTime);
	}

	
}



