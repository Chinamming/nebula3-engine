//------------------------------------------------------------------------------
//  internalplayerbase.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalmultiplayer/raknet/raknetpacketchannel.h"

namespace RakNet
{

//------------------------------------------------------------------------------
/**
*/
Base::PacketChannelBase::Code 
RakNetPacketChannel::AsNebula3Channel(char raknetChannel)
{
    switch ((RakNetChannel)raknetChannel)
    {
    case RakNetDefaultChannel: return Base::PacketChannelBase::DefaultChannel;
    case RakNetChatChannel: return Base::PacketChannelBase::ChatChannel;
    case RakNetMiscChannel: return Base::PacketChannelBase::MiscChannel;
    };
    return Base::PacketChannelBase::DefaultChannel;
}

//------------------------------------------------------------------------------
/**
*/
char 
RakNetPacketChannel::AsRakNetChannel(Base::PacketChannelBase::Code nebulaChannel)
{
    RakNetChannel channel;
    switch (nebulaChannel)
    {
    case Base::PacketChannelBase::DefaultChannel: channel = RakNetDefaultChannel;
    case Base::PacketChannelBase::ChatChannel: channel =  RakNetChatChannel;
    case Base::PacketChannelBase::MiscChannel: channel =  RakNetMiscChannel;
    };
    return (char)channel;
}
} // namespace InternalMultiplayer
