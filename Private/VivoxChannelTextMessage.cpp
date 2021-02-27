/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxChannelTextMessage.h"

#include "Presence.h"
#include "VivoxCore.h"

#include "VivoxGlobals.h"

UVivoxChannelTextMessage *UVivoxChannelTextMessage::CreateVivoxChannelTextMessage(IChannelTextMessage *channelTextMessage) {
    UVivoxChannelTextMessage *vivoxChannelTextMessage = NewObject<UVivoxChannelTextMessage>();
    vivoxChannelTextMessage->VivoxChannelTextMessage = channelTextMessage;

    return vivoxChannelTextMessage;
}

void UVivoxChannelTextMessage::BeginDestroy() {

	VivoxChannelTextMessage = nullptr;

	this->UObject::BeginDestroy();

}

UVivoxChannelSession * UVivoxChannelTextMessage::ChannelSession() const {
    return UVivoxChannelSession::CreateVivoxChannelSession((IChannelSession *)&this->VivoxChannelTextMessage->ChannelSession());
}