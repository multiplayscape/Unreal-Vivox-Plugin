/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxTranscribedMessage.h"

#include "VivoxCore.h"
#include "VivoxAccountId.h"
#include "VivoxChannelSession.h"

#include "VivoxGlobals.h"

UVivoxTranscribedMessage * UVivoxTranscribedMessage::CreateVivoxTranscribedMessage(ITranscribedMessage* transcribedMessage) {
	UVivoxTranscribedMessage *vivoxTranscribedMessage = NewObject<UVivoxTranscribedMessage>();
	vivoxTranscribedMessage->VivoxTranscribedMessage = transcribedMessage;

    return vivoxTranscribedMessage;
}

void UVivoxTranscribedMessage::BeginDestroy() {
	VivoxTranscribedMessage = nullptr;

	this->UObject::BeginDestroy();
}

FDateTime UVivoxTranscribedMessage::ReceivedTime() const {
    return this->VivoxTranscribedMessage->ReceivedTime();
}

const FString& UVivoxTranscribedMessage::Text() const {
    return this->VivoxTranscribedMessage->Text();
}

FVivoxAccountId UVivoxTranscribedMessage::Speaker() const {
    return FVivoxAccountId().fromVivox(this->VivoxTranscribedMessage->Speaker());
}

const FString& UVivoxTranscribedMessage::Language() const {
	return this->VivoxTranscribedMessage->Language();
}

UVivoxChannelSession *UVivoxTranscribedMessage::ChannelSession() const {
	return UVivoxChannelSession::CreateVivoxChannelSession((IChannelSession*)&(this->VivoxTranscribedMessage->ChannelSession()));
}