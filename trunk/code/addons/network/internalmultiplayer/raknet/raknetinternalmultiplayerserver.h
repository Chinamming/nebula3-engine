#pragma once
//------------------------------------------------------------------------------
/**
    @class RakNet::RakNetInternalMultiplayerServer

    The RakNet InternalMultiplayerServer is controlled by the MultiplayerServer on mainthread side.
    
            
    (C) 2009 Radon Labs GmbH
*/
#include "internalmultiplayer/base/internalmultiplayerserverbase.h"
#include "internalmultiplayer/multiplayertype.h"
#include "extlibs/raknet/rakpeer.h"
#include "util/dictionary.h"
#include "internalmultiplayer/internalplayer.h"
#include "extlibs/raknet/rakpeer.h"
#include "internalmultiplayer/packetid.h"
#include "timing/time.h"
#include "internalmultiplayer/playerinfo.h"
#include "internalmultiplayer/raknet/raknetparameterresolver.h"

#define SEARCH_INFO_ADDITONAL_DATA_SIZE 64

//------------------------------------------------------------------------------
namespace RakNet
{
class RakNetInternalMultiplayerServer : public Base::InternalMultiplayerServerBase
{
    __DeclareClass(RakNetInternalMultiplayerServer);
    __DeclareSingleton(RakNetInternalMultiplayerServer);
public:

    /// constructor
    RakNetInternalMultiplayerServer();
    /// destructor
    virtual ~RakNetInternalMultiplayerServer();

    /// open the RakNetInternalMultiplayerServer 
    void Open();
    /// close the RakNetInternalMultiplayerServer
    void Close();
    /// perform client-side per-frame updates
    void OnFrame();

    /// on create player
    void OnCreatePlayers(const Ptr<Multiplayer::CreatePlayers>& msg);
    /// on host a session
    void OnSetGameType(/*const Ptr<Multiplayer::SetGameType>& msg*/);
    /// on host a session
    void OnHostSession(const Ptr<Multiplayer::HostSession>& msg);
    /// on kick a player
    void OnKickPlayer(/*const Ptr<Multiplayer::KickPlayer>& msg*/);
    /// on delete session call
    void OnDeleteSession(const Ptr<Multiplayer::DeleteSession>& msg);

    /// on start game search
    void OnStartGameSearch(const Ptr<Multiplayer::StartGameSearch>& msg);

    /// on join a given session
    void OnJoinSession(const Ptr<Multiplayer::JoinSession>& msg);
    /// on set player ready
    void OnSetPlayerReady(const Ptr<Multiplayer::SetPlayerReady>& msg);
    /// on leave current session
    void OnLeaveSession(const Ptr<Multiplayer::LeaveSession>& msg);

    /// host: on start game
    void OnStartGame(const Ptr<Multiplayer::StartGame>& msg);
    /// on end game called from host
    void OnEndGame(const Ptr<Multiplayer::EndGame>& msg);
    /// client: on ready of starting a game
    void OnStartingGameFinished(const Ptr<Multiplayer::StartingGameFinished>& msg);

    /// on send data to specific player
    void OnSendDataToPlayer(const Ptr<Multiplayer::SendDataToPlayer>& msg);
    /// on send data to all players
    void OnBroadcastData(const Ptr<Multiplayer::BroadcastData>& msg);         
    /// on send data to host
    void OnSendDataToHost(const Ptr<Multiplayer::SendDataToHost>& msg);

    /// write statistics to html writer, overwrite in subclass
    void WriteStatistics(const Ptr<Http::HtmlPageWriter>& writer);
    /// get rakpeer interface
    RakPeerInterface* GetRakPeerInterface() const;

private:
    enum NetworkState
    {
        IDLE,
        SESSION_CREATING,
        SESSION_JOINING,
        SESSION_JOINING_RECEVING_PLAYERS,
        GAME_LOBBY,
        GAME_REGISTERING,
        GAME_STARTING,
        GAME_PLAY,
        GAME_ENDING,
        SESSION_LEAVING,
        SESSION_DELETING
    };
    NetworkState curNetworkState;
    /// set network state, validates from old to new state transition
    void SetNetworkState(NetworkState newState);
    
    /// handle sign in notifications
    void ReceivePackets();
    /// handle active game search
    void CheckActiveGameSearch();
    /// handle game starting
    void CheckGameStarting();
    /// send notifications
    void SendPendingNotifications();

