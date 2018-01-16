//----------------------------------------------------
//--- GAMEGURU - M-Steam
//----------------------------------------------------

#include "gameguru.h"

// flag to switch workshop handling from workshop to game managed, by default set to false, set to true for multiplayer mode
extern bool OnlineMultiplayerModeForSharingFiles;

//  Startup Steam
void steam_init ( void )
{

	timestampactivity(0,"_steam_init:");
	t.steamworks_build = 1121;
	g.steamworks.isRunning = SteamInit();
	g.steamworks.mode = STEAM_MODE_NONE;
	g.steamworks.dontDrawTitles = 0;
	g.steamworks.message = "";
	g.steamworks.messageTime = 0;

	// If a custom character head is used but the image no longer exists, we need to get rid of the avatar file
	characterkit_checkAvatarExists();

return;

//  Steam Main Loop
}

bool OccluderCheckingForMultiplayer ( void )
{
	if ( t.game.runasmultiplayer == 0 ) return false;

	return true;
}

void steam_loop ( void )
{

	SteamLoop (  );

	//  store old positions of entities if in coop mode
	/*      
	if (  g.steamworks.coop  ==  1 ) 
	{
		if (  g.steamworks.madeArrays   ==  0 ) 
		{
			g.steamworks.madeArrays = 1;
			steam_storeOldEntityPositions ( );
		}
	}
	*/    

	//  debug stuff for loading in custom cc avatars
	/*      
	SetCursor (  0,0 );
	Print (  "My avatar string = " + g.steamworks.myAvatar_s );
	for ( t.c = 0 ; t.c<=  7; t.c++ )
	{
		Print (  t.steamworks_playerAvatarOwners_s[t.c] );
		Print (  t.steamworks_playerAvatars_s[t.c] );
	}
	Print (  "have sent mine " + Str(g.steamworks.haveSentMyAvatar) );
	*/    

	if (  g.steamworks.mode  ==  STEAM_MODE_NONE || t.game.runasmultiplayer  ==  0  )
	{
		// usual workshop mode
		OnlineMultiplayerModeForSharingFiles = false;
		return;
	}

	// game managed mode for sharing files
	OnlineMultiplayerModeForSharingFiles = true;

	if (  g.steamworks.mode  !=  STEAM_IN_GAME_CLIENT && g.steamworks.mode  !=  STEAM_IN_GAME_SERVER ) 
	{

		g.steamworks.finishedLoadingMap = 0;

		//  200315 - 021 - flashlight of when starting a game
		steam_flashLightOff ( );
		g.steamworks.originalEntitycount = 0;
		if (  SpriteExist(g.steamchatpanelsprite)  )  DeleteSprite (  g.steamchatpanelsprite );
		g.steamworks.dontDrawTitles = 0;
		//  If not connected to steam, retry
		if (  g.steamworks.isRunning  ==  0 || g.steamworks.needToResetOnStartup  ==  1 ) 
		{
			g.steamworks.goBackToEditor = 0;
			steam_resetSteam ( );
			if (  g.steamworks.isRunning  ==  0 ) 
			{
				t.tsteamlostconnectioncustommessage_s = "Cannot connect to Steam (Error MP001)";
				g.steamworks.backtoeditorforyou = 2;
				steam_lostConnection ( );
				return;
			}
		}
		else
		{
			g.steamworks.backtoeditorforyou = 0;
		}

		//  Debug Info
		t.steamDoDropShadow = 1;
		t.ttstring_s = cstr("Multiplayer Build ") + Str(t.steamworks_build);
		steam_text(-1,98,2,t.ttstring_s.Get());


	}
	else
	{
		//  030315 - 013 - Lobby chat
		t.tchatLobbyMode = 0;
		steam_chat ( );
	}
	if (  g.steamworks.mode  ==  STEAM_MODE_MAIN_MENU ) 
	{
		//  show avatar name if there is one
		if (  g.steamworks.myAvatarName_s != "" ) 
		{
		if (  ImageExist(g.charactercreatorEditorImageoffset)  ==  0 ) 
		{
			t.tShowAvatarSprite = 1;
			characterkit_loadMyAvatarInfo ( );
		}
			t.tYPos_f = 95;
			if (  GetDisplayHeight() > 900  )  t.tYPos_f  =  92;
			steam_text(-1,t.tYPos_f,2,g.steamworks.myAvatarName_s.Get());
			if (  g.charactercreatorEditorImageoffset > 0 ) 
			{
				if (  ImageExist(g.charactercreatorEditorImageoffset)  ==  1 ) 
				{
					t.tYPos_f = GetChildWindowHeight();
					t.tYPos_f = t.tYPos_f * 0.85;
					PasteImage (  g.charactercreatorEditorImageoffset, (GetChildWindowWidth()/2)-32, t.tYPos_f,g.charactercreatorEditorImageoffset );
				}
			}
		}
		g.steamworks.dontDrawTitles = 0;
		if (  g.steamworks.originalpath  ==  "" ) 
		{
			g.steamworks.originalpath = GetDir();
			SteamSetRoot(cstr(g.fpscrootdir_s+"\\Files\\").Get());
		}
		//  110315 - 019 - remove fadeoutsprite if it exists
		if (  t.tspritetouse > 0 ) 
		{
			if (  SpriteExist(t.tspritetouse)  ==  1  )  DeleteSprite (  t.tspritetouse );
			t.tspritetouse = 0;
		}
		/*      
		if (  SpaceKey() && oldspacekey  ==  0 ) 
		{
			SteamSetRoot(g.fpscrootdir_s+"\\Files\\");
			SteamCreateWorkshopItem (  "Awesome Custom Level" );
		}
		if (  LeftKey() && oldleftkey  ==  0 ) 
		{
//steam_save_workshop_files_needed ( );
			SteamDownloadWorkshopItem (  "378579107" );
			Print (  "HELLO" );
		}
		if (  SteamIsWorkshopItemDownloaded()  ==  1 ) 
		{
			steam_text(-1,5,3,"WORKSHOP ITEM DOWNLOADED");
//    `if ImageExist(999) = 0

//     `load image "F:\\TGCSHARED\\fpsc-reloaded\\FPS Creator Files\\Files\\entitybank\\ravey\\fizco\\fizzie.jpg",999

//     `sprite 999,0,0,999

//    `endif

		}
		else
		{
			steam_text(-1,5,3,"WORKSHOP ITEM NOT DOWNLOADED");
		}
		oldleftkey = LeftKey();
		oldspacekey = SpaceKey();
		*/    

//   `print "======================================"

//   `print "(C) Create Lobby"

//   `print "(S) Search for Lobbies"

//   `print "======================================"


		g.steamworks.lobbyscrollbarOn = 0;
		g.steamworks.selectedLobby = 0;
		t.tjoinedLobby = 0;
		g.steamworks.lobbyoffset = 0;
		g.steamworks.lobbycount = 0;
		steam_resetGameStats ( );

		t.game.jumplevel_s="__multiplayerlevel__";
	}
	else
	{
		if (  g.charactercreatorEditorImageoffset > 0 ) 
		{
			if (  ImageExist(g.charactercreatorEditorImageoffset)  )  DeleteImage (  g.charactercreatorEditorImageoffset );
		}
	}

	if (  g.steamworks.mode  ==  STEAM_WAITING_FOR_LOBBY_CREATION ) 
	{
//   `print "======================================"

//   `print "Creating Lobby"

//   `print "======================================"

		if (  SteamIsLobbyCreated()  ==  1 ) 
		{
			g.steamworks.isLobbyCreated = 1;
			SteamGetLobbyList (  );
			g.steamworks.mode = STEAM_MODE_LOBBY;
		}
		else
		{
			g.steamworks.haveToldAboutSolo = 0;
			if (  Timer() - t.tempsteamlobbycreationtimeout > 5000 ) 
			{
				t.tsteamlostconnectioncustommessage_s = "Could not create lobby (Error MP002)";
				steam_lostConnection ( );
				return;
			}
			if (  g.steamworks.isRunning  ==  0 ) 
			{
				t.tsteamlostconnectioncustommessage_s = "Cannot connect to Steam (Error MP003)";
				g.steamworks.backtoeditorforyou = 2;
				steam_lostConnection ( );
				return;
			}
		}
	}

	if (  g.steamworks.mode  ==  STEAM_ASKING_IF_SUBSCRIBE_TO_WORKSHOP_ITEM ) 
	{
		steam_text(-1,45,3,"You do not currently have the workshop item required to");
		steam_text(-1,50,3,"join this game. Do you wish to subscribe to the workshop");
		steam_text(-1,55,3,"item so you can join a game with this level at a later time?");
		steam_text(-1,65,3,"Note: Once you have subscribed the Lobby will remain yellow until");
		steam_text(-1,70,3,"you have downloaded the whole workshop item.");
		t.tempsteamhaveaskedtosubscribeflag = 0;
	}

	if (  g.steamworks.mode  ==  STEAM_TELLING_THEY_NEED_TO_RESTART ) 
	{
		steam_text(-1,45,3,"Your version of this workshop item is outdated.");
		steam_text(-1,50,3,"To enable Steam to download the update you will need to:");
		steam_text(-1,55,3,"Exit multiplayer, then exit Game Guru completely.");
		steam_text(-1,60,3,"Then restart Game Guru and Steam will update all");
		steam_text(-1,65,3,"your subscriptions.");
	}

	if (  g.steamworks.mode  ==  STEAM_ASKING_IF_SUBSCRIBE_TO_WORKSHOP_ITEM_WAITING_FOR_RESULTS ) 
	{
		if (  t.tempsteamhaveaskedtosubscribeflag  ==  0 ) 
		{
			t.tempsteamhaveaskedtosubscribeflag = 1;
			SteamDownloadWorkshopItem (  g.steamworks.workshopidtojoin.Get() );
		}
		if (  SteamHasSubscriptionWorkshopItemFinished()  ==  0 ) 
		{
			if (  Timer() - g.steamworks.oldtime > 150 ) 
			{
				g.steamworks.oldtime = Timer();
				t.tSteamBuildingWorkshopItem_s = t.tSteamBuildingWorkshopItem_s + ".";
				if (  Len(t.tSteamBuildingWorkshopItem_s.Get()) > 5  )  t.tSteamBuildingWorkshopItem_s  =  ".";
			}
			steam_text(-1,50,3,cstr( cstr("Subscribing you") + t.tSteamBuildingWorkshopItem_s).Get() );
		}
		if (  SteamHasSubscriptionWorkshopItemFinished()  ==  1 ) 
		{
			steam_text(-1,50,3,"You are now subscribed to:");
			steam_text(-1,55,3,g.steamworks.levelnametojoin.Get());
			steam_text(-1,65,3,"Press back and wait for this level to install");
			steam_text(-1,70,3,"(the lobby will turn from yellow to white)");
			steam_text(-1,75,3,"then re-join the lobby.");
		}
		if (  SteamHasSubscriptionWorkshopItemFinished()  ==  -1 ) 
		{
			steam_text(-1,50,3,"Subscription failed");
			steam_text(-1,55,3,"Please t.try again in t.a few moments");
		}
	}

	if (  g.steamworks.mode  ==  STEAM_SERVER_CHOOSING_FPM_TO_USE ) 
	{
		steam_text(-1,5,3,"LIST OF LEVELS");
		steam_lobbyListBox ( );
	}

	if (  g.steamworks.mode  ==  STEAM_SERVER_CHOOSING_TO_MAKE_FPS_WORKSHOP ) 
	{
		steam_text(-1,30,3,"This g.level contains custom content.");
		steam_text(-1,35,3,"To share this g.level with others you will need to create t.a workshop item.");
		steam_text(-1,40,3,"(This is done automatically for you)");
		steam_text(-1,50,3,"Once your level is a workshop item other players can play your level.");
		steam_text(-1,60,3,"Do you wish to create (or update if you have share this level before)");
		steam_text(-1,65,3,"A workshop item?");
		steam_text(-1,75,3,"By submitting this item, you agree to the workshop terms of service");
	
//   `steamworks.oldtime = 0

	}

	if (  g.steamworks.mode  ==  STEAM_CREATING_WORKSHOP_ITEM ) 
	{
		if (  Timer() - g.steamworks.oldtime > 150 ) 
		{
			g.steamworks.oldtime = Timer();
			t.tSteamBuildingWorkshopItem_s = t.tSteamBuildingWorkshopItem_s + ".";
			if (  Len(t.tSteamBuildingWorkshopItem_s.Get()) > 5  )  t.tSteamBuildingWorkshopItem_s  =  ".";
		}
		t.tstring_s = t.tSteamBuildingWorkshopItem_s + "Building Workshop Item" + t.tSteamBuildingWorkshopItem_s;
		steam_text(-1,50,3,t.tstring_s.Get());
		t.tstring_s = "";
	
//  `steam_text(0,10,3, "steamworks.buildingWorkshopItemMode = " + Str(steamworks.buildingWorkshopItemMode) )

//  `steam_text(0,20,3, "steamworks.workshopid = " + steamworks.workshopid )

	
	}

	if (  g.steamworks.mode  ==  STEAM_MODE_LOBBY ) 
	{
		if (  g.steamworks.isGameHost  ==  0 ) 
		{

			if (  g.steamworks.isRunning  ==  0 ) 
			{
				t.tsteamlostconnectioncustommessage_s = "Cannot connect to Steam (Error MP004)";
				g.steamworks.backtoeditorforyou = 2;
				steam_lostConnection ( );
				return;
			}
//    `print "======================================"

//    `print "Lobby list"

//    `print "======================================"

			steam_text(-1,5,3,"LIST OF LOBBIES");
			if (  SteamIsLobbyListCreated()  ==  0 ) 
			{

				if (  g.steamworks.lobbycount  ==  0 ) 
				{
					t.tstring_s = "Building Lobby list";
					steam_text(-1,10,1,t.tstring_s.Get());
				}
				else
				{
//      `if steamworks.lobbycount = 1

//       `tstring$ = "1 lobby found"

//      `else

//       `tstring$ = Str(tsize) + " lobbies found"

//      `endif

//      `steam_text(-1,15,1,tstring$)


				}

				if (  Timer() - g.steamworks.oldtime > 3000 ) 
				{
					SteamGetLobbyList (  );
					g.steamworks.oldtime = Timer();
				}
			}
			else
			{
				if (  Timer() - g.steamworks.oldtime > 3000 ) 
				{
					SteamGetLobbyList (  );
					g.steamworks.oldtime = Timer();
				}
			}

			steam_lobbyListBox ( );

		}
		else
		{
			//  030315 - 013 - Lobby chat
			t.tchatLobbyMode = 1;
			steam_chat ( );
			steam_text(-1,85,3,"Press Enter to chat");

			t.tUserCount = SteamGetLobbyUserCount();
			if (  Timer() - t.tempsteamlobbycreationtimeout > 5000 && t.tUserCount  ==  0 ) 
			{
				t.tsteamlostconnectioncustommessage_s = "Could not create lobby (Error MP005)";
				steam_lostConnection ( );
				return;
			}
//    `print "======================================"

//    `print steamworks.playerName + "'s Lobby"

//    `print "======================================"

//    `print "(S) Start Server"

//    `print "======================================"

			if (  t.tUserCount  ==  1 ) 
			{
				t.tstring_s = "There is 1 user (you!) in this lobby";
				g.steamworks.usersInServersLobbyAtServerCreation = 1;
			}
			else
			{
				t.tstring_s = cstr("There are ") + Str(t.tUserCount) + " users in this lobby";
			}
			if (  t.tUserCount  !=  g.steamworks.usersInServersLobbyAtServerCreation ) 
			{
				g.steamworks.haveSentMyAvatar = 0;
			}
			if (  t.tUserCount > g.steamworks.usersInServersLobbyAtServerCreation ) 
			{
				g.steamworks.usersInServersLobbyAtServerCreation = t.tUserCount;
			}
			steam_text(-1,15,1,t.tstring_s.Get());
			t.tsteamy_f = 50.0 - (t.tUserCount * 2.5);
			t.tsteamy = t.tsteamy_f;
			for ( t.tn = 1 ; t.tn<=  t.tUserCount; t.tn++ )
			{
				t.tstring_s = cstr("Player ") + Str(t.tn) + ": " + SteamGetLobbyUserName(t.tn-1);
				if (  SteamGetPlayerName()  !=  SteamGetLobbyUserName(t.tn-1)  )  t.steamworks_joined[t.tn-1]  =  SteamGetLobbyUserName(t.tn-1);
				steam_text(-1,t.tsteamy,1,t.tstring_s.Get());
				t.tsteamy += 5;
			}
			for ( t.tn = t.tUserCount ; t.tn<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.tn++ )
			{
				t.steamworks_joined[t.tn] = "";
			}
				if (  g.steamworks.haveToldAboutSolo  ==  1 && t.tUserCount  <=  1 ) 
				{
					steam_textColor(-1,70,1,"Noone has joined your lobby yet. If you start now you will be playing alone.",255,100,100);
					steam_textColor(-1,75,1,"Press Start Server again to start anyway.",255,100,100);
				}

		if (  g.steamworks.launchServer  ==  1 ) 
		{
					if (  g.steamworks.haveToldAboutSolo  ==  0 && t.tUserCount  <=  1 ) 
					{
						g.steamworks.haveToldAboutSolo = 1;
						g.steamworks.launchServer = 0;
						return;
					}
					SteamStartServer (  );
				g.steamworks.mode = STEAM_WAITING_FOR_SERVER_CREATION;
				g.steamworks.oldtime = Timer();
			}
		}
	}

	if (  g.steamworks.mode  ==  STEAM_JOINING_LOBBY ) 
	{
		if (  SteamIsGameRunning()  ==  1 ) 
		{
			g.steamworks.mode = STEAM_IN_GAME_CLIENT;
			g.steamworks.needToResetOnStartup = 1;
			t.toldsteamfolder_s=GetDir();
			SetDir (  cstr(g.fpscrootdir_s + "\\Files\\editors\\gridedit").Get() );
			t.tsteamtimeoutongamerunning = Timer();
			t.tPlayerIndex = SteamGetMyPlayerIndex();
			if (  t.tPlayerIndex  >=  0 && t.tPlayerIndex < STEAM_MAX_NUMBER_OF_PLAYERS ) 
			{
				t.steamworks_health[t.tPlayerIndex] = 0;
				t.ta = MouseMoveX() + MouseMoveY();
			}
		}
		if (  t.tjoinedLobby  ==  0 ) 
		{
			if (  Timer() - g.steamworks.AttemptedToJoinLobbyTime > STEAM_JOIN_LOBBY_TIMEOUT ) 
			{
				g.steamworks.mode = STEAM_MODE_MAIN_MENU;
				t.tmsg_s = "Could not join Lobby";
				steam_setMessage ( );
			}
		}
		if (  SteamHasJoinedLobby()  ==  1 ) 
		{
			t.tjoinedLobby = 1;
			if (  t.tjoinedLobby  ==  0 ) 
			{
					t.tsteamwaitedforlobbytimer = Timer();
			}
		}
		else
		{
			t.tsteamwaitedforlobbytimer = Timer();
			t.tsteamistheownerpresenttime = t.tsteamwaitedforlobbytimer;
		}
		if (  t.tjoinedLobby  ==  0 ) 
		{
				t.tsteamwaitedforlobbytimer = Timer();
				t.tsteamistheownerpresenttime = Timer();
		}
		if (  t.tjoinedLobby  ==  1 ) 
		{
			if (  SteamHasJoinedLobby()  ==  1 ) 
			{
					//  030315 - 013 - Lobby chat
					t.tchatLobbyMode = 1;
					steam_chat ( );
					steam_text(-1,85,3,"Press Enter to chat");
					t.tsteamlobbertimer = Timer();
//     `print "======================================"

//     `print "In lobby"

//     `print "======================================"

				if (  t.tUserCount  !=  SteamGetLobbyUserCount() ) 
				{
					g.steamworks.haveSentMyAvatar = 0;
				}
				t.tUserCount = SteamGetLobbyUserCount();
				if (  t.tUserCount  ==  1 && Timer() - t.tsteamwaitedforlobbytimer > 15000 ) 
				{
					if (  SteamIsGameRunning()  ==  0 ) 
					{
						SteamLeaveLobby (  );
						t.tsteamlostconnectioncustommessage_s = "Lost connection to lobby (Error MP006)";
						steam_lostConnection ( );
						return;
					}
				}
				else
				{
					t.tsteamwaitedforlobbytimer = Timer();
					steam_text(-1,15,1, cstr(cstr("There are ") + Str(t.tUserCount) + " users in this lobby").Get() );
					steam_text(-1,10,1, cstr(cstr("Game being hosted is '") + g.steamworks.levelnametojoin + "'").Get() );

				}
				t.tsteamistheownerpresent = 0;
				t.tsteamnamewearelookingfor_s = Left(g.steamworks.lobbyjoinedname.Get(),Len(g.steamworks.lobbyjoinedname.Get())-8);

				t.tsteamy_f = 50.0 - (t.tUserCount * 2.5);
				t.tsteamy = t.tsteamy_f;

				if (  t.tsteamnamewearelookingfor_s  ==  SteamGetLobbyUserName(0) ) 
				{
					for ( t.tn = t.tUserCount ; t.tn<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.tn++ )
					{
						t.steamworks_joined[t.tn] = "";
					}
					for ( t.tn = 1 ; t.tn<=  t.tUserCount; t.tn++ )
					{
						if (  SteamGetPlayerName()  !=  SteamGetLobbyUserName(t.tn-1)  )  t.steamworks_joined[t.tn-1]  =  SteamGetLobbyUserName(t.tn-1);
					}
				}
				for ( t.tn = 1 ; t.tn<=  t.tUserCount; t.tn++ )
				{
					if (  t.tn  ==  1 ) 
					{
						t.tstring_s = cstr("Player ") + Str(t.tn) + ": " + SteamGetLobbyUserName(t.tn-1) + " (Host)";
					}
					else
					{
						t.tstring_s = cstr("Player ") + Str(t.tn) + ": " + SteamGetLobbyUserName(t.tn-1);
					}
					steam_text(-1,t.tsteamy,1,t.tstring_s.Get());
					t.tsteamy += 5;
					if (  t.tsteamnamewearelookingfor_s  ==  SteamGetLobbyUserName(t.tn-1) ) 
					{
						t.tsteamistheownerpresent = 1;
						t.tsteamistheownerpresenttime = Timer();
					}
				}
				if (  t.tsteamistheownerpresent  ==  0 && Timer() - t.tsteamistheownerpresenttime > 10000 ) 
				{
					SteamLeaveLobby (  );
					t.tsteamlostconnectioncustommessage_s = "The host left the lobby (Code MP007)";
					steam_lostConnection ( );
					return;
				}

				t.tsteamlobbertimer = Timer();
			}
			else
			{
				steam_textDots(-1,20,3,"Game Starting...Connecting");
				if (  Timer() - t.tsteamlobbertimer > 20000 ) 
				{
					if (  SteamGetClientServerConnectionStatus()  ==  0 ) 
					{
						t.tsteamlostconnectioncustommessage_s = "Lost connection to host (Error MP008)";
						steam_lostConnection ( );
						return;
					}
				}
			}
		}
		else
		{
//    `print "======================================"

//    `print "Joining lobby"

//    `print "======================================"

		}
	}
	if (  g.steamworks.mode  ==  STEAM_WAITING_FOR_SERVER_CREATION ) 
	{
		g.steamworks.dontDrawTitles = 1;
		if (  SteamIsServerRunning()  ==  1 ) 
		{
//    `print "======================================"

//    `print "Server started"

//    `print "======================================"

			steam_textDots(-1,10,3,"Server Started");
			if (  SteamIsGameRunning()  ==  1 ) 
			{
				g.steamworks.mode = STEAM_IN_GAME_SERVER;
				g.steamworks.needToResetOnStartup = 1;
				t.toldsteamfolder_s=GetDir();
				SetDir (  cstr(g.fpscrootdir_s + "\\Files\\editors\\gridedit").Get() );
				t.tPlayerIndex = SteamGetMyPlayerIndex();
				SteamSetSendFileCount (  1 );
				if (  t.tPlayerIndex  >=  0 && t.tPlayerIndex < STEAM_MAX_NUMBER_OF_PLAYERS ) 
				{
//      `position camera ObjectPositionX(entityelement(steamworks_playerEntityID(tPlayerIndex)).obj),ObjectPositionY(entityelement(steamworks_playerEntityID(tPlayerIndex)).obj)+50,ObjectPositionZ(entityelement(steamworks_playerEntityID(tPlayerIndex)).obj)

//      `rotate camera 0,0,0

//      `a = MouseMoveX() + MouseMoveY()

//      `hide object entityelement(steamworks_playerEntityID(tPlayerIndex)).obj

				}
			}
			else
			{
				if (  Timer() - g.steamworks.oldtime > 150 ) 
				{
					g.steamworks.oldtime = Timer();
					t.tStartingServerCount_s = t.tStartingServerCount_s + ".";
					if (  Len(t.tStartingServerCount_s.Get()) > 5  )  t.tStartingServerCount_s  =  ".";
				}
				t.tstring_s = t.tStartingServerCount_s + "Waiting for game to start" + t.tStartingServerCount_s;
				steam_text(-1,25,3,t.tstring_s.Get());
				t.tstring_s = "";
			}
		}
		else
		{
			//  some .... on screen to show progress being made making server
//    `print "======================================"

			if (  Timer() - g.steamworks.oldtime > 150 ) 
			{
				g.steamworks.oldtime = Timer();
				t.tStartingServerCount_s = t.tStartingServerCount_s + ".";
				if (  Len(t.tStartingServerCount_s.Get()) > 5  )  t.tStartingServerCount_s  =  ".";
			}
			t.tstring_s = t.tStartingServerCount_s + "Starting server" + t.tStartingServerCount_s;
			steam_text(-1,15,3,t.tstring_s.Get());
			t.tstring_s = "";
//    `print "======================================"

		}
	}

	if (  g.steamworks.mode  ==  STEAM_IN_GAME_SERVER ) 
	{
			g.steamworks.dontDrawTitles = 1;
//    `print "======================================"

//    `print "GAME RUNNING - You are the server"

//    `print "Player number " + Str(SteamGetMyPlayerIndex())

//    `print "======================================"


			if (  g.steamworks.iHaveSaidIAmReady  ==  0 ) 
			{
				SteamSendIAmLoadedAndReady (  );
				g.steamworks.iHaveSaidIAmReady = 1;
				t.tempsteamingameinitialwaitingdelay = Timer();
//     `sync

				while (  Timer() - t.tempsteamingameinitialwaitingdelay < 20000 ) 
				{
					g.steamworks.syncedWithServerMode = 0;
					g.steamworks.okayToLoadLevel = 0;
					SteamLoop (  );
					steam_textDots(-1,20,3,"Waiting for other players");
//      `sync

					if (  Timer() - t.tsteamiseveryoneloadedandreadytime > 1000 ) 
					{
						t.tsteamiseveryoneloadedandreadytime = Timer();
						if (  SteamIsEveryoneLoadedAndReady()  ==  1  )  t.tempsteamingameinitialwaitingdelay  =  -30000;
					}
				}
				t.tskipLevelSync = Timer();
//     `tmsg$ = "Game is starting!"

				/*      
				tdelay = Timer();
				while (  Timer() - tdelay < 4000 ) 
				{
					SteamLoop (  );
					SetCursor (  0,300 );
					Print (  "Preparing to load" );
					Sync (  );
				}
				*/    
			}

		//wait for everyone before starting to load, at this GetPoint (  they have all the files they need, they just have not loaded them )
			if (  g.steamworks.okayToLoadLevel  ==  0 && g.steamworks.syncedWithServerMode  ==  99 ) 
			{
//     `if SteamIsEveryoneLoadedAndReady() = 1

					t.game.titleloop=0;
					t.game.levelloop=1;
					t.game.runasmultiplayer=1;
					t.game.levelloadprogress=0;
					t.game.cancelmultiplayer=0;
					t.game.quitflag=0;
					t.tescapepress=0 ; t.ttitlesbuttonhighlight=0;

					g.steamworks.playGame = 1;
					g.steamworks.okayToLoadLevel = 1;
					t.tskipLevelSync = Timer();
//     `endif

			}
			else
			{
				if (  g.steamworks.playGame  ==  1 ) 
				{
					if (  t.game.titleloop == 1 ) 
					{
						t.game.titleloop=0;
						t.game.levelloop=1;
						t.game.runasmultiplayer=1;
						t.game.levelloadprogress=0;
						t.game.cancelmultiplayer=0;
						t.game.quitflag=0;
						t.tescapepress=0 ; t.ttitlesbuttonhighlight=0;
					}
				}
				if (  g.steamworks.okayToLoadLevel  ==  0 ) 
				{
					steam_pre_game_file_sync_server ( );
				}
			}

	}
	if (  g.steamworks.mode  ==  STEAM_IN_GAME_CLIENT ) 
	{
			if (  t.titlespage  ==  11 ) 
			{
				g.steamworks.dontDrawTitles = 0;
			}
			else
			{
				g.steamworks.dontDrawTitles = 1;
			}
			g.steamworks.dontDrawTitles = 1;
//    `print "======================================"

//    `print "GAME RUNNING - You are a client"

//    `print "Player number " + Str(SteamGetMyPlayerIndex())

//    `print "======================================"


			if (  g.steamworks.iHaveSaidIAmReady  ==  0 ) 
			{
				SteamSendIAmLoadedAndReady (  );
				g.steamworks.iHaveSaidIAmReady = 1;
				t.tempsteamingameinitialwaitingdelay = Timer();
//     `sync

				while (  Timer() - t.tempsteamingameinitialwaitingdelay < 20000 ) 
				{
					g.steamworks.syncedWithServerMode = 0;
					g.steamworks.okayToLoadLevel = 0;
//      `tmsg$ = "Waiting for other players"

//steam_setMessage ( );
					steam_textDots(-1,50,3,"Waiting for other players");
					SteamLoop (  );
					if (  Timer() - t.tsteamtimeoutongamerunning > 16000 ) 
					{
						if (  SteamGetClientServerConnectionStatus()  ==  0 ) 
						{
							t.tsteamlostconnectioncustommessage_s = "Lost connection to host (Error MP009)";
							steam_lostConnection ( );
							return;
						}
					}
					t.tskipLevelSync = Timer();
//      `sync

//      `if Timer() - tsteamiseveryoneloadedandreadytime > 1000

//       `tsteamiseveryoneloadedandreadytime = Timer()

						if (  SteamIsEveryoneLoadedAndReady()  ==  1  )  t.tempsteamingameinitialwaitingdelay  =  -30000;
//      `endif

				}
				/*      
				tdelay = Timer();
				while (  Timer() - tdelay < 4000 ) 
				{
					SteamLoop (  );
					SetCursor (  0,300 );
					Print (  "Preparing to load" );
					Sync (  );
				}
				*/    
			}

		//wait for everyone before starting to load, at this GetPoint (  they have all the files they need, they just have not loaded them )
			if (  g.steamworks.okayToLoadLevel  ==  0 && g.steamworks.syncedWithServerMode  ==  99 ) 
			{
//     `steam_livelog("Setting okay to play now")

//     `if SteamIsEveryoneLoadedAndReady() = 1

					t.game.titleloop=0;
					t.game.levelloop=1;
					t.game.runasmultiplayer=1;
					t.game.levelloadprogress=0;
					t.game.cancelmultiplayer=0;
					t.game.quitflag=0;
					t.tescapepress=0 ; t.ttitlesbuttonhighlight=0;

					g.steamworks.playGame = 1;
					g.steamworks.okayToLoadLevel = 1;
					t.tskipLevelSync = Timer();
//     `endif

			}
			else
			{
				if (  g.steamworks.playGame  ==  1 ) 
				{
					if (  t.game.titleloop == 1 ) 
					{
						t.game.titleloop=0;
						t.game.levelloop=1;
						t.game.runasmultiplayer=1;
						t.game.levelloadprogress=0;
						t.game.cancelmultiplayer=0;
						t.game.quitflag=0;
						t.tescapepress=0 ; t.ttitlesbuttonhighlight=0;
					}
				}
				if (  g.steamworks.okayToLoadLevel  ==  0 ) 
				{
					steam_pre_game_file_sync_client ( );
				}
			}

	}

	steam_message ( );
	steam_messageDots ( );


return;

//  Shutdown Steam
}

void steam_free ( void )
{

	SteamFree (  );

return;

//  Check if voicechat is enabled
}

void steam_checkVoiceChat ( void )
{
}

void steam_spawn_objects ( void )
{

	//  Grab the list of spawned objects from the server
	//  TO DO - find out how entities are spawned in FPSC and call those routines
	//  LEE - AGREED, no need to repeat code but we can do that during clean-up ;)
	while (  SteamGetSpawnList() ) 
	{
		t.obj = SteamGetSpawnObjectNumber();
		t.sourceobj = SteamGetSpawnObjectSource();
		t.x_f = SteamGetSpawnX();
		t.y_f = SteamGetSpawnY();
		t.z_f = SteamGetSpawnZ();
		InstanceObject (  t.obj, t.sourceobj );
		//  restore any radius settings the original object might have had
		SetSphereRadius (  t.obj,-1 );
		ShowObject (  t.obj );
		PositionObject (  t.obj, t.x_f, t.y_f, t.z_f );
		SteamGetNextSpawn (  );
	}

return;

}

void steam_lua ( void )
{

	while (  SteamGetLuaList() ) 
	{
		t.steamLuaCode = SteamGetLuaCommand();
		t.e = SteamGetLuaE();
		t.v = SteamGetLuaV();
	
		t.tLuaDontSendLua = 1;
	
		switch (  t.steamLuaCode ) 
		{
		case STEAM_LUA_SetActivated:
			if ( steam_check_if_lua_entity_exists(t.e) == 1 ) entity_lua_setactivated() ; ++t.activatedCount;
		break;
		case STEAM_LUA_SetAnimation:
			entity_lua_setanimation() ; ++t.animCount;
		break;
		case STEAM_LUA_PlayAnimation:
			if ( steam_check_if_lua_entity_exists(t.e) == 1 ) entity_lua_playanimation() ; ++t.playanimCount;
		break;
		case STEAM_LUA_ActivateIfUsed:
			if ( steam_check_if_lua_entity_exists(t.e) == 1 ) entity_lua_activateifused() ; ++t.activateCount;
		break;
		case STEAM_LUA_PlaySound:
			entity_lua_playsound ( );
		break;
		case STEAM_LUA_StartTimer:
			entity_lua_starttimer ( );
		break;
		case STEAM_LUA_CollisionOff:
			entity_lua_collisionoff ( );
		break;
		case STEAM_LUA_CollisionOn:
			entity_lua_collisionon ( );
		break;
		case STEAM_LUA_ServerSetLuaGameMode:
			LuaSetInt (  "mp_gameMode",t.v );
		break;
		case STEAM_LUA_ServerSetPlayerKills:
			t.tnothing = LuaExecute( cstr(cstr("mp_playerKills[") + Str(t.e) + "] = " + Str(t.v)).Get() );
		break;
		case STEAM_LUA_ServerSetPlayerDeaths:
			t.tnothing = LuaExecute( cstr(cstr("mp_playerDeaths[") + Str(t.e) + "] = " + Str(t.v)).Get() );
		break;
		case STEAM_LUA_ServerSetPlayerAddKill:
			t.steamworks_kills[t.v] = t.steamworks_kills[t.v] + 1;
			SteamSendLua (  STEAM_LUA_ServerSetPlayerKills,t.v,t.steamworks_kills[t.v] );
			t.tnothing = LuaExecute( cstr(cstr("mp_playerKills[") + Str(t.v) + "] = " + Str(t.steamworks_kills[t.v])).Get() );
		break;
		case STEAM_LUA_ServerSetPlayerRemoveKill:
			//  check if they already have the kills needed to win
			//  because they may kill someone else first, then themselves, which has already triggered a win
			//  so we only remove a kill if they havent yet won
			if (  g.steamworks.setserverkillstowin  <= 0  )  g.steamworks.setserverkillstowin  =  100;
			if (  t.steamworks_kills[t.v] < g.steamworks.setserverkillstowin ) 
			{
				t.steamworks_kills[t.v] = t.steamworks_kills[t.v] - 1;
				SteamSendLua (  STEAM_LUA_ServerSetPlayerKills,t.v,t.steamworks_kills[t.v] );
				t.tnothing = LuaExecute( cstr(cstr("mp_playerKills[") + Str(t.v) + "] = " + Str(t.steamworks_kills[t.v])).Get() );
			}
		break;
		case STEAM_LUA_ServerSetPlayerAddDeath:
			t.steamworks_deaths[t.v] = t.steamworks_deaths[t.v] + 1;
			SteamSendLua (  STEAM_LUA_ServerSetPlayerDeaths,t.v,t.steamworks_deaths[t.v] );
			t.tnothing = LuaExecute( cstr(cstr("mp_playerDeaths[") + Str(t.v) + "] = " + Str(t.steamworks_deaths[t.v])).Get() );
		break;
		case STEAM_LUA_SetServerTimer:
			t.tnothing = LuaExecute( cstr(cstr("mp_servertimer = ") + Str(t.v)).Get() );
		break;
		case STEAM_LUA_ServerRespawnAll:
			steam_restoreEntities ( );
			steam_setLuaResetStats ( );
			steam_respawnEntities ( );
			t.playercontrol.jetpackhidden=0;
			t.playercontrol.jetpackmode=0;
			physics_no_gun_zoom ( );
			t.aisystem.processplayerlogic=1;
			g.steamworks.gameAlreadySpawnedBefore = 0;
			t.steamworks_playerHasSpawned[g.steamworks.me] = 0;
			if (  g.steamworks.myOriginalSpawnPoint  !=  -1 ) 
			{
				t.tindex = g.steamworks.myOriginalSpawnPoint;
			}
			else
			{
				t.tindex = 1;
			}
			if (  t.steamworksmultiplayerstart[t.tindex].active == 1 ) 
			{
				t.terrain.playerx_f=t.steamworksmultiplayerstart[t.tindex].x;
				t.terrain.playery_f=t.steamworksmultiplayerstart[t.tindex].y;
				t.terrain.playerz_f=t.steamworksmultiplayerstart[t.tindex].z;
				t.terrain.playerax_f=0;
				t.terrain.playeray_f=t.steamworksmultiplayerstart[t.tindex].angle;
				t.terrain.playeraz_f=0;

				g.steamworks.lastx=t.terrain.playerx_f;
				g.steamworks.lasty=t.terrain.playery_f;
				g.steamworks.lastz=t.terrain.playerz_f;
				g.steamworks.lastangley=t.terrain.playeray_f;

				t.tobj = t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj;
				if (  t.tobj > 0 ) 
				{
					PositionObject (  t.tobj, t.terrain.playerx_f, t.terrain.playery_f-50, t.terrain.playerz_f );
					RotateObject (  t.tobj, t.terrain.playerax_f, t.terrain.playeray_f, t.terrain.playeraz_f );
				}
			}

			g.autoloadgun=0  ; gun_change ( );
			g.steamworks.endplay = 0;
			t.player[t.plrid].health = 0;
			t.steamworks_health[g.steamworks.me] = 0;
			physics_resetplayer_core ( );
		break;
		case STEAM_LUA_ServerEndPlay:
				t.playercontrol.jetpackhidden=0;
				t.playercontrol.jetpackmode=0;
				physics_no_gun_zoom ( );
				t.aisystem.processplayerlogic=0;
				g.steamworks.endplay = 1;
				g.autoloadgun=0 ; gun_change ( );
		break;
		case STEAM_LUA_AiGoToX:
			t.tSteamX_f = t.v;
		break;
		case STEAM_LUA_AiGoToZ:
			t.tSteamZ_f = t.v;
			if (  t.e > 0 ) 
			{
				if (  ObjectExist(t.e)  ==  1 ) 
				{
					AISetEntityActive (  t.e,1 );
					steam_COOP_aiMoveTo ( );
				}
			}
			for ( t.tee = 1 ; t.tee<=  g.entityelementlist; t.tee++ )
			{
				if (  t.entityelement[t.tee].obj  ==  t.e ) 
				{
					t.entityelement[t.tee].mp_updateOn = 1;
					t.entityelement[t.tee].active = 1;
//      `print "updating object " + Str(tee)

					break;
				}
			}
		break;
		case Steam_LUA_setcharactertowalkrun:
			entity_lua_setcharactertowalkrun ( );
			if (  t.entityelement[t.e].obj > 0 ) 
			{
				if (  ObjectExist(t.entityelement[t.e].obj)  ==  1 ) 
				{
					t.entityelement[t.e].mp_updateOn = 1;
					t.entityelement[t.e].active = 1;
				}
			}
		break;
		case Steam_LUA_CharacterControlManual:
			entity_lua_charactercontrolmanual ( );
			t.entityelement[t.e].mp_updateOn = 1;
			t.entityelement[t.e].active = 1;
		break;
		case Steam_LUA_CharacterControlLimbo:
			entity_lua_charactercontrollimbo ( );
			t.entityelement[t.e].mp_updateOn = 1;
			t.entityelement[t.e].active = 1;
		break;
		case Steam_LUA_CharacterControlArmed:
			entity_lua_charactercontrolarmed ( );
			t.entityelement[t.e].mp_updateOn = 1;
			t.entityelement[t.e].active = 1;
			AISetEntityActive (  t.entityelement[t.e].obj,1 );
		break;
		case Steam_LUA_CharacterControlUnarmed:
			entity_lua_charactercontrolunarmed ( );
			t.entityelement[t.e].mp_updateOn = 1;
			t.entityelement[t.e].active = 1;
		break;
		case Steam_LUA_LookAtPlayer:
			if (  t.entityelement[t.e].obj > 0 ) 
			{
				if (  ObjectExist(t.entityelement[t.e].obj)  ==  1 ) 
				{
					AISetEntityActive (  t.entityelement[t.e].obj,1 );
					steam_entity_lua_lookatplayer ( );
					t.entityelement[t.e].mp_updateOn = 1;
					t.entityelement[t.e].active = 1;
					t.entityelement[t.e].mp_rotateTimer = Timer();
					t.entityelement[t.e].mp_rotateType = 1;
				}
			}
		break;
		case Steam_LUA_TakenAggro:
			if (  t.entityelement[t.e].obj > 0 ) 
			{
				AISetEntityActive (  t.entityelement[t.e].obj,1 );
				if (  ObjectExist(t.entityelement[t.e].obj)  ==  1 ) 
				{
//      `if steamworks.me  ==  0 then SteamSendLua (  Steam_LUA_TakenAggro,e,v )

					t.entityelement[t.e].mp_coopControlledByPlayer = t.v;
					t.entityelement[t.e].mp_coopLastTimeSwitchedTarget = Timer();
//      `if v  ==  steamworks.me then inc entityelement(e).mp_coopLastTimeSwitchedTarget,5000
					
					t.entityelement[t.e].mp_updateOn = 1;
					t.entityelement[t.e].active = 1;
					//  set them to run - not totally ideal for zombies (some walk) but okay for now
					t.v = 1;
					entity_lua_setcharactertowalkrun ( );
//      `AI Entity Stop entityelement(e).obj

				}
			}
		break;
		case Steam_LUA_HaveAggro:
			if (  t.entityelement[t.e].obj > 0 ) 
			{
				AISetEntityActive (  t.entityelement[t.e].obj,1 );
				if (  ObjectExist(t.entityelement[t.e].obj)  ==  1 ) 
				{
//      `if steamworks.me  ==  0 then SteamSendLua (  Steam_LUA_TakenAggro,e,v )
					
					t.entityelement[t.e].mp_coopControlledByPlayer = t.v;
					t.entityelement[t.e].mp_updateOn = 1;
					t.entityelement[t.e].active = 1;
					//  set them to run - not totally ideal for zombies (some walk) but okay for now
					t.v = 1;
					entity_lua_setcharactertowalkrun ( );
//      `AI Entity Stop entityelement(e).obj

				}
			}
		break;
		case Steam_LUA_FireWeaponEffectOnly:
			if (  t.entityelement[t.e].obj > 0 ) 
			{
				if (  ObjectExist(t.entityelement[t.e].obj)  ==  1 ) 
				{
					steam_entity_lua_fireweaponEffectOnly ( );
					t.entityelement[t.e].mp_updateOn = 1;
					t.entityelement[t.e].active = 1;
					AISetEntityActive (  t.entityelement[t.e].obj,1 );
				}
			}
		break;
		case Steam_LUA_RotateToPlayer:
			if (  t.entityelement[t.e].obj > 0 ) 
			{
				if (  ObjectExist(t.entityelement[t.e].obj)  ==  1 ) 
				{
					steam_coop_rotatetoplayer ( );
					t.entityelement[t.e].mp_updateOn = 1;
					t.entityelement[t.e].active = 1;
					t.entityelement[t.e].mp_rotateTimer = Timer();
					t.entityelement[t.e].mp_rotateType = 2;
					AISetEntityActive (  t.entityelement[t.e].obj,1 );
				}
			}
		break;
		case Steam_LUA_SetAnimationFrames:
			entity_lua_setanimationframes ( );
		break;
		case Steam_LUA_AISetEntityControl:
			AISetEntityControl (  t.e,t.v );
		break;
		case Steam_LUA_AIMoveX:
			t.tsteamPosX = t.v;
		break;
		case Steam_LUA_AIMoveZ:
			AISetEntityPosition (  t.e, t.tsteamPosX, BT_GetGroundHeight(t.terrain.TerrainID,t.tsteamPosX,t.v),t.v );
		break;
		case Steam_LUA_SendAvatar:
			t.tsteams_s = SteamGetLuaS();
			t.steamworks_playerAvatars_s[t.e] = t.tsteams_s;
		break;
		case Steam_LUA_SendAvatarName:
			t.tsteams_s = SteamGetLuaS();
			t.steamworks_playerAvatarOwners_s[t.e] = t.tsteams_s;
		break;
		}	//~   
	
		t.tLuaDontSendLua = 0;
	
		SteamGetNextLua (  );
	}

return;

}

void steam_delete_entities ( void )
{

	g.steamworks.ignoreDamageToEntity = 1;
	while (  SteamGetDeleteList() ) 
	{
		t.ttte = SteamGetDeleteObjectNumber();
		if (  t.ttte  <=  g.entityelementlist ) 
		{
			t.tobj = t.entityelement[t.ttte].obj;
			if (  t.tobj > 0 ) 
			{
				if (  ObjectExist(t.tobj)  ==  1 ) 
				{
					t.tdamage = t.entityelement[t.ttte].health;
					t.tdamageforce = 0;
					t.tdamagesource = 0;
					t.brayx1_f = ObjectPositionX(t.tobj);
					t.brayy1_f = ObjectPositionY(t.tobj);
					t.brayz1_f = ObjectPositionZ(t.tobj);
					t.brayx2_f = ObjectPositionX(t.tobj);
					t.brayy2_f = ObjectPositionY(t.tobj);
					t.brayz2_f = ObjectPositionZ(t.tobj);
	
					t.entityelement[t.ttte].mp_networkkill = 1;
					t.entityelement[t.ttte].mp_killedby = SteamGetDeleteSource();
					t.entityelement[t.ttte].health = 0;
	
					entity_applydamage ( );
				}
			}
		}
	
		SteamGetNextDelete (  );
	}
	g.steamworks.ignoreDamageToEntity = 0;

	while (  SteamGetDestroyList() ) 
	{
		t.ttte = SteamGetDestroyObjectNumber();
		if (  t.ttte  <=  g.entityelementlist ) 
		{
			t.tobj = t.entityelement[t.ttte].obj;
			if (  t.tobj > 0 ) 
			{
				if (  ObjectExist(t.tobj)  ==  1 ) 
				{
					t.entityelement[t.ttte].destroyme=1;
				}
			}
		}
		SteamGetNextDestroy (  );
	}


return;

}

void steam_pre_game_file_sync ( void )
{

	if (  g.steamworks.isGameHost  ==  1 ) 
	{
		steam_pre_game_file_sync_server ( );
	}
	else
	{
		steam_pre_game_file_sync_client ( );
	}

return;

}

void steam_pre_game_file_sync_server ( void )
{

//  if we have lost connection, head back to main menu
t.tconnectionStatus = SteamGetClientServerConnectionStatus();
if (  t.tconnectionStatus  ==  0 ) 
{
	t.tsteamconnectionlostmessage_s = "Lost Connection";
	steam_lostConnection ( );
	return;
}

steam_sendAvatarInfo ( );
//  check if we have finished sending and receiving textures with the server
//  (the actual process is handled by steam dll)
if (  g.steamworks.isGameHost  ==  0 || g.steamworks.me  !=  0  )  return;
if (  SteamCheckSyncedAvatarTexturesWithServer()  ==  0 ) 
{
	t.tstring_s = "Syncing Avatars";
	steam_textDots(-1,50,3,t.tstring_s.Get());
	return;
}

	switch (  g.steamworks.syncedWithServerMode ) 
	{

		case 0:
			//  for solo testing to prevent sending files
			if (  g.steamworks.usersInServersLobbyAtServerCreation  ==  1 ) 
			{
				g.steamworks.syncedWithServerMode = 3;
				return;
			}
	
			g.steamworks.serverusingworkshop = 0;
	
			SteamSetSendFileCount (  1 );
			if (  g.steamworks.levelContainsCustomContent  ==  0 ) 
			{
				SteamSendFileBegin (  1,"__multiplayerlevel__.fpm" );
				g.steamworks.serverusingworkshop = 1;
			}
			else
			{
				t.tempsteamfiletosend_s = g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerworkshopitemid__.dat";
				if (  FileExist (t.tempsteamfiletosend_s.Get())  ==  1  )  DeleteAFile (  t.tempsteamfiletosend_s.Get() );
				if (  FileOpen(1)  )  CloseFile (  1 );
				OpenToWrite (  1,t.tempsteamfiletosend_s.Get() );
				WriteString (  1,g.steamworks.workshopid.Get() );
				CloseFile (  1 );
				SteamSendFileBegin (  1,"__multiplayerworkshopitemid__.dat" );
			}
			g.steamworks.syncedWithServerMode = 1;
			steam_textDots(-1,30,3,"Setting up data for clients")  ;    

		break;

		case 1:
			if (  SteamSendFileDone()  ==  1 ) 
			{
					g.steamworks.syncedWithServerMode = 2;
			}
				steam_textDots(-1,50,3,"Sending data to clients");
		break;

		case 2:
			steam_textDots(-1,30,3,"Waiting for clients to receive data");

			if (  SteamIsEveryoneFileSynced()  ==  1 ) 
			{
			//the client hosting the server needs to have loaded everything in also
				SteamSendIAmLoadedAndReady (  );
				g.steamworks.syncedWithServerMode = 3;
				g.steamworks.oldtime = Timer();
			}
		break;

	case 3:
		if (  SteamIsEveryoneLoadedAndReady()  ==  1 ) 
		{
			if (  g.steamworks.serverusingworkshop  ==  1 ) 
			{
					steam_textDots(-1,30,3,"Waiting for clients to receive data");
					if (  Timer() - g.steamworks.oldtime > 3000 ) 
					{
						g.steamworks.oldtime = Timer();
						g.steamworks.syncedWithServer = 1;
						g.steamworks.syncedWithServerMode = 99;
						SetDir (  t.toldsteamfolder_s.Get() );
						SetDir (  g.steamworks.originalpath.Get() );
					}
				}
				else
				{
					g.steamworks.oldtime = Timer();
					g.steamworks.syncedWithServer = 1;
					g.steamworks.syncedWithServerMode = 99;
					SetDir (  t.toldsteamfolder_s.Get() );
					SetDir (  g.steamworks.originalpath.Get() );
				}
			}
			else
			{
				if (  Timer() - g.steamworks.oldtime > 150 ) 
				{
					g.steamworks.oldtime = Timer();
					t.tSteamBuildingWorkshopItem_s = t.tSteamBuildingWorkshopItem_s + ".";
					if (  Len(t.tSteamBuildingWorkshopItem_s.Get()) > 5  )  t.tSteamBuildingWorkshopItem_s  =  ".";
				}
				if (  Timer() - t.tempsteamworkssendingready > 2000 ) 
				{
					SteamSendIAmLoadedAndReady (  );
					t.tempsteamworkssendingready = Timer();
				}
				t.tstring_s = t.tSteamBuildingWorkshopItem_s + "Waiting for everyone to be ready" + t.tSteamBuildingWorkshopItem_s;
				steam_text(-1,50,3,t.tstring_s.Get());
				t.tstring_s = "";
			}
	break;
	}//~ ` 

return;

}

void steam_pre_game_file_sync_client ( void )
{

//  if we have lost connection, head back to main menu
t.tconnectionStatus = SteamGetClientServerConnectionStatus();
if (  t.tconnectionStatus  ==  0 ) 
{
	t.tsteamconnectionlostmessage_s = "Lost Connection";
	steam_lostConnection ( );
	return;
}

steam_sendAvatarInfo ( );
//  check if we have finished sending and receiving textures with the server
//  (the actual process is handled by steam dll)
if (  g.steamworks.isGameHost  ==  1 || g.steamworks.me  ==  0  )  return;
if (  SteamCheckSyncedAvatarTexturesWithServer()  ==  0 ) 
{
	t.tstring_s = "Syncing Avatars";
	steam_textDots(-1,50,3,t.tstring_s.Get());
	return;
}

if (  SteamGetClientServerConnectionStatus()  ==  0 ) 
{
	t.tsteamlostconnectioncustommessage_s = "Lost connect to server (Error MP010)";
	g.steamworks.backtoeditorforyou = 0;
		g.steamworks.mode = 0;
	steam_lostConnection ( );
	return;
}

switch (  g.steamworks.syncedWithServerMode ) 
{
	case 0:
			if (  SteamAmIFileSynced()  ==  1 ) 
			{

			/*      
				t.f_s = steam get next server file();
				while (  t.f_s  !=  "" ) 
				{
					LoadImage (  t.f_s , 100 );
					while (  MouseClick()  ==  0 ) 
					{
						PasteImage (  100,0,0 );
						Sync (  );
					}
					t.f_s = steam get next server file();
				}
				*/    
	
				//  Text (  CODE REMOVE!!!!!!!! )
//     `if steamworks.fileLoaded = 0

//      `if FileExist("ds2.png") = 1

//       `if FileSize("ds2.png") = 1884709

//        `load image "ds2.png" , 2

	
							t.tempsteamworkshopidfile_s = g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerworkshopitemid__.dat";
							if (  FileExist(t.tempsteamworkshopidfile_s.Get()) ) 
							{
								if (  FileOpen(10)  ==  1  )  CloseFile (  10 );
								OpenToRead (  10,t.tempsteamworkshopidfile_s.Get() );
								g.steamworks.workshopid = ReadString ( 10 );
								CloseFile (  10 );
								if (  FileExist( cstr(g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerlevel__.fpm").Get() ) )  DeleteAFile (  cstr(g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerlevel__.fpm").Get() );
								SteamDownloadWorkshopItem (  g.steamworks.workshopid.Get() );
								g.steamworks.syncedWithServerMode = 2;
							}
							else
							{
								g.steamworks.fileLoaded = 1;
								SteamSendIAmLoadedAndReady (  );
								g.steamworks.syncedWithServerMode = 1;
							}
//       `endif

//      `endif

//     `endif

				//  TEST CODE REMOVE!!!!!!!!
	
//     `steam send i am loaded plus ready

			}
			else
			{
				t.tProgress = SteamGetFileProgress();
				t.tstring_s = cstr("Receiving '")+g.steamworks.levelnametojoin+"': " + Str(t.tProgress) + "%";
//     `text (GetDisplayWidth()/2) - (Text (  width(tstring$)/2), GetDisplayHeight() - 100, tstring$ )

				steam_text(-1,85,3,t.tstring_s.Get());
			}
		break;

		case 1:
			if (  SteamIsEveryoneLoadedAndReady()  ==  1 ) 
			{
				g.steamworks.syncedWithServer = 1;
				g.steamworks.syncedWithServerMode = 99;
				SetDir (  t.toldsteamfolder_s.Get() );
				SetDir (  g.steamworks.originalpath.Get() );
			}
			else
			{
				if (  Timer() - g.steamworks.oldtime > 150 ) 
				{
					g.steamworks.oldtime = Timer();
					t.tSteamBuildingWorkshopItem_s = t.tSteamBuildingWorkshopItem_s + ".";
					if (  Len(t.tSteamBuildingWorkshopItem_s.Get()) > 5  )  t.tSteamBuildingWorkshopItem_s  =  ".";
				}
				if (  Timer() - t.tempsteamworkssendingready > 2000 ) 
				{
					SteamSendIAmLoadedAndReady (  );
					t.tempsteamworkssendingready = Timer();
				}
				t.tstring_s = t.tSteamBuildingWorkshopItem_s + "Waiting for everyone to be ready" + t.tSteamBuildingWorkshopItem_s;
				steam_text(-1,50,3,t.tstring_s.Get());
				t.tstring_s = "";
			}
		break;

		case 2:
			if (  SteamIsWorkshopItemDownloaded()  ==  -1 ) 
			{
					t.tsteamconnectionlostmessage_s = "Unable to join, Steam does not yet have all the files needed (Error MP011)";
					steam_lostConnection ( );
					return;
			}
			if (  SteamIsWorkshopItemDownloaded()  ==  1 ) 
			{
				if (  FileExist( cstr(g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerlevel__.fpm").Get() ) ) 
				{
					g.steamworks.fileLoaded = 1;
					SteamSendIAmLoadedAndReady (  );
					g.steamworks.syncedWithServerMode = 1;
				}
				else
				{
					t.tsteamconnectionlostmessage_s = "Unable to join, Steam does not yet have all the files needed (Error MP012)";
					steam_lostConnection ( );
					return;
				}
			}
			else
			{
				if (  Timer() - g.steamworks.oldtime > 150 ) 
				{
					g.steamworks.oldtime = Timer();
					t.tSteamBuildingWorkshopItem_s = t.tSteamBuildingWorkshopItem_s + ".";
					if (  Len(t.tSteamBuildingWorkshopItem_s.Get()) > 5  )  t.tSteamBuildingWorkshopItem_s  =  ".";
				}
				t.tstring_s = t.tSteamBuildingWorkshopItem_s + "Downloading Workshop Item" + t.tSteamBuildingWorkshopItem_s;
				steam_text(-1,50,3,t.tstring_s.Get());
				t.tstring_s = "";
			}
		break;

	}//~ ` 

return;
}

void steam_sendAvatarInfo ( void )
{
//  send avatar info
//  `if Timer() - tlasttimesentavatar > 500

//   `tlasttimesentavatar = Timer()

		if (  g.steamworks.haveSentMyAvatar  ==  0 ) 
		{
			g.steamworks.me = SteamGetMyPlayerIndex();
			if (  g.steamworks.isGameHost  ==  1 || g.steamworks.me  !=  0 ) 
			{
				g.steamworks.haveSentMyAvatar = 1;
				SteamSendLuaString (  Steam_LUA_SendAvatarName,g.steamworks.me,SteamGetPlayerName() );
				SteamSendLuaString (  Steam_LUA_SendAvatar,g.steamworks.me,g.steamworks.myAvatar_s.Get() );

				//  store our own info for loading in our avatar
				t.steamworks_playerAvatarOwners_s[g.steamworks.me] = SteamGetPlayerName();
				t.steamworks_playerAvatars_s[g.steamworks.me] = g.steamworks.myAvatar_s;
				//  send out custom texture (steamworks.myAvatarHeadTexture$ will be "" if we don't have one)
				SteamSetMyAvatarHeadTextureName (  g.steamworks.myAvatarHeadTexture_s.Get() );
			}
		}
//  `endif

	steam_lua ( );
return;

}

void steam_animation ( void )
{

	while (  SteamGetAnimationList() ) 
	{
		t.tEnt = SteamGetAnimationIndex();
		t.astart = SteamGetAnimationStart();
		t.aend = SteamGetAnimationEnd();
		t.aspeed = SteamGetAnimationSpeed();
	
		SetObjectSpeed (  t.entityelement[t.tEnt].obj,t.aspeed );
		PlayObject (  t.entityelement[t.tEnt].obj,t.astart,t.aend );

		SteamGetNextAnimation (  );
	}

return;
//  Send our position and angle to steam
}

void steam_update_player ( void )
{
if (  g.steamworks.endplay  ==  1  )  return;

// once we are alive, no immunity
t.huddamage.immunity = 1000;
if (  Timer() - g.steamworks.invincibleTimer > 6000 ) 
{
	t.huddamage.immunity = 0;
}
else
{
	t.huddamage.immunity = 1000;
	t.tthrowawaythisdamage = SteamGetPlayerDamageAmount();
}
//  check if we have taken damage
t.tdamage = SteamGetPlayerDamageAmount();
// `if player(plrid).health > 100 then player(plrid).health  ==  100

if (  t.tdamage > 0 ) 
{

	t.tsteamlastdamageincounter = t.tsteamlastdamageincounter + 1;
	//  Receives; tdamage, te, tDrownDamageFlag
	t.te = t.steamworks_playerEntityID[SteamGetPlayerDamageSource()];
	t.tDrownDamageFlag = 0;
	physics_player_takedamage ( );
//  `dec entityelement(steamworks_playerEntityID(steamworks.me)).health,tdamage

	if (  t.player[t.plrid].health  <=  0 ) 
	{
// entityelement(steamworks_playerEntityID(steamworks.me)).health = 0
		g.steamworks.killedByPlayerFlag = 1;
		g.steamworks.playerThatKilledMe = SteamGetPlayerDamageSource();
		t.tsteamforce = SteamGetPlayerDamageForce();
		SteamKilledBy (  g.steamworks.playerThatKilledMe , SteamGetPlayerDamageX(), SteamGetPlayerDamageY(), SteamGetPlayerDamageZ(), t.tsteamforce, SteamGetPlayerDamageLimb() );
//   `steam set player score SteamGetPlayerDamageSource(),1

		g.steamworks.dyingTime = Timer();
	}
}

t.steamworks_health[g.steamworks.me] = t.player[t.plrid].health;

	//  check if we have changed guns
	if (  g.steamworks.gunid  !=  t.gunid ) 
	{
		//  send a server message saying we have a new gun
		t.tfound = 0;
		for ( t.ti = 0 ; t.ti<=  g.steamworks.gunCount; t.ti++ )
		{
			if (  t.steamworks_gunname[t.ti]  ==  Lower(t.gun[t.gunid].name_s.Get()) ) 
			{
				t.tfound = t.ti+1;
			}
		}
		if (  t.tfound>0 ) 
		{
			t.hasgunname_s=t.gun[t.gunid].name_s;
			t.steamhasgunname_s=t.steamworks_gunname[t.tfound-1];
			g.steamworks.appearance = t.tfound;
			t.toldappearancevariable = t.tfound;
//    `steamworks.gunid = gunid

		}
		else
		{
			g.steamworks.appearance = 0;
		}
		g.steamworks.gunid = t.gunid;
	}

	//  Send our positional data to the server
//  `t.tTime = Timer()

//  `if t.tTime - steamworks.lastSendPositionTime < STEAM_POSITION_UPDATE_DELAY then return
	
	
//  `steamworks.lastSendPositionTime = t.tTime

	
	SteamSetPlayerPositionX (  CameraPositionX() );
	g.steamworks.lastx = CameraPositionX();

		if (  g.steamworks.crouchOn  ==  0 ) 
		{
			SteamSetPlayerPositionY (  CameraPositionY()-64 );
			g.steamworks.lasty = CameraPositionY()-64;
		}
		else
		{
			SteamSetPlayerPositionY (  CameraPositionY()-64+30 );
			g.steamworks.lasty = CameraPositionY()-64+30;
		}

	SteamSetPlayerPositionZ (  CameraPositionZ() );
	g.steamworks.lastz = CameraPositionZ();

	SteamSetPlayerAngle (  CameraAngleY() );
	g.steamworks.lastangley = CameraAngleY();

	t.tpe = t.steamworks_playerEntityID[g.steamworks.me];
	t.entityelement[t.tpe].x=g.steamworks.lastx;
	t.entityelement[t.tpe].y=g.steamworks.lasty;
	t.entityelement[t.tpe].z=g.steamworks.lastz;
	if (  t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj > 0 ) 
	{
		if (  ObjectExist(t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj) ) 
		{
			PositionObject (  t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj, g.steamworks.lastx, g.steamworks.lasty+10, g.steamworks.lastz );
		}
	}
	t.te = t.tpe;
	t.tolde = t.e;
	t.e = t.tpe;
	entity_updatepos ( );
	entity_lua_rotateupdate ( );
	t.e = t.tolde;

return;

}

void steam_updatePlayerPositions ( void )
{

	if (  g.steamworks.endplay  ==  1  )  return;

	//  Get player data from the server
	for ( t.c = 0 ; t.c<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.c++ )
	{

		if (  t.steamworks_forcePosition[t.c] > 0 && SteamGetPlayerAlive(t.c)  ==  1 ) 
		{
			if (  t.steamworks_forcePosition[t.c]  ==  1  )  t.steamworks_forcePosition[t.c]  =  Timer();
			if (  Timer() - t.steamworks_forcePosition[t.c] > 1000 ) 
			{
				t.steamworks_forcePosition[t.c] = 0;
				t.x_f = SteamGetPlayerPositionX(t.c);
				t.y_f = SteamGetPlayerPositionY(t.c);
				t.z_f = SteamGetPlayerPositionZ(t.c);
				SteamSetTweening (  t.c,1 );
			}
			else
			{
				SteamSetTweening (  t.c,0 );
			}

			t.x_f = SteamGetPlayerPositionX(t.c);
			t.y_f = SteamGetPlayerPositionY(t.c);
			t.z_f = SteamGetPlayerPositionZ(t.c);
			t.angle_f = SteamGetPlayerAngle(t.c);

		}
		//  Get other players tweened positional data
		t.x_f = SteamGetPlayerPositionX(t.c);
		t.y_f = SteamGetPlayerPositionY(t.c);
		t.z_f = SteamGetPlayerPositionZ(t.c);
		t.angle_f = SteamGetPlayerAngle(t.c);
		if (  t.c  !=  g.steamworks.me ) 
		{
			if (  SteamGetPlayerAlive(t.c)  ==  1 && t.steamworks_forcePosition[t.c]  ==  0 ) 
			{
				t.e = t.steamworks_playerEntityID[t.c];
				t.entityelement[t.e].x=t.x_f;
				t.entityelement[t.e].y=t.y_f;
				t.entityelement[t.e].z=t.z_f;
				t.entityelement[t.e].ry=t.angle_f;
				PositionObject (  t.entityelement[t.e].obj, t.entityelement[t.e].x, t.entityelement[t.e].y, t.entityelement[t.e].z );
				t.te = t.e;
				entity_updatepos ( );
				entity_lua_rotateupdate ( );
			}
		}
	}

return;

//  Display message from server
}

void steam_server_message ( void )
{

if (  g.steamworks.endplay  ==  1  )  return;

t.s_s = SteamGetServerMessage();
if (  g.steamworks.coop  ==  1 ) 
{
	t.tplayer_s = FirstToken(t.s_s.Get()," ");
	t.tcheckforkilled_s = NextToken(" ");
	if (  t.tcheckforkilled_s  ==  "was" || t.tcheckforkilled_s  ==  "killed"  )  t.s_s  =  t.tplayer_s + " died!";
}
if (  t.s_s  !=  "" ) 
{
	t.tsteamdisplaymessagetimer = Timer();
	t.s_s = Upper(t.s_s.Get());
}
if (  t.s_s  ==  ""  )  t.s_s  =  g.steamworks.previousMessage_s;
g.steamworks.previousMessage_s = t.s_s;
if (  Timer() - t.tsteamdisplaymessagetimer < 2000  )  steam_text(-1,10,3,t.s_s.Get());
// `text GetDisplayWidth()/2 - Text (  width(s$)/2, 100, s$ )


return;

}

void steam_updatePlayerNamePlates ( void )
{

//  `if steamworks.endplay  ==  1 then return
	

	if (  g.steamworks.nameplatesOff  ==  1 ) 
	{
		for ( t.c = 0 ; t.c<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.c++ )
		{
			if (  ObjectExist(g.steamplayermodelsoffset+500+t.c) ) 
			{
				PositionObject (  g.steamplayermodelsoffset+500+t.c,500000,-500000,500000 );
			}
		}
		return;
	}
	//  Display players names and stats
	for ( t.c = 0 ; t.c<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.c++ )
	{
			//  if it isnt me, display their details above their head
			if (  g.steamworks.sentmyname  ==  1 ) 
			{
				if (  ObjectExist(g.steamplayermodelsoffset+500+t.c)  ==  1  )  DeleteObject (  g.steamplayermodelsoffset+500+t.c );
			}
			if (  ObjectExist(g.steamplayermodelsoffset+500+t.c) ) 
			{
				PositionObject (  g.steamplayermodelsoffset+500+t.c,500000,-500000,500000 );
			}

			if (  t.entityelement[t.steamworks_playerEntityID[t.c]].obj > 0 ) 
			{
				if (  ObjectExist(t.entityelement[t.steamworks_playerEntityID[t.c]].obj)  ==  1 ) 
				{
					if (  t.c  !=  g.steamworks.me ) 
					{
						if (  t.steamworks_forcePosition[t.c]  ==  0 && SteamGetPlayerAlive(t.c)  ==  1 ) 
						{
							if (  GetInScreen(t.entityelement[t.steamworks_playerEntityID[t.c]].obj) ) 
							{
								t.tname_s = SteamGetOtherPlayerName(t.c);
								if (  t.tname_s != "Player" ) 
								{
									t.tobj = t.entityelement[t.steamworks_playerEntityID[t.c]].obj;
									if (  ObjectExist(g.steamplayermodelsoffset+500+t.c)  ==  0 ) 
									{
										t.tResult = MakeNewObjectPanel(g.steamplayermodelsoffset+500+t.c,Len(t.tname_s.Get()));
										if (  t.tResult ) 
										{
											t.index = 3;
											t.twidth=0;
											for ( t.n = 1 ; t.n<=  Len(t.tname_s.Get()); t.n++ )
											{
												t.charindex=Asc(Mid(t.tname_s.Get(),t.n));
												t.twidth += t.bitmapfont[t.index][t.charindex].w;
											}
											t.tx = -(t.twidth/2.0);

											t.timg = g.bitmapfontimagetart+t.index;
											for ( t.n = 1 ; t.n<=  Len(t.tname_s.Get()); t.n++ )
											{
												t.charindex=Asc(Mid(t.tname_s.Get(),t.n));
												t.u1_f=t.bitmapfont[t.index][t.charindex].x1;
												t.v1_f=t.bitmapfont[t.index][t.charindex].y1;
												t.u2_f=t.bitmapfont[t.index][t.charindex].x2;
												t.v2_f=t.bitmapfont[t.index][t.charindex].y2;
												t.r = 255;
												t.g = 50;
												t.b = 50;
												if (  g.steamworks.team  ==  1 ) 
												{
													if (  t.steamworks_team[t.c]  ==  t.steamworks_team[g.steamworks.me] ) 
													{
														t.r = 100;
														t.g = 255;
														t.b = 100;
													}
												}
												SetObjectPanelQuad (  g.steamplayermodelsoffset+500+t.c,t.n-1,t.tx,0,t.bitmapfont[t.index][t.charindex].w,t.bitmapfont[t.index][t.charindex].h,t.u1_f,t.v1_f,t.u2_f,t.v2_f,t.r,t.g,t.b );
												t.tx += t.bitmapfont[t.index][t.charindex].w;
											}
											FinishObjectPanel (  g.steamplayermodelsoffset+500+t.c,32,10 );

											SetCharacterCreatorTones (  g.steamplayermodelsoffset+500+t.c,0,t.r,t.g,t.b,1.0 );
											SetObjectLight (  g.steamplayermodelsoffset+500+t.c,0 );
											YRotateObject (  g.steamplayermodelsoffset+500+t.c,180 );
											FixObjectPivot (  g.steamplayermodelsoffset+500+t.c );
											SetObjectTransparency (  g.steamplayermodelsoffset+500+t.c, 6 );
											ScaleObject (  g.steamplayermodelsoffset+500+t.c,60,60,100 );
											SetSphereRadius (  g.steamplayermodelsoffset+500+t.c,0 );
											SetObjectMask (  g.steamplayermodelsoffset+500+t.c, 1 );
											//  apply special overlay_basic shader which also handles depth render for DOF avoidance
											t.teffectid=loadinternaleffect("effectbank\\reloaded\\overlay_basic.fx");
											TextureObject (  g.steamplayermodelsoffset+500+t.c,t.timg );
											SetObjectEffect (  g.steamplayermodelsoffset+500+t.c,t.teffectid );
										}
									}
									else
									{
//          `show it

										if (  SteamGetPlayerAlive(t.c)  ==  1 && g.steamworks.endplay  ==  0 ) 
										{
											t.tnameplatey_f = ObjectPositionY(t.tobj)+ ObjectSizeY(t.tobj,1);
											if (  t.steamworks_playerAvatars_s[t.c]  !=  ""  )  t.tnameplatey_f  =  t.tnameplatey_f + 15.0;
											ShowObject (  g.steamplayermodelsoffset+500+t.c );
											PositionObject((g.steamplayermodelsoffset+500+t.c), ObjectPositionX(t.tobj), t.tnameplatey_f , ObjectPositionZ(t.tobj));
											PointObject (  g.steamplayermodelsoffset+500+t.c,CameraPositionX(), CameraPositionY(), CameraPositionZ() );
										}
										else
										{
											HideObject (  g.steamplayermodelsoffset+500+t.c );
										}
									}
//          `text GetScreenX(tobj) - Text (  width(tname$)/2,GetScreenY(tobj)+10,tname$ )

//          `text GetScreenX(tobj) - Text (  width(tname$)/2,GetScreenY(tobj)+25,"Health ): " + Str(entityelement(steamworks_playerEntityID(c)).health)

//          `text GetScreenX(tobj) - Text (  width(tname$)/2,GetScreenY(tobj)+25,"Kills ): " + Str(steam get player score(c))

								}
							}
						}
						else
						{
							if (  ObjectExist(g.steamplayermodelsoffset+500+t.c) ) 
							{
//         `hide object steamplayermodelsoffset+500+c

								PositionObject (  g.steamplayermodelsoffset+500+t.c,500000,-500000,500000 );
							}
						}
					}
				}
			}
		}

		g.steamworks.sentmyname = 0;

return;

}

void steam_updatePlayerAnimations ( void )
{

	//  Update animations
	for ( t.c = 0 ; t.c<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.c++ )
	{

			t.tobj = t.entityelement[t.steamworks_playerEntityID[t.c]].obj;

			t.thasNade = 0;
			t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
			if ( t.gun[t.tgunid].projectileframe != 0 ) t.thasNade = 1;

			t.steamworks_playerShooting[t.c] = SteamGetShoot(t.c);

			//  if the player is reloading we will try and show it (only works if idle or ducking at present)
			if (  SteamGetPlayerAppearance(t.c)  ==  201  )  t.steamworks_reload[t.c]  =  1;

			//  update animations
			g.steamworks.isAnimating = 0;
			if (  SteamGetPlayerAlive(t.c)  ==  1 ) 
			{
					t.spinelimbofcharacter=t.entityprofile[t.entityelement[t.steamworks_playerEntityID[t.c]].bankindex].spine;
					RotateLimb (  t.tobj,t.spinelimbofcharacter,LimbAngleX( t.tobj,t.spinelimbofcharacter),0,LimbAngleZ( t.tobj,t.spinelimbofcharacter) );

				if (  (SteamGetPlayerAppearance(t.c) < 102 || SteamGetPlayerAppearance(t.c) > 200) ) 
				{
					//  Melee
					if (  SteamGetKeyState(t.c,16)  ==  1 || t.steamworks_meleePlaying[t.c]  ==  1 ) 
					{
						g.steamworks.isAnimating = 1;
						if (  t.steamworks_meleePlaying[t.c]  ==  0 ) 
						{
							t.steamworks_meleePlaying[t.c] = 1;
						}
						else
						{
							if (  GetPlaying(t.tobj)  ==  0  )  t.steamworks_meleePlaying[t.c]  =  0;
							if (  GetLooping(t.tobj)  ==  1  )  t.steamworks_meleePlaying[t.c]  =  0;
						}
					}
					//  Forwards
					if (  SteamGetKeyState(t.c,17)  ==  1 ) 
					{
						g.steamworks.isAnimating = 1;
						//  are they moving left also
						if (  SteamGetKeyState(t.c,30)  ==  1 ) 
						{
							YRotateObject (  t.tobj, ObjectAngleY(t.tobj) - 45 );
							RotateLimb (  t.tobj,t.spinelimbofcharacter,LimbAngleX(t.tobj,t.spinelimbofcharacter),45,LimbAngleZ(t.tobj,t.spinelimbofcharacter) );
						}
						//  or perhaps they are moving right also
						if (  SteamGetKeyState(t.c,32)  ==  1 ) 
						{
							YRotateObject (  t.tobj, ObjectAngleY(t.tobj) + 45 );
							RotateLimb (  t.tobj,t.spinelimbofcharacter,LimbAngleX(t.tobj,t.spinelimbofcharacter),-45,LimbAngleZ(t.tobj,t.spinelimbofcharacter) );
						}
						if (  SteamGetKeyState(t.c,42)  ==  0 || SteamGetKeyState(t.c,46)  ==  1 ) 
						{
							if (  SteamGetPlayerAppearance(t.c)  ==  101 ) 
							{
								t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.animspeed=300;
							}
							else
							{
								t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.animspeed=100;
							}
						}
						else
						{
							if (  SteamGetPlayerAppearance(t.c)  ==  101 ) 
							{
								t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.animspeed=600;
							}
							else
							{
								t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.animspeed=200;
							}
						}
						if (  SteamGetKeyState(t.c,46)  ==  0 ) 
						{
							if (  t.steamworks_playingAnimation[t.c]  !=  STEAM_ANIMATION_WALKING ) 
							{

								t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
								t.tweapstyle=t.gun[t.tgunid].weapontype;
								if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
								if (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj > 0 && t.thasNade  ==  0 ) 
								{
									t.tplaycsi=t.csi_stoodmoverunANIM[t.tweapstyle];
								}
								else
								{
									t.tplaycsi=g.csi_unarmedmoverunANIM;
								}
								steam_switchAnim ( );

								if (  SteamGetPlayerAppearance(t.c)  ==  101 ) 
								{
									t.tplaycsi=g.csi_unarmedANIM0;
									steam_switchAnim ( );
								}
								entity_lua_setanimationframes ( );
								t.e = t.steamworks_playerEntityID[t.c];
								entity_lua_loopanimation ( );
								g.steamworks.isAnimating = 1;
								t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_WALKING;
							}
						}
						else
						{
							if (  t.steamworks_playingAnimation[t.c]  !=  STEAM_ANIMATION_DUCKINGWALKING ) 
							{

								t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
								t.tweapstyle=t.gun[t.tgunid].weapontype;
								if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
								if (  t.tweapstyle  ==  0  )  t.tweapstyle  =  1;
								t.tplaycsi=t.csi_crouchmoverunANIM[t.tweapstyle];
								steam_switchAnim ( );

								entity_lua_setanimationframes ( );
								t.e = t.steamworks_playerEntityID[t.c];
								entity_lua_loopanimation ( );
								g.steamworks.isAnimating = 1;
								t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_DUCKINGWALKING;
							}
						}
					}
					//  Backwards
					if (  SteamGetKeyState(t.c,31)  ==  1 ) 
					{
						g.steamworks.isAnimating = 1;
						//  are they moving left also
						if (  SteamGetKeyState(t.c,30)  ==  1 ) 
						{
							YRotateObject (  t.tobj, ObjectAngleY(t.tobj) + 45 );
							RotateLimb (  t.tobj,t.spinelimbofcharacter,LimbAngleX(t.tobj,t.spinelimbofcharacter),-45,LimbAngleZ(t.tobj,t.spinelimbofcharacter) );
						}
						//  or perhaps they are moving right also
						if (  SteamGetKeyState(t.c,32)  ==  1 ) 
						{
							YRotateObject (  t.tobj, ObjectAngleY(t.tobj) - 45 );
							RotateLimb (  t.tobj,t.spinelimbofcharacter,LimbAngleX(t.tobj,t.spinelimbofcharacter),45,LimbAngleZ(t.tobj,t.spinelimbofcharacter) );
						}
						if (  SteamGetPlayerAppearance(t.c)  ==  101 ) 
						{
							t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.animspeed=-300;
						}
						else
						{
							t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.animspeed=-100;
						}
						if (  SteamGetKeyState(t.c,46)  ==  0 ) 
						{
							if (  t.steamworks_playingAnimation[t.c]  !=  STEAM_ANIMATION_WALKINGBACKWARDS ) 
							{
								t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
								t.tweapstyle=t.gun[t.tgunid].weapontype;
								if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
								if (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj > 0 && t.thasNade  ==  0 ) 
								{
									t.tplaycsi=t.csi_stoodmoverunANIM[t.tweapstyle];
								}
								else
								{
									t.tplaycsi=g.csi_unarmedmoverunANIM;
								}
								steam_switchAnim ( );

								if (  SteamGetPlayerAppearance(t.c)  ==  101 ) 
								{
									t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
									t.tweapstyle=t.gun[t.tgunid].weapontype;
									if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
									t.tplaycsi=t.csi_stoodnormalANIM[t.tweapstyle];
									steam_switchAnim ( );
								}
								entity_lua_setanimationframes ( );
								t.e = t.steamworks_playerEntityID[t.c];
								entity_lua_loopanimation ( );
								g.steamworks.isAnimating = 1;
								t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_WALKINGBACKWARDS;
							}
						}
						else
						{
							if (  t.steamworks_playingAnimation[t.c]  !=  STEAM_ANIMATION_DUCKINGWALKINGBACKWARDS ) 
							{

								t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
								t.tweapstyle=t.gun[t.tgunid].weapontype;
								if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
								if (  t.tweapstyle  ==  0  )  t.tweapstyle  =  1;
								t.tplaycsi=t.csi_crouchmoverunANIM[t.tweapstyle];
								steam_switchAnim ( );

								entity_lua_setanimationframes ( );
								t.e = t.steamworks_playerEntityID[t.c];
								entity_lua_loopanimation ( );
								g.steamworks.isAnimating = 1;
								t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_DUCKINGWALKINGBACKWARDS;
							}
						}
					}

					//  strafe left
					if (  SteamGetKeyState(t.c,30)  ==  1 ) 
					{
						if (  g.steamworks.isAnimating  ==  0 ) 
						{
							g.steamworks.isAnimating = 1;
							if (  SteamGetKeyState(t.c,42)  ==  0 ) 
							{
								t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.animspeed=100;
							}
							else
							{
								t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.animspeed=150;
							}
							if (  SteamGetKeyState(t.c,46)  ==  1 ) 
							{
								if (  t.steamworks_playingAnimation[t.c]  !=  STEAM_ANIMATION_DUCKINGWALKING ) 
								{

								t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
								t.tweapstyle=t.gun[t.tgunid].weapontype;
								if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
								if (  t.tweapstyle  ==  0  )  t.tweapstyle  =  1;
								t.tplaycsi=t.csi_crouchmoveleftANIM[t.tweapstyle];
								steam_switchAnim ( );

									entity_lua_setanimationframes ( );
									t.e = t.steamworks_playerEntityID[t.c];
									entity_lua_loopanimation ( );
									g.steamworks.isAnimating = 1;
									t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_DUCKINGWALKING;
								}
							}
							else
							{
								if (  t.steamworks_playingAnimation[t.c]  !=  STEAM_ANIMATION_STRAFELEFT ) 
								{
									t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
									t.tweapstyle=t.gun[t.tgunid].weapontype;
									if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
									if (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj > 0 && t.thasNade  ==  0 ) 
									{
										t.tplaycsi=t.csi_stoodmoverunleftANIM[t.tweapstyle];
									}
									else
									{
										RotateLimb (  t.tobj,t.spinelimbofcharacter,LimbAngleX(t.tobj,t.spinelimbofcharacter),-45,LimbAngleZ(t.tobj,t.spinelimbofcharacter) );
										t.tplaycsi=g.csi_unarmedmoverunANIM;
									}
									steam_switchAnim ( );

									entity_lua_setanimationframes ( );
									t.e = t.steamworks_playerEntityID[t.c];
									entity_lua_loopanimation ( );
									t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_STRAFELEFT;
								}
							}
						}
					}

					//  strafe right
					if (  SteamGetKeyState(t.c,32)  ==  1 ) 
					{
						if (  g.steamworks.isAnimating  ==  0 ) 
						{
							g.steamworks.isAnimating = 1;
							if (  SteamGetKeyState(t.c,42)  ==  0 ) 
							{
								t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.animspeed=100;
							}
							else
							{
								t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.animspeed=150;
							}
							if (  SteamGetKeyState(t.c,46)  ==  1 ) 
							{
								if (  t.steamworks_playingAnimation[t.c]  !=  STEAM_ANIMATION_DUCKINGWALKING ) 
								{

									t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
									t.tweapstyle=t.gun[t.tgunid].weapontype;
									if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
									if (  t.tweapstyle  ==  0  )  t.tweapstyle  =  1;
									t.tplaycsi=t.csi_crouchmoverightANIM[t.tweapstyle];
									steam_switchAnim ( );

									entity_lua_setanimationframes ( );
									t.e = t.steamworks_playerEntityID[t.c];
									entity_lua_loopanimation ( );
									g.steamworks.isAnimating = 1;
									t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_DUCKINGWALKING;
								}
							}
							else
							{
								if (  t.steamworks_playingAnimation[t.c]  !=  STEAM_ANIMATION_STRAFERIGHT ) 
								{

									t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
									t.tweapstyle=t.gun[t.tgunid].weapontype;
									if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
									if (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj > 0 && t.thasNade  ==  0 ) 
									{
										t.tplaycsi=t.csi_stoodmoverunrightANIM[t.tweapstyle];
									}
									else
									{
										t.tplaycsi=g.csi_unarmedmoverunANIM;
									}
									steam_switchAnim ( );

									entity_lua_setanimationframes ( );
									t.e = t.steamworks_playerEntityID[t.c];
									entity_lua_loopanimation ( );
								//steamworks_isIdling(c) = 0
									t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_STRAFERIGHT;
								}
							}
						}
					}

					if (  t.steamworks_playingAnimation[t.c]  ==  STEAM_ANIMATION_STRAFELEFT ) 
					{
						if (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj  ==  0 ) 
						{
							RotateLimb (  t.tobj,t.spinelimbofcharacter,LimbAngleX(t.tobj,t.spinelimbofcharacter),45,LimbAngleZ(t.tobj,t.spinelimbofcharacter) );
							YRotateObject (  t.tobj, ObjectAngleY(t.tobj) - 45 );
						}
					}

					if (  t.steamworks_playingAnimation[t.c]  ==  STEAM_ANIMATION_STRAFERIGHT ) 
					{
						if (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj  ==  0 ) 
						{
							RotateLimb (  t.tobj,t.spinelimbofcharacter,LimbAngleX(t.tobj,t.spinelimbofcharacter),-45,LimbAngleZ(t.tobj,t.spinelimbofcharacter) );
							YRotateObject (  t.tobj, ObjectAngleY(t.tobj) + 45 );
						}
					}


					//  Ducking
					if (  SteamGetKeyState(t.c,46)  ==  1 && t.steamworks_jetpackOn[t.c]  ==  0 ) 
					{
						if (  g.steamworks.isAnimating  ==  0 && t.steamworks_reload[t.c]  ==  0 ) 
						{
							g.steamworks.isAnimating = 1;
							if (  t.steamworks_playingAnimation[t.c]  !=  STEAM_ANIMATION_DUCKING ) 
							{

								t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
								t.tweapstyle=t.gun[t.tgunid].weapontype;
								if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
								t.tplaycsi=t.csi_crouchidlenormalANIM1[t.tweapstyle];
								steam_switchAnim ( );

								entity_lua_setanimationframes ( );
								t.e = t.steamworks_playerEntityID[t.c];
								t.entityelement[t.e].eleprof.animspeed=100;
								entity_lua_playanimation ( );
								g.steamworks.isAnimating = 1;
//         `steamworks_isIdling(c) = 0

								t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_DUCKING;
//         `print "PLAYER IS DUCKING <--------------------------------"

							}
						}
					}

				}

				if (  t.thasNade  ==  1 ) 
				{
					if (  t.steamworks_reload[t.c]  ==  1  )  t.steamworks_reload[t.c]  =  0;
				}

				if (  t.steamworks_reload[t.c]  ==  1 ) 
				{
						if (  g.steamworks.isAnimating  ==  0 || t.steamworks_playingAnimation[t.c]  ==  STEAM_ANIMATION_DUCKING ) 
						{
							if (  t.steamworks_playingAnimation[t.c]  !=  STEAM_ANIMATION_RELOAD ) 
							{

								t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
								t.tweapstyle=t.gun[t.tgunid].weapontype;
								if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
								t.tplaycsi=t.csi_stoodreloadANIM[t.tweapstyle];
								steam_switchAnim ( );

								if (  t.steamworks_playingAnimation[t.c]  ==  STEAM_ANIMATION_DUCKING ) 
								{

								t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
								t.tweapstyle=t.gun[t.tgunid].weapontype;
								if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
								t.tplaycsi=t.csi_crouchreloadANIM[t.tweapstyle];
								steam_switchAnim ( );

								}
								entity_lua_setanimationframes ( );
								t.e = t.steamworks_playerEntityID[t.c];
								t.entityelement[t.e].eleprof.animspeed=200;
								entity_lua_playanimation ( );
								g.steamworks.isAnimating = 1;
//         `steamworks_isIdling(c) = 0

								t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_RELOAD;
							}
						}
						g.steamworks.isAnimating = 1;
						//  if the reload anim has finished or the player starts shooting, turn reloading off
						if (  GetFrame(t.tobj)  ==  605 || GetFrame(t.tobj)  ==  2010 || t.steamworks_playerShooting[t.c]  ==  1 ) 
						{
							t.steamworks_reload[t.c] = 0;
							if (  GetFrame(t.tobj)  ==  2010 ) 
							{
								t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_DUCKING;
							}
							else
							{
								t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_NONE;
								g.steamworks.isAnimating = 0;
							}
//        `print "DUCKING SPOT 2 <----------------------------------------------"

						}
				}

				t.tjetpacktempanim = 0;
				if (  SteamGetPlayerAppearance(t.c)  ==  102 ) 
				{
					t.tjetpacktempanim = 1;
					if (  t.steamworks_playingAnimation[t.c]  !=  STEAM_ANIMATION_IDLE ) 
					{
						t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_NONE;
						g.steamworks.isAnimating = 0;
					}
				}

				if (  t.thasNade  ==  1 ) 
				{
					if (  t.steamworks_playingAnimation[t.c]  ==  STEAM_ANIMATION_IDLE ) 
					{
						if (  t.steamworks_playerShooting[t.c]  ==  1 ) 
						{
							if (   GetFrame(t.tobj) < 2390 || GetFrame(t.tobj) > 2444 ) 
							{
								t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_NONE;
								g.steamworks.isAnimating = 0;
							}
						}

						if (  t.steamworks_playerShooting[t.c]  ==  0 ) 
						{
							if (  GetFrame(t.tobj)  ==  2444 ) 
							{
								SetObjectFrame(t.tobj,2443);
								StopObject (  t.tobj );
								g.steamworks.isAnimating = 0;
								t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_NONE;
							}
						}

					}
				}

				if (  g.steamworks.isAnimating  ==  0 ) 
				{
					steam_update_waist_rotation ( );
//      `print "IDLING"

					if (  t.steamworks_playingAnimation[t.c]  !=  STEAM_ANIMATION_IDLE ) 
					{
						if (  abs(t.steamworks_oldplayerx[t.c] - t.entityelement[t.steamworks_playerEntityID[t.c]].x) < 1.0 || t.tjetpacktempanim  ==  1 ) 
						{
							if (  abs(t.steamworks_oldplayery[t.c] - t.entityelement[t.steamworks_playerEntityID[t.c]].y) < 1.0 || t.tjetpacktempanim  ==  1 ) 
							{
								if (  abs(t.steamworks_oldplayerz[t.c] - t.entityelement[t.steamworks_playerEntityID[t.c]].z) < 1.0 || t.tjetpacktempanim  ==  1 ) 
								{
									t.tIsThrowingNade = 0;
									t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
									t.tweapstyle=t.gun[t.tgunid].weapontype;
									if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
									if (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj > 0 && t.thasNade  ==  0 ) 
									{

										t.tplaycsi=t.csi_stoodnormalANIM[t.tweapstyle];
										steam_switchAnim ( );

									}
									else
									{

										if (  t.thasNade  ==  1 && t.steamworks_playerShooting[t.c]  ==  1 ) 
										{
											t.ttentid=t.entityelement[t.steamworks_playerEntityID[t.c]].bankindex;
											t.e=2390;
											t.v=2444;
											entity_lua_setanimationframes ( );
											t.e = t.steamworks_playerEntityID[t.c];
											t.entityelement[t.e].eleprof.animspeed=200;
											t.tLuaDontSendLua = 1;
											t.q=-1;
											entity_lua_playanimation ( );
											t.tLuaDontSendLua = 0;
											t.tIsThrowingNade = 1;
										}
										else
										{
											t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
											t.tweapstyle=t.gun[t.tgunid].weapontype;
											if (  t.tweapstyle > 5  )  t.tweapstyle  =  1;
											t.tplaycsi=g.csi_unarmedANIM0;
											steam_switchAnim ( );
										}

									}
									if (  t.tIsThrowingNade  ==  0 ) 
									{
										entity_lua_setanimationframes ( );
										t.e = t.steamworks_playerEntityID[t.c];
										t.entityelement[t.e].eleprof.animspeed=100;
										entity_lua_loopanimation ( );
									}
									t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_IDLE;
								}
							}
						}
					}
				}
				else
				{
					//  reset the idle turn if animating
					t.steamworks_lastIdleReset[t.c] = 1;
//      `print "last idle reset = 1"

				}

			}

			//  

			if (  SteamGetPlayerAlive(t.c)  ==  0 && g.steamworks.gameAlreadySpawnedBefore  !=  0 ) 
			{
				t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_NONE;
				t.steamworks_lastIdleReset[t.c] = 1;
				t.steamworks_forcePosition[t.c] = 1;

				if (  t.steamworks_jetpackparticles[t.c]  !=  -1 ) 
				{
					t.tRaveyParticlesEmitterID=t.steamworks_jetpackparticles[t.c];
					ravey_particles_delete_emitter ( );
					t.steamworks_jetpackparticles[t.c]=-1;
				}

				if (  t.steamworks_isDying[t.c]  ==  0 && t.steamworks_playerHasSpawned[t.c]  ==  1 ) 
				{

					t.steamworks_isDying[t.c] = 1;
					t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_NONE;
					t.spinelimbofcharacter=t.entityprofile[t.entityelement[t.steamworks_playerEntityID[t.c]].bankindex].spine;
					RotateLimb (  t.tobj,t.spinelimbofcharacter,LimbAngleX( t.tobj,t.spinelimbofcharacter),0,LimbAngleZ( t.tobj,t.spinelimbofcharacter) );
					t.e = t.steamworks_playerEntityID[t.c];
					if (  ObjectExist(g.steamplayermodelsoffset+t.c+121)  ==  1 ) 
					{
						t.tweight=t.entityelement[t.e].eleprof.phyweight;
						t.tfriction=t.entityelement[t.e].eleprof.phyfriction;
						ODECreateDynamicBox (  g.steamplayermodelsoffset+t.c+121,-1,0,t.tweight,t.tfriction,-1 );
					}
//      `play object tobj,4800,4958

//      `set GetSpeed (  tobj,200 )

//      `entityelement(e).eleprof.animspeed=200

//      `steamworks_playingAnimation(c) = STEAM_ANIMATION_NONE


					//  NON-CHARACTER, but can still have ragdoll flagged (like Zombies)
					t.ttentid=t.entityelement[t.e].bankindex;
					t.ttte = t.e;
					t.steamworks_playingRagdoll[t.c] = 1;
					if (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj > 0 ) 
					{
						if (  ObjectExist(t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj)  )  DeleteObject (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj );
						t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj = 0;
					}
					t.entityprofile[t.ttentid].ragdoll=1;
					if (  t.entityprofile[t.ttentid].ragdoll == 1 ) 
					{

						//  can only ragdoll clones not instances
						t.tte=t.ttte;
						entity_converttoclone ( );

						//  create ragdoll and stop any further manipulation of the object
						t.tphye=t.ttte;
						t.tphyobj=t.entityelement[t.ttte].obj;
						t.oldc = t.c;
						ragdoll_setcollisionmask ( t.entityelement[t.ttte].eleprof.colondeath );
						ragdoll_create ( );
						t.c = t.oldc;

						/*      
						MoveObjectLeft (  t.entityelement[t.ttte].obj,60 );
						t.x_f = ObjectPositionX(t.entityelement[t.ttte].obj);
						t.y_f = ObjectPositionY(t.entityelement[t.ttte].obj)+50;
						t.z_f = ObjectPositionZ(t.entityelement[t.ttte].obj);
						MoveObjectRight (  t.entityelement[t.ttte].obj,60 );
						t.tforce_f = 1000;
						tlimb = Rnd(6);
						switch (  tlimb ) 
						{
						case 0; tlimb  =  8 ; break 
:
						case 1; tlimb  =  10 ; break 
:
						case 2; tlimb  =  12 ; break 
:
						case 3; tlimb  =  16 ; break 
:
						case 4; tlimb  =  17 ; break 
:
						case 5; tlimb  =  24 ; break 
:
						case 6; tlimb  =  25 ; break 
:
						}						//~       remend
						*/    

						//  use the real raycast if we shot them
//       `if Steam Get Player Killed Source(c) = steamworks.me

//        `ttx# = brayx2#-brayx1#

//        `tty# = brayy2#-brayy1#

//        `ttz# = brayz2#-brayz1#

//        `ttforce# = tforce#

//        `ttlimb = bulletraylimbhit

//       `else

							//  grab the details from the server if someone else shot them
							t.ttx_f = SteamGetPlayerKilledX(t.c);
							t.tty_f = SteamGetPlayerKilledY(t.c);
							t.ttz_f = SteamGetPlayerKilledZ(t.c);
							t.ttforce_f = SteamGetPlayerKilledForce(t.c);
							t.ttlimb = SteamGetPlayerKilledLimb(t.c);
//       `endif

	
						//  and apply bullet directional force (tforce#=from gun settings)
						t.entityelement[t.ttte].ragdollified=1;
//       `entityelement(ttte).ragdollifiedforcex#=(x#)*0.8

//       `entityelement(ttte).ragdollifiedforcey#=(y#)*1.2

//       `entityelement(ttte).ragdollifiedforcez#=(z#)*0.8

						t.entityelement[t.ttte].ragdollifiedforcex_f=(t.ttx_f)*0.8;
						t.entityelement[t.ttte].ragdollifiedforcey_f=(t.tty_f)*1.2;
						t.entityelement[t.ttte].ragdollifiedforcez_f=(t.ttz_f)*0.8;
						t.entityelement[t.ttte].ragdollifiedforcevalue_f=t.ttforce_f*8000.0;
//       `entityelement(ttte).ragdollifiedforcelimb=tlimb

						t.entityelement[t.ttte].ragdollifiedforcelimb=t.ttlimb;
//       `bulletraylimbhit=-1


					}

				}
//     `steamworks_isIdling(c) = 0

			}
			else
			{
				if (  t.steamworks_forcePosition[t.c]  ==  0 ) 
				{
					if (  t.steamworks_isDying[t.c]  ==  1 ) 
					{
						if (  ObjectExist(g.steamplayermodelsoffset+t.c+121)  ==  1 ) 
						{
							ODEDestroyObject (  g.steamplayermodelsoffset+t.c+121 );
							RotateObject (  g.steamplayermodelsoffset+t.c+121,0,0,0 );
							PositionObject (  g.steamplayermodelsoffset+t.c+121,0,-99999,0 );
							HideObject (  g.steamplayermodelsoffset+t.c+121 );
							t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_NONE;
						}
						t.steamworks_isDying[t.c] = 0;
					}
				}
			}

			t.steamworks_oldplayerx[t.c] = t.entityelement[t.steamworks_playerEntityID[t.c]].x;
			t.steamworks_oldplayery[t.c] = t.entityelement[t.steamworks_playerEntityID[t.c]].y;
			t.steamworks_oldplayerz[t.c] = t.entityelement[t.steamworks_playerEntityID[t.c]].z;

}

return;

}

void steam_switchAnim ( void )
{
	t.ttentid=t.entityelement[t.steamworks_playerEntityID[t.c]].bankindex;
	t.q=t.entityprofile[t.ttentid].startofaianim;
	t.e=t.entityanim[t.ttentid][t.q+t.tplaycsi].start;
	t.v=t.entityanim[t.ttentid][t.q+t.tplaycsi].finish;
return;

}

void steam_update_waist_rotation ( void )
{

return;
t.tobj = t.entityelement[t.steamworks_playerEntityID[t.c]].obj;
//  needs c as multiplayer index, tobj as the multiplayer chars object
	if (  t.steamworks_lastIdleReset[t.c]  ==  1 ) 
	{
		t.steamworks_lastIdleY[t.c] = t.entityelement[t.steamworks_playerEntityID[t.c]].ry;
		t.steamworks_lastIdleReset[t.c] = 0;
	}

//  `print "steam get player angle(c) = " + Str(entityelement(steamworks_playerEntityID(c)).ry)

//  `print "steamworks_lastIdleY(c) = " + Str(steamworks_lastIdleY(c))


	t.tDifference_f = t.entityelement[t.steamworks_playerEntityID[t.c]].ry - t.steamworks_lastIdleY[t.c];
	t.tAmountToRotateSpine_f = t.tDifference_f;
	t.tAmountToRotateObject_f = 0.0;

	if (  t.tAmountToRotateSpine_f > 60.0 ) 
	{
		t.tAmountToRotateObject_f = t.tAmountToRotateSpine_f - 60.0;
		t.tAmountToRotateSpine_f = 60.0;
	}

	if (  t.tAmountToRotateSpine_f < -60.0 ) 
	{
		t.tAmountToRotateObject_f = t.tAmountToRotateSpine_f + 60.0;
		t.tAmountToRotateSpine_f = -60.0;
	}

	YRotateObject (  t.tobj, t.steamworks_lastIdleY[t.c]+t.tAmountToRotateObject_f );
	t.steamworks_lastIdleY[t.c] = t.steamworks_lastIdleY[t.c]+t.tAmountToRotateObject_f;

	t.spinelimbofcharacter=t.entityprofile[t.entityelement[t.steamworks_playerEntityID[t.c]].bankindex].spine;
	RotateLimb (  t.tobj,t.spinelimbofcharacter,LimbAngleX( t.tobj,t.spinelimbofcharacter),t.tAmountToRotateSpine_f,LimbAngleZ( t.tobj,t.spinelimbofcharacter) );

return;

}

void steam_showdeath ( void )
{

	t.characterkitcontrol.showmyhead = 1;
	if (  g.steamworks.haveshowndeath  ==  0 ) 
	{
		g.steamworks.haveshowndeath = 1;
		t.tolddeathcamx_f = 0;
		t.tolddeathcamy_f = 0;
		t.tolddeathcamz_f = 0;
		t.tamountToMoveIn_f = 0.0;
		t.tamountToMoveUp_f = 0.0;
		t.tspawninyoffset_f = 0.0;
		t.tshowdeathlockcam = -1;
		g.steamworks.spectatorfollowdistance = 200;
		t.tdeathamounttotakeoffdistance = 0;
	}

	//  19032015 - 021 - prevent water affect being triggered when in 3rd person
	visuals_underwater_off ( );

		t.tobjtosee = t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj;
//   `playercontrol.jetobjtouse=hudlayersbankoffset+1

		t.playercontrol.jetpackhidden=0;
		t.playercontrol.jetpackmode=0;
//   `plrzoomin# = 0

//   `plrzoominchange = 1

//   `gunzoommode = 0

		//  new subroutine so steam can reset zoom in
		physics_no_gun_zoom ( );
		if (  g.steamworks.endplay  ==  1 && g.steamworks.respawnLeft > 3 ) 
		{
				g.steamworks.respawnLeft = 100;
				return;
		}
		//  if dead switch to 3rd person view to see the action
			t.e = t.steamworks_playerEntityID[g.steamworks.me];
			t.tobj = t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj;
			if (  t.tobj > 0 ) 
			{
				if ( ObjectExist (t.tobj) ) 
				{
//     `if health(steamworks.me) <= 0

					ShowObject (  t.tobj );
					t.tpe = t.steamworks_playerEntityID[g.steamworks.me];
					if (  g.steamworks.ragdollon  ==  0 ) 
					{
						g.steamworks.ragdollon = 1;

					if (  g.steamworks.gameAlreadySpawnedBefore  ==  1 ) 
					{

						//  turn off jetpack sound, turn off particles and reset thrust
						if (  SoundExist(t.playercontrol.soundstartindex+18) == 1  )  StopSound (  t.playercontrol.soundstartindex+18 );
						t.playercontrol.jetpackthrust_f=0.0;
						//  stop particle emitter
						if (  t.playercontrol.jetpackparticleemitterindex>0 ) 
						{
							t.tRaveyParticlesEmitterID=t.playercontrol.jetpackparticleemitterindex;
							ravey_particles_delete_emitter ( );
							t.playercontrol.jetpackparticleemitterindex=0;
						}

						t.spinelimbofcharacter=t.entityprofile[t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].bankindex].spine;
						RotateLimb (  t.tobj,t.spinelimbofcharacter,LimbAngleX( t.tobj,t.spinelimbofcharacter),0,LimbAngleZ( t.tobj,t.spinelimbofcharacter) );
						if (  ObjectExist(g.steamplayermodelsoffset+g.steamworks.me+121)  ==  1 ) 
						{
							t.tweight=t.entityelement[t.e].eleprof.phyweight;
							t.tfriction=t.entityelement[t.e].eleprof.phyfriction;
							ODECreateDynamicBox (  g.steamplayermodelsoffset+g.steamworks.me+121,-1,0,t.tweight,t.tfriction,-1 );
						}
						t.tme = g.steamworks.me;
//       `play object tobj,4800,4958

//       `set GetSpeed (  tobj,200 )

//       `entityelement(e).eleprof.animspeed=200

//       `steamworks_playingAnimation(c) = STEAM_ANIMATION_NONE


						//  NON-CHARACTER, but can still have ragdoll flagged (like Zombies)
						t.ttentid=t.entityelement[t.e].bankindex;
						t.ttte = t.e;
						t.steamworks_playingRagdoll[t.tme] = 1;
						if (  t.entityelement[t.steamworks_playerEntityID[t.tme]].attachmentobj > 0 ) 
						{
							if (  ObjectExist(t.entityelement[t.steamworks_playerEntityID[t.tme]].attachmentobj)  )  DeleteObject (  t.entityelement[t.steamworks_playerEntityID[t.tme]].attachmentobj );
							t.entityelement[t.steamworks_playerEntityID[t.tme]].attachmentobj = 0;
						}
						t.entityprofile[t.ttentid].ragdoll=1;
						if (  t.entityprofile[t.ttentid].ragdoll == 1 ) 
						{

							//  can only ragdoll clones not instances
							t.tte=t.ttte;
							entity_converttoclone ( );

							//  create ragdoll and stop any further manipulation of the object
							t.tphye=t.ttte;
							t.tphyobj=t.entityelement[t.ttte].obj;
							t.oldc = t.c;
							ragdoll_setcollisionmask ( t.entityelement[t.ttte].eleprof.colondeath );
							ragdoll_create ( );
							t.c = t.oldc;

							//  use the real raycast if we shot them
							if (  SteamGetPlayerDamageSource()  ==  g.steamworks.me ) 
							{
								t.ttx_f = t.brayx2_f-t.brayx1_f;
								t.tty_f = t.brayy2_f-t.brayy1_f;
								t.ttz_f = t.brayz2_f-t.brayz1_f;
								t.ttforce_f = t.tforce_f;
								t.ttlimb = t.bulletraylimbhit;
							}
							else
							{
								//  grab the details from the server if someone else shot them
								t.ttx_f = SteamGetPlayerDamageX();
								t.tty_f = SteamGetPlayerDamageY();
								t.ttz_f = SteamGetPlayerDamageZ();
								t.ttforce_f = SteamGetPlayerDamageForce();
								t.ttlimb = SteamGetPlayerDamageLimb();
							}
	
							//  and apply bullet directional force (tforce#=from gun settings)
							t.entityelement[t.ttte].ragdollified=1;
//        `entityelement(ttte).ragdollifiedforcex#=(x#)*0.8

//        `entityelement(ttte).ragdollifiedforcey#=(y#)*1.2

//        `entityelement(ttte).ragdollifiedforcez#=(z#)*0.8

							t.entityelement[t.ttte].ragdollifiedforcex_f=(t.ttx_f)*0.8;
							t.entityelement[t.ttte].ragdollifiedforcey_f=(t.tty_f)*1.2;
							t.entityelement[t.ttte].ragdollifiedforcez_f=(t.ttz_f)*0.8;
							t.entityelement[t.ttte].ragdollifiedforcevalue_f=t.ttforce_f*8000.0;
//        `entityelement(ttte).ragdollifiedforcelimb=tlimb

							t.entityelement[t.ttte].ragdollifiedforcelimb=t.ttlimb;
//        `bulletraylimbhit=-1


						}
					}
					}
//entity_updatepos ( );
//entity_lua_rotateupdate ( );
					if (  1 ) 
					{
						t.tsteamlimb=t.entityprofile[t.entityelement[t.tpe].bankindex].spine2;
						if (  g.steamworks.gameAlreadySpawnedBefore  ==  0 ) 
						{
							if (  g.steamworks.initialSpawnmoveDownCharacterFlag  ==  1 ) 
							{
								PositionObject (  t.entityelement[t.tpe].obj, ObjectPositionX(t.entityelement[t.tpe].obj), ObjectPositionY(t.entityelement[t.tpe].obj)-50, ObjectPositionZ(t.entityelement[t.tpe].obj) );
								if (  ObjectPositionY(t.entityelement[t.tpe].obj) < BT_GetGroundHeight(t.terrain.TerrainID,ObjectPositionX(t.entityelement[t.tpe].obj),ObjectPositionZ(t.entityelement[t.tpe].obj)) ) 
								{
									PositionObject (  t.entityelement[t.tpe].obj, ObjectPositionX(t.entityelement[t.tpe].obj), BT_GetGroundHeight(t.terrain.TerrainID,ObjectPositionX(t.entityelement[t.tpe].obj),ObjectPositionZ(t.entityelement[t.tpe].obj)) , ObjectPositionZ(t.entityelement[t.tpe].obj) );
								}
								g.steamworks.initialSpawnmoveDownCharacterFlag = 0;
							}
						}
						t.x_f = LimbPositionX(t.tobjtosee,t.tsteamlimb);
						t.y_f = LimbPositionY(t.tobjtosee,t.tsteamlimb);
						t.z_f = LimbPositionZ(t.tobjtosee,t.tsteamlimb);
						PositionCamera (  t.x_f,t.y_f+100,t.z_f );
						RotateCamera (  0,g.steamworks.camrotate,0 );
						g.steamworks.camrotate = g.steamworks.camrotate + (0.5*g.timeelapsed_f);


						t.x_f = LimbPositionX(t.tobjtosee,t.tsteamlimb);
						t.y_f = LimbPositionY(t.tobjtosee,t.tsteamlimb)+10;
						t.z_f = LimbPositionZ(t.tobjtosee,t.tsteamlimb);

						MoveCamera (  -g.steamworks.spectatorfollowdistance );

						t.tXOldPos_f = CameraPositionX();
						t.tYOldPos_f = CameraPositionY();
						t.tZOldPos_f = CameraPositionZ();

						MoveCamera (  g.steamworks.spectatorfollowdistance );
						t.ttt=IntersectAll(g.lightmappedobjectoffset,g.lightmappedobjectoffsetfinish,0,0,0,0,0,0,-123);
//       `tEndEntity = entityviewstartobj+entityelementlist

						t.tHitObj=IntersectAll(g.entityviewstartobj,g.entityviewendobj,t.x_f,t.y_f,t.z_f,t.tXOldPos_f,t.tYOldPos_f,t.tZOldPos_f,t.tobjtosee);
						t.tdistancewecanmovecam_f = g.steamworks.spectatorfollowdistance;
						if (  t.tHitObj > 0 ) 
						{
							t.tHitX_f = ChecklistFValueA(6);
							t.tHitY_f = ChecklistFValueB(6);
							t.tHitZ_f = ChecklistFValueC(6);
							t.dx_f = t.x_f - t.tHitX_f;
							t.dy_f = t.y_f - t.tHitY_f;
							t.dz_f = t.z_f - t.tHitZ_f;
							t.tdistancewecanmovecam_f = Sqrt((t.dx_f*t.dx_f)+(t.dy_f*t.dy_f)+(t.dz_f*t.dz_f)) - 30;
						}
						MoveCamera (  -t.tdistancewecanmovecam_f );

//       `move camera -steamworks.spectatorfollowdistance

						PointCamera (  t.x_f,t.y_f,t.z_f );

//       `tEndEntity = entityviewstartobj+entityelementlist


						t.tXOldPos_f = CameraPositionX();
						t.tYOldPos_f = CameraPositionY();
						t.tZOldPos_f = CameraPositionZ();

						t.tXNewPos_f = t.x_f;
						t.tYNewPos_f = t.y_f;
						t.tZNewPos_f = t.z_f;

						//tobjtosee = entityelement(tpe).obj;
						/*      
						t.tHitObj=IntersectAll(g.entityviewstartobj,t.tEndEntity,t.tXOldPos_f,t.tYOldPos_f,t.tZOldPos_f,t.tXNewPos_f,t.tYNewPos_f,t.tZNewPos_f,t.entityelement[t.tpe].obj);
						if (  t.tHitObj>0 ) 
						{
							if (  g.steamworks.spectatorfollowdistance > 10.0 ) 
							{
								g.steamworks.spectatorfollowdistance = g.steamworks.spectatorfollowdistance - 10.0;
								g.steamworks.spectatorfollowdistancedelay = Timer();
							}
						}
						else
						{
							if (  g.steamworks.spectatorfollowdistance < 200.0 && Timer() - g.steamworks.spectatorfollowdistancedelay > 1000 ) 
							{
								g.steamworks.spectatorfollowdistance = g.steamworks.spectatorfollowdistance + 10.0;
							}
						}
						*/    
					}
					else
					{
						t.twhokilledme = SteamGetPlayerDamageSource();
						if (  t.twhokilledme  !=  g.steamworks.me ) 
						{
							t.tsteamlimb=t.entityprofile[t.entityelement[t.tpe].bankindex].spine2;
							t.x_f = LimbPositionX(t.entityelement[t.tpe].obj,t.tsteamlimb);
							t.y_f = LimbPositionY(t.entityelement[t.tpe].obj,t.tsteamlimb);
							t.z_f = LimbPositionZ(t.entityelement[t.tpe].obj,t.tsteamlimb);
							PositionCamera (  t.x_f,t.y_f+100,t.z_f );
							PointCamera (  SteamGetPlayerPositionX(t.twhokilledme),SteamGetPlayerPositionY(t.twhokilledme)+50,SteamGetPlayerPositionZ(t.twhokilledme) );
							/*      
							tcamheight = (200 - g.steamworks.spectatorfollowdistance) / 2;
							PositionCamera (  SteamGetPlayerPositionX(t.twhokilledme), SteamGetPlayerPositionY(t.twhokilledme)+tcamheight+50, SteamGetPlayerPositionZ(t.twhokilledme) );
							SteamSetPlayerPositionX (  SteamGetPlayerPositionX(t.twhokilledme) );
							SteamSetPlayerPositionY (  SteamGetPlayerPositionY(t.twhokilledme) );
							SteamSetPlayerPositionZ (  SteamGetPlayerPositionZ(t.twhokilledme) );
							RotateCamera (  0,SteamGetPlayerAngle(t.twhokilledme),0 );
							MoveCamera (  -g.steamworks.spectatorfollowdistance );
							PointCamera (  SteamGetPlayerPositionX(t.twhokilledme),SteamGetPlayerPositionY(t.twhokilledme)+50,SteamGetPlayerPositionZ(t.twhokilledme) );

							t.tEndEntity = g.entityviewstartobj+g.entityelementlist;

							t.tXOldPos_f = CameraPositionX();
							t.tYOldPos_f = CameraPositionY();
							t.tZOldPos_f = CameraPositionZ();

							t.tXNewPos_f = SteamGetPlayerPositionX(t.twhokilledme);
							t.tYNewPos_f = SteamGetPlayerPositionY(t.twhokilledme);
							t.tZNewPos_f = SteamGetPlayerPositionZ(t.twhokilledme);

							t.tHitObj=IntersectAll(g.entityviewstartobj,t.tEndEntity,t.tXOldPos_f,t.tYOldPos_f,t.tZOldPos_f,t.tXNewPos_f,t.tYNewPos_f,t.tZNewPos_f,t.entityelement[t.tpe].obj);
							if (  t.tHitObj>0 ) 
							{
								if (  g.steamworks.spectatorfollowdistance > 10.0 ) 
								{
									g.steamworks.spectatorfollowdistance = g.steamworks.spectatorfollowdistance - 10.0;
									g.steamworks.spectatorfollowdistancedelay = Timer();
								}
							}
							else
							{
								if (  g.steamworks.spectatorfollowdistance < 200.0 && Timer() - g.steamworks.spectatorfollowdistancedelay > 1000 ) 
								{
									g.steamworks.spectatorfollowdistance = g.steamworks.spectatorfollowdistance + 10.0;
								}
							}
						*/    
						}
//       `if steamworks.respawnLeft  ==  5 && toldrespawnleft  ==  6 then steamworks.spectatorfollowdistance  ==  200
						
						t.toldrespawnleft = g.steamworks.respawnLeft;
					}
				}
				if (  g.steamworks.gameAlreadySpawnedBefore  ==  1 ) 
				{
					if (  CameraPositionY()  <=  BT_GetGroundHeight(t.terrain.TerrainID,CameraPositionX(),CameraPositionZ()) ) 
					{
						PositionCamera (  CameraPositionX(), BT_GetGroundHeight(t.terrain.TerrainID,CameraPositionX(),CameraPositionZ()) + 50, CameraPositionZ() );
					}
					if (  CameraPositionY() < t.terrain.waterliney_f ) 
					{
						t.tshowdeathlockcam = 0;
					}
					if (  t.tshowdeathlockcam > -1 ) 
					{
						if (  t.tshowdeathlockcam  ==  0 ) 
						{
							PositionCamera (  CameraPositionX(), CameraPositionY() + t.tspawninyoffset_f , CameraPositionZ() );
							if (  CameraPositionY() < t.terrain.waterliney_f ) 
							{
								PositionCamera (  CameraPositionX(), t.terrain.waterliney_f+100 , CameraPositionZ() );
								t.tshowdeathlockcam = 1;
								t.tshowdeathlockcamx_f = CameraPositionX();
								t.tshowdeathlockcamy_f = CameraPositionY();
								t.tshowdeathlockcamz_f = CameraPositionZ();
								t.tshowdeathlockcamrotx_f = CameraAngleX();
								t.tshowdeathlockcamroty_f = CameraAngleY();
								t.tshowdeathlockcamrotz_f = CameraAngleZ();
							}
						}
						else
						{
							PositionCamera (  t.tshowdeathlockcamx_f,t.tshowdeathlockcamy_f,t.tshowdeathlockcamz_f );
							RotateCamera (  t.tshowdeathlockcamrotx_f,t.tshowdeathlockcamroty_f, t.tshowdeathlockcamrotz_f );
						}
					}
				}
				else
				{
					if (  CameraPositionY()  <=  BT_GetGroundHeight(t.terrain.TerrainID,CameraPositionX(),CameraPositionZ()) ) 
					{
						MoveCamera (  1.0 );
						t.tdeathamounttotakeoffdistance = t.tdeathamounttotakeoffdistance + 20;
					}
					if (  t.tdeathamounttotakeoffdistance > 0 && g.steamworks.spectatorfollowdistance > 40 ) 
					{
						g.steamworks.spectatorfollowdistance = g.steamworks.spectatorfollowdistance - 1.0;
						t.tdeathamounttotakeoffdistance = t.tdeathamounttotakeoffdistance - 1;
					}
				}

			}

			//  update any character creator people
			t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].x = ObjectPositionX(t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj);
			t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].y = ObjectPositionY(t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj);
			t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].z = ObjectPositionZ(t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj);
			//  for initial spawn in
			if (  g.steamworks.gameAlreadySpawnedBefore  ==  0 && ( g.steamworks.realfirsttimespawn  ==  1 || g.steamworks.coop  ==  1 ) ) 
			{
				characterkit_checkForCharacters ( );
				characterkit_updateAllCharacterCreatorEntitiesInMapFirstSpawn ( );
			}

return;

}

void steam_respawn ( void )
{

	t.characterkitcontrol.showmyhead = 1;
	if ( g.autoloadgun != 0 ) { g.autoloadgun=0 ; gun_change ( ); }
	if (  t.player[t.plrid].health < 100  )  t.player[t.plrid].health  =  100;
	if (  g.steamworks.myOriginalSpawnPoint  !=  -1 ) 
	{
		t.tindex = g.steamworks.me+1;
	}
	else
	{
		t.tindex = g.steamworks.myOriginalSpawnPoint;
	}

	g.steamworks.invincibleTimer = Timer();
	t.huddamage.immunity=1000;

	g.steamworks.damageWasFromAI = 0;

	if (  g.steamworks.coop  ==  1 ) 
	{
//  `remstart

		if (  g.steamworks.originalEntitycount  ==  0 ) 
		{
			//  Store the count here incase other elements get added later (like guns)
			g.steamworks.originalEntitycount = g.entityelementlist;
			Dim (  t.steamStoreentityelement,g.entityelementlist );
			for ( t.te = 1 ; t.te<=  g.entityelementlist; t.te++ )
			{
				t.steamStoreentityelement[t.te]=t.entityelement[t.te];
			}
		}
//   `remend

	}

	t.playercontrol.deadtime = Timer() + 2000;
	t.playercontrol.redDeathFog_f = 0;
//physics_disableplayer ( );
	t.aisystem.processplayerlogic=0;
	g.steamworks.noplayermovement = 1;
	
	if (  g.steamworks.syncedWithServer  ==  0 ) 
	{
		SteamSendIAmReadyToPlay (  );
		g.steamworks.syncedWithServer = 1;
		g.steamworks.sentreadytime = Timer();
//   `print "SENDING I AM READY"

		//  are we the server? if so, let lua know
		if (  g.steamworks.isGameHost  ==  1 ) 
		{
			LuaSetInt (  "mp_isServer",1 );
		}
		else
		{
			LuaSetInt (  "mp_isServer",0 );
		}
		LuaSetInt (  "mp_coop", g.steamworks.coop );
		steam_howManyEnemiesLeftToKill ( );
		LuaSetInt (  "mp_me",g.steamworks.me+1 );
		steam_setLuaResetStats ( );
	}
	
	ravey_particles_delete_all_emitters ( );
	
	if (  g.steamworks.maxHealth  ==  0  )  g.steamworks.maxHealth  =  t.player[t.plrid].health;
	
	if (  g.steamworks.gameAlreadySpawnedBefore  ==  0 || Timer() - g.steamworks.dyingTime > 1500 ) 
	{
			if (  g.steamworks.gameAlreadySpawnedBefore  ==  0 ) 
			{

				//  13032015 0XX - Team Multiplayer
				if (  g.steamworks.team  ==  1 ) 
				{
					for ( t.tteam = 1 ; t.tteam<=  STEAM_MAX_NUMBER_OF_PLAYERS; t.tteam++ )
					{
						t.tnothing = LuaExecute( cstr(cstr("mp_playerTeam[") + Str(t.tteam) + "] = " + Str(t.steamworks_team[t.tteam-1])).Get() );
					}
						t.tnothing = LuaExecute( cstr(cstr("mp_teambased = ") + Str(g.steamworks.team)).Get() );
				}

				t.tindex = g.steamworks.me+1;
				g.steamworks.myOriginalSpawnPoint = t.tindex;

				if (  t.steamworksmultiplayerstart[t.tindex].active == 1 ) 
				{
					t.terrain.playerx_f=t.steamworksmultiplayerstart[t.tindex].x;
					t.terrain.playery_f=t.steamworksmultiplayerstart[t.tindex].y+20;
					t.terrain.playerz_f=t.steamworksmultiplayerstart[t.tindex].z;
					t.terrain.playerax_f=0;
					t.terrain.playeray_f=t.steamworksmultiplayerstart[t.tindex].angle;
					t.terrain.playeraz_f=0;

					g.steamworks.lastx=t.terrain.playerx_f;
					g.steamworks.lasty=t.terrain.playery_f;
					g.steamworks.lastz=t.terrain.playerz_f;
					g.steamworks.lastangley=t.terrain.playeray_f;

				}
				else
				{
					t.tfound = 0;
					t.ttempindex = t.tindex/2;
					if (  t.ttempindex > 0 ) 
					{
						if (  t.steamworksmultiplayerstart[t.ttempindex].active == 1 ) 
						{
							g.steamworks.myOriginalSpawnPoint = t.ttempindex;
							t.tfound = 1;
							t.terrain.playerx_f=t.steamworksmultiplayerstart[t.ttempindex].x;
							t.terrain.playery_f=t.steamworksmultiplayerstart[t.ttempindex].y+20;
							t.terrain.playerz_f=t.steamworksmultiplayerstart[t.ttempindex].z;
							t.terrain.playerax_f=0;
							t.terrain.playeray_f=t.steamworksmultiplayerstart[t.ttempindex].angle;
							t.terrain.playeraz_f=0;

							g.steamworks.lastx=t.terrain.playerx_f;
							g.steamworks.lasty=t.terrain.playery_f;
							g.steamworks.lastz=t.terrain.playerz_f;
							g.steamworks.lastangley=t.terrain.playeray_f;
						}
					}
					if (  t.tfound  ==  0 ) 
					{
						if (  t.steamworksmultiplayerstart[1].active == 1 ) 
						{
							g.steamworks.myOriginalSpawnPoint = 1;
							t.tfound = 1;
							t.terrain.playerx_f=t.steamworksmultiplayerstart[1].x;
							t.terrain.playery_f=t.steamworksmultiplayerstart[1].y+20;
							t.terrain.playerz_f=t.steamworksmultiplayerstart[1].z;
							t.terrain.playerax_f=0;
							t.terrain.playeray_f=t.steamworksmultiplayerstart[1].angle;
							t.terrain.playeraz_f=0;
	
							g.steamworks.lastx=t.terrain.playerx_f;
							g.steamworks.lasty=t.terrain.playery_f;
							g.steamworks.lastz=t.terrain.playerz_f;
							g.steamworks.lastangley=t.terrain.playeray_f;
						}
					}
					if (  t.tfound  ==  0 ) 
					{
						physics_resetplayer_core ( );
					}
	
//      `endif

				}
			}
			else
			{
//     `if steamworks.spawnrnd  ==  -1 then steamworks.spawnrnd  ==  Rnd(STEAM_MAX_NUMBER_OF_PLAYERS-1)

//     `if steamworksmultiplayerstart(steamworks.spawnrnd).active=1

//    terrain.playerx#=steamworksmultiplayerstart(steamworks.spawnrnd).x
	//   terrain.playery#=steamworksmultiplayerstart(steamworks.spawnrnd).y+20
		//  terrain.playerz#=steamworksmultiplayerstart(steamworks.spawnrnd).z
			// terrain.playera#=steamworksmultiplayerstart(steamworks.spawnrnd).angle

			//endif
			}
		//physics_resetplayer_core ( );

			SteamSetPlayerPositionX (  t.terrain.playerx_f );
			SteamSetPlayerPositionY (  t.terrain.playery_f );
			SteamSetPlayerPositionZ (  t.terrain.playerz_f );
			SteamSetPlayerAngle (  t.terrain.playeray_f );
	}

	if (  SteamIsEveryoneReadyToPlay()  ==  0 || g.steamworks.syncedWithServer  ==  0 ) 
	{
		steam_textDots(-1,30,3,"Waiting for other players to join");
		if (  Timer() - g.steamworks.sentreadytime > 30*1000 ) 
		{
				g.steamworks.syncedWithServer = 0;
//     `print "NOT HEARD FROM OTHER PLAYERS"

		}
		t.typos = 40;
		for ( t.tn = 0 ; t.tn<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.tn++ )
		{
			if (  t.steamworks_joined[t.tn]  !=  "" ) 
			{
				if (  cstr(Right(t.steamworks_joined[t.tn].Get(),6 ))  ==  "Joined" ) 
				{
					steam_textColor(-1,t.typos,1,t.steamworks_joined[t.tn].Get(),100,255,100);
				}
				else
				{
					steam_textColor(-1,t.typos,1, cstr(t.steamworks_joined[t.tn] + " - Waiting").Get(),255,200,100);
				}
				t.typos += 5;
			}
		}
		return;
	}

	for ( t.c = 0 ; t.c<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.c++ )
	{
			t.x_f = SteamGetPlayerPositionX(t.c);
			t.y_f = SteamGetPlayerPositionY(t.c);
			t.z_f = SteamGetPlayerPositionZ(t.c);
			t.angle_f = SteamGetPlayerAngle(t.c);
	}

	//steam set voice chat steamworks.voiceChatOn;
	//  Hide gun since we go to 3rd person for spawn in
//  `steam set player position x -100000

//  `steam set player position y -100000

//  `steam set player position z -100000

//  `steam set player alive 0


	t.tobj = t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj;

	if (  g.steamworks.gameAlreadySpawnedBefore  ==  1 ) 
	{
		//  if we have not died to another player, we take one off our kills instead since we killed ourself
		if (  g.steamworks.checkedWhoKilledMe  ==  0 ) 
		{
			g.steamworks.checkedWhoKilledMe = 1;
			if (  g.steamworks.killedByPlayerFlag  ==  0 ) 
			{
//     `steamworks.kills = steamworks.kills -1

				if (  g.steamworks.coop  ==  0 ) 
				{
					SteamSendLua (  STEAM_LUA_ServerSetPlayerRemoveKill,0,g.steamworks.me+1 );
					SteamKilledSelf (  );
				}
//     `tnothing = LuaExecute("mp_playerKills[" + Str(steamworks.me+1) + "] = " + Str(steamworks.kills))

			}
			else
			{
				if (  g.steamworks.coop  ==  0 ) 
				{
					SteamSendLua (  STEAM_LUA_ServerSetPlayerAddKill,0,g.steamworks.playerThatKilledMe+1 );
				}
//     `tnothing = LuaExecute("mp_playerKills[" + Str(steamworks.playerThatKilledMe+1) + "] = mp_playerKills[" + Str(steamworks.playerThatKilledMe+1) + "] + 1")

			}
//    `steamworks.deaths = steamworks.deaths + 1

			SteamSendLua (  STEAM_LUA_ServerSetPlayerAddDeath,0,g.steamworks.me+1 );
//    `tnothing = LuaExecute("mp_playerDeaths[" + Str(steamworks.me+1) + "] = " + Str(steamworks.deaths))

		}
	}

	SetTextSize (  32 );

	if (  SteamReadyToSpawn()  ==  0 ) 
	{
		steam_text(-1,20,3,"WAITING FOR PLAYERS");
//   `text GetDisplayWidth()/2 - Text (  width("WAITING FOR PLAYERS")/2, GetDisplayHeight()/2-30, "WAITING FOR PLAYERS" )

		return;
	}
	if (  g.steamworks.syncedWithServer  ==  0 ) 
	{
		steam_pre_game_file_sync ( );
		if (  SteamGetClientServerConnectionStatus()  ==  0 ) 
		{
			t.tsteamlostconnectioncustommessage_s = "Lost connection with server (Error MP013)";
			steam_lostConnection ( );
			return;
		}
		return;
	}

	if (  g.steamworks.endplay  ==  0 && g.steamworks.showscoresdelay  ==  -2000 ) 
	{
		steam_panel(40,45,60,65);
		steam_text(-1,52,3,"SPAWNING IN");
//   `text GetDisplayWidth()/2 - Text (  width("RESPAWNING")/2, GetDisplayHeight()/2-30, "RESPAWNING" )

		t.s_s = Str(5-g.steamworks.respawnLeft);
		steam_text(-1,58,3,t.s_s.Get());
//   `text GetDisplayWidth()/2 - Text (  width(s$)/2, GetDisplayHeight()/2, s$ )


		if (  g.steamworks.coop  ==  0 ) 
		{
			if (  g.steamworks.killedByPlayerFlag  ==  1 ) 
			{
				t.s_s = cstr("YOU WERE KILLED BY ") + Upper(SteamGetOtherPlayerName(g.steamworks.playerThatKilledMe));
				steam_text(-1,30,3,t.s_s.Get());
			}
			else
			{
				if (  g.steamworks.gameAlreadySpawnedBefore  ==  1 ) 
				{
					t.s_s = "YOU KILLED YOURSELF!";
					steam_text(-1,30,3,t.s_s.Get());
				}
			}
		}
		else
		{
			if (  g.steamworks.gameAlreadySpawnedBefore  ==  1 ) 
			{
				t.s_s = "YOU DIED!";
				steam_text(-1,30,3,t.s_s.Get());
			}
		}
	}
	if (  g.steamworks.oldSpawnTimeLeft  ==  0  )  g.steamworks.oldSpawnTimeLeft  =  Timer();

	if (  Timer() - g.steamworks.oldSpawnTimeLeft  >=  1000 ) 
	{
		++g.steamworks.respawnLeft;
		g.steamworks.oldSpawnTimeLeft = 0;
		if (  g.steamworks.respawnLeft  >=  5 ) 
		{

			g.steamworks.haveshowndeath = 0;
			weapon_steam_projectile_reset ( );
			ravey_particles_delete_all_emitters ( );
			lua_removeplayerweapons ( );
			t.tsteamwasnetworkdamage = 0;
			g.steamworks.checkedWhoKilledMe = 0;
			g.steamworks.killedByPlayerFlag = 0;
			g.plrreloading = 0;
			t.playercontrol.pushforce_f = 0.0;
			t.playercontrol.camerashake_f = 0.0;
			g.steamworks.lastSendTime = 0;
			g.steamworks.spectatorfollowdistance = 200.0;
			t.tme = g.steamworks.me;
			if (  t.steamworks_playingRagdoll[t.tme]  ==  1 ) 
			{

				t.tphyobj=t.entityelement[t.steamworks_playerEntityID[t.tme]].obj;
				ragdoll_destroy ( );
				RotateObject (  t.entityelement[t.steamworks_playerEntityID[t.tme]].obj,0,180,0 );
				FixObjectPivot (  t.entityelement[t.steamworks_playerEntityID[t.tme]].obj );
				t.steamworks_playingRagdoll[t.tme] = 0;

			}

			if (  ObjectExist(g.steamplayermodelsoffset+t.tme+121)  ==  1 ) 
			{
				ODEDestroyObject (  g.steamplayermodelsoffset+t.tme+121 );
				RotateObject (  g.steamplayermodelsoffset+t.tme+121,0,0,0 );
				PositionObject (  g.steamplayermodelsoffset+t.tme+121,0,-99999,0 );
				HideObject (  g.steamplayermodelsoffset+t.tme+121 );
			}

			g.steamworks.ragdollon = 0;
			if (  g.steamworks.endplay  ==  0 ) 
			{
				t.aisystem.processplayerlogic=1;
			}
			t.playercontrol.deadtime = 0;
			t.playercontrol.redDeathFog_f = 0;

			if (  g.steamworks.maxHealth  ==  0  )  g.steamworks.maxHealth  =  100;
			g.steamworks.reloading = 0;
//    `steam set player alive 1

			t.steamworks_health[g.steamworks.me] = g.steamworks.maxHealth;
			t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].health = g.steamworks.maxHealth;
			t.player[t.plrid].health = g.steamworks.maxHealth;
			g.steamworks.killedByPlayer = 0;
			g.steamworks.playedMyDeathAnim = 0;
	
			// courtesy of Ravey
			t.playercontrol.regenrate = 2;
			t.playercontrol.regenspeed = 100;
			t.playercontrol.regendelay = 3000;
			t.playercontrol.regentime = 3000;

			//  16032015 - 020 - MP Team code
			if (  g.steamworks.gameAlreadySpawnedBefore  ==  0 && g.steamworks.team  ==  0 ) 
			{
				t.tindex = g.steamworks.me+1;
	
				if (  t.steamworksmultiplayerstart[t.tindex].active == 1 ) 
				{
					t.terrain.playerx_f=t.steamworksmultiplayerstart[t.tindex].x;
					t.terrain.playery_f=t.steamworksmultiplayerstart[t.tindex].y+20;
					t.terrain.playerz_f=t.steamworksmultiplayerstart[t.tindex].z;
					t.terrain.playerax_f=0;
					t.terrain.playeray_f=t.steamworksmultiplayerstart[t.tindex].angle;
					t.terrain.playeraz_f=0;

					physics_resetplayer_core ( );
				}
				else
				{
					t.tfound = 0;
					t.ttempindex = t.tindex/2;
					if (  t.ttempindex > 0 ) 
					{
						if (  t.steamworksmultiplayerstart[t.ttempindex].active == 1 ) 
						{
							t.tfound = 1;
							t.terrain.playerx_f=t.steamworksmultiplayerstart[t.ttempindex].x;
							t.terrain.playery_f=t.steamworksmultiplayerstart[t.ttempindex].y+20;
							t.terrain.playerz_f=t.steamworksmultiplayerstart[t.ttempindex].z;
							t.terrain.playerax_f=0;
							t.terrain.playeray_f=t.steamworksmultiplayerstart[t.ttempindex].angle;
							t.terrain.playeraz_f=0;
	
							g.steamworks.lastx=t.terrain.playerx_f;
							g.steamworks.lasty=t.terrain.playery_f;
							g.steamworks.lastz=t.terrain.playerz_f;
							g.steamworks.lastangley=t.terrain.playeray_f;
						}
					}
					if (  t.tfound  ==  0 ) 
					{
						if (  t.steamworksmultiplayerstart[1].active == 1 ) 
						{
							t.tfound = 1;
							t.terrain.playerx_f=t.steamworksmultiplayerstart[1].x;
							t.terrain.playery_f=t.steamworksmultiplayerstart[1].y+20;
							t.terrain.playerz_f=t.steamworksmultiplayerstart[1].z;
							t.terrain.playerax_f=0;
							t.terrain.playeray_f=t.steamworksmultiplayerstart[1].angle;
							t.terrain.playeraz_f=0;

							g.steamworks.lastx=t.terrain.playerx_f;
							g.steamworks.lasty=t.terrain.playery_f;
							g.steamworks.lastz=t.terrain.playerz_f;
							g.steamworks.lastangley=t.terrain.playeray_f;
						}
					}
					if (  t.tfound  !=  0 ) 
					{
						physics_resetplayer_core ( );
					}
	
//      `endif

				}
			}
			else
			{
				t.tsteamnumberofmarkers = 0;
				for ( t.tc = 1 ; t.tc<=  STEAM_MAX_NUMBER_OF_PLAYERS; t.tc++ )
				{
					if (  t.steamworksmultiplayerstart[t.tc].active == 1 ) 
					{
						++t.tsteamnumberofmarkers;
					}
				}
				if (  g.steamworks.spawnrnd  ==  -1 && t.tsteamnumberofmarkers > 0  )  g.steamworks.spawnrnd  =  Rnd(t.tsteamnumberofmarkers-1)+1;
				//  13032015 0XX - Team Multiplayer
				if (  g.steamworks.team  ==  1 && g.steamworks.coop  ==  0 ) 
				{
					if (  t.tsteamnumberofmarkers  >=  8 ) 
					{
						if (  t.steamworks_team[g.steamworks.me]  ==  0 ) 
						{
							g.steamworks.spawnrnd = Rnd(4-1)+1;
						}
						else
						{
							g.steamworks.spawnrnd = Rnd(4-1)+1+4;
						}
					}
					if (  t.tsteamnumberofmarkers  ==  4 ) 
					{
						if (  t.steamworks_team[g.steamworks.me]  ==  0 ) 
						{
							g.steamworks.spawnrnd = Rnd(2-1)+1;
						}
						else
						{
							g.steamworks.spawnrnd = Rnd(2-1)+1+2;
						}
					}
					if (  t.tsteamnumberofmarkers  ==  2 ) 
					{
						if (  t.steamworks_team[g.steamworks.me]  ==  0 ) 
						{
							g.steamworks.spawnrnd = 0;
						}
						else
						{
							g.steamworks.spawnrnd = 1;
						}
					}
				}
				if (  t.tsteamnumberofmarkers  ==  1  )  g.steamworks.spawnrnd  =  0;
				steam_getPlaceToSpawn ( );
				if (  t.steamworksmultiplayerstart[g.steamworks.spawnrnd].active == 1 ) 
				{
					t.terrain.playerx_f=t.steamworksmultiplayerstart[g.steamworks.spawnrnd].x;
					t.terrain.playery_f=t.steamworksmultiplayerstart[g.steamworks.spawnrnd].y+20;
					t.terrain.playerz_f=t.steamworksmultiplayerstart[g.steamworks.spawnrnd].z;
					t.terrain.playerax_f=0;
					t.terrain.playeray_f=t.steamworksmultiplayerstart[g.steamworks.spawnrnd].angle;
					t.terrain.playeraz_f=0;
				}
				else
				{
					if (  t.steamworksmultiplayerstart[1].active == 1 ) 
					{
						t.terrain.playerx_f=t.steamworksmultiplayerstart[1].x;
						t.terrain.playery_f=t.steamworksmultiplayerstart[1].y+20;
						t.terrain.playerz_f=t.steamworksmultiplayerstart[1].z;
						t.terrain.playerax_f=0;
						t.terrain.playeray_f=t.steamworksmultiplayerstart[1].angle;
						t.terrain.playeraz_f=0;
					}
				}

				physics_resetplayer_core ( );

				SteamSetPlayerPositionX (  t.terrain.playerx_f );
				SteamSetPlayerPositionY (  t.terrain.playery_f );
				SteamSetPlayerPositionZ (  t.terrain.playerz_f );
				SteamSetPlayerAngle (  t.terrain.playeray_f );
			}
	
			g.steamworks.spawnrnd = -1;
	
			steam_getInitialPlayerCount ( );
	
			if (  g.steamworks.gameAlreadySpawnedBefore  ==  0 ) 
			{
				//  send our name on first respawn to ensure everyone gets it
				//  as this is the moment everyone is definately there
				//  steam can sometimes fail to get the name for a while
				//  so we will send it a few times
//     `if Timer() - steamworks.lastsendmynametime > 1000

//      `steamworks.lastsendmynametime = Timer()

					SteamSendMyName (  );
					g.steamworks.sentmyname = 1;
//     `endif


				if (  t.game.runasmultiplayer == 1 && g.steamworks.coop  ==  1 ) 
				{
					for ( t.e = 1 ; t.e<=  g.entityelementlist; t.e++ )
					{
						t.entid=t.entityelement[t.e].bankindex;
						if (  t.entid>0 ) 
						{
							if (  t.entityprofile[t.entid].ischaracter  ==  1 || t.entityelement[t.e].mp_isLuaChar  ==  1 ) 
							{
								t.entityelement[t.e].mp_coopControlledByPlayer = -1;
								t.entityelement[t.e].mp_coopLastTimeSwitchedTarget = 0;
//         `if entityelement(e).speedmodulator# < 1.0 then entityelement(e).speedmodulator#  ==  1.0

							}
						}
					}
				}

			}

			g.steamworks.realfirsttimespawn = 0;
			g.steamworks.gameAlreadySpawnedBefore = 1;
			for ( t.c = 0 ; t.c<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.c++ )
			{
					t.steamworks_forcePosition[t.c] = 1;
			}
			g.steamworks.respawnLeft = 0;

			for ( t.tc = 0 ; t.tc<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.tc++ )
			{
				t.steamworks_playingAnimation[t.tc] = STEAM_ANIMATION_NONE;
			}

		}
		//Much like mouse move x, calling get player damage amount will wipe it out after
		t.a=SteamGetPlayerDamageAmount();
		t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].eleprof.hasweapon = 0;
		g.steamworks.noplayermovement = 0;
		g.steamworks.invincibleTimer = Timer();
		g.steamworks.lastSpawnedTime = g.steamworks.invincibleTimer;


	}

return;

}

void steam_getPlaceToSpawn ( void )
{
	t.found = -1;
	
	if (  g.steamworks.team  ==  1  )  t.tdisttocheck  =  100; else t.tdisttocheck  =  300;
	
	//  check if the spawnpoint picked is clear, if it is, just use that
	t.failed = 0;
	for ( t.c = 0 ; t.c<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.c++ )
	{

		if (  t.c  !=  g.steamworks.me ) 
		{
			t.tpx_f = SteamGetPlayerPositionX(t.c);
			t.tpy_f = SteamGetPlayerPositionY(t.c);
			t.tpz_f = SteamGetPlayerPositionZ(t.c);

			t.tsx_f = t.steamworksmultiplayerstart[g.steamworks.spawnrnd].x;
			t.tsy_f = t.steamworksmultiplayerstart[g.steamworks.spawnrnd].y-50;
			t.tsz_f = t.steamworksmultiplayerstart[g.steamworks.spawnrnd].z;

			t.dx_f = t.tpx_f - t.tsx_f;
			t.dy_f = t.tpy_f - t.tsy_f;
			t.dz_f = t.tpz_f - t.tsz_f;

			t.dist_f = Sqrt((t.dx_f*t.dx_f)+(t.dy_f*t.dy_f)+(t.dz_f*t.dz_f));

			if (  t.dist_f < t.tdisttocheck  )  t.failed  =  1;
		}

	}

	//  is no one is here, lets use it
	if (  t.failed  ==  0  )  return;
	
	t.tstart = 1;
	t.tend = STEAM_MAX_NUMBER_OF_PLAYERS;

	if (  g.steamworks.team  ==  1 ) 
	{

		t.tsteamnumberofmarkers = 0;
		for ( t.tc = 1 ; t.tc<=  STEAM_MAX_NUMBER_OF_PLAYERS; t.tc++ )
		{
			if (  t.steamworksmultiplayerstart[t.tc].active == 1 ) 
			{
				++t.tsteamnumberofmarkers;
			}
		}

		if (  t.tsteamnumberofmarkers  >=  8 ) 
		{
			if (  t.steamworks_team[g.steamworks.me]  ==  0 ) 
			{
				t.tstart = 1;
				t.tend = 4;
			}
			else
			{
				t.tstart = 5;
				t.tend = 8;
			}
		}
		if (  t.tsteamnumberofmarkers  ==  4 ) 
		{
			if (  t.steamworks_team[g.steamworks.me]  ==  0 ) 
			{
				t.tstart = 1;
				t.tend = 2;
			}
			else
			{
				t.tstart = 3;
				t.tend = 4;
			}
		}
		if (  t.tsteamnumberofmarkers  <=  2 ) 
		{
			return;
		}
	}

	if (  g.steamworks.coop  ==  1 ) 
	{
		t.tstart = 1;
		t.tend = t.tsteamnumberofmarkers;
	}
	//  it failed so lets look for an alternative
	for ( t.tspawnpoints = t.tstart ; t.tspawnpoints<=  t.tend; t.tspawnpoints++ )
	{
		if (  t.tspawnpoints  !=  g.steamworks.spawnrnd ) 
		{
			t.failed = 0;
			if (  t.steamworksmultiplayerstart[t.tspawnpoints].active == 1 ) 
			{

				for ( t.c = 0 ; t.c<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.c++ )
				{

					if (  t.c  !=  g.steamworks.me ) 
					{
						t.tpx_f = SteamGetPlayerPositionX(t.c);
						t.tpy_f = SteamGetPlayerPositionY(t.c);
						t.tpz_f = SteamGetPlayerPositionZ(t.c);

						t.tsx_f = t.steamworksmultiplayerstart[t.tspawnpoints].x;
						t.tsy_f = t.steamworksmultiplayerstart[t.tspawnpoints].y-50;
						t.tsz_f = t.steamworksmultiplayerstart[t.tspawnpoints].z;

						t.dx_f = t.tpx_f - t.tsx_f;
						t.dy_f = t.tpy_f - t.tsy_f;
						t.dz_f = t.tpz_f - t.tsz_f;

						t.dist_f = Sqrt((t.dx_f*t.dx_f)+(t.dy_f*t.dy_f)+(t.dz_f*t.dz_f));

						if (  t.dist_f < t.tdisttocheck  )  t.failed  =  1;
					}

				}

				//  if noone is here lets use this
				if (  t.failed  ==  0 ) 
				{
					g.steamworks.spawnrnd = t.tspawnpoints;
					return;
				}
			}
		}
	}
	
return;

}

void steam_getInitialPlayerCount ( void )
{
	g.steamworks.howmanyjoinedatstart = 0;
	for ( t.c = 0 ; t.c<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.c++ )
	{
		t.tname_s = SteamGetOtherPlayerName(t.c);
		if (  t.tname_s != "Player"  )  ++g.steamworks.howmanyjoinedatstart;
	}
return;

}

void steam_nukeTestmap ( void )
{
		steam_deleteFile ("levelbank\\cfg.cfg");
		steam_deleteFile ("levelbank\\conkit.dat");
		steam_deleteFile ("levelbank\\header.dat");
		steam_deleteFile ("levelbank\\m.dat");
		steam_deleteFile ("levelbank\\map.ele");
		steam_deleteFile ("levelbank\\map.ent");
		steam_deleteFile ("levelbank\\map.way");
		steam_deleteFile ("levelbank\\playerconfig.dat");
		steam_deleteFile ("levelbank\\temparea.txt");
		steam_deleteFile ("levelbank\\vegmaskgrass.dat");
		steam_deleteFile ("levelbank\\visuals.ini");
		steam_deleteFile ("levelbank\\watermask.dds");
	
		steam_deleteFile ("editors\\gridedit\\__multiplayerlevel__.fpm");
		steam_deleteFile ("editors\\gridedit\\__multiplayerworkshopitemid__.dat");
	
return;

}

void steam_respawnEntities ( void )
{
	if (  g.steamworks.destroyedObjectCount > 0 ) 
	{
		for ( t.i = 0 ; t.i<=  g.steamworks.destroyedObjectCount-1; t.i++ )
		{
			t.e = t.steamworks_destroyedObjectList[t.i];
			t.entityelement[t.e].active = 1;
			entity_lua_spawn ( );
			entity_lua_collisionon ( );
			t.entityelement[t.e].lua.firsttime=0;
			t.entityelement[t.e].activated = 0;
			t.entityelement[t.e].collected = 0;
			t.entityelement[t.e].explodefusetime = 0;
			StopObject (  t.entityelement[t.e].obj );
			SetObjectFrame (  t.entityelement[t.e].obj,0 );
			ShowObject (  t.entityelement[t.e].obj );
		}
	}
	for ( t.e = 1 ; t.e<=  g.entityelementlist; t.e++ )
	{
		t.ttentid=t.entityelement[t.e].bankindex;
		if (  t.entityprofile[t.ttentid].strength > 0 || t.entityelement[t.e].activated  !=  0 || t.entityelement[t.e].collected  !=  0 || t.entityelement[t.e].eleprof.strength > 0 ) 
		{
			if (  t.entityelement[t.e].obj > 0 && t.entityelement[t.e].staticflag  ==  0 ) 
			{
				if (  ObjectExist(t.entityelement[t.e].obj)  ==  1 ) 
				{

					t.entityelement[t.e].lua.flagschanged=1;
					if (  g.steamworks.coop  ==  0 ) 
					{
						entity_lua_spawn ( );
					}
					else
					{
						entity_lua_spawn_core ( );
					}
					t.entityelement[t.e].lua.firsttime=0;
					//120916 - seems collisionON sets SetObjectCollisionProperty to 0 (needed for exploding barrel) - left as Dave coded it just in case
					if ( t.entityelement[t.e].eleprof.explodable  ==  0 ) 
					{
						entity_lua_collisionon ( );
					}
					else
					{
						t.tphyobj = t.entityelement[t.e].obj;
						t.entid = t.ttentid;
						physics_setupobject ( );
					}
					t.aisystem.cumilativepauses=0;
					t.entityelement[t.e].mp_updateOn = 0;
					t.entityelement[t.e].mp_coopControlledByPlayer = -1;
					t.entityelement[t.e].active = 1;
					t.entityelement[t.e].activated = 0;
					t.entityelement[t.e].collected = 0;
					t.entityelement[t.e].explodefusetime = 0;
					t.entityelement[t.e].health = t.entityelement[t.e].eleprof.strength;
					StopObject (  t.entityelement[t.e].obj );
					SetObjectFrame (  t.entityelement[t.e].obj,0 );
				}
			}
		}
	}
	g.steamworks.destroyedObjectCount = 0;
}

void steam_addDestroyedObject ( void )
{
	//  if (  it has a quantity  )  we will respawn it after so much time has passed
	if (  t.entityelement[t.e].eleprof.quantity > 0 ) 
	{
		steam_add_respawn_timed ( );
	}
	if (  g.steamworks.destroyedObjectCount < STEAM_DESTROYED_OBJECT_LIST_SIZE ) 
	{
		t.steamworks_destroyedObjectList[g.steamworks.destroyedObjectCount] = t.e;
		++g.steamworks.destroyedObjectCount;
	}
return;

}

void steam_add_respawn_timed ( void )
{
	for ( t.i = 0 ; t.i<=  STEAM_RESPAWN_TIME_OBJECT_LIST_SIZE; t.i++ )
	{
			if (  t.steamworks_respawn_timed[t.i].inuse  ==  0 ) 
			{
				t.steamworks_respawn_timed[t.i].inuse = 1;
				t.steamworks_respawn_timed[t.i].e = t.e;
				t.steamworks_respawn_timed[t.i].time = Timer();
				break;
			}
	}
return;

}

void steam_setLuaPlayerNames ( void )
{
	for ( t.i = 0 ; t.i<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.i++ )
	{
		if (  t.i  ==  g.steamworks.me ) 
		{
			t.tsteamname_s = g.steamworks.playerName;
		}
		else
		{
			t.tsteamname_s = SteamGetOtherPlayerName(t.i);
		}
		//  ensure the string isnt null before doing anything to it
		if (  t.tsteamname_s  !=  "" ) 
		{
			t.tsteamnameNoApos_s = "";
			for ( t.tloop = 1 ; t.tloop<=  Len(t.tsteamname_s.Get()); t.tloop++ )
			{
				if (  cstr(Mid(t.tsteamname_s.Get(),t.tloop))  !=  "'" && cstr(Mid(t.tsteamname_s.Get(),t.tloop))  !=  Chr(34) ) 
				{
					t.tsteamnameNoApos_s = t.tsteamnameNoApos_s + Mid(t.tsteamname_s.Get(),t.tloop);
				}
				else
				{
					t.tsteamnameNoApos_s = t.tsteamnameNoApos_s + "_";
				}
			}
			t.tnothing = LuaExecute( cstr(cstr("mp_playerNames[") + Str(t.i+1) + "] = '" + t.tsteamnameNoApos_s + "'").Get() );
			if (  t.tsteamname_s  ==  "Player" || t.tsteamname_s  ==  "" ) 
			{
				t.tnothing = LuaExecute( cstr(cstr("mp_playerConnected[") + Str(t.i+1) + "] = 0").Get() );
			}
			else
			{
				t.tnothing = LuaExecute( cstr(cstr("mp_playerConnected[") + Str(t.i+1) + "] = 1").Get() );
			}
		}
	}
return;

}

void steam_setLuaResetStats ( void )
{
	for ( t.i = 0 ; t.i<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.i++ )
	{
		t.tnothing = LuaExecute( cstr(cstr("mp_playerKills[") + Str(t.i+1) + "] = 0").Get() );
		t.tnothing = LuaExecute( cstr(cstr("mp_playerDeaths[") + Str(t.i+1) + "] = 0").Get() );
		t.tnothing = LuaExecute( cstr(cstr("mp_playerNames[") + Str(t.i+1) + "] = ''").Get() );
		t.tnothing = LuaExecute( cstr(cstr("mp_playerConnected[") + Str(t.i+1) + "] = 0").Get() );
		t.steamworks_kills[t.i] = 0;
		t.steamworks_deaths[t.i] = 0;
	}
	for ( t.i = 0 ; t.i<=  STEAM_RESPAWN_TIME_OBJECT_LIST_SIZE; t.i++ )
	{
		t.steamworks_respawn_timed[t.i].inuse = 0;
	}

	for ( t.e = 1 ; t.e<=  g.entityelementlist; t.e++ )
	{
			t.entityelement[t.e].mp_networkkill = 0;
	}

	t.tsteamwasnetworkdamage = 0;

return;

}

void steam_updatePlayerInput ( void )
{

//  `print "RELOADING: " + Str(plrreloading)

	
	if (  t.playercontrol.plrhitfloormaterial  ==  0 ) 
	{
		if (  g.steamworks.oldfootfloortime  ==  0  )  g.steamworks.oldfootfloortime  =  Timer();
		if (  Timer()-g.steamworks.oldfootfloortime > 100  )  g.steamworks.footfloor  =  0;
	}
	else
	{
		g.steamworks.oldfootfloortime = 0;
		g.steamworks.footfloor = 1;
	}
	
	if (  g.plrreloading  ==  0 ) 
	{
		g.steamworks.reloadingCount = 0;
	}

	t.tTime = Timer();
	
	if (  t.tTime - g.steamworks.lastSendTimeAppearance > STEAM_APPEARANCE_UPDATE_DELAY ) 
	{
	
		if (  g.plrreloading  !=  0 ) 
		{
			++g.steamworks.reloadingCount;
			if (  g.steamworks.reloadingCount < 4 ) 
			{
				g.steamworks.reloading = 1;
			}
			else
			{
				g.steamworks.reloading = 0;
			}
		}
	
		g.steamworks.lastSendTimeAppearance = t.tTime;
		if (  t.playercontrol.jetpackmode  !=  2 && g.steamworks.reloading  ==  0 ) 
		{
			SteamSetPlayerAppearance (  g.steamworks.appearance );
		}
		else
		{
			if (  t.playercontrol.jetpackmode  !=  0 ) 
			{
				if (  g.steamworks.footfloor  ==  1 ) 
				{
					SteamSetPlayerAppearance (  101 );
				}
				else
				{
					SteamSetPlayerAppearance (  102 );
				}
				g.steamworks.reloading = 0;
			}
			else
			{
				if (  g.steamworks.reloading  ==  1 ) 
				{
						SteamSetPlayerAppearance (  201 );
				}
				if (  g.plrreloading  ==  0 ) 
				{
					g.steamworks.reloading = 0;
				}
			}
		}
//   `print "sending appearance update"

	}
	
	if (  t.tTime - g.steamworks.lastSendTime < STEAM_INPUT_UPDATE_DELAY  )  return;
	
	g.steamworks.lastSendTime = t.tTime;
	
//  `print "sending input update"

	
	if (  g.steamworks.meleeOn  ==  0 ) 
	{
		if (  KeyState(g.keymap[17])  ==  1 || KeyState(g.keymap[200])  ==  1 ) 
		{
			SteamSetKeyState (  17,1 );
		}
		else
		{
			SteamSetKeyState (  17,0 );
		}
		if (  KeyState(g.keymap[31])  ==  1 || KeyState(g.keymap[208])  ==  1 ) 
		{
			SteamSetKeyState (  31,1 );
		}
		else
		{
			SteamSetKeyState (  31,0 );
		}
		if (  KeyState(g.keymap[30])  ==  1 || KeyState(g.keymap[203])  ==  1 ) 
		{
			SteamSetKeyState (  30,1 );
		}
		else
		{
			SteamSetKeyState (  30,0 );
		}
		if (  KeyState(g.keymap[32])  ==  1 || KeyState(g.keymap[205])  ==  1 ) 
		{
			SteamSetKeyState (  32,1 );
		}
		else
		{
			SteamSetKeyState (  32,0 );
		}
	}
if (  KeyState(g.keymap[46])  ==  1 || KeyState(g.keymap[29])  ==  1 || KeyState(g.keymap[157])  ==  1 ) 
{
	SteamSetKeyState (  46,1 );
	g.steamworks.crouchOn = 1;
}
else
{
	SteamSetKeyState (  46,0 );
	g.steamworks.crouchOn = 0;
}
//  shift keys
if (  KeyState(g.keymap[42])  ==  1 || KeyState(g.keymap[54])  ==  1 ) 
{
	SteamSetKeyState (  42,1 );
}
else
{
	SteamSetKeyState (  42,0 );
}

return;

}

void steam_load_guns ( void )
{

/*       Debug info
for ( t.tgindex = 1 ; t.tgindex<=  g.gunmax; t.tgindex++ )
	if (  t.gun[t.tgindex].activeingame == 1 ) 
	{
		t.tweaponname_s=t.gun[t.tgindex].t.name_s;
		tactiveGun = t.tgindex;
		Print (  t.tweaponname_s );
	}
}
*/    

g.steamworks.gunCount = 0;

	//  all vweaps (that are active)
	for ( t.tgindex = 1 ; t.tgindex<=  g.gunmax; t.tgindex++ )
	{
		if (  t.gun[t.tgindex].activeingame == 1 ) 
		{
			t.tweaponname_s=t.gun[t.tgindex].name_s;
			if (  t.tweaponname_s != "" ) 
			{

				//  go and load this gun (attached to calling entity instance)
				t.ttobj=g.steamworks.gunCount+g.steamplayermodelsoffset;
				t.steamworks_gunobj[g.steamworks.gunCount] = t.ttobj;
				t.steamworks_gunname[g.steamworks.gunCount] = Lower(t.tweaponname_s.Get());
				++g.steamworks.gunCount;
				if (  ObjectExist(t.ttobj) == 1  )  DeleteObject (  t.ttobj );

				//  replaced X file load with optional DBO convert/load
				t.tfile_s=cstr("gamecore\\guns\\")+t.tweaponname_s+"\\vweap.x";
				deleteOutOfDateDBO(t.tfile_s.Get());
				if (  cstr(Lower(Right(t.tfile_s.Get(),2))) == ".x"  )  t.tdbofile_s = cstr(Left(t.tfile_s.Get(),Len(t.tfile_s.Get())-2))+cstr(".dbo"); else t.tdbofile_s = "";
				if (  FileExist(t.tfile_s.Get()) == 1 || FileExist(t.tdbofile_s.Get()) == 1 ) 
				{
					if (  FileExist(t.tdbofile_s.Get()) == 1 ) 
					{
						t.tfile_s=t.tdbofile_s;
						t.tdbofile_s="";
					}
					LoadObject (  t.tfile_s.Get(),t.ttobj );
					SetObjectFilter (  t.ttobj,2 );
					if ( Len(t.tdbofile_s.Get())>1 ) 
					{
						if ( FileExist( t.tdbofile_s.Get()) == 0 ) 
						{
							// unnecessary now as LoadObject auto creates DBO file!
							SaveObject ( t.tdbofile_s.Get(), t.ttobj );
						}
						if (  FileExist(t.tdbofile_s.Get()) == 1 ) 
						{
							DeleteObject (  t.ttobj );
							LoadObject (  t.tdbofile_s.Get(),t.ttobj );
							SetObjectFilter (  t.ttobj,2 );
							t.tfile_s=t.tdbofile_s;
						}
					}
				}
				else
				{
					MakeObjectTriangle (  t.ttobj,0,0,0,0,0,0,0,0,0 );
				}

				//  Apply object settings
				SetObjectTransparency (  t.ttobj,1 );
				SetObjectCollisionOff (  t.ttobj );
				HideObject (  t.ttobj );

				//  VWEAP is NOT part of collision universe (prevents rocket hitting launcher)
				SetObjectCollisionProperty (  t.ttobj,1 );

				//  apply texture to vweap
				if (  g.gdividetexturesize == 0 ) 
				{
					t.texuseid=loadinternaltexture("effectbank\\reloaded\\media\\white_D.dds");
				}
				else
				{
					t.texuseid=loadinternaltexture( cstr(cstr("gamecore\\guns\\")+t.tweaponname_s+"\\gun_D.dds").Get() );
				}
				TextureObject (  t.ttobj,0,t.texuseid );
				t.texuseid=loadinternaltexture( cstr(cstr("gamecore\\guns\\")+t.tweaponname_s+"\\gun_N.dds").Get() );
				TextureObject (  t.ttobj,1,loadinternaltexture( "effectbank\\reloaded\\media\\blank_O.dds" ));
				TextureObject (  t.ttobj,2,t.texuseid );
				t.texuseid=loadinternaltexture( cstr(cstr("gamecore\\guns\\")+t.tweaponname_s+"\\gun_S.dds").Get() );
				TextureObject (  t.ttobj,3,t.texuseid );
				TextureObject (  t.ttobj,4,t.terrain.imagestartindex );
				TextureObject (  t.ttobj,5,g.postprocessimageoffset+5 );
				TextureObject (  t.ttobj,6,loadinternaltexture( "effectbank\\reloaded\\media\\blank_I.dds") );

				//  Apply entity shader to vweap model
				t.teffectid=loadinternaleffect("effectbank\\reloaded\\entity_basic.fx");
				SetObjectEffect (  t.ttobj,t.teffectid );

				//  07032015 - 016 - ensure the gun orders are the same on all machines
				for ( t.i = 0 ; t.i<=  g.steamworks.gunCount-2; t.i++ )
				{
					for ( t.j = t.i ; t.j<=  g.steamworks.gunCount-1; t.j++ )
					{
						if (  SteamStrCmp(t.steamworks_gunname[t.i].Get(),t.steamworks_gunname[t.j].Get()) > 0 ) 
						{
							t.ttemp_s = t.steamworks_gunname[t.i];
							t.steamworks_gunname[t.i] = t.steamworks_gunname[t.j];
							t.steamworks_gunname[t.j]=t.ttemp_s;

							t.ttemp = t.steamworks_gunobj[t.i];
							t.steamworks_gunobj[t.i] = t.steamworks_gunobj[t.j];
							t.steamworks_gunobj[t.j]=t.ttemp;
						}
					}
				} 
			}

		}
		SteamLoop (  );
	}
}

void steam_check_for_attachments ( void )
{

	for ( t.c = 0 ; t.c<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.c++ )
	{
		if (  t.c  !=  g.steamworks.me ) 
		{
//    `if SteamGetPlayerAppearance(c) < 100 then Print (  steamworks_gunname(SteamGetPlayerAppearance(c)) )
			
			//  Jetpack
			if (  SteamGetPlayerAppearance(t.c)  !=  t.steamworks_oldAppearance[t.c] ) 
			{
				t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_NONE;
				if (  SteamGetPlayerAppearance(t.c)  ==  101 || SteamGetPlayerAppearance(t.c)  ==  102 ) 
				{
					t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_NONE;
					t.e = t.steamworks_playerEntityID[t.c];
					entity_freeattachment ( );

					if (  t.steamworks_jetpackparticles[t.c]  ==  -1 && SteamGetPlayerAppearance(t.c)  ==  102 ) 
					{
						steam_addJetpackParticles ( );
					}
					if (  SteamGetPlayerAppearance(t.c)  ==  101 && t.steamworks_jetpackparticles[t.c]  !=  -1 ) 
					{
							t.tRaveyParticlesEmitterID=t.steamworks_jetpackparticles[t.c];
							ravey_particles_delete_emitter ( );
							t.steamworks_jetpackparticles[t.c]=-1;
					}

					if (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj > 0 ) 
					{
						DeleteObject (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj );
						t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_NONE;
						t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj = 0;
					}

//      `steamworks_oldAppearance(c) = SteamGetPlayerAppearance(c)

					if (  ObjectExist(g.steamplayermodelsoffset+t.c+121)  ==  0 ) 
					{
						if (  t.playercontrol.jetobjtouse > 0 ) 
						{
							if (  ObjectExist (g.steamplayermodelsoffset+120)  ) 
							{
								CloneObject (  g.steamplayermodelsoffset+t.c+121,g.steamplayermodelsoffset+120 );
							}
						}
					}
				}
			}
			if (  SteamGetPlayerAppearance(t.c)  !=  101 && SteamGetPlayerAppearance(t.c)  !=  102 ) 
			{
				if (  t.steamworks_jetpackparticles[t.c]  !=  -1 ) 
				{
					t.tRaveyParticlesEmitterID=t.steamworks_jetpackparticles[t.c];
					ravey_particles_delete_emitter ( );
					t.steamworks_jetpackparticles[t.c] = -1;
				}
			}
			//  Gun
			if (  SteamGetPlayerAppearance(t.c)  !=  t.steamworks_oldAppearance[t.c] && SteamGetPlayerAppearance(t.c) < 101 ) 
			{
				t.steamworks_playingAnimation[t.c] = STEAM_ANIMATION_NONE;
				if (  ObjectExist(g.steamplayermodelsoffset+t.c+121)  ==  1 ) 
				{
					HideObject (  g.steamplayermodelsoffset+t.c+121 );
				}
				t.e = t.steamworks_playerEntityID[t.c];
				entity_freeattachment ( );
				if (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj > 0 ) 
				{
					DeleteObject (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj );
					t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj = 0;
				}
//     `steamworks_oldAppearance(c) = SteamGetPlayerAppearance(c)

				if (  ObjectExist(g.steamplayermodelsoffset+t.c+100)  ==  0 ) 
				{

					t.tobj = 0;
					if (  SteamGetPlayerAppearance(t.c) > 0 ) 
					{
						t.tobj = t.steamworks_gunobj[SteamGetPlayerAppearance(t.c)-1];
					}

					if (  t.tobj > 0 ) 
					{
						if (  ObjectExist(t.tobj)  ==  1 ) 
						{
							CloneObject (  g.steamplayermodelsoffset+t.c+100,t.tobj );
							ShowObject (  g.steamplayermodelsoffset+t.c+100 );
							SetObjectMask (  g.steamplayermodelsoffset+t.c+100,1 );
							t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj = g.steamplayermodelsoffset+t.c+100;

							t.tfound = 0;
							for ( t.tgindex = 1 ; t.tgindex<=  g.gunmax; t.tgindex++ )
							{
								if (  t.gun[t.tgindex].activeingame == 1 ) 
								{
									if (  t.steamworks_gunname[SteamGetPlayerAppearance(t.c)-1] == Lower(t.gun[t.tgindex].name_s.Get()) ) 
									{
										t.tfound = t.tgindex;
									}
								}
							}

							t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon = t.tfound;

						//  Find firespot for this vweap
						t.entityelement[t.e].attachmentobjfirespotlimb=0;
						PerformCheckListForLimbs (  t.tobj );
						for ( t.tc = 1 ; t.tc<=  ChecklistQuantity(); t.tc++ )
						{
							if (  cstr(Lower(ChecklistString(t.tc))) == "firespot" ) 
							{
								t.entityelement[t.e].attachmentobjfirespotlimb=t.tc-1;
								t.tc=ChecklistQuantity()+1;
							}
						}

						}
					}
					else
					{
						if (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj > 0 ) 
						{
							if (  ObjectExist(t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj) ) 
							{
								DeleteObject (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj );
								t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon = 0;
							}
						}
					}

				}
			}

			//  update jetpack appearance
			if (  SteamGetPlayerAppearance(t.c)  ==  101 || SteamGetPlayerAppearance(t.c)  ==  102 ) 
			{
				if (  ObjectExist(g.steamplayermodelsoffset+t.c+121)  ==  1 ) 
				{
					ShowObject (  g.steamplayermodelsoffset+t.c+121 );
					t.tobj = t.entityelement[t.steamworks_playerEntityID[t.c]].obj;
					if (  SteamGetKeyState(t.c,46)  ==  1 ) 
					{
						PositionObject (  g.steamplayermodelsoffset+t.c+121, ObjectPositionX(t.tobj), ObjectPositionY(t.tobj)+20, ObjectPositionZ(t.tobj) );
					}
					else
					{
						PositionObject (  g.steamplayermodelsoffset+t.c+121, ObjectPositionX(t.tobj), ObjectPositionY(t.tobj)+40, ObjectPositionZ(t.tobj) );
					}
					YRotateObject (  g.steamplayermodelsoffset+t.c+121,ObjectAngleY(t.tobj) );
				}
			}

			//  update gun appearance
			if (  t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj > 0 ) 
			{
				t.e = t.steamworks_playerEntityID[t.c];
				entity_controlattachments ( );

				if (  t.steamworks_playerShooting[t.c]  ==  1 ) 
				{
					t.tgunid=t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
					t.tattachedobj=t.entityelement[t.steamworks_playerEntityID[t.c]].attachmentobj;
					t.te = t.steamworks_playerEntityID[t.c];

					t.tgunid = t.entityelement[t.steamworks_playerEntityID[t.c]].eleprof.hasweapon;
					t.ttrr=Rnd(1);
					for ( t.tt = t.ttrr+0 ; t.tt<=  t.ttrr+1; t.tt++ )
					{
						t.ttsnd=t.gunsoundcompanion[t.tgunid][1][t.tt].soundid;
						if (  t.ttsnd>0 ) 
						{
							if (  SoundExist(t.ttsnd) == 1 ) 
							{
								if (  SoundPlaying(t.ttsnd) == 0 || t.tt == t.ttrr+1 ) 
								{
									t.charanimstate.firesoundindex=t.ttsnd ; t.tt=3;
									t.charanimstate.firesoundexpiry=Timer()+200;
								}
							}
						}
					}

					if (  t.charanimstate.firesoundindex>0 ) 
					{
						darkai_shooteffect ( );
					}
				}

			}
		}

		if (  t.steamworks_oldAppearance[t.c]  !=  SteamGetPlayerAppearance(t.c)  )  t.steamworks_playingAnimation[t.c]  =  STEAM_ANIMATION_NONE;
		t.steamworks_oldAppearance[t.c] = SteamGetPlayerAppearance(t.c);

	}


//  `print "-------------------------------------------------------"

//  `print entityelement(steamworks_playerEntityID(1)).attachmentobj

//  `print "active " + Str(entityelement(steamworks_playerEntityID(1)).active)

//  `print "beenkilled " + Str(entityelement(steamworks_playerEntityID(1)).beenkilled)

//  `print GetVisible(99999)


return;

}

void steam_addJetpackParticles ( void )
{

	t.tpartObj = t.entityelement[t.steamworks_playerEntityID[t.c]].obj;

	ravey_particles_get_free_emitter ( );
	if (  t.tResult>0 ) 
	{
		t.steamworks_jetpackparticles[t.c]=t.tResult;
		g.tEmitter.id = t.tResult;
		g.tEmitter.emitterLife = 0;
		g.tEmitter.parentObject = t.tpartObj;
		g.tEmitter.parentLimb = 0;
		g.tEmitter.isAnObjectEmitter = 0;
		g.tEmitter.imageNumber = RAVEY_PARTICLES_IMAGETYPE_LIGHTSMOKE + g.particlesimageoffset;
		g.tEmitter.isAnimated = 1;
		g.tEmitter.animationSpeed = 1/64.0;
		g.tEmitter.isLooping = 1;
		g.tEmitter.frameCount = 64;
		g.tEmitter.startFrame = 0;
		g.tEmitter.endFrame = 63;
		g.tEmitter.startsOffRandomAngle = 1;
		g.tEmitter.offsetMinX = -20;
		g.tEmitter.offsetMinY = 50;
		g.tEmitter.offsetMinZ = -20;
		g.tEmitter.offsetMaxX = 20;
		g.tEmitter.offsetMaxY = 50;
		g.tEmitter.offsetMaxZ = 20;
		g.tEmitter.scaleStartMin = 5;
		g.tEmitter.scaleStartMax = 10;
		g.tEmitter.scaleEndMin = 90;
		g.tEmitter.scaleEndMax = 100;
		g.tEmitter.movementSpeedMinX = -0.1f;
		g.tEmitter.movementSpeedMinY = -0.9f;
		g.tEmitter.movementSpeedMinZ = -0.1f;
		g.tEmitter.movementSpeedMaxX = 0.1f;
		g.tEmitter.movementSpeedMaxY = -0.1f;
		g.tEmitter.movementSpeedMaxZ = 0.1f;
		g.tEmitter.rotateSpeedMinZ = -0.1f;
		g.tEmitter.rotateSpeedMaxZ = 0.1f;
		g.tEmitter.startGravity = 0;
		g.tEmitter.endGravity = 0;
		g.tEmitter.lifeMin = 1000;
		g.tEmitter.lifeMax = 2000;
		g.tEmitter.alphaStartMin = 40;
		g.tEmitter.alphaStartMax = 75;
		g.tEmitter.alphaEndMin = 0;
		g.tEmitter.alphaEndMax = 0;
		g.tEmitter.frequency = 25;
		ravey_particles_add_emitter ( );
	}

return;

}

void steam_NearOtherPlayers ( void )
{

	for ( t.c = 0 ; t.c<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.c++ )
	{
		if (  t.c  !=  g.steamworks.me ) 
		{
				t.tobj = t.entityelement[t.steamworks_playerEntityID[t.c]].obj;
				if (  t.tobj > 0 ) 
				{
					if (  ObjectExist(t.tobj) ) 
					{
						if (  SteamGetPlayerAlive(t.c)  ==  1 ) 
						{
							t.tplrproxx_f=CameraPositionX()-ObjectPositionX(t.tobj);
							if (  g.steamworks.crouchOn  ==  0 ) 
							{
								t.tplrproyy_f=(CameraPositionY()-64)-ObjectPositionY(t.tobj);
							}
							else
							{
								t.tplrproyy_f=(CameraPositionY()-64+30)-ObjectPositionY(t.tobj);
							}
							t.tplrproxz_f=CameraPositionZ()-ObjectPositionZ(t.tobj);
							t.tplrproxd_f=Sqrt(abs(t.tplrproxx_f*t.tplrproxx_f)+abs(t.tplrproyy_f*t.tplrproyy_f)+abs(t.tplrproxz_f*t.tplrproxz_f));
							t.tplrproxa_f=atan2deg(t.tplrproxx_f,t.tplrproxz_f);
							if (  t.tplrproxd_f<50.0 ) 
							{
								t.playercontrol.pushforce_f=0.5;
								t.playercontrol.pushangle_f=t.tplrproxa_f;
							}
						}
					}
				}
		}
	}

return;

}

void steam_check_respawn_objects ( void )
{
	t.tTime = Timer();
	for ( t.i = 0 ; t.i<=  STEAM_RESPAWN_TIME_OBJECT_LIST_SIZE; t.i++ )
	{
			if (  t.steamworks_respawn_timed[t.i].inuse  ==  1 ) 
			{
				if (  t.tTime - t.steamworks_respawn_timed[t.i].time > STEAM_RESPAWN_TIME_DELAY ) 
				{
					t.steamworks_respawn_timed[t.i].inuse = 0;

					t.e = t.steamworks_respawn_timed[t.i].e;
					t.entityelement[t.e].active = 1;
					entity_lua_spawn ( );
					entity_lua_collisionon ( );
					t.entityelement[t.e].activated = 0;
					t.entityelement[t.e].collected = 0;
					StopObject (  t.entityelement[t.e].obj );
					SetObjectFrame (  t.entityelement[t.e].obj,0 );
					ShowObject (  t.entityelement[t.e].obj );

				}
			}
	}
return;

}

void steam_checkForEveryoneLeft ( void )
{
		if (  g.steamworks.howmanyjoinedatstart > 1 ) 
		{
			t.tsteamhowmanynow = 0;
			for ( t.tcount = 0 ; t.tcount<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.tcount++ )
			{
				t.tname_s = SteamGetOtherPlayerName(t.tcount);
				if (  t.tname_s != "Player"  )  ++t.tsteamhowmanynow;
			}

			if (  t.tsteamhowmanynow  <= 1 ) 
			{
				t.tsteamlostconnectioncustommessage_s = "Everyone else left the game! (Code MP014)";
				g.steamworks.backtoeditorforyou = 1;
				steam_lostConnection ( );
				return;
			}
		}
return;

}

void steam_lostConnection ( void )
{
		t.tTime = Timer();
		editor_hideall3d ( );
		SetDir (  cstr(g.fpscrootdir_s + "\\Files").Get() );
		if (  t.tsteamconnectionlostmessage_s  ==  "GAMEOVER"  )  g.steamworks.backtoeditorforyou  =  1;
		t.tsteamconnectionlostmessage_s = "Lost connection to server";
		if (  t.tsteamlostconnectioncustommessage_s != ""  )  t.tsteamconnectionlostmessage_s  =  t.tsteamlostconnectioncustommessage_s;
		while (  Timer() - t.tTime < 5000 ) 
		{
			Cls (  );
			steam_text(-1,30,3,t.tsteamconnectionlostmessage_s.Get());
			if (  t.tsteamconnectionlostmessage_s  ==  "Could not build workshop item (Error MP015)" ) 
			{
				steam_text(-1,40,3,"The workshop item did not upload to Steam");
				steam_text(-1,45,3,"Please t.try again in t.a few moments.");
				steam_text(-1,50,3,"If the problem persists t.try closing");
				steam_text(-1,55,3,"Game Guru and restarting Steam.");
			}
			SteamLoop (  );
			Sync (  );
		}
		t.tsteamlostconnectioncustommessage_s = "";
//steam_free_game ( );
		steam_setMessage ( );
		if (  g.steamworks.mode  ==  STEAM_IN_GAME_CLIENT || g.steamworks.mode  ==  STEAM_IN_GAME_SERVER || g.steamworks.backtoeditorforyou > 0 ) 
		{
			steam_resetGameStats ( );
			if (  g.steamworks.backtoeditorforyou  !=  2 ) 
			{
				steam_setLuaResetStats ( );
			}
			else
			{
				g.steamworks.goBackToEditor = 1;
			}
		}
		g.steamworks.backtoeditorforyou = 0;
		steam_resetGameStats ( );
		steam_quitGame ( );
}

void steam_gameLoop ( void )
{

//  check we have finished loading, if not exit out
if (  g.steamworks.finishedLoadingMap  ==  0  )  return;

//  HideMouse (  when menu finished )
if (  t.thaveShownMouse >0 ) 
{
	game_hidemouse ( );
	--t.thaveShownMouse;
}

steam_updateAIForCOOP ( );
steam_howManyEnemiesLeftToKill ( );

//  some debug stuff
// `print "destroycount = " + Str(tempsteamdestroycount)

// `if UpKey() then tempsteamdestroycount  ==  0

// `if guntimercount  ==  0 then guntimercount  ==  6

//  `print guntimercount

//  `print ttemppotato

//  `print gunmode


	steam_NearOtherPlayers ( );

	//  if we have lost connection, head back to main menu
	t.tconnectionStatus = SteamGetClientServerConnectionStatus();
	if (  t.tconnectionStatus  ==  0 ) 
	{
		t.tsteamconnectionlostmessage_s = "GAMEOVER";
		steam_lostConnection ( );
		return;
	}
	if (  t.tconnectionStatus  ==  2 ) 
	{
		t.tsteamconnectionlostmessage_s = "GAMEOVER";
		t.tsteamlostconnectioncustommessage_s = "Game Over. The host closed the server.";
		steam_lostConnection ( );
		return;
	}

	steam_lua ( );
	steam_setLuaPlayerNames ( );
	steam_check_respawn_objects ( );

//steam_checkVoiceChat ( );

	if (  Timer() - g.steamworks.showscoresdelay > 2000 ) 
	{
		if (  KeyState(g.keymap[15])  ==  1 && g.steamworks.chaton  ==  0 ) 
		{
			t.tnothing = LuaExecute("mp_showscores = 1");
			g.steamworks.showscoresdelay = Timer();
		}
		else
		{
			t.tnothing = LuaExecute("mp_showscores = 0");
			g.steamworks.showscoresdelay = -2000;
		}
	}

	//  some debug info
//  `for a = 0 to STEAM_MAX_NUMBER_OF_PLAYERS-1

	// Print (  "player objects" )
	// Print (  entityelement(steamworks_playerEntityID(a)).obj )
//  `next a


	//  Find out which index we are (server will always be 0)
	g.steamworks.me = SteamGetMyPlayerIndex();
	//  Hide our own player model but show everyone elses
	//  TO DO; if a player has d/c or never joined, need to hide their model rather than show a zombie standing there doing nothing
	for ( t.a = 0 ; t.a<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.a++ )
	{
			if (  t.steamworks_playingRagdoll[t.a]  ==  1 && SteamGetPlayerAlive(t.a)  ==  1 ) 
			{
				t.steamworks_playingRagdoll[t.a] = 0;
				t.tphyobj=t.entityelement[t.steamworks_playerEntityID[t.a]].obj;
				ragdoll_destroy ( );
				RotateObject (  t.entityelement[t.steamworks_playerEntityID[t.a]].obj,0,180,0 );
				FixObjectPivot (  t.entityelement[t.steamworks_playerEntityID[t.a]].obj );
				t.e = t.steamworks_playerEntityID[t.a];
				t.entityelement[t.e].health=g.steamworks.maxHealth;
				//  set appearance back to default so they repick the gun up they had before
				t.steamworks_oldAppearance[t.a] = 0;
				t.steamworks_playingAnimation[t.a] = STEAM_ANIMATION_NONE;
			}
			if (  t.a  ==  g.steamworks.me ) 
			{
				if (  t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj > 0 ) 
				{
					if (  ObjectExist(t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj) ) 
					{
						HideObject (  t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj );
//       `position object entityelement(steamworks_playerEntityID(steamworks.me)).obj,100000,100000,100000


//       `tsentid = entityelement(steamworks_playerEntityID(steamworks.me)).bankindex

//       `tccobj = charactercreatorrmodelsoffset+((tsentid*3)-characterkitcontrol.offset)

//       `if ObjectExist(tccobj)=1

//        `unglue object tccobj

//        `position object tccobj,100000,100000,100000

//       `endif

//       `if ObjectExist(tccobj+1)=1

//        `unglue object tccobj+1

//        `position object tccobj+1,100000,100000,100000

//       `endif

//       `if ObjectExist(tccobj+2)=1

//        `unglue object tccobj+2

//        `position object tccobj+2,100000,100000,100000

//       `endif


					}
				}
			}
			else
			{
				if (  t.entityelement[t.steamworks_playerEntityID[t.a]].obj > 0 ) 
				{
					if (  ObjectExist(t.entityelement[t.steamworks_playerEntityID[t.a]].obj) ) 
					{
						if (  t.steamworks_forcePosition[t.a] > 0 && SteamGetPlayerAlive(t.a)  ==  1 ) 
						{
							t.steamworks_playerHasSpawned[t.a]=1;
							HideObject (  t.entityelement[t.steamworks_playerEntityID[t.a]].obj );
							if (  t.entityelement[t.steamworks_playerEntityID[t.a]].attachmentobj > 0 ) 
							{
								if (  ObjectExist(t.entityelement[t.steamworks_playerEntityID[t.a]].attachmentobj)  ==  1  )  HideObject (  t.entityelement[t.steamworks_playerEntityID[t.a]].attachmentobj );
							}
						}
						else
						{
							if (  t.steamworks_playerHasSpawned[t.a]  ==  1 ) 
							{
								ShowObject (  t.entityelement[t.steamworks_playerEntityID[t.a]].obj );
								if (  t.entityelement[t.steamworks_playerEntityID[t.a]].attachmentobj > 0 ) 
								{
									if (  ObjectExist(t.entityelement[t.steamworks_playerEntityID[t.a]].attachmentobj)  ==  1  )  ShowObject (  t.entityelement[t.steamworks_playerEntityID[t.a]].attachmentobj );
								}
							}
							else
							{
								HideObject (  t.entityelement[t.steamworks_playerEntityID[t.a]].obj );
								if (  t.entityelement[t.steamworks_playerEntityID[t.a]].attachmentobj > 0 ) 
								{
									if (  ObjectExist(t.entityelement[t.steamworks_playerEntityID[t.a]].attachmentobj)  ==  1  )  HideObject (  t.entityelement[t.steamworks_playerEntityID[t.a]].attachmentobj );
								}
							}
						}
					}
				}
			}
	}

	//  Player is respawning or dead
	t.characterkitcontrol.showmyhead = 0;
	if (  t.steamworks_health[g.steamworks.me]  <=  0 ) 
	{
		t.tTime = Timer();
		if (  t.tTime - g.steamworks.lastSendAliveTime > STEAM_ALIVE_UPDATE_DELAY ) 
		{
			g.steamworks.lastSendAliveTime = t.tTime;
			SteamSetPlayerAlive (  0 );
		}
		steam_showdeath ( );
		steam_respawn ( );
//   `print "RESPAWN"

		steam_updatePlayerPositions ( );
		steam_updatePlayerNamePlates ( );
		steam_updatePlayerAnimations ( );
		steam_delete_entities ( );
		steam_loop ( );
		steam_check_for_attachments ( );
		steam_update_all_projectiles ( );
		if (  g.steamworks.gameAlreadySpawnedBefore  ==  0 ) 
		{
			steam_dontShowOtherPlayers ( );
		}

		if (  t.steamworks_health[g.steamworks.me] > 0  )  g.steamworks.lastSendAliveTime  =  0;
		return;
	}

	//  Player is alive
	t.tTime = Timer();
	if (  t.tTime - g.steamworks.lastSendAliveTime > STEAM_ALIVE_UPDATE_DELAY ) 
	{
		g.steamworks.lastSendAliveTime = t.tTime;
		SteamSetPlayerAlive (  1 );
	}
	steam_update_player ( );
	steam_updatePlayerPositions ( );
	steam_updatePlayerInput ( );
	steam_updatePlayerNamePlates ( );
	steam_updatePlayerAnimations ( );
	steam_delete_entities ( );
	steam_loop ( );
	steam_server_message ( );
	steam_check_for_attachments ( );
	steam_update_all_projectiles ( );

	if ( g.steamworks.endplay == 1 ) steam_ending_game ( );

	if (  t.steamworks_health[g.steamworks.me]  <=  0  )  g.steamworks.lastSendAliveTime  =  0;

	t.tTime = Timer();

if ( g.steamworks.isGameHost == 1 ) steam_checkForEveryoneLeft ( );

return;

}

//  used when restarting a match so you don't see everyone dropping out of the sky
void steam_dontShowOtherPlayers ( void )
{

	for ( t.a = 0 ; t.a<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.a++ )
	{
		if (  t.a  !=  g.steamworks.me ) 
		{
			if (  t.entityelement[t.steamworks_playerEntityID[t.a]].obj > 0 ) 
			{
				if (  ObjectExist(t.entityelement[t.steamworks_playerEntityID[t.a]].obj) ) 
				{
					PositionObject (  t.entityelement[t.steamworks_playerEntityID[t.a]].obj,-100000,-100000,-100000 );
				}
			}
		}
	}

return;
	
}

void steam_ending_game ( void )
{
	PositionCamera (  25500,2000,25500 );
	RotateCamera (  90,t.tendofgamerotate_f,0 );
	t.tendofgamerotate_f = t.tendofgamerotate_f + (0.25*g.timeelapsed_f);
	for ( t.a = 0 ; t.a<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.a++ )
	{
		t.tobj=t.entityelement[t.steamworks_playerEntityID[t.a]].obj;
		if (  t.tobj > 0 ) 
		{
				if (  ObjectExist(t.tobj) ) 
				{
					PositionObject (  t.tobj, t.terrain.playerx_f, t.terrain.playery_f-20, t.terrain.playerz_f );
					HideObject (  t.tobj );
				}
		}
	}
return;

}

void steam_free_game ( void )
{

//  `if steamworks.madeArrays = 1

//   `undim steamworks_oldEntityPositionsX()

//   `undim steamworks_oldEntityPositionsZ()

//   `steamworks.madeArrays = 0

//  `endif

	if (  t.tspritetouse > 0 ) 
	{
		if (  SpriteExist(t.tspritetouse)  ==  1  )  DeleteSprite (  t.tspritetouse );
		t.tspritetouse = 0;
	}

	if (  g.steamworks.coop  ==  1 && g.steamworks.originalEntitycount > 0 ) 
	{
		UnDim (  t.steamStoreentityelement );
		g.steamworks.originalEntitycount = 0;
	}

	if (  g.steamworks.gunCount > 0 ) 
	{
		for ( t.i = 0 ; t.i<=  g.steamworks.gunCount-1; t.i++ )
		{
			if (  t.steamworks_gunobj[t.i] > 0 ) 
			{
				if (  ObjectExist(t.steamworks_gunobj[t.i])  )  DeleteObject (  t.steamworks_gunobj[t.i] );
			}
		}
	}

	g.steamworks.gunCount = 0;
	for ( t.i = 0 ; t.i<=  599; t.i++ )
	{
		if (  ObjectExist (g.steamplayermodelsoffset+t.i)  ==  1  )  DeleteObject (  g.steamplayermodelsoffset+t.i ) ;
	}

	for ( t.tbulletloop = 0 ; t.tbulletloop<=  159; t.tbulletloop++ )
	{

		t.tSteamSoundID = g.steamsoundoffset+200+t.tbulletloop;
		if (  SoundExist(t.tSteamSoundID)  ==  1 ) 
		{
			if (  SoundPlaying(t.tSteamSoundID)  ==  0  )  StopSound(t.tSteamSoundID);
			DeleteSound (  t.tSteamSoundID );
		}

		t.tSteamSoundID = g.steamsoundoffset+t.tbulletloop;
		if (  SoundExist(t.tSteamSoundID)  ==  1 ) 
		{
			if (  SoundPlaying(t.tSteamSoundID)  ==  0  )  StopSound(t.tSteamSoundID);
			if (  SoundLooping(t.tSteamSoundID)  ==  0  )  StopSound(t.tSteamSoundID);
			DeleteSound (  t.tSteamSoundID );
		}

	}

	for ( t.i = 0 ; t.i<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.i++ )
	{
		if (  t.steamworks_jetpackparticles[t.i]  !=  -1 ) 
		{
			t.tRaveyParticlesEmitterID=t.steamworks_jetpackparticles[t.i];
			ravey_particles_delete_emitter ( );
			t.steamworks_jetpackparticles[t.i] = -1;
		}
	}

	steam_resetGameStats ( );

	SteamWorkshopModeOff (  );

	g.steamworks.message = "";
	g.steamworks.messageTime = 0;

	t.game.gameloop = 0;

	if (  ImageExist(g.panelimageoffset+10)  )  DeleteImage (  g.panelimageoffset+10 );
	if (  SpriteExist(g.steamchatpanelsprite)  ==  1  )  DeleteSprite (  g.steamchatpanelsprite );

return;

//  needs tlobbytring$ to be set to the lobby name
}

void steam_subbedToItem ( void )
{
	for ( t.tloop = 0 ; t.tloop<=  20; t.tloop++ )
	{
		if (  t.steamworks_subbedItems[t.tloop]  ==  "" ) 
		{
			t.steamworks_subbedItems[t.tloop] = t.tlobbytring_s;
			break;
		}
	}
return;

//  needs tlobbytring$ to be set to the lobby name
}

void steam_checkItemSubbed ( void )
{
	for ( t.tloop = 0 ; t.tloop<=  20; t.tloop++ )
	{
		if (  t.steamworks_subbedItems[t.tloop]  ==  t.tlobbytring_s && t.tlobbytring_s != "" ) 
		{
			if (  Timer() - t.tsteaminstallingdotstime > 150 ) 
			{
				t.tsteaminstallingdotstime = Timer();
				t.tsteamInstallingDots_s = t.tsteamInstallingDots_s + ".";
				if (  Len(t.tsteamInstallingDots_s.Get()) > 3  )  t.tsteamInstallingDots_s  =  "";
			}
			t.tlobbytring_s = t.tlobbytring_s + " - Installing." + t.tsteamInstallingDots_s;
			break;
		}
	}
return;

}

void steam_resetGameStats ( void )
{

	steam_nukeTestmap ( );
	if (  FileExist( cstr(g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerlevel__.fpm").Get())  )  DeleteAFile (  cstr(g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerlevel__.fpm").Get() );
	if (  FileExist( cstr(g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerworkshopitemid__.dat").Get())  )  DeleteAFile (  cstr(g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerworkshopitemid__.dat").Get() );

	//  empty messages
	for ( t.tloop = 0 ; t.tloop<=  STEAM_MAX_CHAT_LINES-1; t.tloop++ )
	{
		t.steamworks_chat[t.tloop] = "";
	}

	for ( t.e = 1 ; t.e<=  g.entityelementlist; t.e++ )
	{
			t.entityelement[t.e].mp_networkkill = 0;
	}

	for ( t.tloop = 0 ; t.tloop<=  20; t.tloop++ )
	{
		t.steamworks_subbedItems[t.tloop] = "";
	}

	if ( SteamGetPlayerName() != NULL )
	{
		g.steamworks.playerName = SteamGetPlayerName();
		g.steamworks.playerID = SteamGetPlayerID();
	}

	g.steamworks.mode = STEAM_MODE_MAIN_MENU;
	g.steamworks.launchServer = 0;
	g.steamworks.maxHealth = 0;
	g.steamworks.isLobbyCreated = 0;
	g.steamworks.isServerCreated = 0;
	g.steamworks.isGameHost= 0;
	g.steamworks.voiceChatOn = 0;
	g.steamworks.lobbycount = 0;
	g.steamworks.lobbyscrollbarOn = 0;
	g.steamworks.gameAlreadySpawnedBefore = 0;
	g.steamworks.killedByPlayer = 0;
	g.steamworks.previousMessage_s = "";
	g.steamworks.syncedWithServer = 0;
	g.steamworks.syncedWithServerMode = 0;
	g.steamworks.oldtime = 0;
	g.steamworks.me = 0;
	g.steamworks.playedMyDeathAnim = 0;
	g.steamworks.fileLoaded = 0;
	g.steamworks.playGame = 0;
	g.steamworks.oldSpawnTimeLeft = 0;
	g.steamworks.respawnLeft = 0;
	g.steamworks.crouchOn = 0;
	g.steamworks.meleeOn = 0;
	g.steamworks.isAnimating = 0;
	g.steamworks.okayToLoadLevel = 0;
	g.steamworks.iHaveSaidIAmReady = 0;
	g.steamworks.attachmentcount = 0;
	g.steamworks.gunCount = 0;
	g.steamworks.gunid = 0;
	g.steamworks.lastSendTime = 0;
	g.steamworks.lastSendTimeAppearance = 0;
	g.steamworks.appearance = 0;
	g.steamworks.dyingTime = 0;
	g.steamworks.spawnrnd = -1;
	g.steamworks.reloading = 0;
	g.steamworks.syncedWithServer = 0;
	g.steamworks.sentreadytime = 0;
	g.steamworks.AttemptedToJoinLobbyTime = 0;
	g.steamworks.lastSendProjectileTime = 0;
	g.steamworks.dontApplyDamage = 0;
	g.steamworks.ragdollon = 0;
	g.steamworks.spectatorfollowdistance = 200.0;
	g.steamworks.ignoreDamageToEntity = 0;
	g.steamworks.endplay = 0;
	g.steamworks.destroyedObjectCount = 0;
	g.steamworks.message = "";
	g.steamworks.messageTime = 0;
	g.steamworks.oldfootfloortime = 0;
	g.steamworks.footfloor = 0;
	g.steamworks.resetcore = 0;
	g.steamworks.levelContainsCustomContent = 0;
	g.steamworks.workshopid = "0";
	g.steamworks.initialSpawnmoveDownCharacterFlag=1;
	g.steamworks.usersInServersLobbyAtServerCreation = 0;
	g.steamworks.dontDrawTitles = 0;
	g.steamworks.haveshowndeath = 0;
	g.steamworks.checkiflobbiesavailablemode = 0;
	g.steamworks.noplayermovement = 0;
	g.steamworks.team = 0;
	g.steamworks.friendlyfireoff = 0;
	g.steamworks.nameplatesOff = 0;
	g.steamworks.damageWasFromAI = 0;
	g.steamworks.haveSentMyAvatar = 0;
	g.steamworks.myOriginalSpawnPoint = -1;
	g.steamworks.realfirsttimespawn = 1;

	for ( t.tc = 0 ; t.tc<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.tc++ )
	{
		t.steamworks_kills[t.tc] = 0;
		t.steamworks_deaths[t.tc] = 0;
		t.steamworks_reload[t.tc] = 0;
		t.steamworks_playerShooting[t.tc] = 0;
		t.steamworks_playerAttachmentIndex[t.tc] = 0;
		t.steamworks_playerIsRagdoll[t.tc] = 0;
		t.steamworks_playerAttachmentObject[t.tc] = 0;
		t.steamworks_playerHasSpawned[t.tc] = 0;
		t.steamworks_oldAppearance[t.tc] = 0;
		t.steamworks_playingAnimation[t.tc] = 0;
		t.steamworks_playingRagdoll[t.tc] = 0;
		t.steamworks_oldplayerx[t.tc] = 0;
		t.steamworks_oldplayery[t.tc] = 0;
		t.steamworks_oldplayerz[t.tc] = 0;
		t.steamworks_meleePlaying[t.tc] = 0;
//   `steamworks_isIdling(tc) = 0

		t.steamworks_isDying[t.tc] = 0;
		t.steamworks_jetpackOn[t.tc] = 0;
		t.steamworks_lobbies_s[t.tc] = "";
		t.steamworks_playerEntityID[t.tc] = 0;
		t.steamworks_forcePosition[t.tc] = 0;
		t.steamworks_health[t.tc] = 0;
		t.steamworks_lastIdleReset[t.tc] = 1;
		t.steamworks_jetpackparticles[t.tc] = -1;
		t.steamworks_joined[t.tc] = "";
	}

	t.twhichteam = 1;
	for ( t.tc = 0 ; t.tc<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.tc++ )
	{
		t.twhichteam = 1-t.twhichteam;
		t.steamworks_team[t.tc] = t.twhichteam;
	}

	for ( t.tc = 0 ; t.tc<=  99; t.tc++ )
	{
		t.steamworks_attachmentobjects[t.tc] = 0;
		t.steamworks_gunobj[t.tc] = 0;
		t.steamworks_gunname[t.tc] = "";
	}

	for ( t.tc = 0 ; t.tc<=  79; t.tc++ )
	{
		t.steamworks_bullets[t.tc].on = 0;
		t.steamworks_bullets[t.tc].particles = -1;
		t.steamworks_bullets[t.tc].sound = 0;
	}

	for ( t.i = 0 ; t.i<=  STEAM_RESPAWN_TIME_OBJECT_LIST_SIZE; t.i++ )
	{
		t.steamworks_respawn_timed[t.i].inuse = 0;
	}
	
	t.characterkitcontrol.showmyhead = 0;
	

return;

}

void steam_update_all_projectiles ( void )
{
t.debugHowManyInUse = 0;
	for ( t.tbulletloop = 0 ; t.tbulletloop<=  159; t.tbulletloop++ )
	{
			if (  SteamGetBulletOn(t.tbulletloop)  ==  0 ) 
			{
				//  clean up particles
				if (  t.steamworks_bullets[t.tbulletloop].particles  !=  -1 ) 
				{
						t.tRaveyParticlesEmitterID=t.steamworks_bullets[t.tbulletloop].particles;
						ravey_particles_delete_emitter ( );
						t.steamworks_bullets[t.tbulletloop].particles=-1;
				}
			}
			if (  t.tbulletloop < g.steamworks.me*20 || t.tbulletloop > (g.steamworks.me*20)+19 ) 
			{
				t.tSteamSoundID = g.steamsoundoffset+t.tbulletloop;
				if (  SoundExist(t.tSteamSoundID)  ==  1 ) 
				{
					if (  SoundLooping(t.tSteamSoundID)  ==  0 ) 
					{
						DeleteSound (  t.tSteamSoundID );
						t.steamworks_bullets[t.tbulletloop].sound = 0;
					}
				}
				t.tSteamSoundID = g.steamsoundoffset+200+t.tbulletloop;
				if (  SoundExist(t.tSteamSoundID)  ==  1 ) 
				{
					if (  SoundPlaying(t.tSteamSoundID)  ==  0  )  DeleteSound (  t.tSteamSoundID );
				}
				if (  SteamGetBulletOn(t.tbulletloop)  ==  1 ) 
				{
					++t.debugHowManyInUse;
					t.tsteamBObj = g.steamplayermodelsoffset+200+t.tbulletloop;
					t.steamworks_bullets[t.tbulletloop].btype = SteamGetBulletType(t.tbulletloop);
					if (  ObjectExist(t.tsteamBObj)  ==  0 ) 
					{
						t.tfindObj = 0;
//       `tProj = -1

//       `for tProjLoop = 1 to weaponSystem.numProjectiles

							//if WeaponProjectile(tProjLoop).baseType = steamworks_bullets(tbulletloop).btype
								//tfindObj = WeaponProjectile(tProjLoop).obj
//          `tProj = tProjLoop

//          `exit

//         `endif

//       `next tProj

						t.tfindObj = t.WeaponProjectileBase[t.steamworks_bullets[t.tbulletloop].btype].baseObj;
						if (  t.tfindObj  !=  0 ) 
						{
							CloneObject (  t.tsteamBObj, t.tfindObj );
						}
						else
						{
							MakeObjectBox (  t.tsteamBObj,20,20,20 );
						}

						//  setup particle emitters for this projectile
						//  but only if entities not set to LOWEST as particle trails are expensive!
						t.steamworks_bullets[t.tbulletloop].particles = -1;
						t.tokay = 1 ; if (  t.visuals.shaderlevels.entities == 3  )  t.tokay = 0;
						if (  t.WeaponProjectileBase[t.steamworks_bullets[t.tbulletloop].btype].particleType>0 && t.tokay == 1 ) 
						{
							ravey_particles_get_free_emitter ( );
//        `WeaponProjectile(tNewProj).tempEmitter = tResult

							if (  t.tResult>0 ) 
							{
								t.tobj = t.tsteamBObj;
								weapon_add_projectile_particles ( );
								t.steamworks_bullets[t.tbulletloop].particles = t.tResult;
							}
						}

						//  setup sound
						t.steamworks_bullets[t.tbulletloop].sound = g.steamsoundoffset+t.tbulletloop;
						if (  SoundExist(t.steamworks_bullets[t.tbulletloop].sound) == 1 ) 
						{
							if (  SoundPlaying(t.steamworks_bullets[t.tbulletloop].sound)  ==  1  )  StopSound (  t.steamworks_bullets[t.tbulletloop].sound );
							if (  SoundLooping(t.steamworks_bullets[t.tbulletloop].sound)  ==  1  )  StopSound (  t.steamworks_bullets[t.tbulletloop].sound );
							DeleteSound (  t.steamworks_bullets[t.tbulletloop].sound );
						}
						//  if this projectile has a sound that loops, start it now
						if (  t.WeaponProjectileBase[t.steamworks_bullets[t.tbulletloop].btype].sound  ==  0  )  t.steamworks_bullets[t.tbulletloop].sound  =  0;
						if (  t.steamworks_bullets[t.tbulletloop].sound > 0 ) 
						{
							CloneSound (  t.steamworks_bullets[t.tbulletloop].sound,t.WeaponProjectileBase[t.steamworks_bullets[t.tbulletloop].btype].sound );
							if (  t.WeaponProjectileBase[t.steamworks_bullets[t.tbulletloop].btype].soundLoopFlag  ==  1 ) 
							{
								PositionSound (  t.steamworks_bullets[t.tbulletloop].sound,SteamGetBulletX(t.tbulletloop), SteamGetBulletY(t.tbulletloop), SteamGetBulletZ(t.tbulletloop) );
								SetSoundSpeed (  t.steamworks_bullets[t.tbulletloop].sound, t.WeaponProjectileBase[t.steamworks_bullets[t.tbulletloop].btype].soundDopplerBaseSpeed );
								LoopSound (  t.steamworks_bullets[t.tbulletloop].sound );
								if (  t.WeaponProjectileBase[t.steamworks_bullets[t.tbulletloop].btype].soundDopplerFlag  ==  1 ) 
								{
									t.txDist_f = CameraPositionX(0) - SteamGetBulletX(t.tbulletloop);
									t.tyDist_f = CameraPositionY(0) - SteamGetBulletY(t.tbulletloop);
									t.tzDist_f = CameraPositionZ(0) - SteamGetBulletZ(t.tbulletloop);
									t.steamworks_bullets[t.tbulletloop].soundDistFromPlayer = Sqrt(t.txDist_f*t.txDist_f + t.tyDist_f*t.tyDist_f + t.tzDist_f*t.tzDist_f);
								}
							}
						}

					}
					if (  ObjectExist(t.tsteamBObj)  ==  1 ) 
					{
						PositionObject (  t.tsteamBObj, SteamGetBulletX(t.tbulletloop), SteamGetBulletY(t.tbulletloop), SteamGetBulletZ(t.tbulletloop) );
						RotateObject (  t.tsteamBObj, SteamGetBulletAngleX(t.tbulletloop), SteamGetBulletAngleY(t.tbulletloop), SteamGetBulletAngleZ(t.tbulletloop) );

						//  do we need to reposition the 3D sound?
						t.tSndID = t.steamworks_bullets[t.tbulletloop].sound;
						if (  t.tSndID > 0 ) 
						{
							if (  t.WeaponProjectileBase[t.steamworks_bullets[t.tbulletloop].btype].soundLoopFlag  ==  1 ) 
							{
								//  position the sound
								PositionSound (  t.tSndID,SteamGetBulletX(t.tbulletloop), SteamGetBulletY(t.tbulletloop), SteamGetBulletZ(t.tbulletloop) );
								//  calculate and set doppler pitch
								if (  t.WeaponProjectileBase[t.steamworks_bullets[t.tbulletloop].btype].soundDopplerFlag  ==  1 ) 
								{
									t.tOldDist_f = t.steamworks_bullets[t.tbulletloop].soundDistFromPlayer;
									t.txDist_f = CameraPositionX(0) - SteamGetBulletX(t.tbulletloop);
									t.tyDist_f = CameraPositionY(0) - SteamGetBulletY(t.tbulletloop);
									t.tzDist_f = CameraPositionZ(0) - SteamGetBulletZ(t.tbulletloop);
									t.steamworks_bullets[t.tbulletloop].soundDistFromPlayer = Sqrt(t.txDist_f*t.txDist_f + t.tyDist_f*t.tyDist_f + t.tzDist_f*t.tzDist_f);
									t.tDistDiff_f = t.tOldDist_f - t.steamworks_bullets[t.tbulletloop].soundDistFromPlayer;
									t.tSoundMultiplier_f = 1 + (t.tDistDiff_f/t.ElapsedTime_f)*0.00015;
									if (  t.tSoundMultiplier_f < 0.5  )  t.tSoundMultiplier_f  =  0.5;
									if (  t.tSoundMultiplier_f > 2  )  t.tSoundMultiplier_f  =  2;
									t.steamworks_bullets[t.tbulletloop].btype = SteamGetBulletType(t.tbulletloop);
									SetSoundSpeed (  t.tSndID, t.WeaponProjectileBase[t.steamworks_bullets[t.tbulletloop].btype].soundDopplerBaseSpeed * t.tSoundMultiplier_f );
								}
							}
						}

					}
				}
				else
				{
					//  projectile ended, show result

					//  clean up particles
					if (  t.steamworks_bullets[t.tbulletloop].particles  !=  -1 ) 
					{
							t.tRaveyParticlesEmitterID=t.steamworks_bullets[t.tbulletloop].particles;
							ravey_particles_delete_emitter ( );
							t.steamworks_bullets[t.tbulletloop].particles=-1;
					}

					if (  ObjectExist(g.steamplayermodelsoffset+200+t.tbulletloop)  ==  1 ) 
					{

						t.tSteamProjectileType = t.steamworks_bullets[t.tbulletloop].btype;

						//  stop any looping sound
						if (  t.steamworks_bullets[t.tbulletloop].sound > 0 ) 
						{
							StopSound (  t.steamworks_bullets[t.tbulletloop].sound );
							t.steamworks_bullets[t.tbulletloop].sound = 0;
						}

						t.tsteamBObj = g.steamplayermodelsoffset+200+t.tbulletloop;

						t.tDeathSoundSoundID = t.WeaponProjectileBase[t.tSteamProjectileType].soundDeath;
						if (  t.tDeathSoundSoundID > 0 ) 
						{
							t.tSteamSoundID = g.steamsoundoffset+200+t.tbulletloop;
							if (  SoundExist(t.tSteamSoundID)  ==  0 ) 
							{
								CloneSound (  t.tSteamSoundID,t.tDeathSoundSoundID );
							}
							PositionSound (  t.tSteamSoundID,ObjectPositionX(t.tsteamBObj),ObjectPositionY(t.tsteamBObj), ObjectPositionZ(t.tsteamBObj) );
							SetSoundSpeed (  t.tSteamSoundID,38000 + Rnd(8000) );
							PlaySound (  t.tSteamSoundID );
						}

						t.texplodex_f=ObjectPositionX(t.tsteamBObj);
						t.texplodey_f=ObjectPositionY(t.tsteamBObj);
						t.texplodez_f=ObjectPositionZ(t.tsteamBObj);
//       `texploderadius#=300.0

//       `steamworks.dontApplyDamage = 1

						explosion_rocket(t.texplodex_f,t.texplodey_f,t.texplodez_f);
//physics_explodesphere ( );
//       `steamworks.dontApplyDamage = 0

						DeleteObject (  t.tsteamBObj );

					}
				}
			}
	}
	
//  `print "Multiplayer rockets in use: " + Str(debugHowManyInUse)

return;


//Direct Multiplayer Instructions


}

void steam_destroyentity ( void )
{
	//  takes ttte
	SteamDeleteObject (  t.ttte );
return;

}

void steam_refresh ( void )
{
	SteamLoop (  );
return;

}

void steam_setMessage ( void )
{
	//  takes tmsg$
	g.steamworks.message = t.tmsg_s;
	g.steamworks.messageTime = Timer();
return;

}

void steam_setMessageDots ( void )
{
	//  takes tmsg$
	g.steamworks.messageDots = t.tmsg_s;
	g.steamworks.messageTimeDots = Timer();
return;

}

void steam_message ( void )
{
	if (  g.steamworks.message  !=  "" ) 
	{
//   `x = (GetDisplayWidth()/2) - (Text (  width(steamworks.message)/2) )

//   `text x,200,steamworks.message

		steam_text(-1,15,3,g.steamworks.message.Get());
		if (  Timer() - g.steamworks.messageTime > STEAM_MESSAGE_TIMOUT ) 
		{
			g.steamworks.message = "";
		}
	}
return;

}

void steam_messageDots ( void )
{
	if (  g.steamworks.messageDots  !=  "" ) 
	{
//   `x = (GetDisplayWidth()/2) - (Text (  width(steamworks.message)/2) )

//   `text x,200,steamworks.message

		steam_textDots(-1,15,3,g.steamworks.messageDots.Get());
		if (  Timer() - g.steamworks.messageTimeDots > STEAM_MESSAGE_TIMOUT ) 
		{
			g.steamworks.messageDots = "";
		}
	}
return;

}

void steam_update_projectile ( void )
{

	if (  t.tProj > 19  )  return;
	
//  `print "tProj = " + Str(tProj)

//  `print "Steam Bullet = " + Str((steamworks.me*20) + tProj )

	
	t.tSteamBullet = (g.steamworks.me*20) + t.tProj;
	//  if it is a change in state, send immediately
//  `if steamworks_bullets(tSteamBullet).on  ==  0 && tSteamBulletOn  ==  1 then steamworks_bullets_send_time(tSteamBullet)  ==  0
	
//  `if steamworks_bullets(tSteamBullet).on  ==  1 && tSteamBulletOn  ==  0 then steamworks_bullets_send_time(tSteamBullet)  ==  0
	
	t.tTime = Timer();
	if (  t.tTime - t.steamworks_bullets_send_time[t.tSteamBullet] < STEAM_PROJECTILE_UPDATE_DELAY && t.tSteamBulletOn  ==  1  )  return;

	t.steamworks_bullets_send_time[t.tSteamBullet] = t.tTime;
	
//  `Print "sending projectiles"

	
	//  takes tProj, tProjType, xPos#, yPos#, zPos#, xAng#, yAng#, zAng#, tSteamBulletOn
//  `print "BULLET:"

//  `print "type " + Str(tProjType)

//  `print "x = " + Str(WeaponProjectile(tProj).xPos#)

//  `print "y = " + Str(WeaponProjectile(tProj).yPos#)

//  `print "z = " + Str(WeaponProjectile(tProj).zPos#)

//  `print "x angle = " + Str(WeaponProjectile(tProj).xAng#)

//  `print "y angle = " + Str(WeaponProjectile(tProj).yAng#)

//  `print "z angle = " + Str(WeaponProjectile(tProj).zAng#)

//  `print "On = " + Str(tSteamBulletOn)

//  `print "tProj" + Str(tProj)



	t.steamworks_bullets[t.tSteamBullet].on = t.tSteamBulletOn;
	SteamSetBullet (  t.tSteamBullet , t.WeaponProjectile[t.tProj].xPos_f, t.WeaponProjectile[t.tProj].yPos_f, t.WeaponProjectile[t.tProj].zPos_f, t.WeaponProjectile[t.tProj].xAng_f, t.WeaponProjectile[t.tProj].yAng_f, t.WeaponProjectile[t.tProj].zAng_f, t.tProjType, t.tSteamBulletOn );

return;

}

void steam_serverSetLuaGameMode ( void )
{
	SteamSendLua (  STEAM_LUA_ServerSetLuaGameMode,0,t.v );
return;

}

void steam_setServerTimer ( void )
{
	SteamSendLua (  STEAM_LUA_SetServerTimer,0,t.v );
return;

}

void steam_serverRespawnAll ( void )
{
	SteamSendLua (  STEAM_LUA_ServerRespawnAll,0,0 );
	steam_restoreEntities ( );
	steam_setLuaResetStats ( );
	steam_respawnEntities ( );
	t.playercontrol.jetpackhidden=0;
	t.playercontrol.jetpackmode=0;
	physics_no_gun_zoom ( );
	t.aisystem.processplayerlogic=1;
	g.steamworks.gameAlreadySpawnedBefore = 0;
	t.tindex = g.steamworks.me+1;

	//  Find start position for player
	t.tfoundone = 0;
	if (  t.steamworksmultiplayerstart[t.tindex].active == 1 ) 
	{
		t.tfoundone = 1;
	}
	else
	{
		t.tonetotry = t.tindex/2;
		if (  t.tonetotry > 0 ) 
		{
			if (  t.steamworksmultiplayerstart[t.tonetotry].active == 1 ) 
			{
				t.tfoundone = 1;
				t.tindex = t.tonetotry;
			}
		}
	}
	if (  t.tfoundone  ==  0 ) 
	{
		if (  t.steamworksmultiplayerstart[1].active == 1 ) 
		{
			t.tindex = 1;
			t.tfoundone = 1;
		}
	}

	if (  t.steamworksmultiplayerstart[t.tindex].active == 1 ) 
	{

		t.terrain.playerx_f=t.steamworksmultiplayerstart[t.tindex].x;
		t.terrain.playery_f=t.steamworksmultiplayerstart[t.tindex].y+20;
		t.terrain.playerz_f=t.steamworksmultiplayerstart[t.tindex].z;
		t.terrain.playerax_f=0;
		t.terrain.playeray_f=t.steamworksmultiplayerstart[t.tindex].angle;
		t.terrain.playeraz_f=0;

		g.steamworks.lastx=t.terrain.playerx_f;
		g.steamworks.lasty=t.terrain.playery_f;
		g.steamworks.lastz=t.terrain.playerz_f;
		g.steamworks.lastangley=t.terrain.playeray_f;
	}
	physics_resetplayer_core ( );
	t.tobj = t.entityelement[t.steamworks_playerEntityID[g.steamworks.me]].obj;
	if (  t.tobj > 0 ) 
	{
		PositionObject (  t.tobj, t.terrain.playerx_f, t.terrain.playery_f-70, t.terrain.playerz_f );
	}
	g.steamworks.gameAlreadySpawnedBefore = 0;
	t.player[t.plrid].health = 0;
	t.steamworks_health[g.steamworks.me] = 0;
	g.steamworks.endplay = 0;
	g.autoloadgun=0 ; gun_change ( );
return;

//  Put entities back to the original "first played" state
}

void steam_restoreEntities ( void )
{
//  `remstart

	for ( t.te = 1 ; t.te<=  g.steamworks.originalEntitycount; t.te++ )
	{
		t.tentid=t.entityelement[t.te].bankindex;
		if (  t.tentid>0 ) 
		{
			if (  t.entityprofile[t.tentid].ischaracter  ==  1 || t.entityelement[t.te].mp_isLuaChar  ==  1 ) 
			{
//     `entityelement(te) = steamStoreentityelement(te)

				t.entityelement[t.te].x = t.steamStoreentityelement[t.te].x;
				t.entityelement[t.te].y = t.steamStoreentityelement[t.te].y;
				t.entityelement[t.te].z = t.steamStoreentityelement[t.te].z;
				ScaleObject (  t.entityelement[t.te].obj,100,100,100 );
				t.entityelement[t.te].health = 100;
				PositionObject (  t.entityelement[t.te].obj,t.entityelement[t.te].x,t.entityelement[t.te].y,t.entityelement[t.te].z );
				t.entityelement[t.te].mp_coopControlledByPlayer = -1;
				t.entityelement[t.te].mp_updateOn = 0;
				t.entityelement[t.te].active = 1;
				AISetEntityActive (  t.entityelement[t.te].obj,1 );
			}
		}
	}
//  `remend

return;

}

void steam_serverEndPlay ( void )
{
	SteamSendLua (  STEAM_LUA_ServerEndPlay,0,0 );
	t.playercontrol.jetpackhidden=0;
	t.playercontrol.jetpackmode=0;
	physics_no_gun_zoom ( );
	g.steamworks.endplay = 1;
	t.aisystem.processplayerlogic = 0;
	g.autoloadgun=0 ; gun_change ( );
return;

}

void steam_setServerKillsToWin ( void )
{
	g.steamworks.setserverkillstowin = t.v;
return;

}

void steam_networkkill ( void )
{

	//  get damage amount to set it back to 0
	t.tdamage = SteamGetPlayerDamageAmount();

	t.tsteamlastdamageincounter = t.tsteamlastdamageincounter + 1;
	t.tsource = t.entityelement[t.texplodesourceEntity].mp_killedby;
	t.te = t.steamworks_playerEntityID[t.tsource];
	
	g.steamworks.killedByPlayerFlag = 1;
	g.steamworks.playerThatKilledMe = t.tsource;
	t.tsteamforce = 500;
	SteamKilledBy (  g.steamworks.playerThatKilledMe , CameraPositionX(), CameraPositionY(), CameraPositionZ(), t.tsteamforce, 0 );
	g.steamworks.dyingTime = Timer();

return;

}

void steam_lobbyListBox ( void )
{
	t.tluaTextCenterX = 0;
	if (  g.steamworks.listboxmode  ==  0 ) 
	{
		t.tsize = SteamGetLobbyListSize();

		t.tLeft = 5;
		t.tTop = 5;
		t.tRight = (26*10)+10;
		t.tBottom = 98+110+70;
		InkEx ( 20, 20, 20 );// Rgb(20,20,20),Rgb(20,20,20) );
		BoxEx (  t.tLeft,t.tTop,t.tRight,t.tTop );
		InkEx ( 255, 255, 255 );// Rgb (255,255,255),0 );
		LineEx (  t.tLeft,t.tTop,t.tRight,t.tTop );
		LineEx (  t.tLeft,t.tTop,t.tLeft,t.tBottom );
		LineEx (  t.tLeft,t.tBottom,t.tRight,t.tBottom );
		LineEx (  t.tRight,t.tTop,t.tRight,t.tBottom );

		InkEx (  255, 255, 255 );//Rgb ( 255,255,255),0 );
		BoxEx (  20,25,40,45 );
		InkEx (  255, 255, 50 );//Rgb (255,255,50),0 );
		BoxEx (  20,60,40,80 );
		InkEx (  255, 100, 100 );//Rgb (255,100,100),0 );
		BoxEx (  20,195,40,215 );

		t.tluarealcoords = 1;
		steam_textColor(50,20,1,"You can join this Lobby",255,255,255);
		t.tluarealcoords = 1;
		steam_textColor(50,55,1,"Join to subscribe and",255,255,50);
		t.tluarealcoords = 1;
		steam_textColor(50,80,1,"download the content",255,255,50);
		t.tluarealcoords = 1;
		steam_textColor(50,105,1,"for this game. The",255,255,50);
		t.tluarealcoords = 1;
		steam_textColor(50,130,1,"lobby will turn white",255,255,50);
		t.tluarealcoords = 1;
		steam_textColor(50,155,1,"when downloaded",255,255,50);
		t.tluarealcoords = 1;
		steam_textColor(50,190,1,"Please restart",255,100,100);
		t.tluarealcoords = 1;
		steam_textColor(50,215,1,"GameGuru to",255,100,100);
		t.tluarealcoords = 1;
		steam_textColor(50,240,1,"update this game",255,100,100);
	}
	if (  g.steamworks.listboxmode  ==  1 ) 
	{
		t.tsize = t.tempsteamhowmanyfpmsarethere;
	}
	

	t.tTop_f = 20.0 * (GetDisplayHeight() / 100.0);
	t.tleft_f = 30.0 * (GetDisplayWidth() / 100.0);
	t.tBottom_f = 75.0 * (GetDisplayHeight() / 100.0);
	t.tright_f = 70.0 * (GetDisplayWidth() / 100.0);

	t.tTop = t.tTop_f;
	t.tLeft = t.tleft_f;
	t.tBottom = t.tBottom_f;
	t.tRight = t.tright_f;

	t.tempsteamyminY_f = (GetDisplayHeight() * 0.25) - (GetDisplayHeight() * 0.025);
	t.tempsteamymaxY_f = GetDisplayHeight() * 0.75;
	t.tempsteamyminX_f = GetDisplayWidth() * 0.30;
	t.tempsteamymaxX_f = GetDisplayWidth() * 0.65;
	t.tempsteamselected_f = g.steamworks.selectedLobby - g.steamworks.lobbyoffset;
	t.tempmissthisone_f = t.tempsteamselected_f;
	if (  t.tempsteamselected_f  >=  0 && t.tempsteamselected_f  <= 9 ) 
	{
		t.tempsteamselectedY_f = t.tempsteamyminY_f + (t.tempsteamselected_f * (GetDisplayHeight() * 0.05));
		InkEx ( 128, 128, 128 );//  Rgb ( 128,128,128),0 );
		BoxEx (  t.tLeft,t.tempsteamselectedY_f,t.tRight,t.tempsteamselectedY_f+(GetDisplayHeight() * 0.05) );
		if (  t.mc  ==  1 && t.tempsteamoldmc  ==  0  )  g.steamworks.selectedLobby  =  t.tempsteamselected_f+g.steamworks.lobbyoffset;
	}

	t.tempsteamyminY_f = (GetDisplayHeight() * 0.25) - (GetDisplayHeight() * 0.025);
	t.tempsteamymaxY_f = GetDisplayHeight() * 0.75;
	t.tempsteamyminX_f = GetDisplayWidth() * 0.30;
	t.tempsteamymaxX_f = GetDisplayWidth() * 0.65;
	if (  t.mx  >=  t.tempsteamyminX_f && t.mx  <=  t.tempsteamymaxX_f ) 
	{
		if (  t.my  >=  t.tempsteamyminY_f && t.my  <=  t.tempsteamymaxY_f ) 
		{
			t.my_f = t.my;
			t.tempsteamselected_f = Floor((t.my_f - t.tempsteamyminY_f) / (GetDisplayHeight() * 0.05));
			if (  t.tempsteamselected_f  >=  0 && t.tempsteamselected_f < 10 && t.tempsteamselected_f  !=  t.tempmissthisone_f ) 
			{
				if (  t.tempsteamselected_f+g.steamworks.lobbyoffset < t.tsize ) 
				{
					t.tempsteamselectedY_f = t.tempsteamyminY_f + (t.tempsteamselected_f * (GetDisplayHeight() * 0.05));
					InkEx ( 64, 64, 64 );//  Rgb ( 64,64,64),0 );
					BoxEx (  t.tLeft,t.tempsteamselectedY_f,t.tRight,t.tempsteamselectedY_f+(GetDisplayHeight() * 0.05) );
					if (  t.mc  ==  1 && t.tempsteamoldmc  ==  0  )  g.steamworks.selectedLobby  =  t.tempsteamselected_f+g.steamworks.lobbyoffset;
				}
			}
		}
	}

	InkEx ( 255, 255, 255 );// Rgb (255,255,255),0 );
	LineEx (  t.tLeft,t.tTop,t.tRight,t.tTop );
	LineEx (  t.tLeft,t.tTop,t.tLeft,t.tBottom );
	LineEx (  t.tLeft,t.tBottom,t.tRight,t.tBottom );
	LineEx (  t.tRight,t.tTop,t.tRight,t.tBottom );

	t.toffx_f = (1.0 * GetDisplayWidth()) / 100.0;
	t.toffx = t.toffx_f;
	InkEx ( 30, 30, 30 );// Rgb ( 30,30,30),0 );
	BoxEx (  t.tRight-(t.toffx*2)-8,t.tTop+1,t.tRight-1,t.tBottom-1 );

	t.tTop += 4;
	t.tBottom -= 4;
	t.tRight -= 4;

	t.toffx_f = (1.0 * GetDisplayWidth()) / 100.0;
	t.toffx = t.toffx_f;
	t.toffy_f = (1.0 * GetDisplayHeight()) / 100.0;
	t.toffy = t.toffy_f;

	InkEx ( 255, 255, 255 );//  Rgb (255,255,255),0 );
	if (  t.mx > t.tLeft && t.mx < t.tRight ) 
	{
		if (  t.my > t.tTop && t.my < t.tTop+(t.toffy_f*2) ) 
		{
			InkEx ( 128, 128, 128 );//  Rgb (128,128,128),0 ) ;
			if (  t.mc  ==  1 ) 
			{
				if (  Timer() - t.tempsteamscrollclicktimer > 100 ) 
				{
					--g.steamworks.lobbyoffset;
					t.tempsteamscrollclicktimer = Timer();
				}
			}
		}
	}

	LineEx (  t.tRight-t.toffx,t.tTop,t.tRight-(t.toffx*2), t.tTop+(t.toffy*2) );
	LineEx (  t.tRight-t.toffx,t.tTop,t.tRight, t.tTop+(t.toffy*2) );
	LineEx (  t.tRight-(t.toffx*2),t.tTop+(t.toffy*2),t.tRight,t.tTop+(t.toffy*2) );

	InkEx ( 255, 255, 255 );// Rgb (255,255,255),0 ) ;
	if (  t.mx > t.tLeft && t.mx < t.tRight ) 
	{
		if (  t.my > t.tBottom-(t.toffy*2) && t.my < t.tBottom ) 
		{
			InkEx ( 128, 128, 128 );// Rgb (128,128,128),0 ) ;
			if (  t.mc  ==  1 ) 
			{
				if (  Timer() - t.tempsteamscrollclicktimer > 100 ) 
				{
					++g.steamworks.lobbyoffset;
					t.tempsteamscrollclicktimer = Timer();
				}
			}
		}
	}
	

	LineEx (  t.tRight-t.toffx,t.tBottom,t.tRight-(t.toffx*2), t.tBottom-(t.toffy*2) );
	LineEx (  t.tRight-t.toffx,t.tBottom,t.tRight, t.tBottom-(t.toffy*2) );
	LineEx (  t.tRight-(t.toffx*2),t.tBottom-(t.toffy*2),t.tRight,t.tBottom-(t.toffy*2) );

	if (  g.steamworks.lobbyscrollbarOn  ==  0 || t.mc  ==  0 ) 
	{
		g.steamworks.lobbyscrollbarOn = 0;
		t.tboxsize_f = (10.0 * GetDisplayHeight()) / 100.0;
		t.tboxsize = t.tboxsize_f;
		t.tloboffset_f = g.steamworks.lobbyoffset;
		t.tsize_f = t.tsize;
		t.tboxoffset_f = (t.tloboffset_f / t.tsize_f) * 100.0;
		if (  t.tboxoffset_f < 0.0  )  t.tboxoffset_f  =  0.0;
		if (  t.tboxoffset_f > 100.0  )  t.tboxoffset_f  =  100.0;
		t.tboxoffset_f = (t.tboxoffset_f * (GetDisplayHeight() * 0.42)) / 100.0;
		t.tboxoffset = t.tboxoffset_f;
		t.tboxtop = t.tTop+(t.toffy*2) + t.tboxoffset + 2;
		InkEx ( 255, 255, 255 );//  Rgb (255,255,255),0 ) ;
		if (  t.mx > t.tRight-(t.toffx*2) && t.mx < t.tRight ) 
		{
			if (  t.my > t.tboxtop && t.my < t.tboxtop+t.tboxsize ) 
			{
				InkEx ( 160, 160, 160 );// Rgb (160,160,160),0 ) ;
				if (  t.mc  ==  1 && t.tempsteamoldmc  ==  0 ) 
				{
					g.steamworks.lobbyscrollbarOn = 1;
					g.steamworks.lobbyscrollbarOffset = t.tboxtop-t.my;
					g.steamworks.lobbyscrollbarOldY = t.my;
				}
			}
		}

		BoxEx (  t.tRight-(t.toffx*2),t.tboxtop,t.tRight,t.tboxtop+t.tboxsize );

	}
	else
	{
		g.steamworks.lobbyscrollbarOldY = t.my;
		t.tboxtop = t.my+g.steamworks.lobbyscrollbarOffset;
		if (  t.tboxtop < t.tTop+(t.toffy*2)+2  )  t.tboxtop  =  t.tTop+(t.toffy*2)+2;
		if (  t.tboxtop > t.tTop+(t.toffy*2)+2 + ((100.0 * (GetDisplayHeight() * 0.40)) / 100.0)  )  t.tboxtop  =  t.tTop+(t.toffy*2)+2 + ((100.0 * (GetDisplayHeight() * 0.40)) / 100.0);
		t.tboxsize_f = (10.0 * GetDisplayHeight()) / 100.0;
		t.tboxsize = t.tboxsize_f;
		InkEx ( 160, 160, 160 );// Rgb (160,160,160),0 ) ;
		BoxEx (  t.tRight-(t.toffx*2),t.tboxtop,t.tRight,t.tboxtop+t.tboxsize );

		//  update the list to reflect where the bar is
		t.tboxtop_f = t.tboxtop - (t.tTop+(t.toffy*2)+2);
		t.tempsteamperc_f = (t.tboxtop_f / (GetDisplayHeight() * 0.42)) * 100.0;
		if (  t.tempsteamperc_f < 0.0  )  t.tempsteamperc_f  =  0.0;
		if (  t.tempsteamperc_f > 100.0  )  t.tempsteamperc_f  =  100.0;
		t.tsize_f = t.tsize;
		t.tempsteamnewoffset_f = (t.tempsteamperc_f * t.tsize_f) / 100.0;
		g.steamworks.lobbyoffset = t.tempsteamnewoffset_f;

	}

	if (  g.steamworks.lobbyoffset > t.tsize-10  )  g.steamworks.lobbyoffset  =  t.tsize-10;
	if (  g.steamworks.lobbyoffset < 0  )  g.steamworks.lobbyoffset  =  0;

	InkEx ( 255, 255, 255 );// Rgb(255,255,255),0 );

	t.tempsteamoldmc = t.mc;

	if (  g.steamworks.listboxmode  ==  0 ) 
	{
		if (  t.tsize  ==  1 ) 
		{
			t.tstring_s = "1 lobby found";
		}
		else
		{
			t.tstring_s = cstr(cstr(Str(t.tsize)) + " lobbies found");
		}
	}
	if (  g.steamworks.listboxmode  ==  1 ) 
	{
		t.tstring_s = cstr(cstr(Str(t.tsize)) + " levels found");
	}
	steam_text(-1,15,1,t.tstring_s.Get());

	if (  t.tsize > 0  )  g.steamworks.lobbycount  =  t.tsize;
	t.teampsteamy = 25;
	t.tlobbycount = 0;
	for ( t.c = 0 ; t.c<=  t.tsize-1; t.c++ )
	{
			if (  t.c  >=  g.steamworks.lobbyoffset && t.c < g.steamworks.lobbyoffset+10 ) 
			{
				if (  g.steamworks.listboxmode  ==  0 ) 
				{
					t.steamworks_lobbies_s[t.tlobbycount] = SteamGetLobbyListName(t.c);

					if (  cstr(Left(t.steamworks_lobbies_s[t.tlobbycount].Get(),5))  ==  "Lobby" || Len(t.steamworks_lobbies_s[t.tlobbycount].Get()) < 8 ) 
					{
						t.steamworks_lobbies_s[t.tlobbycount] = "Waiting for lobby details...";
					}
					if (  g.steamworks.selectedLobby  ==  t.c  )  g.steamworks.selectedLobbyName  =  t.steamworks_lobbies_s[t.tlobbycount];

					t.tempSteamworksLobbyNameFromList_s = t.steamworks_lobbies_s[t.tlobbycount];
					steam_canIJoinThisLobby ( );
					t.tsteamstring_s = g.steamworks.lobbyjoinedname;
					if (  t.tsteamcanjoinlobby  ==  1 ) 
					{
						t.tr = 255;
						t.tg = 255;
						t.tb = 255;
					}
					else
					{
						if (  t.tsteamcanjoinlobby  ==  2 ) 
						{
							t.tr = 255;
							t.tg = 100;
							t.tb = 100;
						}
						else
						{
							t.tr = 255;
							t.tg = 255;
							t.tb = 50;
						}
					}
				}
				if (  g.steamworks.listboxmode  ==  1 ) 
				{
					t.steamworks_lobbies_s[t.tlobbycount] = t.tfpmfilelist_s[t.c];
					t.tsteamstring_s = t.steamworks_lobbies_s[t.tlobbycount];
					t.tr = 255;
					t.tg = 255;
					t.tb = 255;
				}

				t.tlobbytring_s = t.tsteamstring_s;
				if ( t.tsteamcanjoinlobby == 0  ) steam_checkItemSubbed ( );
				steam_textColor(32,t.teampsteamy,1,t.tlobbytring_s.Get(),t.tr,t.tg,t.tb);
				t.teampsteamy += 5;
				++t.tlobbycount;
			}
	}
}

void steam_createLobby ( void )
{
	g.steamworks.haveToldAboutSolo = 0;
	t.tempsteamhostlobbyname_s = cstr(SteamGetPlayerName()) + cstr("'s Lobby:");
	if (  g.steamworks.fpmpicked  ==  "Level I just worked on" ) 
	{
		t.tempsteamlevelname_s = cstr(SteamGetPlayerName()) + cstr("'s Level:");
	}
	else
	{
		t.tempsteamlevelname_s = cstr(Left(g.steamworks.fpmpicked.Get(),Len(g.steamworks.fpmpicked.Get())-4)) + cstr(":");
	}
	//  grab version number
	if (  g.steamworks.fpmpicked  ==  "Level I just worked on" ) 
	{
		t.tempsteammaptocheck_s = g.fpscrootdir_s+"\\Files\\mapbank\\worklevel.dat";
	}
	else
	{
		t.tempsteammaptocheck_s = g.fpscrootdir_s+"\\Files\\mapbank\\"+Left(g.steamworks.fpmpicked.Get(),Len(g.steamworks.fpmpicked.Get())-3)+"dat";
	}

	t.tsteamworkshopitemtocheckifchangedandversion_s = t.tempsteammaptocheck_s;
	steam_grabWorkshopChangedFlagAndVersion ( );

	SteamSetLobbyName (  cstr(t.tempsteamhostlobbyname_s+t.tempsteamlevelname_s+g.steamworks.workshopid+":"+Str(t.tsteamworkshopTheVersionNumber)).Get() );
	SteamCreateLobby (  );
	g.steamworks.isGameHost = 1;
	g.steamworks.mode = STEAM_WAITING_FOR_LOBBY_CREATION;
	t.tempsteamlobbycreationtimeout = Timer();
return;

}

void steam_searchForLobbies ( void )
{
	SteamGetLobbyList (  );
	g.steamworks.mode = STEAM_MODE_LOBBY;
	g.steamworks.isGameHost = 0;
return;

}

void steam_searchForFpms ( void )
{
	g.steamworks.mode = STEAM_SERVER_CHOOSING_FPM_TO_USE;
	t.told_s=GetDir();
	SetDir (  cstr(g.fpscrootdir_s + "\\Files\\mapbank").Get() );
	ChecklistForFiles (  );
	Dim (  t.tfpmfilelist_s,ChecklistQuantity( ) );
	t.tempsteamhowmanyfpmsarethere = 0;

	if (  FileExist( cstr(g.fpscrootdir_s+"\\Files\\editors\\gridedit\\worklevel.fpm").Get()) ) 
	{
//   `tempsteamhowmanyfpmsarethere = 1

//   `tfpmfilelist$(0) = "Level I just worked on"

	}

	for ( t.c = 1 ; t.c<=  ChecklistQuantity(); t.c++ )
	{
		if (  ChecklistValueA(t.c) == 0 ) 
		{
			t.tfile_s=ChecklistString(t.c);
			if (  t.tfile_s != "." && t.tfile_s != ".." ) 
			{
				if (  cstr(Lower(Right(t.tfile_s.Get(),4)))  ==  ".fpm" ) 
				{
					t.tfpmfilelist_s[t.tempsteamhowmanyfpmsarethere] = t.tfile_s;
					++t.tempsteamhowmanyfpmsarethere;
				}
			}
		}
	}
	SetDir (  t.told_s.Get() );
return;

}

void steam_launchGame ( void )
{
	g.steamworks.launchServer = 1;
return;

}

void steam_backToStart ( void )
{
	steam_resetGameStats ( );
return;

}

void steam_selectedALevel ( void )
{

	if (  FileExist( cstr( g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerlevel__.fpm").Get())  )  DeleteAFile (  cstr(g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerlevel__.fpm").Get() );
	g.steamworks.fpmpicked = t.tfpmfilelist_s[g.steamworks.selectedLobby];
	steam_checkIfLevelHasCustomContent ( );
	if (  g.steamworks.fpmpicked  ==  "Level I just worked on" ) 
	{
		CopyAFile (  cstr(g.fpscrootdir_s+"\\Files\\editors\\gridedit\\worklevel.fpm" ).Get(),cstr(g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerlevel__.fpm").Get() );
	}
	else
	{
		CopyAFile (  cstr(g.fpscrootdir_s+"\\Files\\mapbank\\"+g.steamworks.fpmpicked).Get(),cstr(g.fpscrootdir_s+"\\Files\\editors\\gridedit\\__multiplayerlevel__.fpm").Get() );
	}
	if (  g.steamworks.levelContainsCustomContent  ==  1 ) 
	{
		//  first we check if the changed flag is set (they have saved since hosting) if not, we dont need to upload to steam
		if (  g.steamworks.fpmpicked  ==  "Level I just worked on" ) 
		{
			t.tempsteammaptocheck_s = g.fpscrootdir_s+"\\Files\\mapbank\\worklevel.dat";
		}
		else
		{
			t.tempsteammaptocheck_s = g.fpscrootdir_s+"\\Files\\mapbank\\"+Left(g.steamworks.fpmpicked.Get(),Len(g.steamworks.fpmpicked.Get())-3)+"dat";
		}

		t.tsteamworkshopitemtocheckifchangedandversion_s = t.tempsteammaptocheck_s;
		steam_grabWorkshopChangedFlagAndVersion ( );
		g.steamworks.workshopItemChangedFlag = t.tsteamworkshopHasItemChangedFlag;
		if (  t.tsteamworkshopHasItemChangedFlag  ==  1 ) 
		{
			g.steamworks.mode = STEAM_SERVER_CHOOSING_TO_MAKE_FPS_WORKSHOP;
		}
		else
		{
			g.steamworks.workshopid = t.tsteamworkshopTheIDNumber_s;
		}
	}

return;

}

void steam_checkIfLevelHasCustomContent ( void )
{

	if (  g.steamworks.fpmpicked  ==  "Level I just worked on" ) 
	{
		t.tempsteammaptocheck_s = g.fpscrootdir_s+"\\Files\\mapbank\\worklevel.dat";
	}
	else
	{
		t.tempsteammaptocheck_s = g.fpscrootdir_s+"\\Files\\mapbank\\"+Left(g.steamworks.fpmpicked.Get(),Len(g.steamworks.fpmpicked.Get())-3)+"dat";
	}

	if (  FileExist(t.tempsteammaptocheck_s.Get()) ) 
	{
		g.steamworks.levelContainsCustomContent = 1;
	}
	else
	{
		g.steamworks.levelContainsCustomContent = 0;
	}

return;

}

void steam_buildWorkShopItem ( void )
{
	g.steamworks.mode = STEAM_CREATING_WORKSHOP_ITEM;
	switch (  g.steamworks.buildingWorkshopItemMode ) 
	{
		case 0:
			if (  PathExist( cstr(g.fpscrootdir_s+"\\Files\\editors\\workshopItem").Get() )  ==  0 ) 
			{
				MakeDirectory ( cstr(g.fpscrootdir_s+"\\Files\\editors\\workshopItem").Get() );
			}
			else
			{
				t.told_s=GetDir();
				SetDir (  cstr(g.fpscrootdir_s + "\\Files\\editors\\workshopItem").Get() );
				ChecklistForFiles (  );

				for ( t.c = 1 ; t.c<=  ChecklistQuantity(); t.c++ )
				{
					if (  ChecklistValueA(t.c) == 0 ) 
					{
						t.tfile_s=ChecklistString(t.c);
						if (  t.tfile_s != "." && t.tfile_s != ".." ) 
						{
							DeleteAFile (  t.tfile_s.Get() );
						}
					}
				}
				SetDir (  t.told_s.Get() );
			}

			if (  FileOpen(1)  ==  1  )  CloseFile (  1 );

			if (  g.steamworks.fpmpicked  ==  "Level I just worked on" ) 
			{
				t.tempsteammaptocheck_s = g.fpscrootdir_s+"\\Files\\mapbank\\worklevel.dat";
				t.tempsteamleveltocopy_s = g.fpscrootdir_s+"\\Files\\mapbank\\worklevel.fpm";
			}
			else
			{
				t.tempsteammaptocheck_s = g.fpscrootdir_s+"\\Files\\mapbank\\"+Left(g.steamworks.fpmpicked.Get(),Len(g.steamworks.fpmpicked.Get())-3)+"dat";
				t.tempsteamleveltocopy_s = g.fpscrootdir_s+"\\Files\\mapbank\\" + g.steamworks.fpmpicked;
			}

			t.tempsteamdestworkshopfolder_s = g.fpscrootdir_s + "\\Files\\editors\\workshopItem\\";
			CopyAFile (  t.tempsteamleveltocopy_s.Get(), cstr(t.tempsteamdestworkshopfolder_s + "editors_gridedit___multiplayerlevel__.fpm").Get() );
			//  grab clanged flag and version number (we dont need the changed flag here tho)
			t.tsteamworkshopitemtocheckifchangedandversion_s = t.tempsteammaptocheck_s;
			steam_grabWorkshopChangedFlagAndVersion ( );

			OpenToRead (  1,t.tempsteammaptocheck_s.Get() );
			g.steamworks.buildingWorkshopItemMode = 1;
			//  skip over the warning, change flag, version number and workshop id
			t.tempsteamstring_s = ReadString ( 1 );
			t.tempsteamstring_s = ReadString ( 1 );
			t.tempsteamstring_s = ReadString ( 1 );
			t.tempsteamstring_s = ReadString ( 1 );
			//  skip over the fpm in the file
			t.tempsteamstring_s = ReadString ( 1 );

		break;
//   ``````````````````````````````````

		case 1:
			t.tempsteamdestworkshopfolder_s = g.fpscrootdir_s + "\\Files\\editors\\workshopItem\\";
			//  Write out version number
			if (  FileOpen(2)  )  CloseFile (  2 );
			OpenToWrite (  2, cstr(t.tempsteamdestworkshopfolder_s+"version.dat").Get() );
			WriteString (  2,Str(t.tsteamworkshopTheVersionNumber) );
			CloseFile (  2 );

			t.tempsteamstring_s = ReadString ( 1 );
			t.tempsteamworkshopfilename_s = "";
			for ( t.c = 1 ; t.c<=  Len(t.tempsteamstring_s.Get()); t.c++ )
			{
				if (  cstr(Mid(t.tempsteamstring_s.Get(),t.c))  ==  "\\" || cstr(Mid(t.tempsteamstring_s.Get(),t.c))  ==  "/" ) 
				{
					t.tempsteamworkshopfilename_s=t.tempsteamworkshopfilename_s+"_";
				}
				else
				{
					if (  cstr(Mid(t.tempsteamstring_s.Get(),t.c))  ==  "_" ) 
					{
						t.tempsteamworkshopfilename_s=t.tempsteamworkshopfilename_s+"@";
					}
					else
					{
						t.tempsteamworkshopfilename_s=t.tempsteamworkshopfilename_s+Mid(t.tempsteamstring_s.Get(),t.c);
					}
				}
			}

			if (  t.tempsteamstring_s  !=  "" ) 
			{
				CopyAFile (  cstr(g.fpscrootdir_s+"\\Files\\"+t.tempsteamstring_s).Get() , cstr(t.tempsteamdestworkshopfolder_s + t.tempsteamworkshopfilename_s).Get() );
				t.tsteamyesencrypt = 0;
				//  models
				if (  cstr(Right(t.tempsteamworkshopfilename_s.Get(),4))  ==  ".dbo"  )  t.tsteamyesencrypt  =  1;
				if (  cstr(Right(t.tempsteamworkshopfilename_s.Get(),2))  ==  ".x"  )  t.tsteamyesencrypt  =  1;
				//  images
				if (  cstr(Right(t.tempsteamworkshopfilename_s.Get(),4))  ==  ".png"  )  t.tsteamyesencrypt  =  1;
				if ( cstr( Right(t.tempsteamworkshopfilename_s.Get(),4))  ==  ".jpg"  )  t.tsteamyesencrypt  =  1;
				if (  cstr(Right(t.tempsteamworkshopfilename_s.Get(),4))  ==  ".dds"  )  t.tsteamyesencrypt  =  1;
				if (  cstr(Right(t.tempsteamworkshopfilename_s.Get(),4))  ==  ".bmp"  )  t.tsteamyesencrypt  =  1;
				//  sounds and music
				if ( cstr( Right(t.tempsteamworkshopfilename_s.Get(),4))  ==  ".wav"  )  t.tsteamyesencrypt  =  1;
				if ( cstr( Right(t.tempsteamworkshopfilename_s.Get(),4))  ==  ".mp3"  )  t.tsteamyesencrypt  =  1;
				if ( cstr( Right(t.tempsteamworkshopfilename_s.Get(),4) ) ==  ".ogg"  )  t.tsteamyesencrypt  =  1;
				//  scripts
//     `if Right(tempsteamworkshopfilename$,4)  ==  ".lua" then tsteamyesencrypt  ==  1

				if (  cstr(Right(t.tempsteamworkshopfilename_s.Get(),4))  ==  ".fpe"  )  t.tsteamyesencrypt  =  1;

				if (  t.tsteamyesencrypt  ==  1 ) 
				{
					EncryptWorkshopDBPro ( cstr(t.tempsteamdestworkshopfolder_s + t.tempsteamworkshopfilename_s).Get() );
					DeleteAFile (  cstr(t.tempsteamdestworkshopfolder_s + t.tempsteamworkshopfilename_s).Get() );
				}
			}
			if (  FileEnd(1) ) 
			{
				g.steamworks.buildingWorkshopItemMode = 2;
			}
		break;
//   ``````````````````````````````````

		case 2:

			if (  g.steamworks.fpmpicked  ==  "Level I just worked on" ) 
			{
				t.tempsteamworkshopname_s = "My Level";
			}
			else
			{
				t.tempsteamworkshopname_s = Left(g.steamworks.fpmpicked.Get(),Len(g.steamworks.fpmpicked.Get())-4);
			}

			SteamSetRoot( cstr( g.fpscrootdir_s+"\\Files\\" ).Get() );
			SteamCreateWorkshopItem (  t.tempsteamworkshopname_s.Get() );

			g.steamworks.buildingWorkshopItemMode = 3;
		break;
//   ``````````````````````````````````

		case 3:
			if (  SteamIsWorkshopItemUploaded()  ==  1 ) 
			{
				//  set changed flag to 0
				if (  g.steamworks.fpmpicked  ==  "Level I just worked on" ) 
				{
					t.tempsteammaptocheck_s = g.fpscrootdir_s+"\\Files\\mapbank\\worklevel.dat";
				}
				else
				{
					t.tempsteammaptocheck_s = g.fpscrootdir_s+"\\Files\\mapbank\\"+Left(g.steamworks.fpmpicked.Get(),Len(g.steamworks.fpmpicked.Get())-3)+"dat";
				}
				if (  FileOpen(1)  ==  1  )  CloseFile (  1 );
				//  Work out how many lines there are so we can Dim (  the right amount )
				t.thowmanylines = 0;
				OpenToRead (  1,t.tempsteammaptocheck_s.Get() );
					while (  FileEnd(1) == 0 ) 
					{
						t.tthrowawaystring_s = ReadString ( 1 );
						++t.thowmanylines;
					}
				CloseFile (  1 );
				////  now read in the whole thing
				Dim (  t.templines,t.thowmanylines );
				t.thowmanylines = 0;
				OpenToRead (  1,t.tempsteammaptocheck_s.Get() );
					while (  FileEnd(1) == 0 ) 
					{
						t.templines[t.thowmanylines] = ReadString ( 1 );
						++t.thowmanylines;
					}
				CloseFile (  1 );
				DeleteAFile (  t.tempsteammaptocheck_s.Get() );
				OpenToWrite (  1,t.tempsteammaptocheck_s.Get() );
				g.steamworks.workshopid = SteamGetWorkshopID();

				t.templines[1] = "0";
				t.templines[3] = g.steamworks.workshopid;
				for ( t.tloop = 0 ; t.tloop<=  t.thowmanylines-1; t.tloop++ )
				{
					WriteString (  1,t.templines[t.tloop].Get() );
				}
				CloseFile (  1 );

				//  get rid of the array
				UnDim (  t.templines );

				g.steamworks.buildingWorkshopItemMode = 99;
			}

			if (  SteamIsWorkshopItemUploaded()  ==  -1  )  g.steamworks.buildingWorkshopItemMode  =  98;
		break;
	}//~ return
return;

}

void steam_buildingWorkshopItemFailed ( void )
{
	t.tsteamlostconnectioncustommessage_s = "Could not build workshop item (Error MP015)";
	steam_lostConnection ( );
	g.steamworks.mode = STEAM_SERVER_CHOOSING_FPM_TO_USE;
return;

}

void steam_joinALobby ( void )
{
	t.a = g.steamworks.selectedLobby;
	if (  g.steamworks.selectedLobbyName  !=  "Getting Lobby details..." ) 
	{
		g.steamworks.lobbyjoinedname = g.steamworks.selectedLobbyName;
		t.tempsteamstringlobbyname_s = "";
		t.tempsteamgotto = 0;
		for ( t.tc = 1 ; t.tc<=  Len(g.steamworks.lobbyjoinedname.Get()); t.tc++ )
		{
			++t.tempsteamgotto;
			if (  cstr(Mid(g.steamworks.lobbyjoinedname.Get(),t.tc))  ==  ":" ) { t.tempsteamgotto+=2 ; break; }
			t.tempsteamstringlobbyname_s = t.tempsteamstringlobbyname_s + Mid(g.steamworks.lobbyjoinedname.Get(),t.tc);
		}
		g.steamworks.levelnametojoin = "";
		t.tempsteamfoundone = 0;
		for ( t.tc = 1 ; t.tc<=  Len(g.steamworks.lobbyjoinedname.Get()); t.tc++ )
		{
			if (  cstr(Mid(g.steamworks.lobbyjoinedname.Get(),t.tc))  ==  ":" )
			{
				++t.tempsteamfoundone;
			}
			else
			{
				if (  t.tempsteamfoundone == 1 ) 
				{
					g.steamworks.levelnametojoin = g.steamworks.levelnametojoin + Mid(g.steamworks.lobbyjoinedname.Get(),t.tc);
				}
			}
		}
		g.steamworks.workshopidtojoin = "";
		t.tempsteamfoundone = 0;
		for ( t.tc = 1 ; t.tc<=  Len(g.steamworks.lobbyjoinedname.Get()); t.tc++ )
		{
			if (  cstr(Mid(g.steamworks.lobbyjoinedname.Get(),t.tc))  ==  ":" )
			{
				++t.tempsteamfoundone;
			}
			else
			{
				if (  t.tempsteamfoundone == 2 ) 
				{
					g.steamworks.workshopidtojoin = g.steamworks.workshopidtojoin + Mid(g.steamworks.lobbyjoinedname.Get(),t.tc);
				}
			}
		}
		g.steamworks.workshopVersionNumberToJoin = "";
		t.tempsteamfoundone = 0;
		for ( t.tc = 1 ; t.tc<=  Len(g.steamworks.lobbyjoinedname.Get()); t.tc++ )
		{
			if (  cstr(Mid(g.steamworks.lobbyjoinedname.Get(),t.tc))  ==  ":" )
			{
				++t.tempsteamfoundone;
			}
			else
			{
				if (  t.tempsteamfoundone == 3 ) 
				{
					g.steamworks.workshopVersionNumberToJoin = g.steamworks.workshopVersionNumberToJoin + Mid(g.steamworks.lobbyjoinedname.Get(),t.tc);
				}
			}
		}
		g.steamworks.lobbyjoinedname = t.tempsteamstringlobbyname_s;

		//  Check here if there is a workshop item, if the user has subbed and downloaded
		if (  g.steamworks.workshopidtojoin  !=  "0" ) 
		{

			t.tempSteamworksLobbyNameFromList_s = g.steamworks.selectedLobbyName;
			steam_canIJoinThisLobby ( );
			t.tsteamstring_s = g.steamworks.lobbyjoinedname;

			if (  SteamIsWorkshopItemInstalled(g.steamworks.workshopidtojoin.Get())  ==  0 ) 
			{
				//  show screen asking if they want to subscribe
				g.steamworks.mode = STEAM_ASKING_IF_SUBSCRIBE_TO_WORKSHOP_ITEM;
				titles_steamdoyouwanttosubscribetoworkshopitem ( );
				return;
			}
			else
			{
				if (  t.tsteamcanjoinlobby  ==  2 ) 
				{
					//  show screen asking if they want to subscribe
					g.steamworks.mode = STEAM_TELLING_THEY_NEED_TO_RESTART;
					titles_steamdTellingToRestart ( );
					return;
				}
			}
		}

		SteamJoinLobby(t.a);
		g.steamworks.mode = STEAM_JOINING_LOBBY;
		t.tsteamwaitedforlobbytimer = Timer();
		g.steamworks.AttemptedToJoinLobbyTime = Timer();
		g.steamworks.lobbycount = 0;
	}
return;

}

void steam_canIJoinThisLobby ( void )
{
	if (  g.steamworks.selectedLobbyName  !=  "Getting Lobby details..." ) 
	{
		g.steamworks.lobbyjoinedname = t.tempSteamworksLobbyNameFromList_s;
		t.tempsteamstringlobbyname_s = "";
		t.tempsteamgotto = 0;
		for ( t.tc = 1 ; t.tc<=  Len(g.steamworks.lobbyjoinedname.Get()); t.tc++ )
		{
			++t.tempsteamgotto;
			if (  cstr(Mid(g.steamworks.lobbyjoinedname.Get(),t.tc) ) ==  ":" ) { t.tempsteamgotto += 2 ; break; }
			t.tempsteamstringlobbyname_s = t.tempsteamstringlobbyname_s + Mid(g.steamworks.lobbyjoinedname.Get(),t.tc);
		}
		g.steamworks.levelnametojoin = "";
		t.tempsteamfoundone = 0;
		for ( t.tc = 1 ; t.tc<=  Len(g.steamworks.lobbyjoinedname.Get()); t.tc++ )
		{
			if (  cstr(Mid(g.steamworks.lobbyjoinedname.Get(),t.tc))  ==  ")" )
			{
				++t.tempsteamfoundone;
			}
			else
			{
				if (  t.tempsteamfoundone == 1 ) 
				{
					g.steamworks.levelnametojoin = g.steamworks.levelnametojoin + Mid(g.steamworks.lobbyjoinedname.Get(),t.tc);
				}
			}
		}
		g.steamworks.workshopidtojoin = "";
		t.tempsteamfoundone = 0;
		for ( t.tc = 1 ; t.tc<=  Len(g.steamworks.lobbyjoinedname.Get()); t.tc++ )
		{
			if (  cstr(Mid(g.steamworks.lobbyjoinedname.Get(),t.tc))  ==  ":" )
			{
				++t.tempsteamfoundone;
			}
			else
			{
				if (  t.tempsteamfoundone == 2 ) 
				{
					g.steamworks.workshopidtojoin = g.steamworks.workshopidtojoin + Mid(g.steamworks.lobbyjoinedname.Get(),t.tc);
				}
			}
		}
		//  grab version number
		g.steamworks.workshopVersionNumberToJoin = "";
		t.tempsteamfoundone = 0;
		for ( t.tc = 1 ; t.tc<=  Len(g.steamworks.lobbyjoinedname.Get()); t.tc++ )
		{
			if (  cstr(Mid(g.steamworks.lobbyjoinedname.Get(),t.tc))  ==  ":" )
			{
				++t.tempsteamfoundone;
			}
			else
			{
				if (  t.tempsteamfoundone == 3 ) 
				{
					g.steamworks.workshopVersionNumberToJoin = g.steamworks.workshopVersionNumberToJoin + Mid(g.steamworks.lobbyjoinedname.Get(),t.tc);
				}
			}
		}
		g.steamworks.lobbyjoinedname = t.tempsteamstringlobbyname_s;

		//  Check here if there is a workshop item, if the user has subbed and downloaded
		if (  g.steamworks.workshopidtojoin  !=  "0" && g.steamworks.workshopidtojoin  !=  "" ) 
		{
			if (  SteamIsWorkshopItemInstalled(g.steamworks.workshopidtojoin.Get())  ==  0 ) 
			{
				t.tsteamcanjoinlobby = 0;
			}
			else
			{
				t.tsteamcanjoinlobby = 0;
				if (  SteamIsWorkshopItemInstalled(g.steamworks.workshopidtojoin.Get())  ==  2  )  t.tsteamcanjoinlobby  =  2;
				if (  t.tsteamcanjoinlobby  ==  0 ) 
				{
					t.tpath_s = SteamGetWorkshopItemPath();
					t.tfiletocheck_s = t.tpath_s + "\\version.dat";
					if (  FileExist(t.tfiletocheck_s.Get())  ==  1 ) 
					{
						if (  FileOpen(1)  )  CloseFile (  1 );
						OpenToRead (  1,t.tfiletocheck_s.Get() );
						t.tversioncheck_s = ReadString ( 1 );
						CloseFile (  1 );
						if (  t.tversioncheck_s  ==  g.steamworks.workshopVersionNumberToJoin ) 
						{
							t.tsteamcanjoinlobby = 1;
						}
						else
						{
							t.tsteamcanjoinlobby = 2;
						}
					}
				}
			}
		}
		else
		{
			t.tsteamcanjoinlobby = 1;
		}

	}
	else
	{
		t.tsteamcanjoinlobby = 0;
	}
return;

}

void steam_leaveALobby ( void )
{
	SteamLeaveLobby (  );
	steam_resetGameStats ( );
return;

}

void steam_SubscribeToWorkShopItem ( void )
{
	t.tlobbytring_s = g.steamworks.lobbyjoinedname;
	steam_subbedToItem ( );
	g.steamworks.mode = STEAM_ASKING_IF_SUBSCRIBE_TO_WORKSHOP_ITEM_WAITING_FOR_RESULTS;
}

void steam_save_workshop_files_needed ( void )
{
	cstr toriginalMasterLevelFile_s = "";
	cstr toriginalprojectname_s = "";

	toriginalMasterLevelFile_s = t.tmasterlevelfile_s;
	toriginalprojectname_s = g.projectfilename_s;
	//  If there is no baseList.dat file we cant proceed
	if (  FileExist("editors\\baseList.dat")  ==  0  )  return;

	//  Work out how many lines there are so we can Dim (  the right amount )
	t.thowmanyfpefiles = 0;
	OpenToRead (  1,"editors\\baseList.dat" );
		while (  FileEnd(1) == 0 ) 
		{
			t.tthrowawaystring_s = ReadString ( 1 );
			++t.thowmanyfpefiles;
		}
	CloseFile (  1 );

	//  Store the count in our global steamworks type
	g.steamworks.howmanyfpefiles = t.thowmanyfpefiles;

	Dim (  t.tallfpefiles_s,t.thowmanyfpefiles  );
	t.thowmanyfpefiles = 0;
	OpenToRead (  1,"editors\\baseList.dat" );
		while (  FileEnd(1) == 0 ) 
		{
			t.tallfpefiles_s[t.thowmanyfpefiles] = ReadString ( 1 );
			++t.thowmanyfpefiles;
		}
	CloseFile (  1 );

	//  work out what files used in this level do not come with the base install

	//  Name without EXE
//  `potato - use ttempprojfilename$ but it may contain full path

	t.exename_s=t.tsteamsavefilename_s;
	if (  cstr(Lower(Right(t.exename_s.Get(),4))) == ".fpm" ) 
	{
		t.exename_s=Left(t.exename_s.Get(),Len(t.exename_s.Get())-4);
	}
	for ( t.n = Len(t.exename_s.Get()) ; t.n >= 1 ; t.n+= -1 )
	{
		if (  cstr(Mid(t.exename_s.Get(),t.n)) == "\\" || cstr(Mid(t.exename_s.Get(),t.n)) == "/" ) 
		{
			t.exename_s=Right(t.exename_s.Get(),Len(t.exename_s.Get())-t.n);
			break;
		}
	}
	if (  Len(t.exename_s.Get())<1  )  t.exename_s = "sample";

	//  the level
	t.tmasterlevelfile_s=cstr("mapbank\\")+t.exename_s+".fpm";
	t.strwork = "" ; t.strwork = t.strwork + "Saving required files list for "+ t.tmasterlevelfile_s;
	timestampactivity(0, t.strwork.Get() );

	//  Get absolute My Games folder
	g.exedir_s="?";
	t.told_s=GetDir();
	t.tworkshoplistfile_s = t.told_s+"\\mapbank\\" + t.exename_s + ".dat";
	t.tsteamworkshopTheVersionNumber = 1;
	if (  FileExist(t.tworkshoplistfile_s.Get())  ==  1 ) 
	{
		t.tsteamworkshopitemtocheckifchangedandversion_s = t.tworkshoplistfile_s;
		steam_grabWorkshopChangedFlagAndVersion ( );
		++t.tsteamworkshopTheVersionNumber;
		DeleteAFile (  t.tworkshoplistfile_s.Get() );
	}
	OpenToWrite (  1,t.tworkshoplistfile_s.Get() );

	//  set the changed flag since we are saving, this way we dont rely on workshop info to know if a file is new or not
	WriteString (  1,"DO NOT MANUALY EDIT THIS FILE" );
	WriteString (  1,"1" );
	WriteString (  1,Str(t.tsteamworkshopTheVersionNumber) );
	WriteString (  1,"0" );

	//  Collect ALL files in string array list
	g.filecollectionmax = 0;
	Dim (  t.filecollection_s,500  );

	//  include original FPM
	addtocollection(t.tmasterlevelfile_s.Get());

	//  Stage 2 - collect all files
	t.tlevelfile_s="";
	g.projectfilename_s=t.tmasterlevelfile_s;

	//  load in level FPM
	if (  Len(t.tlevelfile_s.Get())>1 ) 
	{
		g.projectfilename_s=t.tlevelfile_s;
	}

	//  chosen sky, terrain and veg
	addfoldertocollection( cstr(cstr("skybank\\")+t.skybank_s[g.skyindex]).Get() );
	addfoldertocollection("skybank\\night");
	if ( stricmp ( g.terrainstyle_s.Get(), "CUSTOM" ) != NULL )
	{
		addfoldertocollection( cstr(cstr("terrainbank\\")+g.terrainstyle_s).Get() );
	}
	addfoldertocollection( cstr(cstr("vegbank\\")+g.vegstyle_s).Get() );

	//  choose all entities and associated files
	for ( t.e = 1 ; t.e<=  g.entityelementlist; t.e++ )
	{
		t.entid=t.entityelement[t.e].bankindex;
		if (  t.entid>0 ) 
		{
			//  check for lua scripts
			if (  t.entityelement[t.e].eleprof.aimain_s  !=  "" ) 
			{
				if (  steam_check_if_entity_is_from_install(t.entityelement[t.e].eleprof.aimain_s.Get())  ==  0 ) 
				{
					addtocollection( cstr(cstr("scriptbank\\")+t.entityelement[t.e].eleprof.aimain_s).Get() );
				}
			}
			//  entity profile file
			t.tentityname1_s=cstr("entitybank\\")+t.entitybank_s[t.entid];
			t.tentityname2_s=cstr(Left(t.tentityname1_s.Get(),Len(t.tentityname1_s.Get())-4))+".fpe";
			if (  FileExist( cstr(g.fpscrootdir_s+"\\Files\\"+t.tentityname2_s).Get() ) == 1 ) 
			{
				t.tentityname_s=t.tentityname2_s;
			}
			else
			{
				t.tentityname_s=t.tentityname1_s;
			}
			//  Check to see if the entity is part of the base install
			//  If it is, we can skip checking any further with it
			if (  steam_check_if_entity_is_from_install(t.tentityname_s.Get())  ==  0 ) 
			{

				addtocollection(t.tentityname_s.Get());
				//  entity files in folder
				t.tentityfolder_s=t.tentityname_s;
				for ( t.n = Len(t.tentityname_s.Get()) ; t.n >=  1 ; t.n+= -1 )
				{
					if (  cstr(Mid(t.tentityname_s.Get(),t.n)) == "\\" || cstr(Mid(t.tentityname_s.Get(),t.n)) == "/" ) 
					{
						t.tentityfolder_s=Left(t.tentityfolder_s.Get(),t.n);
						break;
					}
				}
				//  model file
				t.tlocaltofpe=1;
				for ( t.n = 1 ; t.n<=  Len(t.entityprofile[t.entid].model_s.Get()); t.n++ )
				{
					if (  cstr(Mid(t.entityprofile[t.entid].model_s.Get(),t.n)) == "\\" || cstr(Mid(t.entityprofile[t.entid].model_s.Get(),t.n)) == "/" ) 
					{
						t.tlocaltofpe=0 ; break;
					}
				}
				if (  t.tlocaltofpe == 1 ) 
				{
					t.tfile1_s=t.tentityfolder_s+t.entityprofile[t.entid].model_s;
				}
				else
				{
					t.tfile1_s=t.entityprofile[t.entid].model_s;
				}
				t.tfile2_s=cstr(Left(t.tfile1_s.Get(),Len(t.tfile1_s.Get())-2))+".dbo";
				if (  FileExist( cstr(g.fpscrootdir_s+"\\Files\\"+t.tfile2_s).Get() ) == 1 ) 
				{
					t.tfile_s=t.tfile2_s;
				}
				else
				{
					t.tfile_s=t.tfile1_s;
				}
				t.tmodelfile_s=t.tfile_s;
				addtocollection(t.tmodelfile_s.Get());
				//  entity characterpose file (if any)
				t.tfile3_s=cstr(Left(t.tfile1_s.Get(),Len(t.tfile1_s.Get())-2))+".dat";
				if (  FileExist(cstr(g.fpscrootdir_s+"\\Files\\"+t.tfile3_s).Get()) == 1 ) 
				{
					addtocollection(t.tfile3_s.Get());
				}

				//  texture files
				t.tlocaltofpe=1;
				for ( t.n = 1 ; t.n<=  Len(t.entityelement[t.e].eleprof.texd_s.Get()); t.n++ )
				{
					if (  cstr(Mid(t.entityelement[t.e].eleprof.texd_s.Get(),t.n)) == "\\" || cstr(Mid(t.entityelement[t.e].eleprof.texd_s.Get(),t.n)) == "/" ) 
					{
						t.tlocaltofpe=0 ; break;
					}
				}
				if (  t.tlocaltofpe == 1 ) 
				{
					t.tfile_s=t.tentityfolder_s+t.entityelement[t.e].eleprof.texd_s;
				}
				else
				{
					t.tfile_s=t.entityelement[t.e].eleprof.texd_s;
				}
				addtocollection(t.tfile_s.Get());
				timestampactivity(0, cstr(cstr("Exporting ")+t.entitybank_s[t.entid]+" texd:"+t.tfile_s).Get() );
				if (  cstr(Left(Lower(Right(t.tfile_s.Get(),6)),2)) == "_d" ) 
				{
					t.tfileext_s=Right(t.tfile_s.Get(),3);
					t.tfile_s=cstr(Left(t.tfile_s.Get(),Len(t.tfile_s.Get())-6))+"_n."+t.tfileext_s ; addtocollection(t.tfile_s.Get());
					t.tfile_s=cstr(Left(t.tfile_s.Get(),Len(t.tfile_s.Get())-6))+"_s."+t.tfileext_s ; addtocollection(t.tfile_s.Get());
					t.tfile_s=cstr(Left(t.tfile_s.Get(),Len(t.tfile_s.Get())-6))+"_i."+t.tfileext_s ; addtocollection(t.tfile_s.Get());
					t.tfile_s=cstr(Left(t.tfile_s.Get(),Len(t.tfile_s.Get())-6))+"_o."+t.tfileext_s ; addtocollection(t.tfile_s.Get());
				}
				if (  t.tlocaltofpe == 1 ) 
				{
					t.tfile_s=t.tentityfolder_s+t.entityelement[t.e].eleprof.texaltd_s;
				}
				else
				{
					t.tfile_s=t.entityelement[t.e].eleprof.texaltd_s;
				}
				addtocollection(t.tfile_s.Get());
				//  if entity did not specify texture it is multi-texture, so interogate model file
				findalltexturesinmodelfile(t.tmodelfile_s.Get(),t.tentityfolder_s.Get(),t.entityprofile[t.entityelement[t.e].bankindex].texpath_s.Get());
				//  shader file
				t.tfile_s=t.entityelement[t.e].eleprof.effect_s ; addtocollection(t.tfile_s.Get());
				//  script files
				t.tfile_s=cstr("scriptbank\\")+t.entityelement[t.e].eleprof.aiinit_s ; addtocollection(t.tfile_s.Get());
				t.tfile_s=cstr("scriptbank\\")+t.entityelement[t.e].eleprof.aimain_s ; addtocollection(t.tfile_s.Get());
				t.tfile_s=cstr("scriptbank\\")+t.entityelement[t.e].eleprof.aidestroy_s ; addtocollection(t.tfile_s.Get());
				t.tfile_s=cstr("scriptbank\\")+t.entityelement[t.e].eleprof.aishoot_s ; addtocollection(t.tfile_s.Get());
				//  sound files
				t.tfile_s=t.entityelement[t.e].eleprof.soundset_s ; addtocollection(t.tfile_s.Get());
				t.tfile_s=t.entityelement[t.e].eleprof.soundset1_s ; addtocollection(t.tfile_s.Get());
				t.tfile_s=t.entityelement[t.e].eleprof.soundset2_s ; addtocollection(t.tfile_s.Get());
				t.tfile_s=t.entityelement[t.e].eleprof.soundset3_s ; addtocollection(t.tfile_s.Get());
				t.tfile_s=t.entityelement[t.e].eleprof.soundset4_s ; addtocollection(t.tfile_s.Get());
				//  collectable guns
				if (  Len(t.entityprofile[t.entid].isweapon_s.Get())>1 ) 
				{
					t.tfile_s=cstr("gamecore\\guns\\")+t.entityprofile[t.entid].isweapon_s ; addfoldertocollection(t.tfile_s.Get());
					t.foundgunid=t.entityprofile[t.entid].isweapon;
					if (  t.foundgunid>0 ) 
					{
						for ( t.x = 0 ; t.x<=  1; t.x++ )
						{
							t.tpoolindex=g.firemodes[t.foundgunid][t.x].settings.poolindex;
							if (  t.tpoolindex>0 ) 
							{
								t.tfile_s=cstr("gamecore\\ammo\\")+t.ammopool[t.tpoolindex].name_s ; addfoldertocollection(t.tfile_s.Get());
							}
						}
					}
				}
				//  associated guns and ammo
				if (  Len(t.entityelement[t.e].eleprof.hasweapon_s.Get())>1 ) 
				{
					t.tfile_s=cstr("gamecore\\guns\\")+t.entityelement[t.e].eleprof.hasweapon_s ; addfoldertocollection(t.tfile_s.Get());
					t.foundgunid=t.entityelement[t.e].eleprof.hasweapon;
					if (  t.foundgunid>0 ) 
					{
						for ( t.x = 0 ; t.x<=  1; t.x++ )
						{
							t.tpoolindex=g.firemodes[t.foundgunid][t.x].settings.poolindex;
							if (  t.tpoolindex>0 ) 
							{
								t.tfile_s=cstr("gamecore\\ammo\\")+t.ammopool[t.tpoolindex].name_s ; addfoldertocollection(t.tfile_s.Get());
							}
						}
					}
				}
				//  zone marker can reference other levels to jump to
				if (  t.entityprofile[t.entid].ismarker == 3 ) 
				{
					t.tlevelfile_s=t.entityelement[t.e].eleprof.ifused_s;
					if (  Len(t.tlevelfile_s.Get())>1 ) 
					{
						t.tlevelfile_s=cstr("mapbank\\")+t.tlevelfile_s+".fpm";
						++t.levelmax;
						t.levellist_s[t.levelmax]=t.tlevelfile_s;
						addtocollection(t.tlevelfile_s.Get());
					}
				}
			}

		}
	}

	//  fill in the .dat file
	SetDir (  cstr(g.fpscrootdir_s+"\\Files\\").Get() );
	t.filesmax=g.filecollectionmax;
	t.thowmanyadded = 0;
	for ( t.fileindex = 1 ; t.fileindex<=  t.filesmax; t.fileindex++ )
	{
		t.name_s=t.filecollection_s[t.fileindex];
		if (  cstr(Left(t.name_s.Get(),12))  ==  "entitybank\\\\"  )  t.name_s  =  cstr("entitybank\\") + Right(t.name_s.Get(), Len(t.name_s.Get())-12);
		if (  cstr(Left(t.name_s.Get(),12))  ==  "scriptbank\\\\"  )  t.name_s  =  cstr("scriptbank\\") + Right(t.name_s.Get(), Len(t.name_s.Get())-12);
		if (  FileExist(t.name_s.Get()) == 1 ) 
		{
			if (  steam_check_if_entity_is_from_install(t.name_s.Get())  ==  0 ) 
			{
				WriteString (  1,t.name_s.Get() );
				//  check if it is character creator, if it is, check for the existance of a texture
				if (  cstr(Lower(Left(t.name_s.Get(),32)))  ==  "entitybank\\user\\charactercreator" ) 
				{
					t.tname_s = cstr(Left(t.name_s.Get(), Len(t.name_s.Get())-4)) + "_cc.dds";
					if (  FileExist(t.tname_s.Get())  ==  1 ) 
					{
						WriteString (  1,t.tname_s.Get() );
						++t.thowmanyadded;
					}
				}

				++t.thowmanyadded;
				//  09032015 - 017 - If its a gun, grab the muzzleflash, decals and include them
				if (  cstr(Right(t.name_s.Get(),11))  ==  "gunspec.txt" ) 
				{
					if (  FileOpen(3)  )  CloseFile (  3 );
					t.tfoundflash = 0;
					OpenToRead (  3,t.name_s.Get() );
					t.tfoundflash = 0;
					while (  FileEnd(3)  ==  0 && t.tfoundflash  ==  0 ) 
					{
						t.tisthisflash_s = ReadString ( 3 );
						if (  cstr(Left(t.tisthisflash_s.Get(),11))  ==  "muzzleflash" ) 
						{
							t.tlocationofequals = FindLastChar(t.tisthisflash_s.Get(),"=");
							if (  t.tlocationofequals > 1 ) 
							{
								if (  cstr(Mid(t.tisthisflash_s.Get(),t.tlocationofequals+1))  ==  " " ) 
								{
									t.tflash_s = Right(t.tisthisflash_s.Get(),Len(t.tisthisflash_s.Get())-(t.tlocationofequals+1));
								}
								else
								{
									t.tflash_s = Right(t.tisthisflash_s.Get(),Len(t.tisthisflash_s.Get())-(t.tlocationofequals));
								}
								t.tfext_s = "";
								if (  FileExist( cstr(cstr("gamecore\\muzzleflash\\flash")+t.tflash_s+".png").Get() )  ==  1  )  t.tfext_s  =  ".png";
								if (  FileExist( cstr(cstr("gamecore\\muzzleflash\\flash")+t.tflash_s+".dds").Get() )  ==  1  )  t.tfext_s  =  ".dds";
								if (  t.tfext_s  !=  "" ) 
								{
									WriteString (  1,cstr(cstr("gamecore\\muzzleflash\\flash")+t.tflash_s+t.tfext_s).Get() );
									++t.thowmanyadded;
								}
								t.tfext_s = "";
								if (  FileExist(cstr(cstr("gamecore\\decals\\muzzleflash")+t.tflash_s+"\\decal.png").Get() )  ==  1  )  t.tfext_s  =  ".png";
								if (  FileExist(cstr(cstr("gamecore\\decals\\muzzleflash")+t.tflash_s+"\\decal.dds").Get() )  ==  1  )  t.tfext_s  =  ".dds";
								if (  t.tfext_s  !=  "" ) 
								{
									WriteString (  1,cstr(cstr("gamecore\\decals\\muzzleflash")+t.tflash_s+"\\decal"+t.tfext_s).Get() );
									++t.thowmanyadded;
								}
								t.tfext_s = "";
								if (  FileExist(cstr(cstr("gamecore\\decals\\muzzleflash")+t.tflash_s+"\\decalspec.txt").Get() )  ==  1  )  t.tfext_s  =  ".txt";
								if (  t.tfext_s  !=  "" ) 
								{
									WriteString (  1,cstr(cstr("gamecore\\decals\\muzzleflash")+t.tflash_s+"\\decalspec"+t.tfext_s).Get() );
									++t.thowmanyadded;
								}
								t.tfoundflash = 1;
							}
						}
					}
					CloseFile (  3 );
				}
			}
		}
	}

	CloseFile (  1 );

	//  if (  it is just the fpm  )  there are is no custom media with this level
	if (  t.thowmanyadded  <=  1  )  DeleteAFile (  t.tworkshoplistfile_s.Get() );

	//  cleanup file array
	UnDim (  t.filecollection_s );

	//  Restore directory
	SetDir (  t.told_s.Get() );

	UnDim (  t.tallfpefiles );

	t.tmasterlevelfile_s = toriginalMasterLevelFile_s;
	g.projectfilename_s = toriginalprojectname_s;

}

void steam_grabWorkshopChangedFlagAndVersion ( void )
{
	if (  FileExist(t.tsteamworkshopitemtocheckifchangedandversion_s.Get())  ==  1 ) 
	{
		if (  FileOpen(1)  )  CloseFile (  1 );
		OpenToRead (  1,t.tsteamworkshopitemtocheckifchangedandversion_s.Get() );
		//  skip the warning message
		t.tnothing_s = ReadString ( 1 );
		//  read in flag changed
		t.tnothing_s = ReadString ( 1 );
		t.tsteamworkshopHasItemChangedFlag = ValF(t.tnothing_s.Get());
		//  read in version number
		t.tnothing_s = ReadString ( 1 );
		t.tsteamworkshopTheVersionNumber = ValF(t.tnothing_s.Get());
		//  read in workshop id
		t.tnothing_s = ReadString ( 1 );
		t.tsteamworkshopTheIDNumber_s = t.tnothing_s;
		CloseFile (  1 );
	}
return;

//  Check to see if this file is part of the base install
}

int steam_check_if_entity_is_from_install ( char* name_s )
{
	int ttemploop = 0;
	int ttresult = 0;
	ttresult = 0;
	if (  cstr(Left(name_s,12))  ==  "entitybank\\\\"  )  strcpy ( name_s  , cstr(cstr("entitybank\\") + Right(name_s, Len(name_s)-12)).Get() );
	if (  cstr(Right(name_s,3))  ==  "bin" ) 
	{
		strcpy ( name_s , cstr(cstr(Lower(Left(name_s,Len(name_s)-3))) + cstr("fpe")).Get() );
	}
	else
	{
		name_s = Lower(name_s);
	}
	cstr nameCheck = cstr(name_s);
	for ( ttemploop = 0 ; ttemploop<=  g.steamworks.howmanyfpefiles-1; ttemploop++ )
	{
		cstr listFile = cstr( _strlwr(t.tallfpefiles_s[ttemploop].Get()) );
		if (  nameCheck  ==  listFile ) 
		{
			ttresult = 1;
			break;
		}
	}
//endfunction ttresult
	return ttresult;
}

void steam_resetSteam ( void )
{
		steam_free ( );
		steam_init ( );
		steam_resetGameStats ( );
		g.steamworks.needToResetOnStartup = 0;
return;

}

void steam_shoot ( void )
{
	if (  t.weaponammo[g.weaponammoindex+g.ammooffset]>0 ) 
	{
		SteamShoot (  );
	}
return;

}

void steam_chat ( void )
{

	//  check for chat
	t.tchat_s = SteamGetChat();
	if (  t.tchat_s  !=  "" ) 
	{
		steam_chatNew ( );
	}

	t.tscancode = ScanCode();
	if (  KeyState(g.keymap[28])  ==  1 && t.oldchatscancode  !=  28 ) 
	{
		g.steamworks.chaton = 1-g.steamworks.chaton;
		if (  g.steamworks.chaton  ==  1 ) 
		{
			//  start a new chat message
			ClearEntryBuffer (  );
			g.steamworks.chatstring = "";
		}
		else
		{
			//  send the chat message
			//  local send
			if (  Len(g.steamworks.chatstring.Get())  ==  1 ) 
			{
				if (  Asc(Mid(g.steamworks.chatstring.Get(),1))  <=  31  )  g.steamworks.chatstring  =  "";
			}
			else
			{
				if (  Asc(Mid(g.steamworks.chatstring.Get(),1))  <=  31  )  g.steamworks.chatstring  =  Right(g.steamworks.chatstring.Get(),Len(g.steamworks.chatstring.Get())-1);
			}
			if (  g.steamworks.chatstring !=  "" ) 
			{
				t.tchat_s = cstr(cstr(Str(g.steamworks.me)) + Left( cstr(cstr("<") + SteamGetPlayerName() + "> " + g.steamworks.chatstring).Get(),80)).Get();
				steam_chatNew ( );
				g.steamworks.chatstring = "";
				if (  t.tchatLobbyMode  ==  0 ) 
				{
					SteamSendChat (  t.tchat_s.Get() );
				}
				else
				{
					//  030315 - 013 - Lobby chat
					SteamSendLobbyChat (  t.tchat_s.Get() );
				}
			}
		}
	}
	if (  g.steamworks.chaton  ==  1 ) 
	{
		if (  Timer() - g.steamworks.lastSpawnedTime > 1000 ) 
		{
			t.aisystem.processplayerlogic=0;
		}
		else
		{
			t.aisystem.processplayerlogic=1;
		}
		g.steamworks.chattimer = Timer();

		g.steamworks.chatstring = Entry(1);
		//  show the Text (  we have typed )
		if (  Timer() - t.chatcursortime > 250 ) 
		{
			t.chatcursortime = Timer();
			g.steamworks.cursoron = 1-g.steamworks.cursoron;
		}
		if (  g.steamworks.cursoron  ==  0 ) 
		{
			t.tstringtoshow_s = cstr(Left(cstr(cstr("<") + SteamGetPlayerName() + "> " + g.steamworks.chatstring).Get(),80));
		}
		else
		{
			t.tstringtoshow_s = cstr(Left(cstr(cstr("<") + SteamGetPlayerName() + "> " + g.steamworks.chatstring).Get(),80)) + cstr("|");
		}
	}
	else
	{
		t.aisystem.processplayerlogic=1;
	}
	t.oldchatscancode = t.tscancode;

	if (  Timer() - g.steamworks.chattimer  <=  STEAM_CHAT_DELAY+2550 ) 
	{
		t.ttimegone = Timer()-t.toldchattime;
		if (  t.ttimegone > 50 ) 
		{
			t.toldchattime = Timer();
			t.ttimegone = 16;
		}
		t.toldchattime = Timer();
		if (  Timer() - g.steamworks.chattimer  >=  STEAM_CHAT_DELAY ) 
		{
			t.tsteamalpha = t.tsteamalpha - t.ttimegone;
		}
		else
		{
			t.tsteamalpha = t.tsteamalpha + t.ttimegone;
		}
		if (  t.tsteamalpha < 0  )  t.tsteamalpha  =  0;
		if (  t.tsteamalpha > 255  )  t.tsteamalpha  =  255;
		
		if (  t.tsteamalpha > 0 ) 
		{
			InkEx ( 20, 20, 20 );//  Rgb(20,20,20),Rgb(20,20,20) );
			BoxEx (  5,5,(40*10)+5,((STEAM_MAX_CHAT_LINES+1)*15)+10 );
		}

		t.tsteamy = 10;
		for ( t.tloop = 0 ; t.tloop<=  STEAM_MAX_CHAT_LINES-1; t.tloop++ )
		{
			if (  t.steamworks_chat[t.tloop] != "" ) 
			{
				if (  cstr(Left(t.steamworks_chat[t.tloop].Get(),1))  ==  "0" ) { t.r  =  255 ; t.g  =  255 ; t.b  =  50; }
				else if (  cstr(Left(t.steamworks_chat[t.tloop].Get(),1))  ==  "1" ) { t.r  =  255  ; t.g  =  100 ; t.b  =  100; }
				else if (  cstr(Left(t.steamworks_chat[t.tloop].Get(),1))  ==  "2" ) { t.r  =  100  ; t.g  =  255 ; t.b  =  100; }
				else if (  cstr(Left(t.steamworks_chat[t.tloop].Get(),1))  ==  "3" ) { t.r  =  100  ; t.g  =  100 ; t.b  =  255; }
				else if (  cstr(Left(t.steamworks_chat[t.tloop].Get(),1))  ==  "4" ) { t.r  =  255  ; t.g  =  255 ; t.b  =  100; }
				else if (  cstr(Left(t.steamworks_chat[t.tloop].Get(),1))  ==  "5" ) { t.r  =  255  ; t.g  =  100 ; t.b  =  255; }
				else if (  cstr(Left(t.steamworks_chat[t.tloop].Get(),1))  ==  "6" ) { t.r  =  100  ; t.g  =  255 ; t.b  =  255; }
				else if (  cstr(Left(t.steamworks_chat[t.tloop].Get(),1))  ==  "7" ) { t.r  =  200  ; t.g  =  255 ; t.b  =  200; }
				else if (  cstr(Left(t.steamworks_chat[t.tloop].Get(),1))  ==  "s" ) { t.r  =  255  ; t.g  =  255 ; t.b  =  255; }
				t.tluarealcoords = 1;
				t.tluatextalpha = t.tsteamalpha;
				steam_textColor(10,t.tsteamy,2,Right(t.steamworks_chat[t.tloop].Get(),Len(t.steamworks_chat[t.tloop].Get())-1),t.r,t.g,t.b);
			}
			t.tsteamy += 14;
		}

		if (  g.steamworks.chaton  ==  1 ) 
		{
			t.tluarealcoords = 1;
			t.tluatextalpha = t.tsteamalpha;
			t.tsteamy = 10+((STEAM_MAX_CHAT_LINES)*15);
			steam_textColor(10,t.tsteamy,2,t.tstringtoshow_s.Get(),255,255,255);
		}
	}
	InkEx ( 255, 255, 255 );// Rgb(255,255,255),Rgb(0,0,0) );
}

void steam_chatNew ( void )
{
	for ( t.tloop = 0 ; t.tloop<=  STEAM_MAX_CHAT_LINES-2; t.tloop++ )
	{
		t.steamworks_chat[t.tloop] = t.steamworks_chat[t.tloop+1];
	}
	if (  Len(t.tchat_s.Get()) > 80  )  t.tchat_s  =  Left(t.tchat_s.Get(),80);
	if (  cstr(Left(t.tchat_s.Get(),1))  !=  "s" ) 
	{
		t.steamworks_chat[STEAM_MAX_CHAT_LINES-1] = t.tchat_s;
		g.steamworks.chattimer = Timer();
	}
	//  200315 - 021 - pick up users joining the game from the server message sent
	if (  cstr(Left(t.tchat_s.Get(),1))  ==  "s" ) 
	{
		t.tnametocheckforjoining_s = Right(t.tchat_s.Get(),Len(t.tchat_s.Get())-1);
		for ( t.tn = 0 ; t.tn<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.tn++ )
		{
			if (  cstr(Left(t.tnametocheckforjoining_s.Get(),Len(t.steamworks_joined[t.tn].Get())))  ==  t.steamworks_joined[t.tn] && t.steamworks_joined[t.tn]  !=  "" && cstr(Right(t.steamworks_joined[t.tn].Get(),6))  !=  "Joined" ) 
			{
				t.steamworks_joined[t.tn] = t.steamworks_joined[t.tn] + " - Joined";
//     `if tlastchatline$  !=  tchat$ then steamworks_chat(STEAM_MAX_CHAT_LINES-1)  ==  tchat$

//     `tlastchatline$ = tchat$

			}
		}
	}
}

void steam_quitGame ( void )
{
	// exit current game and return to multiplayer menu
	// 110315 - 019 - first lets fade out nice
	t.tstartoffade = Timer();
	t.tfadestealpha_f = 0.0;
	t.tspritetouse = 0;
	for ( t.tloop = 2000 ; t.tloop<=  3000; t.tloop++ )
	{
		if (  SpriteExist(t.tloop)  ==  0 ) { t.tspritetouse  =  t.tloop  ; break; }
	}
	while (  Timer() - t.tstartoffade < 500 ) 
	{
		t.tfadestealpha_f = (Timer() - t.tstartoffade)*2;
		if (  t.tfadestealpha_f < 0  )  t.tfadestealpha_f  =  0.0;
		if (  t.tfadestealpha_f > 255.0  )  t.tfadestealpha_f  =  255.0;
		if (  t.tspritetouse > 0 && ImageExist(g.panelimageoffset+1)  ==  1 ) 
		{
			Sprite (  t.tspritetouse,0,0,g.panelimageoffset+1 );
			SizeSprite (  t.tspritetouse,GetDisplayWidth()*10, GetDisplayHeight()*10 );
			SetSpriteDiffuse (  t.tspritetouse,0,0,0 );
			SetSpriteAlpha (  t.tspritetouse,t.tfadestealpha_f );
		}
		SteamLoop (  );
		Sync (  );
	}
	//  if the server, let everyone know instantly the server is dropping
	//  020315 - 012 - Send an end game message when the host decides to leave
	if (  g.steamworks.isGameHost  ==  1 ) 
	{
		SteamEndGame (  );
	}

	t.game.gameloop=0;
	t.game.levelloop=0;
	t.game.titleloop=0;
	t.game.quitflag=1;
}

void steam_freefadesprite ( void )
{
	// 240316 - v1.13b1 - free sprite now finished with fade
	if ( t.tspritetouse > 0 )
	{
		DeleteSprite ( t.tspritetouse );
		t.tspritetouse = 0;
	}
}

void steam_backToEditor ( void )
{
	t.game.gameloop=0;
	t.game.levelloop=0;
	t.game.titleloop=0;
	t.game.quitflag=1;
	g.steamworks.goBackToEditor = 1;
return;

//  remove all entities and lightmaps that are left from our gaming session
}

void steam_cleanupGame ( void )
{

	//  default start position is edit-camera XZ
	t.terrain.playerx_f=t.cx_f;
	t.terrain.playerz_f=t.cy_f;
	if (  t.terrain.TerrainID>0 ) 
	{
		t.terrain.playery_f=BT_GetGroundHeight(t.terrain.TerrainID,t.terrain.playerx_f,t.terrain.playerz_f)+150.0;
	}
	else
	{
		t.terrain.playery_f=1000.0+150.0;
	}
	t.terrain.playerax_f=0.0;
	t.terrain.playeray_f=0.0;
	t.terrain.playeraz_f=0.0;

	//  remove light map objects for return to IDE editor
	lm_restoreall ( );

	//  remove all entities
	if (  g.entityelementlist>0 ) 
	{
		for ( t.e = 1 ; t.e<=  g.entityelementlist; t.e++ )
		{
			t.obj=t.entityelement[t.e].obj;
			if (  t.obj>0 ) 
			{
				if (  ObjectExist(t.obj) == 1 ) 
				{
					DeleteObject (  t.obj );
				}
			}
			t.entityelement[t.e].obj=0;
			t.entityelement[t.e].bankindex=0;
		}
		g.entityelementlist=0;
	}

return;

//  Send Steam User ID to editor via file mapping

}

void steam_sendSteamIDToEditor ( void )
{

	if (  g.steamworks.isRunning  ==  0 ) 
	{
		//  was 60*1000, changing to 5 to keep try and connecting
		if (  Timer() - g.steamworks.lastTimeTriedToConnectToSteamFromEditor > 5*1000 ) 
		{
			g.steamworks.lastTimeTriedToConnectToSteamFromEditor = Timer();
			steam_resetSteam ( );
		}
		if (  g.steamworks.isRunning  ==  0 ) 
		{
			return;
		}
	}
	else
	{
		//  send user id

		//  Debug code put it for a user that had issues with the downloader (which needs to know the steam id from steam, if steam doesnt give it, it wont work)
//   `set cursor 0,30

//   `print "steam id = " + tSteamGetID$


//   `print Timer()

//   `print steamworks.lastTimeISentMySteamID


		if (  Timer() - g.steamworks.lastTimeISentMySteamID > 5000 ) 
		{

			t.tSteamGetID_s = SteamGetPlayerID();

			if (  t.tSteamGetID_s  !=  "" ) 
			{
//     `print "sending id"

				g.steamworks.lastTimeISentMySteamID = Timer();

				OpenFileMap (  1, "FPSEXCHANGE" );

				//  params; filemap number, offset in bytes, value
				SetFileMapDWORD (  1, 6145, 1 );
				SetFileMapString (  1, 6149 , t.tSteamGetID_s.Get() );
				SetEventAndWait (  1 );

				//  Close when set all defaults
				//CloseFileMap (  1 );

				g.steamworks.haveSentSteamIDToEditor = 1;
			}
			else
			{
				steam_resetSteam ( );
				g.steamworks.lastTimeISentMySteamID = Timer()-3000;
//     `print "resetting steam"

			}


		}
	}

return;

//  020315 - 012 - enable check for lobbies while in editor
}

void steam_checkIfLobbiesAvailable ( void )
{
	if (  t.thowlongbetweenlobbychecks  <=  0  )  t.thowlongbetweenlobbychecks  =  15*1000;
	if (  Timer() - t.tlasttimecheckedforlobbiestimer > t.thowlongbetweenlobbychecks ) 
	{
		SteamLoop (  );
		t.tlasttimecheckedforlobbiestimer = Timer();
		if (  g.steamworks.checkiflobbiesavailablemode  ==  0 ) 
		{
			SteamGetLobbyList (  );
			g.steamworks.checkiflobbiesavailablemode = 1;
			return;
		}
		if (  g.steamworks.checkiflobbiesavailablemode  ==  1 ) 
		{
			g.steamworks.checkiflobbiesavailablemode = 0;

			if (  SteamIsLobbyListCreated()  ==  0 ) 
			{
				return;
			}


			t.thowmanylobbiesavailable = SteamGetLobbyListSize();
			if (  t.thowmanylobbiesavailable > 0 ) 
			{
				if (  t.thowmanylobbiesavailable > 10 ) 
				{
					t.thowlongbetweenlobbychecks = 60*1000;
				}
				if (  t.thowmanylobbiesavailable > 20 ) 
				{
					t.thowlongbetweenlobbychecks = 150*1000;
				}
				if (  t.thowmanylobbiesavailable > 40 ) 
				{
					t.thowlongbetweenlobbychecks = 300*1000;
				}
				t.steamStatusBar_s = "        |        Multiplayer Lobbies available to join";
			}
			else
			{
				t.thowlongbetweenlobbychecks = 15*1000;
				t.steamStatusBar_s = "";
			}
		}
	}
return;

//  200315 - 021 - flashlight of when starting a game
}

void steam_flashLightOff ( void )
{
	t.playerlight.flashlightcontrol_f=0.0;
return;

//  set everyone to team A for coop mode
}

void steam_setupCoopTeam ( void )
{
	for ( t.tc = 0 ; t.tc<=  STEAM_MAX_NUMBER_OF_PLAYERS-1; t.tc++ )
	{
		t.steamworks_team[t.tc] = 0;
	}
return;

//  requires; e, tSteamX# and tSteamZ#
}

void steam_COOP_aiMoveTo ( void )
{

	if (  g.steamworks.endplay  ==  1  )  return;

	if (  t.game.runasmultiplayer  ==  1 && g.steamworks.coop  ==  1 && t.tLuaDontSendLua  ==  0 ) 
	{

		SteamSendLua (  STEAM_LUA_AiGoToX,t.e,t.tSteamX_f );
		SteamSendLua (  STEAM_LUA_AiGoToZ,t.e,t.tSteamZ_f );
	}
	else
	{
		//  lets check if the distance is worth the effort of moving
		t.tdx_f=t.tSteamX_f-ObjectPositionX(t.e);
		t.tdz_f=t.tSteamZ_f-ObjectPositionZ(t.e);
		t.tdist_f=Sqrt((t.tdx_f*t.tdx_f)+(t.tdz_f*t.tdz_f));

		//  is it isn't very far, lets just stop the ai so it doesnt jerk about
		if (  t.tdist_f < 75.0 ) 
		{
//    `AI Set Entity Position e,ObjectPositionX(e),ObjectPositionY(e),ObjectPositionZ(e)

			AISetEntityPosition (  t.e,t.tSteamX_f,BT_GetGroundHeight(t.terrain.TerrainID,t.tSteamX_f,t.tSteamZ_f),t.tSteamZ_f );
			AIEntityStop (  t.e );
		//  if it is a real destination, lets head there
		}
		else
		{
			AIEntityGoToPosition (  t.e, t.tSteamX_f, t.tSteamZ_f );
		}

	}
return;

}

void steam_entity_lua_lookatplayer ( void )
{
	entity_lua_findcharanimstate ( );
	if (  t.tcharanimindex != -1 ) 
	{

		//  Simply look in direction of player
		t.ee = t.steamworks_playerEntityID[t.v];
		t.tdx_f= ObjectPositionX (t.entityelement[t.ee].obj) - ObjectPositionX(t.entityelement[t.e].obj);
		t.tdz_f= ObjectPositionZ (t.entityelement[t.ee].obj) - ObjectPositionZ(t.entityelement[t.e].obj);
		AISetEntityAngleY (  t.charanimstate.obj,atan2deg(t.tdx_f,t.tdz_f) );
		
		//  If angle beyond 'look angle range', perform full rotation
		t.tangley_f=AIGetEntityAngleY(t.charanimstate.obj) ;
		t.headangley_f=t.tangley_f-ObjectAngleY(t.charanimstate.obj) ;
		if (  t.headangley_f<-180  )  t.headangley_f = t.headangley_f+360;
		if (  t.headangley_f>180  )  t.headangley_f = t.headangley_f-360;
		if (  t.headangley_f<-75 || t.headangley_f>75 ) 
		{
			t.charanimstate.currentangle_f=t.tangley_f;
			t.charanimstate.updatemoveangle=1;
			AISetEntityAngleY (  t.charanimstate.obj,t.charanimstate.currentangle_f );
			t.charanimstates[t.tcharanimindex] = t.charanimstate;
		}

	}
return;

}

void steam_entity_lua_fireweaponEffectOnly ( void )
{
	//  update gun appearance
	if (  t.entityelement[t.e].attachmentobj > 0 ) 
	{
		entity_controlattachments ( );

		t.tgunid=t.entityelement[t.e].eleprof.hasweapon;
		t.tattachedobj=t.entityelement[t.e].attachmentobj;
		t.te = t.e;

		t.tgunid = t.entityelement[t.e].eleprof.hasweapon;
		t.ttrr=Rnd(1);
		for ( t.tt = t.ttrr+0 ; t.tt<=  t.ttrr+1; t.tt++ )
		{
			t.ttsnd=t.gunsoundcompanion[t.tgunid][1][t.tt].soundid;
			if (  t.ttsnd>0 ) 
			{
				if (  SoundExist(t.ttsnd) == 1 ) 
				{
					if (  SoundPlaying(t.ttsnd) == 0 || t.tt == t.ttrr+1 ) 
					{
						t.charanimstate.firesoundindex=t.ttsnd ; t.tt=3;
						t.charanimstate.firesoundexpiry=Timer()+200;
					}
				}
			}
		}

		if (  t.charanimstate.firesoundindex>0 ) 
		{
			entity_lua_findcharanimstate ( );
			darkai_shooteffect ( );
		}

	}

	//  charanimstate is purely temporary, the firesoundindex will NOT be persistent!!
	t.charanimstate.firesoundindex=0;

return;

//  cycle through entities, pick out the ai and either take aggro or update depending on distance/ownership
}

void steam_updateAIForCOOP ( void )
{

		if (  g.steamworks.endplay  ==  1  )  return;

		t.tsentone = 0;
//   `set cursor 0,0


		if (  t.game.runasmultiplayer == 1 && g.steamworks.coop  ==  1 ) 
		{
			t.thowManyDoIHave = 0;
			//  only send one update to other players max
			//  so everyone gets a chance to update we keep track of where we were up to in the list last time we sent
			if (  t.tcoopyentityupdatetostartat  ==  0 || t.tcoopyentityupdatetostartat > g.entityelementlist ) 
			{
				t.tcoopyentityupdatetostartat = 1;
				++t.tcoopSendPositionUpdate;
				if (  t.tcoopSendPositionUpdate > 3  )  t.tcoopSendPositionUpdate  =  0;
			}
			for ( t.e = 1 ; t.e<=  g.entityelementlist; t.e++ )
			{
				t.entid=t.entityelement[t.e].bankindex;
				if (  t.entid>0 ) 
				{
					if (  (t.entityprofile[t.entid].ischaracter  ==  1 || t.entityelement[t.e].mp_isLuaChar  ==  1) && t.entityprofile[t.entid].ismultiplayercharacter  ==  0 ) 
					{
						if (  t.entityelement[t.e].health  ==  0 ) 
						{
							t.distx_f = CameraPositionX() - ObjectPositionX(t.entityelement[t.e].obj);
							t.distz_f = CameraPositionZ() - ObjectPositionZ(t.entityelement[t.e].obj);
							t.tdist_f = Sqrt((t.distx_f*t.distx_f)+(t.distz_f*t.distz_f));
							if (  t.tdist_f > 3000  )  ScaleObject (  t.entityelement[t.e].obj,0,0,0 );
						}
						if (  t.entityelement[t.e].mp_coopControlledByPlayer  !=  g.steamworks.me && t.entityelement[t.e].active == 1 && t.entityelement[t.e].health > 0 ) 
						{
							// rotate or look at player for 1 second after receiving lua command, to cut down packets being sent
							if (  t.entityelement[t.e].mp_rotateType  ==  1 ) 
							{
								if (  Timer() - t.entityelement[t.e].mp_rotateTimer > 1000  )  t.entityelement[t.e].mp_rotateType  =  0;
								if (  t.entityelement[t.e].obj > 0 ) 
								{
									if (  ObjectExist(t.entityelement[t.e].obj)  ==  1 ) 
									{
										if (  t.entityelement[t.e].mp_coopControlledByPlayer > -1 && t.entityelement[t.e].mp_coopControlledByPlayer < STEAM_MAX_NUMBER_OF_PLAYERS ) 
										{
											t.v = t.entityelement[t.e].mp_coopControlledByPlayer;
											steam_entity_lua_lookatplayer ( );
										}
									}
								}
							}
							if (  t.entityelement[t.e].mp_rotateType  ==  2 ) 
							{
								if (  Timer() - t.entityelement[t.e].mp_rotateTimer > 1000  )  t.entityelement[t.e].mp_rotateType  =  0;
								if (  t.entityelement[t.e].obj > 0 ) 
								{
									if (  ObjectExist(t.entityelement[t.e].obj)  ==  1 ) 
									{
										if (  t.entityelement[t.e].mp_coopControlledByPlayer > -1 && t.entityelement[t.e].mp_coopControlledByPlayer < STEAM_MAX_NUMBER_OF_PLAYERS ) 
										{
											t.v = t.entityelement[t.e].mp_coopControlledByPlayer;
											steam_entity_lua_lookatplayer ( );
										}
									}
								}
							}

							t.distx_f = CameraPositionX() - ObjectPositionX(t.entityelement[t.e].obj);
							t.distz_f = CameraPositionZ() - ObjectPositionZ(t.entityelement[t.e].obj);
							t.tdist_f = Sqrt((t.distx_f*t.distx_f)+(t.distz_f*t.distz_f));
							if (  t.tdist_f < 1200 || ( AIGetEntityHeardSound(t.entityelement[t.e].obj)  ==  1 && t.entityelement[t.e].mp_coopControlledByPlayer  ==  -1 ) ) 
							{
								if (  t.entityelement[t.e].mp_coopControlledByPlayer  ==  -1 ) 
								{
									t.tsteamplayeralive = 0;
								}
								else
								{
									t.tsteamplayeralive = SteamGetPlayerAlive(t.entityelement[t.e].mp_coopControlledByPlayer);
								}
								if (  Timer() - t.entityelement[t.e].mp_coopLastTimeSwitchedTarget > 5000 || t.tsteamplayeralive  ==  0 ) 
								{
									t.tthrowaway = Rnd(1);
									if (  t.tsteamplayeralive  ==  0 || t.entityelement[t.e].mp_coopControlledByPlayer  ==  -1  )  t.tthrowaway  =  1;
									if (  t.tthrowaway  ==  1 ) 
									{
										t.entityelement[t.e].mp_coopControlledByPlayer = g.steamworks.me;
										SteamSendLua (  Steam_LUA_TakenAggro,t.e,g.steamworks.me );
										SteamSendLua (  STEAM_LUA_AiGoToX,t.entityelement[t.e].obj,ObjectPositionX(t.entityelement[t.e].obj) );
										SteamSendLua (  STEAM_LUA_AiGoToZ,t.entityelement[t.e].obj,ObjectPositionZ(t.entityelement[t.e].obj) );
//           `AI Entity Stop entityelement(e).obj

										t.entityelement[t.e].mp_updateOn = 1;
										t.entityelement[t.e].mp_lastUpdateSent = 0;
									}
									t.entityelement[t.e].mp_coopLastTimeSwitchedTarget = Timer()+5000;
								}
							}
						}
						else
						{
							if (  t.entityelement[t.e].mp_coopControlledByPlayer  ==  g.steamworks.me ) 
							{
								if (  t.entityelement[t.e].mp_updateOn  ==  1 && t.tsentone  ==  0 ) 
								{
									if (  ObjectExist(t.entityelement[t.e].obj)  ==  1 ) 
									{
										AISetEntityActive (  t.entityelement[t.e].obj,1 );
									}
									if (  t.tcoopyentityupdatetostartat  <=  t.e ) 
									{
//           ``

										if (  t.entityelement[t.e].active == 1 && t.entityelement[t.e].health > 0 ) 
										{

											if (  Timer() - t.tcoopLastUpdateSent > 500 || t.tcoopLastUpdateSent < 0 ) 
											{
													t.tsentone = 1;
													SteamSendLua (  Steam_LUA_HaveAggro,t.e,g.steamworks.me );
													SteamSendLua (  STEAM_LUA_AiGoToX,t.entityelement[t.e].obj,ObjectPositionX(t.entityelement[t.e].obj) );
													SteamSendLua (  STEAM_LUA_AiGoToZ,t.entityelement[t.e].obj,ObjectPositionZ(t.entityelement[t.e].obj) );
													t.entityelement[t.e].mp_lastUpdateSent = Timer();
													t.tcoopLastUpdateSent = Timer();
													t.tcoopyentityupdatetostartat = t.e+1;

//             `endif

											}
											else
											{
												//  pretend way have sent one this time, since we need to wait a little while
												t.tsentone = 1;
											}

										}
//           ``

									}
								}
							}

						}
					}
				}
			}
			//  if we havent sent anything, reset the list
			if (  t.tsentone  ==  0 ) 
			{
				t.tcoopyentityupdatetostartat = 1;
				++t.tcoopSendPositionUpdate;
				if (  t.tcoopSendPositionUpdate > 3  )  t.tcoopSendPositionUpdate  =  0;
			}
		}

return;

}

void steam_coop_rotatetoplayer ( void )
{
	//  only rotate to player if enemy ai with proper rig
	if (  t.entityelement[t.e].mp_isLuaChar  ==  1  )  return;
	entity_lua_findcharanimstate ( );
	if (  t.tcharanimindex == -1 ) 
	{
		//  regular entity
		t.entityelement[t.e].ry=t.v;
		entity_lua_rotateupdate ( );
	}
	else
	{
		// character subsystem
		t.charanimstate.currentangle_f=t.v;
		t.charanimstate.updatemoveangle=1;
		AISetEntityAngleY ( t.charanimstate.obj,t.charanimstate.currentangle_f );
		t.charanimstates[t.tcharanimindex] = t.charanimstate;
		t.entityelement[t.e].ry=t.v;

		// 240217 - and update visually
		entity_lua_rotateupdate ( );
	}
}

void steam_storeOldEntityPositions ( void )
{

	Dim (  t.steamworks_oldEntityPositionsX,g.entityelementlist );
	Dim (  t.steamworks_oldEntityPositionsZ,g.entityelementlist );

	for ( t.e = 1 ; t.e<=  g.entityelementlist; t.e++ )
	{
		t.entid=t.entityelement[t.e].bankindex;
		if (  t.entid>0 ) 
		{
			if (  (t.entityprofile[t.entid].ischaracter  ==  1 || t.entityelement[t.e].mp_isLuaChar  ==  1) && t.entityprofile[t.entid].ismultiplayercharacter  ==  0 ) 
			{
				if (  t.entityelement[t.e].obj > 0 ) 
				{
					if (  ObjectExist(t.entityelement[t.e].obj)  ==  1 ) 
					{
						t.steamworks_oldEntityPositionsX[t.e] = ObjectPositionX(t.entityelement[t.e].obj);
						t.steamworks_oldEntityPositionsZ[t.e] = ObjectPositionZ(t.entityelement[t.e].obj);
					}
				}
			}
		}
	}

return;

}

void steam_howManyEnemiesLeftToKill ( void )
{
		if (  g.steamworks.coop  ==  1 ) 
		{
			t.thowmanyenemies = 0;
			for ( t.e = 1 ; t.e<=  g.entityelementlist; t.e++ )
			{
				t.entid=t.entityelement[t.e].bankindex;
				if (  t.entid>0 ) 
				{
					if (  t.entityprofile[t.entid].ischaracter  ==  1 || t.entityelement[t.e].mp_isLuaChar  ==  1 ) 
					{
						if (  t.entityelement[t.e].active  ==  1 && t.entityelement[t.e].health > 0 ) 
						{
							++t.thowmanyenemies;
						}
					}
				}
			}
			LuaSetInt (  "mp_enemiesLeftToKill", t.thowmanyenemies );
		}
return;

}

void steam_IKilledAnAI ( void )
{
	t.steamworks_kills[g.steamworks.me+1] = t.steamworks_kills[g.steamworks.me+1] + 1;
	SteamSendLua (  STEAM_LUA_ServerSetPlayerKills,g.steamworks.me+1,t.steamworks_kills[g.steamworks.me+1] );
	t.tnothing = LuaExecute( cstr(cstr("mp_playerKills[") + Str(g.steamworks.me+1) + "] = " + Str(t.steamworks_kills[g.steamworks.me+1])).Get());
return;

// `////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

void steam_text ( int x, int y, int size, char* txt_s )
{
	t.luaText.txt = txt_s;
	t.luaText.x = x;
	t.luaText.y = y;
	t.luaText.size = size;
	lua_text ( );
//endfunction

}

void steam_textDots ( int x, int y, int size, char* txt_s )
{

	if (  Timer() - g.steamworks.steamdotsoldtime > 150 ) 
	{
		g.steamworks.steamdotsoldtime = Timer();
		g.steamworks.buildingDots = g.steamworks.buildingDots + ".";
		if (  Len(g.steamworks.buildingDots.Get()) > 5  )  g.steamworks.buildingDots  =  ".";
	}

	t.luaText.txt = g.steamworks.buildingDots + txt_s + g.steamworks.buildingDots;
	t.luaText.x = x;
	t.luaText.y = y;
	t.luaText.size = size;
	lua_text ( );
//endfunction

}

void steam_textColor ( int x, int y, int size, char* txt_s, int r, int gg, int b )
{
	g.steamworks.steamDoColorText = 1;
	t.luaText.txt = txt_s;
	t.luaText.x = x;
	t.luaText.y = y;
	t.luaText.size = size;
	g.steamworks.steamColorRed = r;
	g.steamworks.steamColorGreen = gg;
	g.steamworks.steamColorBlue = b;
	lua_text ( );
//endfunction

}

void steam_panel ( int x, int y, int x2, int y2 )
{
	t.luaPanel.x = x;
	t.luaPanel.y = y;
	t.luaPanel.x2 = x2;
	t.luaPanel.y2 = y2;
	lua_panel ( );
//endfunction

}

void steam_livelog ( char* t_s )
{
	SetTextSize (  32 );
	while (  SpaceKey()  ==  0 ) 
	{
		Cls (  );
		Print (  t_s );
		Sync (  );
	}
	while (  SpaceKey()  ==  1 ) 
	{
		Cls (  );
		Print (  t_s );
		Sync (  );
	}
//endfunction

}

void steam_deleteFile ( char* tempFileToDelete_s )
{
	cstr fileToDelete;
	fileToDelete =  cstr(g.fpscrootdir_s + "\\Files\\" + tempFileToDelete_s).Get();
	if (  FileExist(fileToDelete.Get() ) )  DeleteAFile (  fileToDelete.Get() );
//endfunction

}

// needs e;
int steam_check_if_lua_entity_exists ( int tentitytocheck )
{
	int tcheckobj = 0;
	int result = 0;
	result = 0;

	if (  tentitytocheck  <=  g.entityelementlist ) 
	{
		tcheckobj = t.entityelement[tentitytocheck].obj;
		if (  tcheckobj > 0 ) 
		{
			if (  ObjectExist(tcheckobj)  ==  1 ) 
			{
				result = 1;
			}
		}
	}

//endfunction result
	return result
;
}