    //////////////////////////////////////////////////////////////////////////
    // Notification handling methods
    //////////////////////////////////////////////////////////////////////////
    /// on successful hosting game
    void HandleHostSessionSucceeded();
    /// on failed hosting game
    void HandleHostSessionFailed();
    /// on successful connecting to session
    void HandleConnectionSucceeded(Packet* packet);
    /// on failed connecting to session
    void HandleConnectionFailed();
    /// on successful joining game
    void HandleJoinSessionSucceeded(Packet* packet);
    /// on failed joining game
    void HandleJoinSessionFailed();
    /// on game search completed, after a given time
    void HandleGameSearchCompleted();
    /// on new game search result received
    void HandleGameSearchResultReceived(Packet* packet);
    /// on player joined
    void HandlePlayerJoined(Packet* packet);
    /// on player joined
    void HandleRemotePlayerJoined(Packet* packet);
    /// on player left
    void HandleDisconnection(Packet* packet);
    /// notify of left player
    void NotifyOfLeftPlayer(const Ptr<InternalMultiplayer::InternalPlayer>& player);
    /// on remote player left
    void HandleRemotePlayerLeft(Packet* packet);
    /// on player changed its ready state
    void HandlePlayerReadinessChanged(Packet* packet);
    /// on a successful game start
    void HandleStartGameSucceded(bool ranked);
    /// on a failed game start
    void HandleStartGameFailed();
    /// on a successful game end
    void HandleEndGameSucceded();
    /// on a failed game end
    void HandleEndGameFailed();
    /// on a successful game play start on a client received from host
    void HandlePlayerGameStartFinished(Packet* p);
    /// on a successful game play start on all clients send from host to all clients
    void HandleAllPlayersStartFinished();
    /// on a successful return from gameplay to lobby
    void HandleReturnToLobbySuccedded();
    /// on data received
    void HandleDataReceived(Packet* packet);
    /// on host invoked game start
    void HandleGameStartInvoked();
    /// on end game
    void HandleEndGameReceived(Packet* packet);
    //////////////////////////////////////////////////////////////////////////


    /// send player info to target
    void SendPlayerInfoTo(MessageID infoType, const RakNetGUID& guid, const Util::Array<Ptr<InternalMultiplayer::InternalPlayer> >& playerArray);
    /// add remote players from player infos
    Util::Array<Ptr<InternalMultiplayer::InternalPlayer> > CreateSessionPlayers(Packet* packet);
    /// create a bit netstream from raknet packet 
    Ptr<InternalMultiplayer::NetStream> CreateStreamFromPacket(Packet* packet);
    /// close connection to all connected players
    void CloseConnectionToAll();
    /// update session info data for anybody who pings for it
    void UpdateSharedSessionInfo(Ptr<RakNetParameterResolver> paraResolver = 0);
    /// send readiness changed to all connected players
    void BroadcastPlayerReadiness(bool ready, const Ptr<InternalMultiplayer::InternalPlayer>& excludePlayer = 0);
    /// broadcast a system message to all connected players
    bool BroadcastSystemMessage(InternalMultiplayer::PacketId::PacketIdCode code, const Ptr<InternalMultiplayer::InternalPlayer>& excludePlayer = 0);
    /// send a system message to a given receiver
    bool SendSystemMessageTo(InternalMultiplayer::PacketId::PacketIdCode code, const RakNetGUID& guid);

    /// update statistics, overwrite in subclass
    virtual void UpdateStatistics();
    /// compute min, max and average counts for history 
    void ComputeMinMaxAvgCounts(const Util::Array<int>& counterValues, int& outMin, int& outMax, int& outAvg) const;

    /// migrate host
    void MigrateHost();
    
    // raknet interface
    RakPeerInterface* raknetInterface;

    // host game
    Ptr<Multiplayer::HostSession> hostSessionMsg; 

    // search game result
    /// check if there is any session info from given system address
    bool HaveSessionInfoFrom(const SystemAddress& address);
    Timing::Time pingIntervall;
    Util::Array<InternalMultiplayer::SessionInfo> sessionInfos;

    // current search game message
    Ptr<Multiplayer::StartGameSearch> activeGameSearchMsg;

    Timing::Time curSearchTime;
    struct GameSearchSessionInfo    // struct is used for ping answer with session data
    {
       ushort openPublicSlots;
       ushort openPrivateSlots;
       ushort filledPublicSlots;
       ushort filledPrivateSlots;
       ushort sessionType;
       unsigned char additionalData[SEARCH_INFO_ADDITONAL_DATA_SIZE];
    } sharedSessionData;

    // join info
    Ptr<Multiplayer::JoinSession> joinMsg;
    Util::Array<Ptr<Messaging::Message> > pendingNotifcations;

    // players
    Util::Dictionary<IndexT, InternalMultiplayer::PlayerInfo> playerInfo;
    Util::Dictionary<IndexT, Ptr<InternalMultiplayer::InternalPlayer> > pendingPlayers;

    bool updateStatistics;
    Timing::Tick lastStatsUpdated;
};


//------------------------------------------------------------------------------
/**
*/
inline RakPeerInterface* 
RakNetInternalMultiplayerServer::GetRakPeerInterface() const
{
    return this->raknetInterface;
}
} // namespace RakNet
//------------------------------------------------------------------------------
